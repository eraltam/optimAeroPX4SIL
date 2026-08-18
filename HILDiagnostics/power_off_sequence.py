"""Power off the physical ANELLO/Cube bench, safely, in the required order.

Mirrors power_on_sequence.py's relay map/protocol. Required order is the reverse of power-on
(plan Section 9): Cube first, then ANELLO -- so the Cube is never left powered with the ANELLO
serial link already dead, and the ANELLO is never yanked out from under a still-booted Cube.

Only call this AFTER the onboard ULog logger has been stopped and confirmed flushed
(ulog_session.stop_recording) -- cutting power while a recording is still open risks a truncated/
corrupt log.

Usage:
    py -3.12 power_off_sequence.py
"""
from __future__ import annotations

import argparse
import datetime as dt
import time
from pathlib import Path

import serial

from power_on_sequence import (
    CMD_OFF,
    LOG_FIELDS,
    LOG_PATH,
    RELAY_MAP_PATH,
    append_log,
    load_relay_map,
    log_event,
)


def relay_write_off(port: str) -> None:
    with serial.Serial(port, 9600, timeout=1) as ser:
        ser.write(CMD_OFF)


def power_off(gyro_relay_name: str = "GYRO_POWER", cube_relay_name: str = "CUBE_POWER") -> None:
    relays = load_relay_map()
    gyro_port = relays[gyro_relay_name]["port"]
    cube_port = relays[cube_relay_name]["port"]

    rows: list[dict] = []
    try:
        log_event(rows, "cube_power_off", cube_port, f"relay={cube_relay_name}")
        relay_write_off(cube_port)
        time.sleep(1.0)
        log_event(rows, "gyro_power_off", gyro_port, f"relay={gyro_relay_name}")
        relay_write_off(gyro_port)
    finally:
        append_log(rows)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--gyro-relay-name", default="GYRO_POWER")
    parser.add_argument("--cube-relay-name", default="CUBE_POWER")
    args = parser.parse_args()
    power_off(args.gyro_relay_name, args.cube_relay_name)
    print(f"OK: power-off sequence complete (Cube then ANELLO), logged to {LOG_PATH}")


if __name__ == "__main__":
    main()
