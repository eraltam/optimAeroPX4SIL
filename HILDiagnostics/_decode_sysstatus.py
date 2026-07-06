import sys
from pymavlink import mavutil

mav = mavutil.mavlink_connection(sys.argv[1] if len(sys.argv) > 1 else "udpin:0.0.0.0:14550")
mav.wait_heartbeat(timeout=10)
msg = mav.recv_match(type="SYS_STATUS", blocking=True, timeout=8)
if msg is None:
    print("No SYS_STATUS received")
    sys.exit(1)

present = msg.onboard_control_sensors_present
enabled = msg.onboard_control_sensors_enabled
health = msg.onboard_control_sensors_health
unhealthy = present & ~health & 0xFFFFFFFF

enum = mavutil.mavlink.enums["MAV_SYS_STATUS_SENSOR"]

def names(mask):
    out = []
    for bit_value, entry in enum.items():
        if isinstance(bit_value, int) and (mask & bit_value):
            out.append(entry.name)
    return out

print("present  :", names(present))
print("enabled  :", names(enabled))
print("unhealthy (present but not healthy):", names(unhealthy))
