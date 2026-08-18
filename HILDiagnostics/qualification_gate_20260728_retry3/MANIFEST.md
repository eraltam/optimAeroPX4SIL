# Qualification gate session — 2026-07-28 (retry 3) — RESULT: PASS

Supersedes `qualification_gate_20260728/MANIFEST.md` (FAILED, logger dropout) and
`qualification_gate_20260728_retry/` (FAILED, `-b 96` still insufficient).

## Root cause and fix

Root cause confirmed quantitatively: sustained ULog write rate here is ~88 KiB/s. Observed
dropout stalls were up to ~3s, requiring a buffer of ~260 KiB+ to absorb without loss --
the stock `LOGGER_BUF=8` (from `ROMFS/px4fmu_common/init.d/rcS`) and even a manually-set 96 KiB
buffer were both far too small. Fixed via NSH `logger stop` + `logger start -b 384 -t`
(384 KiB), well within the board's available free heap (~558 KiB free, ~467 KiB largest
contiguous block at the time -- confirmed via `free` before allocating). Applied at runtime,
not yet made a permanent ROMFS/ default -- **must be reapplied after every fresh boot/relay
power-cycle** until a permanent fix is made (see "Follow-up" below).

## Capture
- Log ID 62, file `buf384_full_log62.ulg`
- Requested duration 360s, actual 365.38s
- Size 33,809,188 bytes, SHA-256 `240dc158f7fd2f0d67586a9393b46334255f1b35f581d1aff25a18ee38fd375c`
- SiPhOG (devtype `0x13`) at `vehicle_imu` instance 1, gyro_device_id=1245197, 36533 samples
- **0 dropout events** (ULog-native), **0 gaps** > 3x mean interval
- Effective sample rate: 100.00 Hz exactly (matches `X3_ODATA_RATE=100`)
- Timestamp dt: mean=10000.2us, min=5026us, max=15334us, std=478.1us (down from std~19000-24000us
  pre-fix)
- Timestamps monotonic: yes
- Clipping: zero
- `gyro_calibration_count`: constant (0), stable
- Temperature: 37.30-37.44C, drift 0.14C over the full 6 minutes

## Progression (for the record)
| Attempt | Buffer | Duration | Dropouts | Max gap |
|---|---|---|---|---|
| `qualification_gate_20260728` (log 59) | 12 KiB (accidental, from manual daemon restart) | 360s | 33 | 1.83s |
| `qualification_gate_20260728_retry` (log 60) | 96 KiB | 360s | not re-checked, gaps still present | 2.95s |
| `qualification_gate_20260728_retry2` (log 61) | 384 KiB | 120s smoke test | 0 | -- |
| `qualification_gate_20260728_retry3` (log 62) | 384 KiB | 360s | **0** | **none** |

## Gate verdict: PASS

Per plan Section 8, this qualification gate now passes cleanly. Phase 6 (Allan-deviation
conversion pipeline) and Phase 7 (long multi-hour campaign) may proceed.

## Follow-up required before Phase 7 (unattended campaign)

`-b 384` was applied by hand over NSH this session and is **not persistent** -- a fresh boot
(and Phase 7's campaign power-cycles the board between every duration) will revert to the
ROMFS default of 8 KiB unless this is fixed properly. Before Phase 7 starts, either:
1. Add a `SDLOG_BUF`-style override or edit `rc.logging`/the airframe script to set
   `LOGGER_BUF=384` (or higher, with margin) permanently and rebuild+reflash, or
2. Have the campaign script (`run_static_allan_campaign.py`, not yet built) explicitly issue
   `logger stop` + `logger start -b 384 -t` immediately after every boot, before starting any
   capture.
Option 1 is more robust (survives any startup path, including a manual bench reboot); option 2
avoids a rebuild/reflash cycle. Not yet decided -- flag for the user before Phase 7 tooling is
built.
