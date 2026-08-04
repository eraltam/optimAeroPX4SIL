"""Produce a machine-readable PX4 mission verdict from a SIL ULog."""

from __future__ import annotations

import argparse
import json
import math
from pathlib import Path

import numpy as np
from pyulog import ULog


def dataset(ulog: ULog, name: str, multi_id: int = 0):
    return next(
        (item.data for item in ulog.data_list if item.name == name and item.multi_id == multi_id),
        None,
    )


def finite_range(values) -> dict[str, float | None]:
    array = np.asarray(values, dtype=float)
    array = array[np.isfinite(array)]
    if not len(array):
        return {"min": None, "max": None}
    return {"min": float(np.min(array)), "max": float(np.max(array))}


def analyze(path: Path, expected_waypoints: int | None) -> dict:
    ulog = ULog(str(path))
    mission = dataset(ulog, "mission_result")
    status = dataset(ulog, "vehicle_status")
    land = dataset(ulog, "vehicle_land_detected")
    local = dataset(ulog, "vehicle_local_position")
    attitude = dataset(ulog, "vehicle_attitude")
    controller = dataset(ulog, "position_controller_status")
    airspeed = dataset(ulog, "airspeed_validated")
    if mission is None:
        raise RuntimeError("mission_result is missing from the ULog")

    reached = []
    last_reached = -1
    for index, value in enumerate(mission["seq_reached"]):
        sequence = int(value)
        if sequence > last_reached:
            reached.append(
                {"sequence": sequence, "time_s": float(mission["timestamp"][index] / 1e6)}
            )
            last_reached = sequence

    # Sequence zero is the NAV_TAKEOFF item. Navigation waypoints start at one.
    waypoint_sequences = [item["sequence"] for item in reached if item["sequence"] >= 1]
    finished = bool(np.any(mission["finished"]))
    failure = bool(np.any(mission["failure"]))
    armed = bool(status is not None and np.any(np.asarray(status["arming_state"]) == 2))
    landed_at_end = bool(land is not None and land["landed"][-1])
    expected_reached = expected_waypoints is None or len(waypoint_sequences) >= expected_waypoints
    waypoints_passed = armed and not failure and expected_reached
    passed = waypoints_passed and finished

    result = {
        "ulog": str(path.resolve()),
        "duration_s": float((ulog.last_timestamp - ulog.start_timestamp) / 1e6),
        "armed": armed,
        "finished": finished,
        "failure": failure,
        "landed_at_end": landed_at_end,
        "reached": reached,
        "navigation_waypoints_reached": len(waypoint_sequences),
        "expected_navigation_waypoints": expected_waypoints,
        "waypoints_passed": waypoints_passed,
        "passed": passed,
    }

    if controller is not None:
        result["navigation"] = {
            "waypoint_distance_m": finite_range(controller["wp_dist"]),
            "cross_track_error_m": finite_range(controller["xtrack_error"]),
            "last_waypoint_distance_m": float(controller["wp_dist"][-1]),
        }
    if local is not None:
        result["local_position"] = {
            "north_m": finite_range(local["x"]),
            "east_m": finite_range(local["y"]),
            "down_m": finite_range(local["z"]),
            "vertical_velocity_m_s": finite_range(local["vz"]),
        }
    if airspeed is not None:
        result["indicated_airspeed_m_s"] = finite_range(airspeed["indicated_airspeed_m_s"])
    if attitude is not None:
        q0, q1, q2, q3 = (np.asarray(attitude[f"q[{i}]"]) for i in range(4))
        roll = np.degrees(np.arctan2(2 * (q0 * q1 + q2 * q3), 1 - 2 * (q1**2 + q2**2)))
        pitch = np.degrees(np.arcsin(np.clip(2 * (q0 * q2 - q3 * q1), -1, 1)))
        result["attitude_deg"] = {"roll": finite_range(roll), "pitch": finite_range(pitch)}
    return result


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("ulog", type=Path)
    parser.add_argument("--expected-waypoints", type=int)
    parser.add_argument("--json", type=Path, help="also save the verdict to this file")
    args = parser.parse_args()
    result = analyze(args.ulog, args.expected_waypoints)
    output = json.dumps(result, indent=2)
    print(output)
    if args.json:
        args.json.write_text(output + "\n", encoding="utf-8")
    return 0 if result["passed"] else 2


if __name__ == "__main__":
    raise SystemExit(main())
