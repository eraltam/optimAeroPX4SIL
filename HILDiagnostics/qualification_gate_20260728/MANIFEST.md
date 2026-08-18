# Qualification gate session — 2026-07-28 — RESULT: FAILED (logger dropout)

## Firmware / parameters
- Firmware git hash (live, verified): `dda0ed3dd6` (matches archived artifact `cubepilot_cubeorangeplus_dda0ed3dd6_static_siphog_test.px4`)
- `SYS_AUTOSTART=1003`, `SENS_ANELLX3_CFG=102`, `SYS_HITL=0` (fixed this session, was stale at 1), `SDLOG_MODE=0`, `SDLOG_PROFILE=1`
- `X3_ODATA_RATE=100`, `X3_LPA=X3_LPW=X3_LPO=45` (driver defaults, not yet reviewed)
- See `params_pre_capture.txt`

## Capture
- Log ID 59, file `qual_gate_6min_log59.ulg`
- Requested duration 360s, actual 364.96s
- Size 32,613,336 bytes (SHA-256 in `sha256.txt`), rate ≈ 89.5 KB/s ≈ 322 MB/hour
- SiPhOG (devtype `0x13`) confirmed at `vehicle_imu` instance 1, gyro_device_id=1245197, 35236 samples
- Temperature: 35.99-36.60°C, drift 0.61°C over the session (thermal warm-up pre-check: stable within 0.07°C/60s before starting)
- Effective sample rate 96.62 Hz (within tolerance of `X3_ODATA_RATE=100`)
- Timestamps monotonic: yes
- Clipping: zero
- `gyro_calibration_count`: constant (0) throughout -- stable

## FAILURE: logger dropouts

33 dropout events recorded in the ULog's own binary DROPOUT records, up to **1830 ms**, clustered
in ~4 bursts roughly every 70-90 seconds (around t=62s, 153s, 226s, 318s). These line up exactly
with 18 large gaps (up to ~1.85s) found in the reconstructed `vehicle_imu` (SiPhOG) timestamp
series.

**Root-caused to the PX4 logger, not the ANELLO sensor/driver.** `anello_x3 status`, queried
immediately after the capture, shows the driver's own internal publish-interval statistics
(accumulated since boot, spanning the whole capture) as clean: `avg=9999.6us,
min=9868-9870us, max=10124-10126us` for all three internal streams (accel, optical gyro, MEMS
gyro), with `com_err=0`, `poll error=0`, `read error=0`, `checksum bad=0`. The sensor/driver
never saw a gap anywhere close to 1.8s. The data loss happens between uORB publication and the
logger's SD-card write.

Likely cause: `ROMFS/px4fmu_common/init.d/rcS` sets the default logger buffer
(`LOGGER_BUF`) to only **8 KiB** system-wide (no airframe-specific override for
`1003_optim_aero_hex.hil`), consumed by `rc.logging`'s `logger start -b ${LOGGER_BUF} -t
${LOGGER_ARGS}`. Combined with `SDLOG_MODE`'s default "all" profile (110 subscriptions) and
whatever this specific microSD card's sustained/periodic write latency looks like (consumer SD
cards commonly have periodic multi-hundred-ms garbage-collection stalls), an 8 KiB ring buffer
is plausibly too small to absorb an SD write stall without dropping data. Note: today's manual
session actually ran with a 12 KiB buffer (the bare CLI default, from restarting the daemon
by hand mid-session while debugting on/off vs start/stop -- see ulog_session.py's docstring),
which is *larger* than the stock 8 KiB boot default -- so a fresh, un-touched boot would likely
show equal or worse dropout behavior, not better.

## Per Section 11 of the plan

"ULog shows unacceptable loss, gaps, clipping or calibration changes" is an explicit stop
condition. This qualification session does not pass. Do not proceed to Phase 7 (the long
multi-hour campaign) until this is resolved and a qualification re-run passes clean.

## Suggested next steps (not yet actioned, pending user direction)

1. Increase `LOGGER_BUF` substantially (a `param set-default`-style override or a rc.logging /
   airframe-level change) and re-run this same 5-10 minute qualification gate.
2. Consider a lighter `SDLOG_PROFILE`/logging topic set than "all" if only the SiPhOG channel
   (plus minimal health telemetry) is actually needed for the Allan-deviation analysis --
   reduces sustained write bandwidth pressure directly.
3. Independently check this specific microSD card's sustained write performance (a slow/aging
   card would explain periodic stalls regardless of buffer size).
