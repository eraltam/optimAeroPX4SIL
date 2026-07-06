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

ack = mav.recv_match(type="COMMAND_ACK", blocking=True, timeout=5)
if ack:
    print(f"COMMAND_ACK: command={ack.command} result={ack.result}")
else:
    print("No COMMAND_ACK received")

# Watch a few heartbeats after the arm attempt
deadline = time.time() + 8
while time.time() < deadline:
    hb = mav.recv_match(type="HEARTBEAT", blocking=True, timeout=1)
    if hb is None:
        continue
    armed = bool(hb.base_mode & mavutil.mavlink.MAV_MODE_FLAG_SAFETY_ARMED)
    print(f"base_mode={hb.base_mode:#010b} ARMED={armed}")
