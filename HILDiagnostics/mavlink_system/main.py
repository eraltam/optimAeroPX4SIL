"""Run MAVSDK as instructor and pymavlink as read-only monitor."""

from __future__ import annotations

import argparse
import asyncio
import shutil
import threading
from pathlib import Path
from typing import Any

import yaml

from controller_mavsdk import MavsdkInstructor
from listener_pymavlink import PymavlinkListener
from safety import print_safety_warning


def load_config(path: Path) -> dict[str, Any]:
    with path.open("r", encoding="utf-8") as stream:
        config = yaml.safe_load(stream) or {}
    return config


async def run(config: dict[str, Any], base_dir: Path) -> None:
    print_safety_warning(config)

    listener = None
    listener_thread = None
    if config.get("listener", {}).get("enabled", True):
        listener = PymavlinkListener(config, base_dir=base_dir)

        def listener_main() -> None:
            try:
                listener.connect()
                listener.listen_forever()
            except Exception as exc:  # noqa: BLE001 - background thread must report and exit cleanly.
                print(f"pymavlink: listener stopped with error: {exc}")

        listener_thread = threading.Thread(target=listener_main, name="pymavlink-listener", daemon=True)
        listener_thread.start()

    instructor = MavsdkInstructor(config)
    try:
        await instructor.connect()
        try:
            await instructor.dump_params(base_dir / "px4_params_before.txt")
        except Exception as exc:  # noqa: BLE001 - param dump must not abort the session.
            print(f"MAVSDK: could not dump params before run: {exc}")

        await instructor.run_demo_sequence()

        try:
            await instructor.dump_params(base_dir / "px4_params_after.txt")
        except Exception as exc:  # noqa: BLE001 - param dump must not abort the session.
            print(f"MAVSDK: could not dump params after run: {exc}")
    finally:
        if listener is not None:
            listener.stop()
        if listener_thread is not None:
            listener_thread.join(timeout=2.0)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--config", default="config.yaml", help="Path to YAML config")
    parser.add_argument(
        "--session-dir",
        default=None,
        help="Optional directory for logs and a copied run_config.yaml",
    )
    args = parser.parse_args()

    script_dir = Path(__file__).resolve().parent
    config_path = Path(args.config)
    if not config_path.is_absolute():
        config_path = script_dir / config_path
    config = load_config(config_path)

    base_dir = Path(args.session_dir).resolve() if args.session_dir else script_dir
    base_dir.mkdir(parents=True, exist_ok=True)
    shutil.copy2(config_path, base_dir / "run_config.yaml")

    try:
        asyncio.run(run(config, base_dir=base_dir))
    except KeyboardInterrupt:
        print("main: Ctrl+C received")


if __name__ == "__main__":
    main()
