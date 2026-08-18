# Firmware build manifest — static SiPhOG bench-test image (v3, adds LOGGER_BUF=384)

| Field | Value |
|---|---|
| Artifact file | `cubepilot_cubeorangeplus_388ced1441_static_siphog_test.px4` |
| SHA-256 | `0a57c15ba9348395fdd422e1335d598af26de9d40286d27aa544271e43249f2a` |
| Source tree | `Anello-Photonics/PX4-Autopilot.git`, branch `anello-dev_edison_working_branch` |
| Git commit (committed, pushed) | `388ced1441` — "Set LOGGER_BUF=384 in the HIL bench airframe to fix logger dropouts" |
| `git_identity` (embedded) | `v1.3.16-26-g388ced1441` |
| `board_id` (embedded) | `1063` (Cube Orange+) |
| `image_size` | 1,909,644 bytes (97.13% flash used) |
| Build target | `make cubepilot_cubeorangeplus_default` |
| Build host | WSL (`/home/edison/PX4-Autopilot`) |
| Build date | 2026-07-28 |

Supersedes `cubepilot_cubeorangeplus_dda0ed3dd6_static_siphog_test.px4`: that build passed
Phase 4 flash verification and Section 4 SiPhOG-channel validation, but its qualification-gate
run (Section 8) failed with logger dropouts (up to 1.83s, worsening to 2.95s with a manually
bumped 96 KiB buffer) traced to the stock `LOGGER_BUF=8` (KiB) default. See
`PLAN_STATIC_A1_REPEAT_SIPHOG_LONGDURATION.md` Sections 5/8 and
`qualification_gate_20260728_retry3/MANIFEST.md` for the full investigation.

## What changed in this build

Single-line airframe change: `set LOGGER_BUF 384` added to
`ROMFS/px4fmu_common/init.d/airframes/1003_optim_aero_hex.hil`, which runs before `rc.logging`
consumes that variable at boot (confirmed by reading `rcS`'s boot order — airframe autostart
runs around line 224, `rc.logging` around line 508). This makes the fix that was previously only
applied by hand over NSH each session (and confirmed clean via a live 6-minute capture: 0
dropouts, 0 gaps, exactly 100.00 Hz) permanent across reboots/power-cycles, which the Phase 7
unattended campaign requires (it power-cycles the board between every duration).

## Deliberate, uncommitted local patch present in THIS build only

Same convention as the prior two builds: `param set SYS_HITL 1` locally commented out (not
committed, not pushed) before this build, for the static-bench use case only. Reverted in the
source tree immediately after building — committed tree has `SYS_HITL=1` restored (confirmed via
`git status --short` showing clean after the revert).

## Not yet done

- Not yet flashed to the physical Cube Orange+ (in progress).
- `X3_LPA`/`X3_LPW`/`X3_LPO`/`X3_ODATA_RATE` still at driver defaults (100 Hz, 45 Hz LPF) — still
  not reviewed against qualification-gate findings, unchanged from the prior two builds.
