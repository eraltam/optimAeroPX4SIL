"""Read-only pymavlink listener for low-level MAVLink debugging."""

from __future__ import annotations

import time
from pathlib import Path
from typing import Any, Iterable

from pymavlink import mavutil

from logger import JsonlLogger
from safety import confirm_pymavlink_commands_allowed


class PymavlinkListener:
    """Listen to selected raw MAVLink messages and write them to JSONL."""

    def __init__(self, config: dict[str, Any], base_dir: str | Path | None = None) -> None:
        self.config = config
        self.listener_config = config.get("listener", {})
        self.vehicle_config = config.get("vehicle", {})
        self.address = self.vehicle_config.get("pymavlink_address", "udpin:0.0.0.0:14550")
        self.message_types = set(self.listener_config.get("message_types", []))
        log_file = Path(self.listener_config.get("log_file", "logs/mavlink_raw_log.jsonl"))
        self.log_path = log_file if log_file.is_absolute() else Path(base_dir or ".") / log_file
        self.master = None
        self.logger: JsonlLogger | None = None
        self._stop = False

    def connect(self, timeout_s: float = 30.0) -> None:
        print(f"pymavlink: connecting to {self.address}")
        self.master = mavutil.mavlink_connection(self.address)
        heartbeat = self.master.wait_heartbeat(timeout=timeout_s)
        if heartbeat is None:
            raise TimeoutError(f"pymavlink: no HEARTBEAT on {self.address} within {timeout_s}s")
        print(
            "pymavlink: heartbeat from "
            f"system={self.master.target_system} component={self.master.target_component}"
        )
        self.logger = JsonlLogger(self.log_path)
        self.logger.write("LISTENER_CONNECTED", {"address": self.address})

    def listen_once(self, timeout_s: float = 1.0) -> bool:
        if self.master is None:
            raise RuntimeError("pymavlink listener is not connected")
        msg = self.master.recv_match(blocking=True, timeout=timeout_s)
        if msg is None:
            return False

        msg_type = msg.get_type()
        if not self.message_types or msg_type in self.message_types:
            payload = msg.to_dict()
            print(_format_message(msg_type, payload))
            if self.logger is not None:
                self.logger.write(msg_type, payload)
        return True

    def listen_forever(self) -> None:
        print("pymavlink: listener running")
        try:
            while not self._stop:
                self.listen_once(timeout_s=0.5)
        except KeyboardInterrupt:
            print("pymavlink: Ctrl+C received")
        finally:
            self.close()

    def stop(self) -> None:
        self._stop = True

    def close(self) -> None:
        if self.logger is not None:
            self.logger.close()
            self.logger = None
        if self.master is not None:
            try:
                self.master.close()
            finally:
                self.master = None
        print("pymavlink: listener closed")

    def send_px4_shell_command(self, command: str) -> None:
        """Experimental placeholder. Disabled unless explicitly allowed in config."""
        if not confirm_pymavlink_commands_allowed(self.config):
            raise PermissionError(
                "pymavlink command sending is disabled. Set "
                "safety.allow_pymavlink_commands=true only for explicit experiments."
            )
        raise NotImplementedError(
            "PX4 shell command transport is not implemented here. Use MAVSDK for flight "
            "commands and a dedicated mavlink_shell.py path for shell experiments."
        )


def _format_message(msg_type: str, payload: dict[str, Any]) -> str:
    compact_keys: dict[str, Iterable[str]] = {
        "HEARTBEAT": ("type", "autopilot", "base_mode", "custom_mode", "system_status"),
        "SYS_STATUS": ("voltage_battery", "current_battery", "battery_remaining"),
        "BATTERY_STATUS": ("voltages", "current_battery", "battery_remaining"),
        "ATTITUDE": ("roll", "pitch", "yaw", "rollspeed", "pitchspeed", "yawspeed"),
        "LOCAL_POSITION_NED": ("x", "y", "z", "vx", "vy", "vz"),
        "GLOBAL_POSITION_INT": ("lat", "lon", "alt", "relative_alt", "vx", "vy", "vz"),
        "GPS_RAW_INT": ("fix_type", "lat", "lon", "alt", "eph", "epv", "vel", "satellites_visible"),
        "VFR_HUD": ("airspeed", "groundspeed", "heading", "throttle", "alt", "climb"),
        "ACTUATOR_OUTPUT_STATUS": ("active", "actuator"),
        "SERVO_OUTPUT_RAW": ("servo1_raw", "servo2_raw", "servo3_raw", "servo4_raw"),
        "HOME_POSITION": ("latitude", "longitude", "altitude", "x", "y", "z"),
        "COMMAND_ACK": ("command", "result"),
        "MISSION_ACK": ("type",),
        "MISSION_CURRENT": ("seq",),
        "STATUSTEXT": ("severity", "text"),
    }
    keys = compact_keys.get(msg_type)
    if not keys:
        return f"{time.strftime('%H:%M:%S')} {msg_type}: {payload}"
    fields = ", ".join(f"{key}={payload.get(key)}" for key in keys if key in payload)
    return f"{time.strftime('%H:%M:%S')} {msg_type}: {fields}"
