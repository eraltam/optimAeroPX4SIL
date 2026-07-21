"""Diagnostic: set EKF2_GPS_CHECK to match the historically-working session
(session_relay_passthrough_01: EKF2_GPS_CHECK=1023) vs the current board's 245.
Reboots after confirming the write.
"""
import struct
import sys
import time
from pymavlink import mavutil
from connection import connect


def set_param_int(mav, name, value, timeout=3):
    raw_float = struct.unpack("<f", struct.pack("<i", value))[0]
    mav.mav.param_set_send(
        mav.target_system, mav.target_component,
        name.encode(), raw_float, mavutil.mavlink.MAV_PARAM_TYPE_INT32,
    )
    deadline = time.time() + timeout
    while time.time() < deadline:
        msg = mav.recv_match(type="PARAM_VALUE", blocking=True, timeout=timeout)
        if msg is None:
            return None
        if msg.param_id.strip("\x00") != name:
            continue
        return struct.unpack("<i", struct.pack("<f", msg.param_value))[0]
    return None


def main(device, baud):
    mav = connect(device, baud=baud)
    print(f"Connected: sysid={mav.target_system} compid={mav.target_component}\n")

    readback = set_param_int(mav, "EKF2_GPS_CHECK", 1023)
    ok = readback == 1023
    print(f"  EKF2_GPS_CHECK -> 1023     readback={readback!s:<8} {'OK' if ok else 'MISMATCH'}")

    if not ok:
        print("\nParam did not confirm - not rebooting. Investigate before continuing.")
        sys.exit(1)

    print("\nParam confirmed. Rebooting vehicle...")
    mav.mav.command_long_send(
        mav.target_system, mav.target_component,
        mavutil.mavlink.MAV_CMD_PREFLIGHT_REBOOT_SHUTDOWN, 0,
        1, 0, 0, 0, 0, 0, 0,
    )
    print("Reboot command sent. Wait ~10-15s before reconnecting.")


if __name__ == "__main__":
    device = sys.argv[1] if len(sys.argv) > 1 else "COM4"
    baud = int(sys.argv[2]) if len(sys.argv) > 2 else 921600
    main(device, baud)
