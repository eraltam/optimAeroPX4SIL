"""Run ONE static SiPhOG Allan-characterization session, end to end, for a single duration.

Per PLAN_STATIC_A1_REPEAT_SIPHOG_LONGDURATION.md Section 9/11 and the project owner's stated
preference: this is meant to be run BY HAND, one duration at a time (1h, then 3h, then 6h, then
10h, then 20h), not orchestrated unattended by an agent across all five. Run it, let it sit for
the requested duration, then hand the resulting `.ulg` (and this script's own session log) back
for analysis. It is safe to walk away during the capture -- the whole point of the "duration"
step is a long unattended wait; only the pre-flight and post-flight steps need nothing external.

Fails closed at every step (plan Section 11 stop conditions) -- it will refuse to start a long
capture on a wrong firmware hash, wrong parameters, or an absent/ambiguous SiPhOG channel, and it
will not report itself successful unless the qualification-style validation on the downloaded log
passes.

Usage:
    py -3.12 run_static_allan_session.py --duration-h 1 --session-id static_a1_siphog_1h
    py -3.12 run_static_allan_session.py --duration-h 3 --session-id static_a1_siphog_3h
    ... etc for 6, 10, 20

Always power off the bench yourself if this script exits early/crashes before reaching its own
`finally`-block power-off (it tries, but a hard Ctrl+C during the relay call itself could still
leave things powered) -- check the console output.
"""
from __future__ import annotations

import argparse
import re
import subprocess
import sys
import time
from pathlib import Path

import numpy as np
from pymavlink import mavutil
from pyulog import ULog

SCRIPT_DIR = Path(__file__).resolve().parent
sys.path.insert(0, str(SCRIPT_DIR))
sys.path.insert(0, str(SCRIPT_DIR / "mavlink_system"))

from resolve_cube_port import resolve_cube_port  # noqa: E402
from px4_int_param import get_param, param_float_to_int32  # noqa: E402
from anello_siphog_probe import MavlinkShell as ShellProbe  # noqa: E402
from ulog_session import (  # noqa: E402
    MavlinkShell as RecShell,
    ensure_logger_daemon_running,
    start_recording,
    stop_recording,
    download_log_id,
)
import power_off_sequence  # noqa: E402

EXPECTED_FIRMWARE_HASH = "388ced1441"
EXPECTED_PARAMS = {"SYS_AUTOSTART": 1003, "SENS_ANELLX3_CFG": 102, "SYS_HITL": 0}
SIPHOG_DEVTYPE = 0x13


def fail(msg: str) -> None:
    print(f"\nSTOP CONDITION: {msg}", file=sys.stderr)
    sys.exit(1)


def connect_mavlink(port: str, baud: int = 57600, timeout_s: float = 20.0):
    """DTR/RTS + self-announced-heartbeat connect (see plan Section 0.5 -- required once
    SYS_AUTOSTART=1003 is active, a passive wait_heartbeat() alone can hang forever)."""
    master = mavutil.mavlink_connection(port, baud=baud)
    master.port.dtr = True
    master.port.rts = True
    hb = None
    end = time.time() + timeout_s
    while hb is None and time.time() < end:
        master.mav.heartbeat_send(mavutil.mavlink.MAV_TYPE_GCS, mavutil.mavlink.MAV_AUTOPILOT_INVALID, 0, 0, 0)
        hb = master.wait_heartbeat(timeout=2)
    if hb is None:
        raise TimeoutError(f"no heartbeat on {port}")
    return master


def resolve_and_connect(attempts: int = 6, retry_wait_s: float = 3.0):
    """Resolve the Cube port and connect, retrying on the known port-drift/transient-serial
    failure modes (plan Section 0.3/0.5-0.6): the board can hop COM numbers for a few seconds
    right after a relay power-cycle (observed COM3<->COM4 within the same settle window), and a
    stale port handle can raise a Windows `ClearCommError`/`PermissionError` even though the
    device is healthy. Re-resolving fresh before every attempt (never reusing a port number from
    even a few seconds earlier) is the documented fix -- do this rather than fail an hours-long
    session over a few seconds of USB re-enumeration."""
    last_exc = None
    for attempt in range(1, attempts + 1):
        try:
            port = resolve_cube_port()
            print(f"resolve_and_connect: attempt {attempt}/{attempts}, resolved port = {port}")
            master = connect_mavlink(port)
            return port, master
        except Exception as exc:  # noqa: BLE001 -- deliberately broad, see docstring
            last_exc = exc
            print(f"resolve_and_connect: attempt {attempt}/{attempts} failed ({exc!r}), retrying in {retry_wait_s:.0f}s")
            time.sleep(retry_wait_s)
    fail(f"could not resolve+connect to the Cube after {attempts} attempts: {last_exc!r}")


