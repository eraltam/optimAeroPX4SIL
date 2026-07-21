"""Non-interactive MAVLink shell probe (adapted from PX4-Autopilot/Tools/mavlink_shell.py's
MavlinkSerialPort class, which requires a TTY for its interactive main() -- this version sends a
fixed list of `listener <topic> -n 1` commands and captures the response, no terminal needed.

Use this to see PX4's own internal uORB topics directly (vehicle_thrust_setpoint, actuator_motors,
vehicle_land_detected) during a live armed/AUTO.TAKEOFF HIL session, to find exactly where thrust
becomes zero -- see HIL_ZERO_THRUST_AND_PARAM_RELIABILITY_NEXT_STEPS.md section 3.2.

Run this DURING an active Simulink HIL session, connecting over the QGC UDP mirror (14550) since
the serial port itself is owned by Simulink for the session's duration.
"""
import sys
import time
from pymavlink import mavutil

TOPICS = [
    "vehicle_thrust_setpoint",
    "vehicle_torque_setpoint",
    "actuator_motors",
    "vehicle_land_detected",
    "vehicle_control_mode",
    "actuator_armed",
]


class MavlinkShell:
    def __init__(self, address, devnum=10):
        self.mav = mavutil.mavlink_connection(address, autoreconnect=True)
        self.mav.mav.heartbeat_send(mavutil.mavlink.MAV_TYPE_GENERIC, mavutil.mavlink.MAV_AUTOPILOT_INVALID, 0, 0, 0)
        self.mav.wait_heartbeat(timeout=15)
        self.port = devnum
        self.buf = ""

    def write(self, s):
        b = s.encode("utf-8")
        while len(b) > 0:
            n = min(len(b), 70)
            chunk = list(b[:n]) + [0] * (70 - n)
            self.mav.mav.serial_control_send(
                self.port,
                mavutil.mavlink.SERIAL_CONTROL_FLAG_EXCLUSIVE | mavutil.mavlink.SERIAL_CONTROL_FLAG_RESPOND,
                0, 0, n, chunk,
            )
            b = b[n:]

    def read_for(self, duration_s):
        deadline = time.time() + duration_s
        out = []
        while time.time() < deadline:
            m = self.mav.recv_match(type="SERIAL_CONTROL", blocking=True, timeout=0.2)
            if m is not None and m.count:
                out.append(bytes(m.data[: m.count]).decode("utf-8", errors="replace"))
        return "".join(out)

    def close(self):
        self.mav.mav.serial_control_send(self.port, 0, 0, 0, 0, [0] * 70)


def main(address):
    print(f"Connecting MAVLink shell probe to {address} ...")
    shell = MavlinkShell(address)
    print("Connected. Priming shell...")
    shell.write("\n")
    time.sleep(1.0)
    shell.read_for(1.0)  # discard the prompt/banner

    for topic in TOPICS:
        print(f"\n=== listener {topic} ===")
        shell.write(f"listener {topic} -n 1\n")
        output = shell.read_for(3.0)
        print(output if output.strip() else "(no output)")

    shell.close()


if __name__ == "__main__":
    address = sys.argv[1] if len(sys.argv) > 1 else "udpin:0.0.0.0:14550"
    main(address)
