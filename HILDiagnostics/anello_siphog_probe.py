"""Section 4 qualification probe: validate the real ANELLO X3 SiPhOG channel over the NSH shell.

Adapted from mavlink_shell_probe.py's non-interactive MAVLink SERIAL_CONTROL shell wrapper, but
pointed at a direct serial connection (not the QGC UDP mirror) since this is a standalone static
bench session with no Simulink/QGC relay involved.

Checks, per PLAN_STATIC_A1_REPEAT_SIPHOG_LONGDURATION.md Section 4:
  - `anello_x3 status` reports a healthy driver
  - both X3 MEMS (devtype 0x12) and optical (devtype 0x13) identities are present
  - `listener sensor_accel/sensor_gyro -n 1` for each vehicle_imu instance, to see gyro_device_id

Usage:
    py -3.12 anello_siphog_probe.py --port COM4 --baud 57600
"""
import argparse
import sys
import time

from pymavlink import mavutil


class MavlinkShell:
    def __init__(self, address, baud, devnum=10):
        device = f"{address},{baud}"
        self.mav = mavutil.mavlink_connection(device, autoreconnect=True)
        # Confirmed 2026-07-28 (PLAN_STATIC_A1_REPEAT_SIPHOG_LONGDURATION.md Section 0.5): with
        # SYS_AUTOSTART=1003 active, the board stays silent (DTR/RTS notwithstanding) until it
        # sees a HEARTBEAT from us -- force DTR/RTS and keep announcing ourselves until one lands,
        # rather than a single heartbeat_send() + one passive wait.
        self.mav.port.dtr = True
        self.mav.port.rts = True
        hb = None
        end = time.time() + 20
        while hb is None and time.time() < end:
            self.mav.mav.heartbeat_send(mavutil.mavlink.MAV_TYPE_GENERIC, mavutil.mavlink.MAV_AUTOPILOT_INVALID, 0, 0, 0)
            hb = self.mav.wait_heartbeat(timeout=2)
        if hb is None:
            raise TimeoutError(f"no heartbeat on {address}")
        self.port = devnum

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

    def run(self, cmd, read_s=2.0):
        self.write(cmd + "\n")
        return self.read_for(read_s)

    def close(self):
        self.mav.mav.serial_control_send(self.port, 0, 0, 0, 0, [0] * 70)
        # BUG FIX 2026-07-28: this only disabled the NSH serial-control passthrough over MAVLink,
        # it never released the underlying pyserial handle -- the port stayed open, so a
        # subsequent fresh connection to the same COM port failed with a Windows
        # "Access is denied" PermissionError. Always release the actual connection too.
        self.mav.close()


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--port", default="COM4")
    parser.add_argument("--baud", type=int, default=57600)
    args = parser.parse_args()

    print(f"Connecting MAVLink shell probe to {args.port} @ {args.baud} ...")
    shell = MavlinkShell(args.port, args.baud)
    print("Connected. Priming shell...")
    shell.write("\n")
    time.sleep(1.0)
    shell.read_for(1.0)

    commands = [
        ("anello_x3 status", 3.0),
        ("listener sensor_accel -n 3", 3.0),
        ("listener sensor_gyro -n 3", 3.0),
        ("listener sensor_mag -n 3", 3.0),
        ("listener vehicle_imu -n 3", 3.0),
        ("param show X3_*", 3.0),
    ]
    for cmd, read_s in commands:
        print(f"\n=== {cmd} ===")
        out = shell.run(cmd, read_s)
        print(out if out.strip() else "(no output)")

    shell.close()


if __name__ == "__main__":
    main()
