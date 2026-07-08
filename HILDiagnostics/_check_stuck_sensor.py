import sys, time
from pymavlink import mavutil

# Samples HIGHRES_IMU repeatedly over a stationary window and checks whether consecutive
# samples are suspiciously identical (a frozen/stale HIL_SENSOR payload -- e.g. a stuck Rate
# Transition block on the Simulink side) vs. genuinely varying sensor data.
mav = mavutil.mavlink_connection(sys.argv[1] if len(sys.argv) > 1 else "udpin:0.0.0.0:14550")
hb = mav.wait_heartbeat(timeout=10)
if hb is None:
    print("NO HEARTBEAT within 10s -- link is down")
    sys.exit(1)
print(f"Heartbeat OK: sysid={mav.target_system} compid={mav.target_component} "
      f"armed={bool(hb.base_mode & 128)}")

n_samples = int(sys.argv[2]) if len(sys.argv) > 2 else 10
gap_s = float(sys.argv[3]) if len(sys.argv) > 3 else 2.0

samples = []
deadline_total = time.time() + n_samples * (gap_s + 3)
while len(samples) < n_samples and time.time() < deadline_total:
    m = mav.recv_match(type="HIGHRES_IMU", blocking=True, timeout=3)
    if m is None:
        continue
    samples.append((m.time_usec, m.xgyro, m.ygyro, m.zgyro, m.xacc, m.yacc, m.zacc))
    print(f"t_usec={m.time_usec:14d} xgyro={m.xgyro:+.6f} ygyro={m.ygyro:+.6f} zgyro={m.zgyro:+.6f} "
          f"xacc={m.xacc:+.5f} yacc={m.yacc:+.5f} zacc={m.zacc:+.5f}")
    time.sleep(gap_s)

if len(samples) < 2:
    print(f"\nOnly got {len(samples)} sample(s) -- can't compare, link may be too lossy/contended.")
    sys.exit(1)

identical_pairs = 0
for i in range(1, len(samples)):
    prev, cur = samples[i - 1], samples[i]
    if prev[1:] == cur[1:]:
        identical_pairs += 1

print(f"\n{len(samples)} samples over ~{(samples[-1][0]-samples[0][0])/1e6:.1f}s of wall time "
      f"(gyro/accel fields bit-identical to previous sample: {identical_pairs}/{len(samples)-1})")
if identical_pairs == len(samples) - 1:
    print("VERDICT: STUCK -- gyro/accel payload never changed across all samples despite advancing "
          "timestamps. This points to a frozen HIL_SENSOR payload on the Simulink side (e.g. the "
          "Rate Transition block in PX4HITLConnector.slx), not genuine plant dynamics.")
elif identical_pairs > 0:
    print("VERDICT: PARTIALLY STUCK -- some but not all consecutive samples repeat exactly. Worth a "
          "longer capture to see if it's intermittent freezing.")
else:
    print("VERDICT: DYNAMIC -- values are changing sample to sample, consistent with genuine sensor "
          "noise/motion rather than a frozen payload.")
