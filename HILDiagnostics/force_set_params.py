"""One-off remediation: force-set params that QGC's Parameters UI would not commit
correctly (SYS_AUTOSTART is an enum-type param and QGC's combo-box widget only
knows about airframes bundled in its own metadata, so a brand-new custom
SYS_AUTOSTART value like 1003 may not actually get written - confirmed on real
hardware 2026-07-06). This writes each value directly via PARAM_SET and verifies
the ack, then reboots the vehicle so 1003_optim_aero_hex.hil actually runs.
"""
import struct
import sys
import time
from pymavlink import mavutil
from connection import connect

TO_SET = {
    "SYS_AUTOSTART": 1003,
    "CA_ROTOR_COUNT": 6,
    "HIL_ACT_FUNC1": 101,
    "HIL_ACT_FUNC2": 102,
    "HIL_ACT_FUNC3": 103,
    "HIL_ACT_FUNC4": 104,
    "HIL_ACT_FUNC5": 105,
    "HIL_ACT_FUNC6": 106,
    "CBRK_SUPPLY_CHK": 894281,
}


def set_param_int(mav, name, value, timeout=3):
    # PX4 expects int32 params on the wire as the raw int32 bits reinterpreted as
    # float32 (same convention as the read path) - a plain float(value) cast sends
    # the wrong bit pattern entirely (e.g. 1003 -> 1003.0's bits, not int 1003's bits).
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

    all_ok = True
    for name, value in TO_SET.items():
        readback = set_param_int(mav, name, value)
        ok = readback == value
        all_ok = all_ok and ok
        print(f"  {name:16s} -> {value:<8} readback={readback!s:<8} {'OK' if ok else 'MISMATCH'}")

    if not all_ok:
        print("\nOne or more params did not confirm - not rebooting. Investigate before continuing.")
        sys.exit(1)

    print("\nAll params confirmed. Rebooting vehicle...")
    mav.mav.command_long_send(
        mav.target_system, mav.target_component,
        mavutil.mavlink.MAV_CMD_PREFLIGHT_REBOOT_SHUTDOWN, 0,
        1, 0, 0, 0, 0, 0, 0,
    )
    print("Reboot command sent. Wait ~5-10s, then re-run preflight_check.py.")


if __name__ == "__main__":
    device = sys.argv[1] if len(sys.argv) > 1 else "COM7"
    baud = int(sys.argv[2]) if len(sys.argv) > 2 else 57600
    main(device, baud)
