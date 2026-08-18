"""Read or set a PX4 INT32 parameter over a direct serial MAVLink connection, with correct
bit-reinterpretation (PX4 transmits non-float params as their raw bits punned into the
PARAM_VALUE/PARAM_SET float field -- sending a plain float like 4001.0 does NOT work for an
INT32 param, it must be struct.pack/unpack'd as int32 bits reinterpreted as float).

Usage:
    py -3.12 px4_int_param.py --port COM4 --get SYS_AUTOSTART
    py -3.12 px4_int_param.py --port COM4 --set SYS_AUTOSTART 6001 --save
"""
from __future__ import annotations

import argparse
import struct
import sys
import time

from pymavlink import mavutil


def int32_to_param_float(value: int) -> float:
    return struct.unpack("<f", struct.pack("<i", value))[0]


def param_float_to_int32(value: float) -> int:
    return struct.unpack("<i", struct.pack("<f", value))[0]


def get_param(master, name: str, timeout_s: float = 5.0):
    master.mav.param_request_read_send(master.target_system, master.target_component, name.encode(), -1)
    end = time.time() + timeout_s
    while time.time() < end:
        msg = master.recv_match(type="PARAM_VALUE", blocking=True, timeout=1)
        if msg and msg.param_id.strip("\x00") == name:
            return msg
    return None


def set_param(master, name: str, int_value: int, param_type=mavutil.mavlink.MAV_PARAM_TYPE_INT32, timeout_s: float = 5.0):
    master.mav.param_set_send(
        master.target_system, master.target_component, name.encode(), int32_to_param_float(int_value), param_type
    )
    end = time.time() + timeout_s
    while time.time() < end:
        msg = master.recv_match(type="PARAM_VALUE", blocking=True, timeout=1)
        if msg and msg.param_id.strip("\x00") == name:
            return msg
    return None


def save_to_storage(master, timeout_s: float = 10.0):
    master.mav.command_long_send(
        master.target_system, master.target_component,
        mavutil.mavlink.MAV_CMD_PREFLIGHT_STORAGE, 0, 1, 0, 0, 0, 0, 0, 0,
    )
    end = time.time() + timeout_s
    while time.time() < end:
        msg = master.recv_match(type="COMMAND_ACK", blocking=True, timeout=1)
        if msg and msg.command == mavutil.mavlink.MAV_CMD_PREFLIGHT_STORAGE:
            return msg
    return None


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--port", default="COM4")
    parser.add_argument("--baud", type=int, default=921600)
    parser.add_argument("--get", metavar="NAME")
    parser.add_argument("--set", metavar="NAME")
    parser.add_argument("value", nargs="?", type=int, help="Integer value for --set")
    parser.add_argument("--save", action="store_true", help="Also write parameters to storage after --set")
    args = parser.parse_args()

    if not args.get and not args.set:
        parser.error("pass --get NAME or --set NAME VALUE")
    if args.set and args.value is None:
        parser.error("--set requires a value")

    master = mavutil.mavlink_connection(args.port, baud=args.baud)
    # Confirmed 2026-07-28: with SYS_AUTOSTART pointed at the real 1003 HIL airframe, this board's
    # USB MAVLink instance stays completely silent (no bytes at all, DTR/RTS notwithstanding) until
    # it sees a HEARTBEAT from the connecting side first -- a passive wait_heartbeat() alone hangs
    # forever even on a fully healthy board. Force DTR/RTS and proactively announce ourselves before
    # waiting.
    master.port.dtr = True
    master.port.rts = True
    for _ in range(5):
        master.mav.heartbeat_send(mavutil.mavlink.MAV_TYPE_GCS, mavutil.mavlink.MAV_AUTOPILOT_INVALID, 0, 0, 0)
        if master.wait_heartbeat(timeout=2):
            break
    else:
        print("FAILED: no heartbeat", file=sys.stderr)
        sys.exit(1)

    if args.get:
        msg = get_param(master, args.get)
        if msg is None:
            print(f"FAILED: no PARAM_VALUE for {args.get}", file=sys.stderr)
            sys.exit(1)
        print(f"{args.get} = {param_float_to_int32(msg.param_value)} (param_type={msg.param_type})")

    if args.set:
        msg = set_param(master, args.set, args.value)
        if msg is None:
            print(f"FAILED: no PARAM_VALUE readback for {args.set}", file=sys.stderr)
            sys.exit(1)
        print(f"{args.set} set readback = {param_float_to_int32(msg.param_value)}")
        if args.save:
            ack = save_to_storage(master)
            print(f"storage save ack: {ack}")

    master.close()


if __name__ == "__main__":
    main()
