"""Cross-check a MATLAB + MAVLink HIL session against the checklists in
HIL_MAVSDK_PYMAVLINK_AUTOMATED_DEBUG_PLAN.md sections 8 and 9.

Reads mavlink_raw_log.jsonl and matlab_summary.json from a session directory
and produces session_report.json plus a human-readable console summary, so an
AI/coding agent can tell where the HIL chain broke without opening
QGroundControl (plan section 12, Definition of Done).

Stdlib only -- this runs after a session, independent of mavsdk/pymavlink.
"""

from __future__ import annotations

import argparse
import json
from datetime import datetime
from pathlib import Path
from typing import Any, Iterable

MAV_MODE_FLAG_SAFETY_ARMED = 128
DEFAULT_COORD_AGREEMENT_DEG = 0.01  # ~1.1 km at the equator
DEFAULT_NEAR_ZERO_DEG = 0.5
DEFAULT_SERVO_PWM_DELTA = 50.0
DEFAULT_ACTUATOR_DELTA = 0.02
DEFAULT_POSITION_DELTA_M = 0.5


def load_jsonl(path: Path) -> Iterable[dict[str, Any]]:
    with path.open("r", encoding="utf-8") as stream:
        for line in stream:
            line = line.strip()
            if not line:
                continue
            try:
                yield json.loads(line)
            except json.JSONDecodeError:
                continue


def parse_ts(record: dict[str, Any]) -> datetime | None:
    raw = record.get("timestamp_utc")
    if not raw:
        return None
    try:
        return datetime.fromisoformat(raw)
    except ValueError:
        return None


class MavlinkFindings:
    def __init__(self) -> None:
        self.gps_raw_int_first: dict[str, float] | None = None
        self.gps_raw_int_last: dict[str, float] | None = None
        self.global_position_int_first: dict[str, float] | None = None
        self.global_position_int_last: dict[str, float] | None = None
        self.home_position: dict[str, float] | None = None
        self.armed_first_time: datetime | None = None
        self.disarmed_seen_after_arm = False
        self.command_acks: list[dict[str, Any]] = []
        self.statustexts: list[str] = []
        self._servo_baseline: dict[str, float] | None = None
        self._actuator_baseline: list[float] | None = None
        self.servo_output_changed_after_arm = False
        self.actuator_output_changed_after_arm = False
        self.message_counts: dict[str, int] = {}
        self.first_timestamp: datetime | None = None
        self.last_timestamp: datetime | None = None

    def observe(self, record: dict[str, Any]) -> None:
        msg_type = record.get("message_type")
        payload = record.get("message", {}) or {}
        ts = parse_ts(record)
        if ts is not None:
            if self.first_timestamp is None:
                self.first_timestamp = ts
            self.last_timestamp = ts

        self.message_counts[msg_type] = self.message_counts.get(msg_type, 0) + 1

        if msg_type == "GPS_RAW_INT":
            sample = {
                "lat_deg": payload.get("lat", 0) / 1e7,
                "lon_deg": payload.get("lon", 0) / 1e7,
                "alt_m": payload.get("alt", 0) / 1000.0,
                "fix_type": payload.get("fix_type"),
            }
            if self.gps_raw_int_first is None:
                self.gps_raw_int_first = sample
            self.gps_raw_int_last = sample

        elif msg_type == "GLOBAL_POSITION_INT":
            sample = {
                "lat_deg": payload.get("lat", 0) / 1e7,
                "lon_deg": payload.get("lon", 0) / 1e7,
                "alt_m": payload.get("alt", 0) / 1000.0,
            }
            if self.global_position_int_first is None:
                self.global_position_int_first = sample
            self.global_position_int_last = sample

        elif msg_type == "HOME_POSITION" and self.home_position is None:
            self.home_position = {
                "lat_deg": payload.get("latitude", 0) / 1e7,
                "lon_deg": payload.get("longitude", 0) / 1e7,
                "alt_m": payload.get("altitude", 0) / 1000.0,
            }

        elif msg_type == "HEARTBEAT":
            base_mode = payload.get("base_mode", 0) or 0
            armed = bool(base_mode & MAV_MODE_FLAG_SAFETY_ARMED)
            if armed and self.armed_first_time is None and ts is not None:
                self.armed_first_time = ts
            if not armed and self.armed_first_time is not None:
                self.disarmed_seen_after_arm = True

        elif msg_type == "COMMAND_ACK":
            self.command_acks.append(
                {"command": payload.get("command"), "result": payload.get("result"), "timestamp_utc": record.get("timestamp_utc")}
            )

        elif msg_type == "STATUSTEXT":
            text = payload.get("text")
            if text and len(self.statustexts) < 200:
                self.statustexts.append(str(text))

        elif msg_type == "SERVO_OUTPUT_RAW":
            values = {k: v for k, v in payload.items() if k.startswith("servo") and k.endswith("_raw")}
            if self._servo_baseline is None:
                self._servo_baseline = values
            elif self.armed_first_time is not None and ts is not None and ts >= self.armed_first_time:
                for key, value in values.items():
                    baseline = self._servo_baseline.get(key)
                    if baseline is not None and abs(value - baseline) > DEFAULT_SERVO_PWM_DELTA:
                        self.servo_output_changed_after_arm = True

        elif msg_type == "ACTUATOR_OUTPUT_STATUS":
            values = list(payload.get("actuator", []) or [])
            if self._actuator_baseline is None:
                self._actuator_baseline = values
            elif self.armed_first_time is not None and ts is not None and ts >= self.armed_first_time:
                for baseline_v, current_v in zip(self._actuator_baseline, values):
                    if abs(current_v - baseline_v) > DEFAULT_ACTUATOR_DELTA:
                        self.actuator_output_changed_after_arm = True
                        break


