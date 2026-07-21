import sys
import time
from mavlink_shell_probe import MavlinkShell

address = sys.argv[1] if len(sys.argv) > 1 else r"\\.\COM4"
shell = MavlinkShell(address)
print("connected")
shell.write("\n")
time.sleep(1.0)
shell.read_for(1.0)
for topic in ["actuator_armed", "vehicle_control_mode", "vehicle_thrust_setpoint", "actuator_motors"]:
    shell.write(f"listener {topic} -n 1\n")
    print(f"=== {topic} ===")
    print(shell.read_for(3.0))
shell.close()
