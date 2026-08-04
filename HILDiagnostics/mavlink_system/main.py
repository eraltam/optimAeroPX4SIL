"""Run MAVSDK as instructor and pymavlink as read-only monitor."""

from __future__ import annotations

import argparse
import asyncio
import re
import shutil
import socket
import subprocess
import sys
import threading
import traceback
from pathlib import Path
from typing import Any, TextIO

import yaml

from controller_mavsdk import MavsdkInstructor
from safety import print_safety_warning
from session_paths import resolve_session_dir


def _extract_udp_port(address: str, default: int = 14540) -> int:
    """Pull the port out of a mavsdk 'udp://:14540'-style address string."""
    match = re.search(r":(\d+)\s*$", address)
    return int(match.group(1)) if match else default


def free_stale_udp_port(port: int) -> None:
    """Kill whatever process is already bound to `port`, if any.

    Self-healing for a recurring failure mode: a previous session's `mavsdk_server.exe` child
    process (spawned internally by mavsdk-python's `System()`) can survive an abrupt harness
    kill (e.g. the WSL PX4 process being force-killed out from under it, or a Ctrl+C that didn't
    reach the child) and keep squatting on the MAVSDK UDP port, causing the *next* session's
    `System.connect()` to fail immediately with "bind error: No error" / "Connection failed:
    Bind error" -- observed repeatedly across f22run19/run20 starts. No `psutil` dependency
    available in this environment, so this shells out to `netstat`/`taskkill` (Windows-only,
    matches the rest of this harness's Windows-hosted assumptions -- same as the WSL-relay split
    used elsewhere in this repo). Best-effort: any failure here is non-fatal, the normal
    connect_timeout_s-bounded connect attempt below will just fail with its usual clear error if
    this doesn't actually clear the port.
    """
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as probe:
            probe.bind(("0.0.0.0", port))
        return  # port was free
    except OSError:
        pass

    try:
        result = subprocess.run(
            ["netstat", "-ano", "-p", "UDP"], capture_output=True, text=True, timeout=10
        )
    except Exception as exc:  # noqa: BLE001 - best-effort cleanup, never block the real connect attempt
        print(f"main: could not enumerate UDP sockets to clear port {port}: {exc}")
        return

    pids: set[str] = set()
    for line in result.stdout.splitlines():
        parts = line.split()
        if len(parts) >= 4 and parts[0].upper() == "UDP" and parts[1].endswith(f":{port}"):
            pids.add(parts[-1])

    for pid in pids:
        print(f"main: port {port} still bound by PID {pid} from a prior session -- killing it")
        try:
            subprocess.run(["taskkill", "/PID", pid, "/F"], capture_output=True, timeout=10)
        except Exception as exc:  # noqa: BLE001 - see docstring
            print(f"main: could not kill stale PID {pid}: {exc}")


class _Tee:
    """Mirror writes to multiple streams (used to log console output to a file)."""

    def __init__(self, *streams: TextIO) -> None:
        self._streams = streams

    def write(self, data: str) -> int:
        for stream in self._streams:
            stream.write(data)
        return len(data)

    def flush(self) -> None:
        for stream in self._streams:
            stream.flush()


def load_config(path: Path) -> dict[str, Any]:
    with path.open("r", encoding="utf-8") as stream:
        config = yaml.safe_load(stream) or {}
    return config


def resolve_mission_path(config: dict[str, Any], config_dir: Path) -> Path | None:
    mission_file = config.get("vehicle", {}).get("mission_file")
    if not mission_file:
        return None
    mission_path = Path(str(mission_file))
    if not mission_path.is_absolute():
        mission_path = config_dir / mission_path
    return mission_path