def analyze_mavlink_log(path: Path) -> MavlinkFindings:
    findings = MavlinkFindings()
    for record in load_jsonl(path):
        findings.observe(record)
    return findings


def load_matlab_summary(path: Path) -> dict[str, Any] | None:
    if not path.exists():
        return None
    with path.open("r", encoding="utf-8") as stream:
        return json.load(stream)


def coordinate_checklist(findings: MavlinkFindings, matlab_summary: dict[str, Any] | None) -> dict[str, Any]:
    sources: dict[str, dict[str, float]] = {}
    if findings.gps_raw_int_first:
        sources["mavlink_gps_raw_int_first"] = findings.gps_raw_int_first
    if findings.global_position_int_first:
        sources["mavlink_global_position_int_first"] = findings.global_position_int_first
    if findings.home_position:
        sources["mavlink_home_position"] = findings.home_position
    matlab_gps = None
    if matlab_summary:
        matlab_gps = (matlab_summary.get("coordinates") or {}).get("gps_initial")
        if matlab_gps:
            sources["matlab_gps_initial"] = matlab_gps

    near_zero = {
        name: sample
        for name, sample in sources.items()
        if abs(sample.get("lat_deg", 0)) < DEFAULT_NEAR_ZERO_DEG and abs(sample.get("lon_deg", 0)) < DEFAULT_NEAR_ZERO_DEG
    }

    max_delta_deg = None
    if len(sources) >= 2:
        lats = [s["lat_deg"] for s in sources.values()]
        lons = [s["lon_deg"] for s in sources.values()]
        max_delta_deg = max(max(lats) - min(lats), max(lons) - min(lons))

    return {
        "sources": sources,
        "near_zero_zero_sources": list(near_zero.keys()),
        "max_pairwise_delta_deg": max_delta_deg,
        "coordinates_agree": (max_delta_deg is not None and max_delta_deg <= DEFAULT_COORD_AGREEMENT_DEG),
        "no_source_near_ocean_zero": len(near_zero) == 0,
    }


def actuator_checklist(findings: MavlinkFindings, matlab_summary: dict[str, Any] | None) -> dict[str, Any]:
    matlab_actuator = (matlab_summary or {}).get("actuator") or {}
    matlab_coords = (matlab_summary or {}).get("coordinates") or {}
    matlab_px4_controls = (matlab_summary or {}).get("px4_controls") or {}

    armed_detected = findings.armed_first_time is not None
    mavlink_actuator_changed = findings.servo_output_changed_after_arm or findings.actuator_output_changed_after_arm
    matlab_actuator_nonzero = matlab_actuator.get("actuator_nonzero_after_arm")
    matlab_position_changed = None
    displacement = matlab_coords.get("max_position_displacement_from_initial_m")
    if displacement is not None:
        matlab_position_changed = displacement > DEFAULT_POSITION_DELTA_M

    if not armed_detected:
        diagnosis = "No HEARTBEAT armed bit observed in MAVLink log -- vehicle was never armed during this session."
    elif not mavlink_actuator_changed and not matlab_px4_controls.get("nonzero_at_any_point"):
        diagnosis = (
            "PX4 reports armed but neither its own actuator/servo outputs nor the deserialized "
            "PX4 controls entering Simulink ever moved -- check PX4 mode/arming state, commander/nav "
            "state, mixer/output setup, or failsafe (plan section 9, case 3)."
        )
    elif mavlink_actuator_changed and matlab_actuator_nonzero is False:
        diagnosis = (
            "PX4 actuator output changed but Simulink's final actuator command bus "
            "(ActuatorCommandBus_final) stayed zero after arming -- check the HIL connector/wiring "
            "(plan section 9, case 1)."
        )
    elif matlab_actuator_nonzero and matlab_position_changed is False:
        diagnosis = (
            "Simulink received nonzero actuator commands after arming but plant position did not "
            "change -- check plant dynamics, motor mapping, arming logic, or ground/contact model "
            "(plan section 9, case 2)."
        )
    elif mavlink_actuator_changed and matlab_actuator_nonzero and matlab_position_changed:
        diagnosis = "PASS: PX4 commanded actuators, Simulink received them, and plant state changed."
    else:
        diagnosis = (
            "Insufficient evidence to place this run on the plan's decision tree -- matlab_summary.json "
            "may be missing/incomplete, or the run never reached an armed state. See individual fields."
        )

    return {
        "armed_detected": armed_detected,
        "armed_first_time_utc": findings.armed_first_time.isoformat() if findings.armed_first_time else None,
        "disarmed_seen_after_arm": findings.disarmed_seen_after_arm,
        "mavlink_servo_output_changed_after_arm": findings.servo_output_changed_after_arm,
        "mavlink_actuator_output_status_changed_after_arm": findings.actuator_output_changed_after_arm,
        "matlab_actuator_nonzero_after_arm": matlab_actuator_nonzero,
        "matlab_max_position_displacement_from_initial_m": displacement,
        "matlab_position_changed": matlab_position_changed,
        "matlab_px4_controls_nonzero_at_any_point": matlab_px4_controls.get("nonzero_at_any_point"),
        "diagnosis": diagnosis,
    }


