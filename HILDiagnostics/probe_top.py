import sys
import time
from mavlink_shell_probe import MavlinkShell

address = sys.argv[1] if len(sys.argv) > 1 else r"\\.\COM4"
shell = MavlinkShell(address)
print("connected")
shell.write("\n")
time.sleep(1.0)
shell.read_for(1.0)
shell.write("top once\n")
print(shell.read_for(4.0))
shell.close()
