"""Standalone ESTIMATOR_STATUS/STATUSTEXT logger for the SITL-vs-HITL comparison test.

Runs inside WSL (same network namespace as PX4 SITL) and connects to the "Normal" GCS-facing
MAVLink instance PX4 SITL starts by default (udp:127.0.0.1:14550) -- these instances only
broadcast on localhost unless MAV_{i}_BROADCAST=1 is set, so this must run inside WSL, not on
the Windows host, to actually receive anything (see SITL_VS_HITL_ESTIMATOR_COMPARISON.md).

Usage: python3 sitl_estimator_listener.py --duration 320
"""

from __future__ import annotations

import argparse
import json
import time

from pymavlink import mavutil


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--address", default="udpin:127.0.0.1:14550")
    parser.add_argument("--duration", type=float, default=320.0)
    parser.add_argument("--out", default="sitl_estimator_status.jsonl")
    args = parser.parse_args()

    print(f"connecting to {args.address}")
    master = mavutil.mavlink_connection(args.address)
    hb = master.wait_heartbeat(timeout=60)
    if hb is None:
        raise SystemExit("no heartbeat received within 60s")
    print(f"heartbeat from system={master.target_system} component={master.target_component}")

    start = time.time()
    count = 0
    with open(args.out, "w", encoding="utf-8") as f:
        while time.time() - start < args.duration:
            msg = master.recv_match(
                type=["ESTIMATOR_STATUS", "STATUSTEXT", "COMMAND_ACK"], blocking=True, timeout=1.0
            )
            if msg is None:
                continue
            record = {"t_wall": time.time(), "type": msg.get_type(), "data": msg.to_dict()}
            f.write(json.dumps(record) + "\n")
            f.flush()
            count += 1
            if msg.get_type() == "STATUSTEXT":
                print(f"STATUSTEXT: {msg.to_dict().get('text')}")

    print(f"done, {count} messages captured to {args.out}")


if __name__ == "__main__":
    main()
