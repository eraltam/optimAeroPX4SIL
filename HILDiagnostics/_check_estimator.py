import sys
from pymavlink import mavutil

mav = mavutil.mavlink_connection(sys.argv[1] if len(sys.argv) > 1 else "udpin:0.0.0.0:14550")
mav.wait_heartbeat(timeout=10)
msg = mav.recv_match(type="ESTIMATOR_STATUS", blocking=True, timeout=8)
if msg is None:
    print("No ESTIMATOR_STATUS received")
    sys.exit(1)

flags = msg.flags
enum = mavutil.mavlink.enums["ESTIMATOR_STATUS_FLAGS"]
names = [e.name for v, e in enum.items() if isinstance(v, int) and (flags & v)]
print(f"flags={flags:#x}")
print("set flags:", names)
print(f"pos_horiz_accuracy={msg.pos_horiz_accuracy:.2f} pos_vert_accuracy={msg.pos_vert_accuracy:.2f}")
