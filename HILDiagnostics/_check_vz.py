import sys, time, statistics
from pymavlink import mavutil

mav = mavutil.mavlink_connection(sys.argv[1] if len(sys.argv) > 1 else "udpin:0.0.0.0:14550")
mav.wait_heartbeat(timeout=10)

vzs = []
deadline = time.time() + 10
while time.time() < deadline:
    msg = mav.recv_match(type="LOCAL_POSITION_NED", blocking=True, timeout=1)
    if msg is None:
        continue
    vzs.append(msg.vz)
    print(f"t={msg.time_boot_ms:8d} vz={msg.vz:+.4f} z={msg.z:+.4f}")

if len(vzs) > 1:
    print(f"\nvz: min={min(vzs):.4f} max={max(vzs):.4f} mean={statistics.mean(vzs):.4f} "
          f"stdev={statistics.pstdev(vzs):.4f} n={len(vzs)}")