async def run(config: dict[str, Any], base_dir: Path, config_dir: Path) -> None:
    print_safety_warning(config)

    mavsdk_address = config.get("vehicle", {}).get("mavsdk_address", "udp://:14540")
    free_stale_udp_port(_extract_udp_port(mavsdk_address))

    listener = None
    listener_thread = None
    if config.get("listener", {}).get("enabled", True):
        try:
            from listener_pymavlink import PymavlinkListener
        except ModuleNotFoundError as exc:
            if exc.name != "pymavlink":
                raise
            print("pymavlink: dependency not installed; raw MAVLink listener disabled")
        else:
            listener = PymavlinkListener(config, base_dir=base_dir)

            def listener_main() -> None:
                try:
                    # Default of 30s (the method's own default) is too tight in practice: the
                    # HITL Simulink session needs real time to compile/initialize before it ever
                    # starts producing HEARTBEAT traffic, and that startup window is often
                    # started independently of this process (e.g. a human or another tool
                    # launching the Simulink run around the same time, not perfectly
                    # synchronized). Reuses vehicle.connect_timeout_s (already used for MAVSDK's
                    # own connect, default 60s) rather than adding a separate config key.
                    pymavlink_connect_timeout_s = float(
                        config.get("vehicle", {}).get("connect_timeout_s", 60.0)
                    )
                    listener.connect(timeout_s=pymavlink_connect_timeout_s)
                    listener.listen_forever()
                except Exception as exc:  # noqa: BLE001 - background thread must report and exit cleanly.
                    print(f"pymavlink: listener stopped with error: {exc}")
                    traceback.print_exc()

            listener_thread = threading.Thread(target=listener_main, name="pymavlink-listener", daemon=True)
            listener_thread.start()

    instructor = MavsdkInstructor(config, config_dir=config_dir, base_dir=base_dir)
    try:
        await instructor.connect()
        # Start the mission immediately after connection. A full parameter
        # enumeration takes several wall-clock minutes; with synchronous
        # JSBSim that allowed the unarmed fixed-wing plant to roll more than
        # a kilometre before mission upload. Preserve the snapshot, but take
        # it after the time-critical flight sequence.
        await instructor.run_demo_sequence()

        try:
            await instructor.dump_params(base_dir / "px4_params_before.txt")
            await instructor.dump_params(base_dir / "px4_params_after.txt")
        except Exception as exc:  # noqa: BLE001 - param dump must not abort the session.
            print(f"MAVSDK: could not dump post-run parameter snapshots: {exc}")
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
        help="Directory for logs, or 'latest'/'auto' for the newest MATLAB session",
    )
    parser.add_argument(
        "--session-id",
        default=None,
        help="Use HILDiagnostics/logs/session_<id>, e.g. --session-id 1",
    )
    args = parser.parse_args()

    script_dir = Path(__file__).resolve().parent
    config_path = Path(args.config)
    if not config_path.is_absolute():
        config_path = script_dir / config_path
    config = load_config(config_path)

    try:
        base_dir = resolve_session_dir(
            script_dir,
            session_dir=args.session_dir,
            session_id=args.session_id,
            fallback_dir=script_dir,
        )
    except (FileNotFoundError, ValueError) as exc:
        parser.error(str(exc))
    base_dir.mkdir(parents=True, exist_ok=True)

    console_log_path = base_dir / "console.log"
    console_log_file = console_log_path.open("w", encoding="utf-8")
    original_stdout, original_stderr = sys.stdout, sys.stderr
    sys.stdout = _Tee(original_stdout, console_log_file)
    sys.stderr = _Tee(original_stderr, console_log_file)
    print(f"main: console output is being logged to {console_log_path}")

    if args.session_dir is None and args.session_id is None and base_dir != script_dir:
        print(f"main: using latest MATLAB session directory: {base_dir}")
    shutil.copy2(config_path, base_dir / "run_config.yaml")
    mission_path = resolve_mission_path(config, config_path.parent)
    if mission_path is not None and mission_path.exists():
        shutil.copy2(mission_path, base_dir / "run_mission.yaml")

    try:
        try:
            asyncio.run(run(config, base_dir=base_dir, config_dir=config_path.parent))
        except KeyboardInterrupt:
            print("main: Ctrl+C received")
    finally:
        sys.stdout, sys.stderr = original_stdout, original_stderr
        console_log_file.close()


if __name__ == "__main__":
    main()
