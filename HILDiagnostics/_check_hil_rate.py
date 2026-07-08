import sys, time
from pymavlink import mavutil

# Checks the actual delivery rate/gaps of HIGHRES_IMU (mirrors HIL_SENSOR ingestion) over the UDP
# mirror, to see if the HIL link is falling behind real time (which would explain PX4-side sensor
# timeouts and repeated "GCS connection regained" messages).
mav = mavutil.mavlink_connection(sys.argv[1] if len(sys.argv) > 1 else "udpin:0.0.0.0:14550")
hb = mav.wait_heartbeat(timeout=10)
if hb is None:
    print("NO HEARTBEAT within 10s -- link is down")
    sys.exit(1)
print(f"Heartbeat OK: sysid={mav.target_system} compid={mav.target_component}")

window_s = float(sys.argv[2]) if len(sys.argv) > 2 else 10.0
deadline = time.time() + window_s
timestamps = []
gaps_over_100ms = []
last_t = None
while time.time() < deadline:
    m = mav.recv_match(type="HIGHRES_IMU", blocking=True, timeout=1)
    if m is None:
        continue
    now = time.time()
    timestamps.append((now, m.time_usec))
    if last_t is not None:
        gap = now - last_t
        if gap > 0.1:
            gaps_over_100ms.append(gap)
    last_t = now

n = len(timestamps)
print(f"\nReceived {n} HIGHRES_IMU messages over {window_s:.1f}s wall time "
      f"({n / window_s:.1f} Hz average)")
if n >= 2:
    span_usec = timestamps[-1][1] - timestamps[0][1]
    print(f"PX4-side time_usec span: {span_usec/1e6:.3f}s vs wall-clock span: "
          f"{timestamps[-1][0]-timestamps[0][0]:.3f}s "
          f"(ratio={((timestamps[-1][0]-timestamps[0][0])/(span_usec/1e6)) if span_usec else float('nan'):.3f}, "
          f"1.0 = perfect real-time lockstep, <1.0 = PX4 clock running slow vs wall clock, "
          f">1.0 = PX4 clock running fast)")
print(f"Gaps >100ms between consecutive messages: {len(gaps_over_100ms)}")
if gaps_over_100ms:
    print(f"  max gap: {max(gaps_over_100ms):.3f}s, all gaps: {[round(g,3) for g in gaps_over_100ms]}")
