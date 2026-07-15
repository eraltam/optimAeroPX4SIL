"""Read-only, no-arm confirmation of the flight_mode_manager root cause found in
watch_gate_02: is `1003_optim_aero_hex.hil` (and pwm_out_sim) actually compiled into this
board's firmware ROMFS, or was it built from a board config that excludes the `.hil`
airframes (gated behind CONFIG_MODULES_SIMULATION_PWM_OUT_SIM, default n -- see the
2026-07-15 source audit in HIL_ZERO_THRUST_AND_PARAM_RELIABILITY_NEXT_STEPS.md section 3.5)?

Connects directly over the serial port at the board's normal HITL baud rate -- no
Simulink/HIL_SENSOR feed needed, so this is safe to run standalone with the board just
powered on, no arming risk at all. Reuses mavlink_shell_probe.py's write/read_for pattern
but constructs its own connection so a real baud rate can be passed (MavlinkShell's fixed
constructor has no baud parameter).
"""
import sys
import time
from pymavlink import mavutil

COMMANDS = [
    "pwm_out_sim status",
    "ls /etc/init.d/airframes/ | grep 1003",
    "param show SYS_AUTOSTART",
    'dmesg | grep -i -E "airframe|autostart|vehicle_type"',
]


def write(mav, port, s):
    b = s.encode("utf-8")
    while len(b) > 0:
        n = min(len(b), 70)
        chunk = list(b[:n]) + [0] * (70 - n)
        mav.mav.serial_control_send(
            port,
            mavutil.mavlink.SERIAL_CONTROL_FLAG_EXCLUSIVE | mavutil.mavlink.SERIAL_CONTROL_FLAG_RESPOND,
            0, 0, n, chunk,
        )
        b = b[n:]


def read_for(mav, duration_s):
    deadline = time.time() + duration_s
    out = []
    while time.time() < deadline:
        m = mav.recv_match(type="SERIAL_CONTROL", blocking=True, timeout=0.2)
        if m is not None and m.count:
            out.append(bytes(m.data[: m.count]).decode("utf-8", errors="replace"))
    return "".join(out)


def main(port_name, baud):
    print(f"Connecting boot-config probe directly to {port_name} @ {baud} baud...")
    mav = mavutil.mavlink_connection(port_name, baud=baud, autoreconnect=True)
    mav.mav.heartbeat_send(mavutil.mavlink.MAV_TYPE_GENERIC, mavutil.mavlink.MAV_AUTOPILOT_INVALID, 0, 0, 0)
    mav.wait_heartbeat(timeout=15)
    print("Connected. Priming shell...")
    devnum = 10
    write(mav, devnum, "\n")
    time.sleep(1.0)
    read_for(mav, 1.0)  # discard the prompt/banner

    for cmd in COMMANDS:
        print(f"\n=== {cmd} ===")
        write(mav, devnum, cmd + "\n")
        output = read_for(mav, 3.0)
        print(output if output.strip() else "(no output)")

    mav.mav.serial_control_send(devnum, 0, 0, 0, 0, [0] * 70)
    mav.close()


if __name__ == "__main__":
    port_name = sys.argv[1] if len(sys.argv) > 1 else "COM4"
    baud = int(sys.argv[2]) if len(sys.argv) > 2 else 921600
    main(port_name, baud)
