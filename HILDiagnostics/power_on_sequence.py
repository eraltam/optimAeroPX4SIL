"""Codifies the mandatory physical power-on order for any test involving the real ANELLO X3 IMU.

Required order (see HIL_TEST_STATUS_AND_NEXT_PRIORITIES.md section 2):

  1. Power the ANELLO IMU first (relay "GYRO_POWER", COM5).
  2. Wait for it to finish powering up / initializing.
  3. Power the Cube/PX4 relay ("CUBE_POWER", COM7), which lets PX4 enumerate.
  4. Poll until PX4 enumerates on COM4.
  5. Only then is the ANELLO IMU on TELEM2 guaranteed to read correctly.

If step 3 happens before step 1 has settled, PX4 still enumerates on COM4 but the ANELLO's
static gyro/accel channels silently read 0 -- no error, no disconnect, just zeros. This script
exists so that failure mode can never happen from a missed manual step, and so every session has
a timestamped record of when each relay was actually toggled (previously this only happened by
hand via relay_gui.py, with no log at all).

Reuses relay_gui.py's exact serial protocol (LCUS-1 style 4-byte ON/OFF frames) and
run_hitl_session.py's COM-port-presence check (PowerShell Win32_PnPEntity query), so this doesn't
diverge from what's already proven to work elsewhere in this repo.

Usage:
    py -3.12 power_on_sequence.py
    py -3.12 power_on_sequence.py --gyro-settle-s 8 --px4-port COM4 --px4-timeout-s 60
    py -3.12 power_on_sequence.py --use-reset   # power-cycle the Cube relay instead of a plain ON
"""

from __future__ import annotations

import argparse
import csv
import datetime as dt
import json
import subprocess
import sys
import time
from pathlib import Path

import serial

sys.path.insert(0, str(Path(__file__).resolve().parent / "mavlink_system"))
from resolve_cube_port import DEFAULT_VID_PID  # noqa: E402

RELAY_MAP_PATH = Path(r"C:\Users\Edison Altamirano\Documents\USB-Relay\relay_map.json")
LOG_PATH = Path(r"C:\AS\optimAeroPX4SIL\HILDiagnostics\logs\power_sequence_log.csv")

CMD_ON = bytes([0xA0, 0x01, 0x01, 0xA2])
CMD_OFF = bytes([0xA0, 0x01, 0x00, 0xA1])

LOG_FIELDS = ["timestamp_utc", "event", "port", "detail"]


def load_relay_map() -> dict:
    with RELAY_MAP_PATH.open("r", encoding="utf-8") as f:
        return json.load(f)


def relay_write(port: str, state: bool) -> None:
    payload = CMD_ON if state else CMD_OFF
    with serial.Serial(port, 9600, timeout=1) as ser:
        ser.write(payload)


def log_event(rows: list[dict], event: str, port: str, detail: str = "") -> None:
    row = {
        "timestamp_utc": dt.datetime.now(dt.timezone.utc).isoformat(timespec="seconds"),
        "event": event,
        "port": port,
        "detail": detail,
    }
    rows.append(row)
    print(f"[{row['timestamp_utc']}] {event} ({port}) {detail}")


def append_log(rows: list[dict]) -> None:
    LOG_PATH.parent.mkdir(parents=True, exist_ok=True)
    write_header = not LOG_PATH.exists()
    with LOG_PATH.open("a", newline="", encoding="utf-8") as f:
        writer = csv.DictWriter(f, fieldnames=LOG_FIELDS)
        if write_header:
            writer.writeheader()
        writer.writerows(rows)


def poll_for_cube(vid_pid: str, timeout_s: float, poll_interval_s: float = 1.0) -> str | None:
    """Poll until a device matching vid_pid enumerates, returning its COM port.

    Polls by USB device identity (resolve_cube_port.py), not a hardcoded port number --
    Windows can hand the Cube a different COM number on a later boot/replug, and a
    fixed-port-number poll would then silently wait on the wrong (or a nonexistent) port
    forever. Returns None on timeout (mirrors the old poll_com_port's False-on-timeout).
    """
    from resolve_cube_port import resolve_cube_port

    deadline = time.time() + timeout_s
    while time.time() < deadline:
        try:
            return resolve_cube_port(vid_pid, log=False)
        except RuntimeError:
            time.sleep(poll_interval_s)
    return None


