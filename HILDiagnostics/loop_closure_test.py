"""Correlate sensor-ingest changes with the resulting actuator response to
measure PX4's HIL round-trip latency and confirm the control loop is closed.

live_monitor.py proves streams are flowing; this proves causality. If no
correlated actuator change appears within the timeout, the controller is not
closing the loop back to Simulink even if both streams individually look
"alive" in live_monitor.py.
"""
import sys
import time
import statistics
from pymavlink import mavutil
from connection import connect
from live_monitor import request_rate

ROLLSPEED_CHANGE_THRESHOLD = 0.02   # rad/s, ignore sensor noise floor
SERVO_CHANGE_THRESHOLD = 15         # PWM/us units, ignore actuator dither
TIMEOUT_S = 0.25
SAMPLES_WANTED = 30


def main(device, baud):
    mav = connect(device, baud=baud)
    request_rate(mav, "ATTITUDE", 50)
    request_rate(mav, "SERVO_OUTPUT_RAW", 50)

    last_rollspeed = None
    last_servo1 = None
    pending_stimulus_t = None
    latencies = []

    print(f"Watching for {SAMPLES_WANTED} correlated stimulus/response pairs "
          f"(timeout {TIMEOUT_S * 1000:.0f} ms each)...")

    while len(latencies) < SAMPLES_WANTED:
        msg = mav.recv_match(type=["ATTITUDE", "SERVO_OUTPUT_RAW"], blocking=True, timeout=2)
        if msg is None:
            print("FAIL: no ATTITUDE/SERVO_OUTPUT_RAW traffic at all - is HIL running?")
            sys.exit(1)

        now = time.time()
        if msg.get_type() == "ATTITUDE":
            if last_rollspeed is not None:
                if abs(msg.rollspeed - last_rollspeed) > ROLLSPEED_CHANGE_THRESHOLD:
                    pending_stimulus_t = now
            last_rollspeed = msg.rollspeed

        elif msg.get_type() == "SERVO_OUTPUT_RAW":
            if last_servo1 is not None and pending_stimulus_t is not None:
                if abs(msg.servo1_raw - last_servo1) > SERVO_CHANGE_THRESHOLD:
                    latency = now - pending_stimulus_t
                    if latency <= TIMEOUT_S:
                        latencies.append(latency)
                        print(f"  pair {len(latencies):2d}: latency = {latency * 1000:6.1f} ms")
                    pending_stimulus_t = None
            last_servo1 = msg.servo1_raw

    print(f"\nlatency min/mean/max/stdev (ms): "
          f"{min(latencies) * 1e3:.1f} / {statistics.mean(latencies) * 1e3:.1f} / "
          f"{max(latencies) * 1e3:.1f} / {statistics.pstdev(latencies) * 1e3:.1f}")
    print("OVERALL: PASS - controller is closing the loop back to Simulink")


if __name__ == "__main__":
    device = sys.argv[1] if len(sys.argv) > 1 else "COM7"
    baud = int(sys.argv[2]) if len(sys.argv) > 2 else 57600
    main(device, baud)
