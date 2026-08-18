"""One-off validation of the 2026-07-28 qualification-gate ULog against plan Section 5.3/8.

Run from HILDiagnostics/qualification_gate_20260728/.
"""
import numpy as np
from pyulog import ULog

ULOG_PATH = "qual_gate_retry_log60.ulg"
SIPHOG_DEVTYPE = 0x13
REQUESTED_DURATION_S = 360.0

u = ULog(ULOG_PATH)

duration_s = (u.last_timestamp - u.start_timestamp) / 1e6
print(f"ULog total duration: {duration_s:.2f}s (requested {REQUESTED_DURATION_S:.0f}s, "
      f"delta {duration_s - REQUESTED_DURATION_S:+.2f}s)")

siphog = None
for d in u.data_list:
    if d.name != "vehicle_imu":
        continue
    gyro_ids = set(int(x) for x in d.data["gyro_device_id"])
    devtypes = {(gid >> 16) & 0xFF for gid in gyro_ids}
    if SIPHOG_DEVTYPE in devtypes:
        siphog = d
        print(f"Found SiPhOG (devtype 0x13) at vehicle_imu instance {d.multi_id}, gyro_device_id(s)={gyro_ids}")

assert siphog is not None, "SiPhOG devtype 0x13 NOT FOUND in vehicle_imu -- FAIL"

data = siphog.data
n = len(data["timestamp"])
print(f"\nSample count: {n}")

ts = data["timestamp"]
dts = np.diff(ts)
monotonic = np.all(dts > 0)
print(f"Timestamps monotonic increasing: {monotonic}")
print(f"timestamp dt: mean={dts.mean():.1f}us, min={dts.min()}us, max={dts.max()}us, std={dts.std():.1f}us")

effective_rate_hz = 1e6 / dts.mean()
print(f"Effective sample rate: {effective_rate_hz:.2f} Hz (X3_ODATA_RATE param = 100 Hz)")

gap_threshold_us = 3 * dts.mean()
gaps = dts[dts > gap_threshold_us]
print(f"Gaps (> 3x mean interval): {len(gaps)} found" + (f", max={gaps.max()}us" if len(gaps) else ""))

da_dt = data["delta_angle_dt"]
dv_dt = data.get("delta_velocity_dt", None)
invalid_da_dt = np.sum(da_dt <= 0)
print(f"delta_angle_dt: min={da_dt.min()}, max={da_dt.max()}, invalid(<=0) count={invalid_da_dt}")

da_keys = [k for k in data.keys() if k.startswith("delta_angle[")]
nonfinite = sum(int(np.sum(~np.isfinite(data[k]))) for k in da_keys)
print(f"Non-finite delta_angle samples (across {da_keys}): {nonfinite}")

clip = data["delta_angle_clipping"]
clip_nonzero = np.sum(clip != 0)
print(f"delta_angle_clipping nonzero count: {clip_nonzero} / {n}")

print(f"\nGap timing (index, gap_us, timestamp_s_from_start):")
gap_idx = np.where(dts > gap_threshold_us)[0]
for i in gap_idx:
    print(f"  idx={i}, gap={dts[i]}us, t={(ts[i]-ts[0])/1e6:.2f}s")

gyro_cal = data["gyro_calibration_count"]
print(f"gyro_calibration_count: unique values = {sorted(set(int(x) for x in gyro_cal))}")

# Temperature comes from sensor_gyro, not vehicle_imu -- find the matching instance.
gyro_topic = None
for d in u.data_list:
    if d.name == "sensor_gyro":
        ids = set(int(x) for x in d.data["device_id"])
        devtypes = {(gid >> 16) & 0xFF for gid in ids}
        if SIPHOG_DEVTYPE in devtypes:
            gyro_topic = d
if gyro_topic is not None:
    temp = gyro_topic.data["temperature"]
    print(f"\nSiPhOG temperature: min={temp.min():.2f}C, max={temp.max():.2f}C, "
          f"drift={temp.max()-temp.min():.2f}C, mean={temp.mean():.2f}C")
else:
    print("\nWARNING: no matching sensor_gyro instance found for temperature")

# Rate/duration tolerance check
rate_ok = abs(effective_rate_hz - 100.0) < 5.0
duration_ok = abs(duration_s - REQUESTED_DURATION_S) < 10.0
print("\n--- PASS/FAIL summary ---")
print(f"duration within tolerance: {duration_ok}")
print(f"timestamps monotonic: {monotonic}")
print(f"rate within tolerance of X3_ODATA_RATE: {rate_ok}")
print(f"zero gaps > 3x mean interval: {len(gaps) == 0}")
print(f"zero invalid delta_angle_dt: {invalid_da_dt == 0}")
print(f"zero non-finite samples: {nonfinite == 0}")
print(f"zero clipping: {clip_nonzero == 0}")
