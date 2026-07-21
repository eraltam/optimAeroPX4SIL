"""Truly passive HITL telemetry capture -- no MAVSDK, no command traffic at all.

For HITL_SERIAL_TIMING_HYPOTHESIS_TEST_PLAN.md Test 1/2/3/4: main.py always starts MAVSDK and
runs the instructor sequence (connect, dump_params, print_basic_telemetry at minimum), which is
real traffic on the wire even with auto_arm/auto_takeoff/auto_mission all false. This script
reuses the same, already-proven PymavlinkListener class (listener_pymavlink.py) in read-only mode
-- forward_to_mavsdk and relay_commands_to_simulink both forced off regardless of config.yaml, so
nothing sends anything back to PX4. Confirms a heartbeat before returning, so callers know capture
is actually live before starting the Simulink run (the sequencing bug that produced an empty
mavlink_raw_log.jsonl in passive_passthrough_exec_01).

Usage: py -3.12 passive_listener.py --duration 300 --session-id passive_passthrough_02
"""

from __future__ import annotations

import argparse
import copy
from pathlib import Path

import yaml

from listener_pymavlink import PymavlinkListener
from session_paths import resolve_session_dir


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--config", default="config.yaml")
    parser.add_argument("--duration", type=float, required=True, help="Capture duration in seconds")
    parser.add_argument("--session-id", default=None, help="HILDiagnostics/logs/session_<id>")
    parser.add_argument("--session-dir", default=None)
    args = parser.parse_args()

    script_dir = Path(__file__).resolve().parent
    config_path = Path(args.config)
    if not config_path.is_absolute():
        config_path = script_dir / config_path
    with config_path.open("r", encoding="utf-8") as f:
        config = yaml.safe_load(f) or {}

    # Force passive: no forwarding to MAVSDK, no command relay back to Simulink, regardless of
    # what config.yaml says -- this script's whole point is zero outbound traffic.
    config = copy.deepcopy(config)
    config.setdefault("listener", {})["forward_to_mavsdk"] = False
    config["listener"]["relay_commands_to_simulink"] = False

    base_dir = resolve_session_dir(
        script_dir, session_dir=args.session_dir, session_id=args.session_id, fallback_dir=script_dir
    )
    base_dir.mkdir(parents=True, exist_ok=True)
    print(f"passive_listener: logging to {base_dir}")

    listener = PymavlinkListener(config, base_dir=base_dir)
    connect_timeout_s = float(config.get("vehicle", {}).get("connect_timeout_s", 180.0))
    listener.connect(timeout_s=connect_timeout_s)
    print(f"passive_listener: confirmed live, capturing for {args.duration:.0f}s (read-only, no MAVSDK)")

    import time

    start = time.time()
    count = 0
    while time.time() - start < args.duration:
        if listener.listen_once(timeout_s=0.5):
            count += 1
    listener.close()
    print(f"passive_listener: done, {count} logged messages")


if __name__ == "__main__":
    main()
