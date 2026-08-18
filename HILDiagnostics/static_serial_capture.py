"""Direct-serial, read-only static IMU capture for Block A1 (no Simulink required).

The existing mavlink_system/ tooling (passive_listener.py, main.py) assumes a full HIL session:
PymavlinkListener connects to a UDP port (vehicle.pymavlink_address, default
"udpin:0.0.0.0:14550" in config.yaml) that only Simulink's running "PX4 HITL Interface" relays
serial traffic onto. Without Simulink in the loop, nothing ever arrives there and the listener
hangs until connect_timeout_s (600s in config.yaml) and then fails -- confirmed directly: a
passive_listener.py smoke test hung for 10+ minutes with zero output and no session directory ever
created, because it was waiting on a UDP relay that was never going to appear.

That UDP-relay design is correct for full HIL sessions, but wrong for a standalone static
noise-characterization bench test (Block A1 of the validation plan), where the goal is
specifically to record the real ANELLO IMU through PX4 WITHOUT Simulink running at all.

This script instead connects directly over serial (same approach as reboot_autopilot.py: a plain
pymavlink mavutil.mavlink_connection(port, baud=...)), confirms a heartbeat, then passively logs
whichever MAVLink messages PX4 streams by default over that link (message type is recorded
per-line so post-hoc analysis can pick whichever ended up carrying real IMU data -- HIGHRES_IMU /
SCALED_IMU* / RAW_IMU are the usual candidates, but the actual stream config on this board hasn't
been characterized yet) to a timestamped JSONL file, for a configurable duration. Sends nothing
else -- no arm, no parameter writes, no mode changes, matching the same "truly passive" intent as
passive_listener.py.

Usage:
    py -3.12 static_serial_capture.py --duration 60 --session-id static_a1_smoketest_02
    py -3.12 static_serial_capture.py --duration 3600 --session-id static_a1_run_01
    py -3.12 static_serial_capture.py --duration 60 --all-types --session-id probe_stream_types
"""

from __future__ import annotations

import argparse
import json
import sys
import time
from pathlib import Path

from pymavlink import mavutil

LOGS_ROOT = Path(r"C:\AS\optimAeroPX4SIL\HILDiagnostics\logs")

IMU_RELEVANT_TYPES = {
    "HEARTBEAT",
    "SYS_STATUS",
    "HIGHRES_IMU",
    "SCALED_IMU",
    "SCALED_IMU2",
    "SCALED_IMU3",
    "RAW_IMU",
    "ATTITUDE",
    "ATTITUDE_QUATERNION",
    "SCALED_PRESSURE",
}


def main() -> None:
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument("--port", default="COM4")
    parser.add_argument("--baud", type=int, default=921600)
    parser.add_argument("--duration", type=float, required=True, help="Capture duration in seconds")
    parser.add_argument("--session-id", required=True)
    parser.add_argument("--connect-timeout-s", type=float, default=30.0)
    parser.add_argument(
        "--all-types",
        action="store_true",
        help="Log every message type received, not just IMU_RELEVANT_TYPES. Useful the first time, "
        "to see what this board actually streams over this link before narrowing down.",
    )
    args = parser.parse_args()

    session_dir = LOGS_ROOT / f"session_{args.session_id}"
    session_dir.mkdir(parents=True, exist_ok=True)
    log_path = session_dir / "static_serial_capture.jsonl"

    print(
        f"static_serial_capture: connecting directly to {args.port} @ {args.baud} baud "
        "(no Simulink, no UDP relay)"
    )
    master = mavutil.mavlink_connection(args.port, baud=args.baud)
    hb = master.wait_heartbeat(timeout=args.connect_timeout_s)
    if hb is None:
        print(
            f"static_serial_capture: FAILED, no heartbeat on {args.port} within "
            f"{args.connect_timeout_s:.0f}s",
            file=sys.stderr,
        )
        sys.exit(1)
    print(
        f"static_serial_capture: heartbeat OK (system={master.target_system} "
        f"component={master.target_component})"
    )
    print(
        f"static_serial_capture: logging to {log_path}, capturing for {args.duration:.0f}s, "
        "read-only (nothing sent)"
    )

    seen_types: dict[str, int] = {}
    count = 0
    start = time.time()
    with log_path.open("w", encoding="utf-8") as f:
        while time.time() - start < args.duration:
            msg = master.recv_match(blocking=True, timeout=0.5)
            if msg is None:
                continue
            msg_type = msg.get_type()
            if msg_type == "BAD_DATA":
                continue
            if not args.all_types and msg_type not in IMU_RELEVANT_TYPES:
                continue
            record = msg.to_dict()
            record["_mavlink_type"] = msg_type
            record["_recv_wall_time_s"] = time.time()
            f.write(json.dumps(record, default=str) + "\n")
            count += 1
            seen_types[msg_type] = seen_types.get(msg_type, 0) + 1

    master.close()
    print(f"static_serial_capture: done, {count} messages logged")
    print("static_serial_capture: message type counts:")
    for t, n in sorted(seen_types.items(), key=lambda kv: -kv[1]):
        print(f"  {t}: {n}")


if __name__ == "__main__":
    main()
