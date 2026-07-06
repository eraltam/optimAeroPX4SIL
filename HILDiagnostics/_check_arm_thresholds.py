import sys, struct
from pymavlink import mavutil

mav = mavutil.mavlink_connection(sys.argv[1] if len(sys.argv) > 1 else "udpin:0.0.0.0:14550")
mav.wait_heartbeat(timeout=10)

def fetch(name, timeout=3):
    mav.mav.param_request_read_send(mav.target_system, mav.target_component, name.encode(), -1)
    msg = mav.recv_match(type="PARAM_VALUE", blocking=True, timeout=timeout)
    if msg is None or msg.param_id.strip("\x00") != name:
        return None
    return msg.param_value

for name in ["COM_ARM_EKF_POS", "COM_ARM_EKF_HGT", "COM_ARM_EKF_VEL", "COM_ARM_EKF_YAW",
             "COM_ARM_WO_GPS", "EKF2_REQ_EPH", "EKF2_REQ_EPV", "EKF2_REQ_SACC"]:
    print(f"  {name:20s} = {fetch(name)}")
