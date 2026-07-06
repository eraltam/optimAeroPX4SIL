import sys, time
from pymavlink import mavutil

mav = mavutil.mavlink_connection(sys.argv[1] if len(sys.argv) > 1 else "udpin:0.0.0.0:14550")
mav.wait_heartbeat(timeout=10)
print(f"Connected: sysid={mav.target_system} compid={mav.target_component}")

mav.mav.command_long_send(
    mav.target_system, mav.target_component,
    mavutil.mavlink.MAV_CMD_COMPONENT_ARM_DISARM, 0,
    1, 0, 0, 0, 0, 0, 0,
)

deadline = time.time() + 6
while time.time() < deadline:
    msg = mav.recv_match(type=["COMMAND_ACK", "STATUSTEXT"], blocking=True, timeout=1)
    if msg is None:
        continue
    if msg.get_type() == "COMMAND_ACK":
        print(f"COMMAND_ACK: command={msg.command} result={msg.result}")
    else:
        print(f"STATUSTEXT[{msg.severity}]: {msg.text}")