def step_power_on() -> None:
    print("\n=== Step 1/9: relay power-on (ANELLO then Cube) ===")
    result = subprocess.run(
        [sys.executable, str(SCRIPT_DIR / "power_on_sequence.py"), "--use-reset"],
        cwd=str(SCRIPT_DIR),
    )
    if result.returncode != 0:
        fail("power_on_sequence.py failed -- check wiring/relay before proceeding.")


def step_verify_firmware(master) -> None:
    print("\n=== Step 3/9: verify live firmware hash ===")
    master.mav.command_long_send(
        master.target_system, master.target_component,
        mavutil.mavlink.MAV_CMD_REQUEST_MESSAGE, 0,
        mavutil.mavlink.MAVLINK_MSG_ID_AUTOPILOT_VERSION, 0, 0, 0, 0, 0, 0,
    )
    msg = master.recv_match(type="AUTOPILOT_VERSION", blocking=True, timeout=10)
    if msg is None:
        fail("no AUTOPILOT_VERSION response -- cannot verify firmware identity.")
    raw = bytes(msg.flight_custom_version)
    live_hash = raw[::-1].rstrip(b"\x00").hex()
    print(f"live hash = {live_hash}, expected prefix = {EXPECTED_FIRMWARE_HASH}")
    if not live_hash or not live_hash.startswith(EXPECTED_FIRMWARE_HASH.lower()):
        fail(f"live firmware hash {live_hash!r} does not match expected {EXPECTED_FIRMWARE_HASH!r}.")


def step_verify_params(master) -> None:
    print("\n=== Step 4/9: verify post-flash parameters ===")
    for name, expected in EXPECTED_PARAMS.items():
        pm = get_param(master, name)
        if pm is None:
            fail(f"could not read {name}.")
        value = param_float_to_int32(pm.param_value)
        print(f"{name} = {value} (expected {expected})")
        if value != expected:
            fail(f"{name}={value}, expected {expected}. Do not proceed with a long capture.")


def step_verify_siphog(port: str) -> int:
    print("\n=== Step 5/9: verify SiPhOG channel (devtype 0x13) and driver health ===")
    shell = ShellProbe(port, 57600)
    shell.write("\n")
    time.sleep(1.0)
    shell.read_for(1.0)

    status = shell.run("anello_x3 status", 3.0)
    print(status)
    if "com_err: 0" not in status or "poll error: 0" not in status or "checksum bad: 0" not in status:
        shell.close()
        fail("anello_x3 status reports nonzero errors -- do not proceed with a long capture.")

    found_instance = None
    for i in range(4):
        out = shell.run(f"listener vehicle_imu -i {i} -n 1", 2.5)
        if "Type: 0x13" in out or "Type: 0x13)" in out.replace(" ", ""):
            found_instance = i
            break
    shell.close()
    if found_instance is None:
        fail("SiPhOG devtype 0x13 not found at any vehicle_imu instance 0-3.")
    print(f"SiPhOG (0x13) confirmed at vehicle_imu instance {found_instance}")
    # NOTE: this is the vehicle_imu instance index for THIS boot only (plan Section 4 -- never
    # hardcode it across boots). sensor_gyro's own instance numbering is expected to match
    # vehicle_imu's (same underlying driver instantiation), used below for the warm-up temperature
    # read, but is re-resolved fresh every session, never assumed from a prior run.
    return found_instance


def step_thermal_warmup(port: str, warmup_s: float, siphog_instance: int) -> None:
    print(f"\n=== Step 6/9: thermal warm-up wait ({warmup_s:.0f}s) ===")
    print(
        "NOTE: the numeric warm-up acceptance threshold is not yet frozen (plan Open Decision, "
        "Section 14 item 4-adjacent) -- this is a fixed wait with temperature logged for the "
        "record, not a pass/fail gate. Do not treat this duration as scientifically justified."
    )
    shell = ShellProbe(port, 57600)
    shell.write("\n")
    time.sleep(1.0)
    shell.read_for(1.0)
    end = time.time() + warmup_s
    temps = []
    while time.time() < end:
        out = shell.run(f"listener sensor_gyro -i {siphog_instance} -n 1", 2.0)
        m = re.search(r"temperature:\s*([\d.\-]+)", out)
        if m:
            t = float(m.group(1))
            temps.append(t)
            print(f"  t-{end - time.time():.0f}s remaining, SiPhOG temp={t:.2f}C")
        time.sleep(max(0.0, min(10.0, end - time.time())))
    shell.close()
    if temps:
        print(f"warm-up temperature range: {min(temps):.2f}C - {max(temps):.2f}C")


