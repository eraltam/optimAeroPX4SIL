"""Run the pymavlink listener without MAVSDK forwarding or command relay.

This is the passive observer path used by the HITL serial timing hypothesis
tests. It records the selected MAVLink messages to the session directory but
does not forward telemetry to MAVSDK and does not relay any commands back to
Simulink.
"""

from __future__ import annotations

import argparse
import time
from pathlib import Path
from typing import Any

import yaml

from listener_pymavlink import PymavlinkListener
from session_paths import resolve_session_dir


def load_config(path: Path) -> dict[str, Any]:
    with path.open("r", encoding="utf-8") as stream:
        return yaml.safe_load(stream) or {}


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--config", default="config.yaml", help="Path to YAML config")
    parser.add_argument(
        "--session-dir",
        default=None,
        help="Directory for logs, or 'latest'/'auto' for the newest MATLAB session",
    )
    parser.add_argument(
        "--session-id",
        default=None,
        help="Use HILDiagnostics/logs/session_<id>, e.g. --session-id passive_01",
    )
    parser.add_argument("--duration-s", type=float, default=360.0)
    parser.add_argument("--connect-timeout-s", type=float, default=None)
    args = parser.parse_args()

    script_dir = Path(__file__).resolve().parent
    config_path = Path(args.config)
    if not config_path.is_absolute():
        config_path = script_dir / config_path

    config = load_config(config_path)
    config.setdefault("listener", {})
    config["listener"]["enabled"] = True
    config["listener"]["forward_to_mavsdk"] = False
    config["listener"]["relay_commands_to_simulink"] = False
    config["listener"].setdefault("log_file", "mavlink_raw_log.jsonl")

    base_dir = resolve_session_dir(
        script_dir,
        session_dir=args.session_dir,
        session_id=args.session_id,
        fallback_dir=script_dir,
    )
    base_dir.mkdir(parents=True, exist_ok=True)

    timeout_s = args.connect_timeout_s
    if timeout_s is None:
        timeout_s = float(config.get("vehicle", {}).get("connect_timeout_s", 180.0))

    listener = PymavlinkListener(config, base_dir=base_dir)
    try:
        listener.connect(timeout_s=timeout_s)
        deadline = time.monotonic() + args.duration_s
        while time.monotonic() < deadline:
            listener.listen_once(timeout_s=0.5)
    finally:
        listener.stop()
        listener.close()


if __name__ == "__main__":
    main()
