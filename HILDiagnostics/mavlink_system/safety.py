"""Conservative safety gates for MAVSDK command sending."""

from __future__ import annotations

from typing import Any, Mapping


def validate_takeoff_altitude(altitude: float, max_altitude: float) -> None:
    if altitude <= 0:
        raise ValueError(f"Takeoff altitude must be positive, got {altitude}")
    if altitude > max_altitude:
        raise ValueError(
            f"Takeoff altitude {altitude} m exceeds configured max {max_altitude} m"
        )


def confirm_real_vehicle_allowed(config: Mapping[str, Any]) -> bool:
    safety = config.get("safety", {})
    return bool(safety.get("allow_real_vehicle_commands", False))


def confirm_pymavlink_commands_allowed(config: Mapping[str, Any]) -> bool:
    safety = config.get("safety", {})
    return bool(safety.get("allow_pymavlink_commands", False))


def print_safety_warning(config: Mapping[str, Any]) -> None:
    if confirm_real_vehicle_allowed(config):
        print("SAFETY: real MAVSDK vehicle commands are ENABLED.")
        print("SAFETY: use only in a controlled HIL/bench setup with a kill switch available.")
    else:
        print("SAFETY: real MAVSDK vehicle commands are disabled; actions will be printed only.")

    if confirm_pymavlink_commands_allowed(config):
        print("SAFETY: experimental pymavlink command sending is ENABLED.")
    else:
        print("SAFETY: pymavlink is read-only.")
