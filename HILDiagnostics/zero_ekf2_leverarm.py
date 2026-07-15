"""Diagnostic: zero EKF2_IMU_POS_*/EKF2_GPS_POS_* to test the lever-arm-mismatch hypothesis
(gps.slx/ins.slx feed CG-referenced truth directly, with no lever-arm offset applied, while the
airframe tells EKF2 to expect a real physical mount offset) -- see conversation notes, 2026-07-14.
Reboots after confirming the writes.
"""
import struct
import sys
import time
from pymavlink import mavutil
from connection import connect

TO_SET = {
    "EKF2_IMU_POS_X": 0.0,
    "EKF2_IMU_POS_Y": 0.0,
    "EKF2_IMU_POS_Z": 0.0,
    "EKF2_GPS_POS_X": 0.0,
    "EKF2_GPS_POS_Y": 0.0,
    "EKF2_GPS_POS_Z": 0.0,
}


def set_param_float(mav, name, value, timeout=3):
    mav.mav.param_set_send(
        mav.target_system, mav.target_component,
        name.encode(), float(value), mavutil.mavlink.MAV_PARAM_TYPE_REAL32,
    )
    deadline = time.time() + timeout
    while time.time() < deadline:
        msg = mav.recv_match(type="PARAM_VALUE", blocking=True, timeout=timeout)
        if msg is None:
            return None
        if msg.param_id.strip("\x00") != name:
            continue
        return msg.param_value
    return None


def main(device, baud):
    mav = connect(device, baud=baud)
    print(f"Connected: sysid={mav.target_system} compid={mav.target_component}\n")

    all_ok = True
    for name, value in TO_SET.items():
        readback = set_param_float(mav, name, value)
        ok = readback is not None and abs(readback - value) < 1e-6
        all_ok = all_ok and ok
        print(f"  {name:16s} -> {value:<8} readback={readback!s:<10} {'OK' if ok else 'MISMATCH'}")

    if not all_ok:
        print("\nOne or more params did not confirm - not rebooting. Investigate before continuing.")
        sys.exit(1)

    print("\nAll params confirmed. Rebooting vehicle...")
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