def build_report(session_dir: Path) -> dict[str, Any]:
    mavlink_path = session_dir / "mavlink_raw_log.jsonl"
    matlab_path = session_dir / "matlab_summary.json"

    findings = analyze_mavlink_log(mavlink_path) if mavlink_path.exists() else MavlinkFindings()
    matlab_summary = load_matlab_summary(matlab_path)

    report = {
        "session_dir": str(session_dir),
        "mavlink_log_present": mavlink_path.exists(),
        "matlab_summary_present": matlab_summary is not None,
        "mavlink_message_counts": findings.message_counts,
        "mavlink_first_timestamp_utc": findings.first_timestamp.isoformat() if findings.first_timestamp else None,
        "mavlink_last_timestamp_utc": findings.last_timestamp.isoformat() if findings.last_timestamp else None,
        "coordinate_checklist": coordinate_checklist(findings, matlab_summary),
        "actuator_movement_checklist": actuator_checklist(findings, matlab_summary),
        "command_acks": findings.command_acks[-20:],
        "statustext_tail": findings.statustexts[-20:],
    }
    return report


def print_human_summary(report: dict[str, Any]) -> None:
    coord = report["coordinate_checklist"]
    act = report["actuator_movement_checklist"]

    print("=" * 70)
    print(f"HIL session report: {report['session_dir']}")
    print("=" * 70)
    print(f"MAVLink log present:  {report['mavlink_log_present']}")
    print(f"MATLAB summary present: {report['matlab_summary_present']}")
    print()
    print("-- Coordinate checklist (plan section 8) --")
    for name, sample in coord["sources"].items():
        print(f"  {name}: lat={sample.get('lat_deg')} lon={sample.get('lon_deg')} alt={sample.get('alt_m')}")
    print(f"  max pairwise delta (deg): {coord['max_pairwise_delta_deg']}")
    print(f"  coordinates agree (<= {DEFAULT_COORD_AGREEMENT_DEG} deg): {coord['coordinates_agree']}")
    print(f"  no source near (0,0): {coord['no_source_near_ocean_zero']}")
    if coord["near_zero_zero_sources"]:
        print(f"  WARNING sources near (0,0): {coord['near_zero_zero_sources']}")
    print()
    print("-- Actuator/movement checklist (plan section 9) --")
    print(f"  armed detected: {act['armed_detected']} (first at {act['armed_first_time_utc']})")
    print(f"  mavlink servo/actuator output changed after arm: "
          f"{act['mavlink_servo_output_changed_after_arm'] or act['mavlink_actuator_output_status_changed_after_arm']}")
    print(f"  matlab actuator nonzero after arm: {act['matlab_actuator_nonzero_after_arm']}")
    print(f"  matlab position changed: {act['matlab_position_changed']} "
          f"(displacement={act['matlab_max_position_displacement_from_initial_m']} m)")
    print()
    print("DIAGNOSIS:")
    print(f"  {act['diagnosis']}")
    print("=" * 70)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--session-dir", required=True, help="Session folder with mavlink_raw_log.jsonl / matlab_summary.json")
    parser.add_argument("--out", default=None, help="Output report path (default: <session-dir>/session_report.json)")
    args = parser.parse_args()

    session_dir = Path(args.session_dir).resolve()
    report = build_report(session_dir)

    out_path = Path(args.out).resolve() if args.out else session_dir / "session_report.json"
    out_path.write_text(json.dumps(report, indent=2, sort_keys=True), encoding="utf-8")

    print_human_summary(report)
    print(f"\nFull report written to: {out_path}")


if __name__ == "__main__":
    main()
