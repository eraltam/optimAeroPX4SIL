"""Continuously poll actuator_armed every ~8s over a live HIL session, to catch the real state
during the armed/AUTO.TAKEOFF window (not just after it has already failed and disarmed).
"""
import sys
import time
from mavlink_shell_probe import MavlinkShell

address = sys.argv[1] if len(sys.argv) > 1 else "udpin:0.0.0.0:14550"
duration_s = float(sys.argv[2]) if len(sys.argv) > 2 else 120.0
interval_s = float(sys.argv[3]) if len(sys.argv) > 3 else 8.0

print(f"Connecting to {address} ...")
shell = MavlinkShell(address)
shell.write("\n")
time.sleep(1.0)
shell.read_for(1.5)

start = time.time()
while time.time() - start < duration_s:
    t = time.time() - start
    shell.write("listener actuator_armed -n 1\n")
    out = shell.read_for(4.0)
    print(f"--- t={t:5.1f}s ---")
    print(out if out.strip() else "(no response)")
    time.sleep(max(0.0, interval_s - 4.0))

shell.close()
