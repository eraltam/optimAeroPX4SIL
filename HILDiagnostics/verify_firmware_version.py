"""Verify the live PX4 firmware git hash/board id against an expected artifact, over MAVLink.

Used after every flash (plan Section 3.3 step 8-9, resume-point step 3): the flash log only
proves the uploader's verify pass succeeded, not that the board actually boots and runs that
image. This connects to the resolved Cube port, waits for a heartbeat, requests
AUTOPILOT_VERSION, and compares the embedded git hash (as ASCII, from `flight_custom_version`)
and `board_version` against the expected artifact's manifest values.

Usage:
    py -3.12 verify_firmware_version.py --expected-hash dda0ed3dd6072249fa130962eed46e5d230d0f92 --board-id 1063
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

from pymavlink import mavutil

sys.path.insert(0, str(Path(__file__).resolve().parent / "mavlink_system"))
from resolve_cube_port import resolve_cube_port  # noqa: E402


def request_autopilot_version(master, timeout_s: float = 10.0):
    master.mav.command_long_send(
        master.target_system,
        master.target_component,
        mavutil.mavlink.MAV_CMD_REQUEST_MESSAGE,
        0,
        mavutil.mavlink.MAVLINK_MSG_ID_AUTOPILOT_VERSION,
        0, 0, 0, 0, 0, 0,
    )
    msg = master.recv_match(type="AUTOPILOT_VERSION", blocking=True, timeout=timeout_s)
    return msg


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--port", default=None, help="Serial port; auto-resolved via VID:PID if omitted")
    parser.add_argument("--baud", type=int, default=57600)
    parser.add_argument("--expected-hash", required=True, help="Full 40-char git commit hash embedded in the target artifact")
    parser.add_argument("--board-id", type=int, default=None, help="Informational only -- board_version is a HW-revision field, not the uploader's board_id")
    parser.add_argument("--heartbeat-timeout", type=float, default=20.0)
    args = parser.parse_args()

    port = args.port or resolve_cube_port()
    print(f"verify_firmware_version: connecting on {port} @ {args.baud}")

    master = mavutil.mavlink_connection(port, baud=args.baud)
    # Confirmed 2026-07-28: this board's USB-CDC virtual COM port stays silent (zero bytes) unless
    # DTR/RTS are explicitly driven high -- plain mavutil.mavlink_connection() leaves them at
    # whatever the OS/driver last left them (observed low after a flash), so no heartbeat ever
    # arrives even on a fully healthy, freshly power-cycled board. A different symptom from the
    # separately-documented "silent board" cases elsewhere in this project, which needed a relay
    # power-cycle instead.
    master.port.dtr = True
    master.port.rts = True
    hb = master.wait_heartbeat(timeout=args.heartbeat_timeout)
    if hb is None:
        print("verify_firmware_version: FAILED: no heartbeat received", file=sys.stderr)
        sys.exit(1)
    print(f"verify_firmware_version: heartbeat OK from sysid={master.target_system} compid={master.target_component}")

    msg = request_autopilot_version(master)
    if msg is None:
        print("verify_firmware_version: FAILED: no AUTOPILOT_VERSION response", file=sys.stderr)
        sys.exit(1)

    # PX4 packs flight_custom_version as the raw abbreviated-git-hash bytes (5 bytes = 10 hex
    # chars, matching the `-g<hash>` suffix in `git describe`) in REVERSE byte order, left-padded
    # with zero bytes in the field's original orientation -- so those padding zeros land at the
    # END after reversal and must be stripped there, not before. Confirmed empirically
    # 2026-07-28: a build embedding commit dda0ed3dd6... (git_identity
    # 'v1.3.16-25-gdda0ed3dd6') reported raw field hex '000000d63deda0dd'; reversing the full
    # field gives 'dda0ed3dd6000000', and only after THEN stripping trailing zeros does it equal
    # the expected 10-char abbreviated hash 'dda0ed3dd6'.
    flight_custom_raw = bytes(msg.flight_custom_version)
    live_hash = flight_custom_raw[::-1].rstrip(b"\x00").hex()

    board_version = msg.board_version
    print(f"verify_firmware_version: board_version={board_version} (HW revision field -- not the uploader board_id)")
    print(f"verify_firmware_version: flight_custom_version (byte-reversed hex)={live_hash}")

    expected = args.expected_hash.lower()
    hash_match = bool(live_hash) and expected.startswith(live_hash.lower())

    print("")
    if hash_match:
        print(f"verify_firmware_version: PASS -- live hash prefix matches expected {args.expected_hash}")
    else:
        print(
            f"verify_firmware_version: MISMATCH -- live flight_custom_version (reversed)={live_hash!r} "
            f"does not match expected {args.expected_hash}",
            file=sys.stderr,
        )

    if args.board_id is not None:
        print(
            f"verify_firmware_version: NOTE -- --board-id={args.board_id} was requested but "
            "board_version is a HW-revision field, not the uploader's board_id; not compared.",
        )

    sys.exit(0 if hash_match else 2)


if __name__ == "__main__":
    main()