def main() -> None:
    parser = argparse.ArgumentParser(
        description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument("--gyro-relay-name", default="GYRO_POWER")
    parser.add_argument("--cube-relay-name", default="CUBE_POWER")
    parser.add_argument(
        "--gyro-settle-s",
        type=float,
        default=8.0,
        help="Wait after powering the ANELLO IMU before powering the Cube relay. NOT yet "
        "empirically calibrated -- this is a conservative starting guess. Tighten it once "
        "you've measured the minimum safe value (e.g. by checking the first few seconds of "
        "static gyro/accel data for the all-zero failure mode at progressively shorter waits).",
    )
    parser.add_argument(
        "--cube-reset-settle-s",
        type=float,
        default=2.0,
        help="OFF->wait->ON delay when --use-reset is passed, matches relay_gui.py's RESET button.",
    )
    parser.add_argument(
        "--px4-vid-pid", default=DEFAULT_VID_PID,
        help="USB VID:PID identity of the Cube Orange+ (see resolve_cube_port.py). Not a fixed "
        "COM port number -- Windows can reassign that across boots/replugs.",
    )
    parser.add_argument("--px4-timeout-s", type=float, default=60.0)
    parser.add_argument(
        "--use-reset",
        action="store_true",
        help="Power-cycle the Cube relay (OFF then ON) instead of a plain ON. Use this if the "
        "Cube relay may already be latched on from a previous session.",
    )
    args = parser.parse_args()

    relays = load_relay_map()
    missing = [name for name in (args.gyro_relay_name, args.cube_relay_name) if name not in relays]
    if missing:
        print(
            f"ERROR: relay_map.json is missing {missing}. Found: {list(relays)}",
            file=sys.stderr,
        )
        sys.exit(1)

    gyro_port = relays[args.gyro_relay_name]["port"]
    cube_port = relays[args.cube_relay_name]["port"]

    rows: list[dict] = []

    try:
        log_event(rows, "gyro_power_on", gyro_port, f"relay={args.gyro_relay_name}")
        relay_write(gyro_port, True)

        log_event(rows, "gyro_settle_wait_start", gyro_port, f"{args.gyro_settle_s:.1f}s")
        time.sleep(args.gyro_settle_s)
        log_event(rows, "gyro_settle_wait_end", gyro_port, "")

        if args.use_reset:
            log_event(
                rows, "cube_power_off", cube_port, f"relay={args.cube_relay_name} (reset 1/2)"
            )
            relay_write(cube_port, False)
            time.sleep(args.cube_reset_settle_s)
            log_event(
                rows, "cube_power_on", cube_port, f"relay={args.cube_relay_name} (reset 2/2)"
            )
            relay_write(cube_port, True)
        else:
            log_event(rows, "cube_power_on", cube_port, f"relay={args.cube_relay_name}")
            relay_write(cube_port, True)

        log_event(
            rows, "px4_enumeration_poll_start", args.px4_vid_pid, f"timeout={args.px4_timeout_s:.0f}s"
        )
        resolved_port = poll_for_cube(args.px4_vid_pid, args.px4_timeout_s)
        if resolved_port:
            log_event(rows, "px4_enumeration_confirmed", resolved_port, f"vid_pid={args.px4_vid_pid}")
        else:
            log_event(
                rows,
                "px4_enumeration_FAILED",
                args.px4_vid_pid,
                f"not seen within {args.px4_timeout_s:.0f}s -- check wiring/USB before proceeding",
            )
    finally:
        append_log(rows)

    if not resolved_port:
        print(
            f"\nFAILED: no device matching {args.px4_vid_pid} enumerated. "
            "Do not start a recording session.",
            file=sys.stderr,
        )
        sys.exit(1)

    print(f"\nCube Orange+ resolved on {resolved_port} (logged to resolve_cube_port.py's own log)")

    print(f"\nOK: power-on sequence complete and logged to {LOG_PATH}")
    print("You can now start a session, e.g.:")
    print(
        "  py -3.12 optimAeroPX4SIL/HILDiagnostics/mavlink_system/passive_listener.py "
        "--duration <seconds> --session-id <name>"
    )


if __name__ == "__main__":
    main()
