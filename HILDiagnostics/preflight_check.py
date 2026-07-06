"""Run once after flashing/rebooting, before starting the Simulink HIL run.
Verifies the static, pre-flight-relevant subset of section 12's acceptance criteria.
"""
import struct
import sys
from pymavlink import mavutil
from connection import connect

PARAMS = [
    "SYS_AUTOSTART",   # expect 1003
    "SYS_HITL",        # expect 1
    "CA_ROTOR_COUNT",  # expect 6
    "HIL_ACT_FUNC1", "HIL_ACT_FUNC2", "HIL_ACT_FUNC3",
    "HIL_ACT_FUNC4", "HIL_ACT_FUNC5", "HIL_ACT_FUNC6",
    "MAV_1_CONFIG",
    "SER_TEL2_BAUD",   # expect 921600 (once a TELEM2 adapter is wired, section 1a)
    "SYS_USB_AUTO",    # expect 2 during the interim USB-only phase, section 1a
    "CBRK_IO_SAFETY",
    "CBRK_SUPPLY_CHK",
]

EXPECTED = {
    "SYS_AUTOSTART": 1003,
    "SYS_HITL": 1,
    "CA_ROTOR_COUNT": 6,
}


INT_PARAM_TYPES = {
    mavutil.mavlink.MAV_PARAM_TYPE_INT8,
    mavutil.mavlink.MAV_PARAM_TYPE_INT16,
    mavutil.mavlink.MAV_PARAM_TYPE_INT32,
    mavutil.mavlink.MAV_PARAM_TYPE_UINT8,
    mavutil.mavlink.MAV_PARAM_TYPE_UINT16,
    mavutil.mavlink.MAV_PARAM_TYPE_UINT32,
}


def fetch_param(mav, name, timeout=3):
    mav.mav.param_request_read_send(mav.target_system, mav.target_component, name.encode(), -1)
    msg = mav.recv_match(type="PARAM_VALUE", blocking=True, timeout=timeout)
    if msg is None or msg.param_id.strip("\x00") != name:
        return None
    if msg.param_type in INT_PARAM_TYPES:
        # PX4 sends integer-typed params as the raw int32 bit pattern reinterpreted as
        # float32 (a long-standing MAVLink convention), not a numeric cast - undo that.
        return struct.unpack("<i", struct.pack("<f", msg.param_value))[0]
    return msg.param_value


def main(device, baud):
    mav = connect(device, baud=baud)
    hb = mav.messages["HEARTBEAT"]

    print(f"Connected: sysid={mav.target_system} compid={mav.target_component}")
    print(f"autopilot={hb.autopilot} type={hb.type} base_mode={hb.base_mode:#010b} "
          f"system_status={hb.system_status}")

    hil_flag = bool(hb.base_mode & mavutil.mavlink.MAV_MODE_FLAG_HIL_ENABLED)
    print(f"MAV_MODE_FLAG_HIL_ENABLED set: {hil_flag} "
          f"({'PASS' if hil_flag else 'FAIL - reboot after SYS_HITL=1, SYS_AUTOSTART=1003'})")

    all_pass = hil_flag
    print("\nParameter checks:")
    for name in PARAMS:
        value = fetch_param(mav, name)
        expected = EXPECTED.get(name)
        if value is None:
            status = "FAIL (no response)"
            all_pass = False
        elif expected is not None:
            status = "PASS" if value == expected else f"FAIL (expected {expected})"
            all_pass = all_pass and value == expected
        else:
            status = "info"
        print(f"  {name:16s} = {value!s:12s} {status}")

    print("\nOVERALL:", "PASS" if all_pass else "FAIL")
    sys.exit(0 if all_pass else 1)


if __name__ == "__main__":
    device = sys.argv[1] if len(sys.argv) > 1 else "COM7"
    baud = int(sys.argv[2]) if len(sys.argv) > 2 else 57600
    main(device, baud)