def step_capture(port: str, duration_s: float, output_dir: Path, session_id: str) -> dict:
    print(f"\n=== Step 7/9: capture ULog for {duration_s:.0f}s ({duration_s/3600:.2f}h) ===")
    shell = RecShell(port, 57600)
    ensure_logger_daemon_running(shell)
    before_ids = set(shell.list_log_ids().keys())
    print(f"before capture: {len(before_ids)} logs on board")

    file_path = start_recording(shell)
    t_start = time.time()
    print(f"recording started ({file_path}); capturing for {duration_s:.0f}s -- safe to walk away")

    # Sleep in chunks so a Ctrl+C lands promptly rather than blocking on one giant sleep.
    end = t_start + duration_s
    while time.time() < end:
        time.sleep(min(60.0, end - time.time()))

    stop_recording(shell)
    t_end = time.time()
    print("recording stopped (confirmed via logger status)")

    time.sleep(2.0)
    after = shell.list_log_ids()
    after_ids = set(after.keys())
    new_ids = after_ids - before_ids
    shell.close()

    if len(new_ids) != 1:
        fail(f"expected exactly 1 new log id, found {len(new_ids)}: {sorted(new_ids)}")
    new_id = next(iter(new_ids))
    reported_size = after[new_id]["size"]
    print(f"new log id = {new_id}, board-reported size = {reported_size} bytes")

    output_path = output_dir / f"{session_id}_log{new_id}.ulg"
    download_log_id(port, 57600, new_id, output_path)
    print(f"downloaded to {output_path}")

    return {
        "log_id": new_id, "output_path": output_path,
        "requested_duration_s": duration_s, "actual_duration_s": t_end - t_start,
    }


def step_validate(ulog_path: Path, requested_duration_s: float) -> bool:
    print("\n=== Step 8/9: validate downloaded log ===")
    u = ULog(str(ulog_path))
    duration_s = (u.last_timestamp - u.start_timestamp) / 1e6
    print(f"ULog duration: {duration_s:.1f}s (requested {requested_duration_s:.0f}s)")

    siphog = None
    for d in u.data_list:
        if d.name != "vehicle_imu":
            continue
        gyro_ids = set(int(x) for x in d.data["gyro_device_id"])
        devtypes = {(gid >> 16) & 0xFF for gid in gyro_ids}
        if SIPHOG_DEVTYPE in devtypes:
            siphog = d
    if siphog is None:
        print("FAIL: SiPhOG devtype 0x13 not found in downloaded vehicle_imu data.")
        return False

    data = siphog.data
    ts = data["timestamp"]
    dts = np.diff(ts)
    monotonic = bool(np.all(dts > 0))
    gap_threshold_us = 3 * dts.mean()
    gaps = dts[dts > gap_threshold_us]
    da_dt = data["delta_angle_dt"]
    invalid_da_dt = int(np.sum(da_dt <= 0))
    da_keys = [k for k in data.keys() if k.startswith("delta_angle[")]
    nonfinite = sum(int(np.sum(~np.isfinite(data[k]))) for k in da_keys)
    clip_nonzero = int(np.sum(data["delta_angle_clipping"] != 0))
    rate_hz = 1e6 / dts.mean()

    checks = {
        "duration within 60s of requested": abs(duration_s - requested_duration_s) < 60.0,
        "timestamps monotonic": monotonic,
        "rate within tolerance of 100 Hz": abs(rate_hz - 100.0) < 5.0,
        "zero gaps > 3x mean interval": len(gaps) == 0,
        "zero invalid delta_angle_dt": invalid_da_dt == 0,
        "zero non-finite samples": nonfinite == 0,
        "zero clipping": clip_nonzero == 0,
    }
    for name, ok in checks.items():
        print(f"  {'PASS' if ok else 'FAIL'}: {name}")
    return all(checks.values())


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--duration-h", type=float, required=True)
    parser.add_argument("--session-id", required=True)
    parser.add_argument("--warmup-s", type=float, default=300.0)
    parser.add_argument("--output-dir", type=Path, default=None)
    args = parser.parse_args()

    output_dir = args.output_dir or (SCRIPT_DIR / args.session_id)
    output_dir.mkdir(parents=True, exist_ok=True)
    duration_s = args.duration_h * 3600.0

    step_power_on()

    print("\n=== Step 2/9: resolve Cube port and connect (with drift/transient-error retry) ===")
    port, master = resolve_and_connect()

    step_verify_firmware(master)
    step_verify_params(master)
    master.close()

    siphog_instance = step_verify_siphog(port)
    step_thermal_warmup(port, args.warmup_s, siphog_instance)

    result = None
    validated = False
    try:
        result = step_capture(port, duration_s, output_dir, args.session_id)
        validated = step_validate(result["output_path"], duration_s)
    finally:
        print("\n=== Step 9/9: relay power-off (Cube then ANELLO) ===")
        power_off_sequence.power_off()

    print("\n" + "=" * 70)
    if result is not None and validated:
        print(f"SESSION COMPLETE AND VALIDATED: {result['output_path']}")
        print("Hand this .ulg back for Phase 6 Allan-deviation analysis.")
    else:
        print("SESSION DID NOT VALIDATE CLEANLY -- do not mark this duration complete.")
        if result is not None:
            print(f"(log was still downloaded to {result['output_path']} for inspection)")
        sys.exit(1)


if __name__ == "__main__":
    main()
