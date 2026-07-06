import sys, time
from pymavlink import mavutil

mav = mavutil.mavlink_connection(sys.argv[1] if len(sys.argv) > 1 else "udpin:0.0.0.0:14550")
deadline = time.time() + (float(sys.argv[2]) if len(sys.argv) > 2 else 10)
seen = False
while time.time() < deadline:
    msg = mav.recv_match(type="HEARTBEAT", blocking=True, timeout=1)
    if msg is None:
        continue
    seen = True
    armed = bool(msg.base_mode & mavutil.mavlink.MAV_MODE_FLAG_SAFETY_ARMED)
    hil = bool(msg.base_mode & mavutil.mavlink.MAV_MODE_FLAG_HIL_ENABLED)
    print(f"base_mode={msg.base_mode:#010b} custom_mode={msg.custom_mode} "
          f"system_status={msg.system_status} ARMED={armed} HIL={hil}")
if not seen:
    print("No HEARTBEAT seen - is the mirror/Simulink connection still active?")
