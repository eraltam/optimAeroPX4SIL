# Qualification Gate — Persistent `LOGGER_BUF=384` Fix, Fresh Boot, No Manual NSH Override

**Date:** 2026-07-28
**Purpose:** Confirm the `LOGGER_BUF=384` fix baked into firmware commit `388ced1441` (ROMFS
`1003_optim_aero_hex.hil`) survives a completely fresh boot with **no manual NSH intervention** —
the real point of rebuilding/reflashing, per `PLAN_STATIC_A1_REPEAT_SIPHOG_LONGDURATION.md`
Section 0.3 item 2 and Section 0.5.

## Firmware under test

- Artifact: `HILDiagnostics/firmware/cubepilot_cubeorangeplus_388ced1441_static_siphog_test.px4`
- Artifact SHA-256: `0a57c15ba9348395fdd422e1335d598af26de9d40286d27aa544271e43249f2a`
- Live git hash verified via MAVLink (`verify_firmware_version.py`): `388ced1441` match, PASS
- Flashed via QGroundControl's own firmware-upload UI (not `px_uploader.py` — see plan Section 0.4
  for why the flashing mechanism changed)
- Board: Cube Orange+, `board_version=4184`

## Post-flash parameters confirmed correct (Section 3.4), no override needed

- `SYS_AUTOSTART = 1003`
- `SENS_ANELLX3_CFG = 102`
- `SYS_HITL = 0` (this artifact was built with the airframe's `SYS_HITL` force patched out at
  build time — no explicit override was needed, unlike every prior artifact; see plan Section 0.5)

## SiPhOG channel validation (`anello_siphog_probe.py`)

`anello_x3 status`: 0 com errors, 0 poll errors, 0 checksum bad, ~100 Hz publish on accel/optical
gyro/MEMS gyro streams. `vehicle_imu` instance mapping unchanged from every prior session:
instance 0 = Cube onboard (`0x34`), instance 1 = SiPhOG gyro (`0x13`) + onboard accel pairing
artifact, instance 2 = ANELLO MEMS pair (`0x12`), instance 3 = never published. Clipping zero on
all instances checked.

## Qualification-gate capture

- Session: `ulog_session.py`, no manual `-b`/`LOGGER_BUF` NSH override — fix is baked into ROMFS.
- Log id 63, board-reported size 33,750,221 bytes, requested duration 360s, actual capture window
  364.3s (script-measured wall clock; ULog's own recorded duration below).
- Downloaded file: `static_a1_qualgate_persistent_flash_log63.ulg`
- SHA-256: `8ff33d6944450fb170fc9840ff0212acd58f030fb5057a0ca72ca306e96c1fad`

### Validation (`validate_qual_gate.py`, same script/methodology as `qualification_gate_20260728_retry3`)

```
ULog total duration: 365.10s (requested 360s, delta +5.10s)
Found SiPhOG (devtype 0x13) at vehicle_imu instance 1, gyro_device_id(s)={1245197}

Sample count: 36177
Timestamps monotonic increasing: True
timestamp dt: mean=10091.1us, min=5028us, max=20774us, std=1675.5us
Effective sample rate: 99.10 Hz (X3_ODATA_RATE param = 100 Hz)
Gaps (> 3x mean interval): 0 found
delta_angle_dt: min=9866, max=19999, invalid(<=0) count=0
Non-finite delta_angle samples: 0
delta_angle_clipping nonzero count: 0 / 36177
gyro_calibration_count: unique values = [0]
SiPhOG temperature: min=35.44C, max=36.29C, drift=0.85C, mean=35.87C

--- PASS/FAIL summary ---
duration within tolerance: True
timestamps monotonic: True
rate within tolerance of X3_ODATA_RATE: True
zero gaps > 3x mean interval: True
zero invalid delta_angle_dt: True
zero non-finite samples: True
zero clipping: True
```

**Verdict: PASS.** Zero dropout events, zero gaps, effective rate 99.10 Hz (within the ±5 Hz
tolerance of the 100 Hz `X3_ODATA_RATE` setting), all other checks clean.

### Open discrepancy vs. the hand-applied-buffer retry3 result (not a failure, worth tracking)

`qualification_gate_20260728_retry3` (same `-b 384` value, applied by hand over NSH that session)
reported timestamp-dt std of 478.1us and effective rate exactly 100.00 Hz. This session's baked-in
firmware, same buffer size, reports a higher std of 1675.5us and a slightly lower effective rate of
99.10 Hz, plus a wider min/max spread (5028-20774us vs. a presumably tighter band in retry3 — see
retry3's own MANIFEST for its exact min/max). All gate criteria still pass with margin (max gap
well under the 3x-mean threshold), so this is **not** a stop condition, but the jitter increase is
real and unexplained — candidates worth checking if it recurs or worsens on a longer run: thermal
warm-up state at capture start (this session's SD card/logger was cold — 63 pre-existing logs vs.
whatever retry3's board state was), or a difference between a hand-applied runtime `LOGGER_BUF` set
via NSH vs. the same value taking effect from ROMFS boot-time. Not investigated further here;
flag for Phase 7's first real duration if it shows up again.

## Conclusion

Phase 6 (Allan-deviation conversion pipeline) and Phase 7 (5-duration unattended campaign) are now
unblocked per plan Section 0.2 — the `LOGGER_BUF` persistence follow-up that was blocking them is
resolved and confirmed on a genuinely fresh boot with zero manual intervention.
