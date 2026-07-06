import sys, time
from pymavlink import mavutil

mav = mavutil.mavlink_connection(sys.argv[1] if len(sys.argv) > 1 else "udpin:0.0.0.0:14550")
mav.wait_heartbeat(timeout=10)
print(f"Connected: sysid={mav.target_system} compid={mav.target_component}")

# Grab a SYS_STATUS first for sensor health context
sys_status = mav.recv_match(type="SYS_STATUS", blocking=True, timeout=5)
if sys_status:
    present = sys_status.onboard_control_sensors_present
    enabled = sys_status.onboard_control_sensors_enabled
    health = sys_status.onboard_control_sensors_health
    unhealthy = present & ~health
    print(f"present={present:#034b}")
    print(f"enabled={enabled:#034b}")
    print(f"health ={health:#034b}")
    print(f"present-but-unhealthy bits={unhealthy:#034b}")

print("\nSending arm command, listening 12s for any STATUSTEXT / COMMAND_ACK...")
mav.mav.command_long_send(
    mav.target_system, mav.target_component,
    mavutil.mavlink.MAV_CMD_COMPONENT_ARM_DISARM, 0,
    1, 0, 0, 0, 0, 0, 0,
)

deadline = time.time() + 12
while time.time() < deadline:
    msg = mav.recv_match(type=["COMMAND_ACK", "STATUSTEXT", "EXTENDED_SYS_STATE"], blocking=True, timeout=1)
    if msg is None:
        continue
    if msg.get_type() == "COMMAND_ACK":
        print(f"COMMAND_ACK: command={msg.command} result={msg.result}")
    elif msg.get_type() == "STATUSTEXT":
        print(f"STATUSTEXT[{msg.severity}]: {msg.text}")
    else:
        print(f"EXTENDED_SYS_STATE: landed_state={msg.landed_state}")
