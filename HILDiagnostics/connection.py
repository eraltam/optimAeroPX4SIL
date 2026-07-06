"""Shared MAVLink connection helper for HIL diagnostics.

Connect over the independent MAVLink instance (section 14.1, option 1) or the
optional passive UDP mirror (option 2). Never point this at the same serial
device string passed to initVehicleSIL's hitlSerialPort - that port is owned
exclusively by Simulink for the duration of the HIL run.

Right now (micro-USB only, no TELEM adapter - claude_hil_instruction.md section 1a),
there is no free second physical link either: run this only before Simulink
connects, after it disconnects, or against the passive UDP mirror.
"""
from pymavlink import mavutil


def connect(device, baud=57600, source_system=250, timeout=30):
    """device examples:
    - 'COM7'                  serial instance (verify against MAV_0_CONFIG, section 14.1)
    - 'udpin:0.0.0.0:14560'   passive mirror (only if configured per section 14.1 option 2)
    """
    mav = mavutil.mavlink_connection(device, baud=baud, source_system=source_system)
    hb = mav.wait_heartbeat(timeout=timeout)
    if hb is None:
        raise TimeoutError(f"No HEARTBEAT received on {device} within {timeout}s")
    return mav
