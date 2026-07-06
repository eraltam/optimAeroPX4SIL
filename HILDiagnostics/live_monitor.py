"""Run concurrently with an active Simulink HIL sim to watch PX4's live state.

If only micro-USB is wired (no TELEM2 adapter, section 1a), this cannot open the
same port Simulink is holding. Point 'device' at the passive UDP mirror instead
(e.g. 'udpin:0.0.0.0:14560') once the MAVLink Bridge Sink's FieldTable mask is
configured with a second destination row - see section 14.1.
"""
import sys
import time
import collections
from pymavlink import mavutil
from connection import connect

STREAMS = {
    "HEARTBEAT": 1,
    "SYS_STATUS": 2,
    "ATTITUDE": 20,
    "HIGHRES_IMU": 20,
    "LOCAL_POSITION_NED": 10,
    "GPS_RAW_INT": 5,
    "SERVO_OUTPUT_RAW": 20,
    "EXTENDED_SYS_STATE": 2,
    "ESTIMATOR_STATUS": 2,
}

STALE_AFTER_S = 1.0


def request_rate(mav, msg_name, hz):
    msg_id = getattr(mavutil.mavlink, f"MAVLINK_MSG_ID_{msg_name}")
    interval_us = int(1e6 / hz)
    mav.mav.command_long_send(
        mav.target_system, mav.target_component,
        mavutil.mavlink.MAV_CMD_SET_MESSAGE_INTERVAL, 0,
        msg_id, interval_us, 0, 0, 0, 0, 0,
    )


def main(device, baud, duration_s):
    mav = connect(device, baud=baud)
    for name, hz in STREAMS.items():
        request_rate(mav, name, hz)

    counts = collections.Counter()
    last_seen = {}
    start = time.time()
    last_print = start

    while time.time() - start < duration_s:
        msg = mav.recv_match(blocking=True, timeout=0.5)
        now = time.time()
        if msg is not None:
            t = msg.get_type()
            if t in STREAMS:
                counts[t] += 1
                last_seen[t] = now

        if now - last_print >= 1.0:
            elapsed = now - start
            print(f"\n--- t={elapsed:5.1f}s ---")
            for name in STREAMS:
                age = now - last_seen.get(name, start)
                hz = counts[name] / elapsed if elapsed > 0 else 0
                flag = "STALE" if age > STALE_AFTER_S else "ok"
                print(f"  {name:20s} {hz:6.1f} Hz   last seen {age:5.2f}s ago   [{flag}]")
            last_print = now

    stale = [n for n in STREAMS if time.time() - last_seen.get(n, start) > STALE_AFTER_S]
    print("\nOVERALL:", "FAIL - stale streams: " + ", ".join(stale) if stale else "PASS")


if __name__ == "__main__":
    device = sys.argv[1] if len(sys.argv) > 1 else "COM7"
    baud = int(sys.argv[2]) if len(sys.argv) > 2 else 57600
    duration = float(sys.argv[3]) if len(sys.argv) > 3 else 30.0
    main(device, baud, duration)
