"""Watch the raw HIL_SENSOR / HIL_ACTUATOR_CONTROLS wire messages directly on the
UDP mirror, sidestepping any ambiguity about whether this PX4 version mirrors
actuator_outputs_sim onto the plain actuator_outputs/SERVO_OUTPUT_RAW telemetry
path (confirmed on real hardware 2026-07-06: it does not - live_monitor.py's
SERVO_OUTPUT_RAW read 0.0 Hz the whole run even though HIL_ACTUATOR_CONTROLS was
flowing at ~21 Hz on this same mirror the whole time). This is the more direct
answer to "is the controller actually responding" for the interim USB-only phase
(section 1a) - see section 14.6a.

Also confirmed: HIL_SENSOR/HIL_GPS (the Simulink -> PX4 direction) never appear on
this mirror, only the PX4 -> Simulink direction does - the relay is one-way, not
bidirectional as section 10 originally assumed.
"""
import sys, time, collections
from pymavlink import mavutil

mav = mavutil.mavlink_connection(sys.argv[1] if len(sys.argv) > 1 else "udpin:0.0.0.0:14550")
counts = collections.Counter()
start = time.time()
duration = float(sys.argv[2]) if len(sys.argv) > 2 else 15.0
while time.time() - start < duration:
    msg = mav.recv_match(blocking=True, timeout=1)
    if msg is not None:
        counts[msg.get_type()] += 1

elapsed = time.time() - start
print(f"Over {elapsed:.1f}s:")
for name in ["HEARTBEAT", "HIL_SENSOR", "HIL_GPS", "HIL_ACTUATOR_CONTROLS", "HIL_STATE_QUATERNION"]:
    print(f"  {name:24s} count={counts.get(name, 0):5d}  ({counts.get(name, 0)/elapsed:.1f} Hz)")
print("\nAll message types seen:", dict(counts))
