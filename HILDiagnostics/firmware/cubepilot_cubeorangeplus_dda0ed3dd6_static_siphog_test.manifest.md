# Firmware build manifest — static SiPhOG bench-test image (v2, adds SENS_ANELLX3_CFG)

| Field | Value |
|---|---|
| Artifact file | `cubepilot_cubeorangeplus_dda0ed3dd6_static_siphog_test.px4` |
| SHA-256 | `8b905f55483d4f79051ebaa8adf28994b8d8ea81c697e7e563e272d315307d4d` |
| Source tree | `Anello-Photonics/PX4-Autopilot.git`, branch `anello-dev_edison_working_branch` |
| Git commit (committed, pushed) | `dda0ed3dd6072249fa130962eed46e5d230d0f92` — "Set SENS_ANELLX3_CFG=102 (TELEM2) in the HIL bench airframe" |
| `git_identity` (embedded) | `v1.3.16-25-gdda0ed3dd6` |
| `board_id` (embedded) | `1063` (Cube Orange+) |
| `image_size` | 1,909,628 bytes (97.13% flash used) |
| Build target | `make cubepilot_cubeorangeplus_default` |
| Build host | WSL (`/home/edison/PX4-Autopilot`) |
| Build date | 2026-07-27 |

Supersedes `cubepilot_cubeorangeplus_4e21bbfe12_static_siphog_test.px4` (deleted): that build
predated confirming the ANELLO X3's real wiring (TELEM2, not TELEM3) and had no
`SENS_ANELLX3_CFG` set at all.

## Confirmed serial port assignment

`SENS_ANELLX3_CFG = 102` ("TELEM 2"), added as `param set-default` in
`ROMFS/px4fmu_common/init.d/airframes/1003_optim_aero_hex.hil`. Value confirmed directly from
this build's own generated parameter metadata (`generated_params/serial_params.c`: `@value 102
TELEM 2`), matching both `power_on_sequence.py`'s own docstring and explicit project-owner
confirmation (2026-07-27) that TELEM2, not TELEM3, is the physical ANELLO wiring.

## Deliberate, uncommitted local patch present in THIS build only

Same as the prior build: `param set SYS_HITL 1` locally commented out (not committed, not
pushed) before this build, for the static-bench use case only. Reverted in the source tree
immediately after building — committed tree has `SYS_HITL=1` restored.

## Confirmed present in this build

- `SENS_ANELLX3_CFG` param present and defaulted to `102` in the built ROMFS.
- ANELLO driver symbols compiled in (verified in the prior build; unchanged here — only the
  ROMFS airframe script changed, all C++ objects were reused from cache).

## Not yet done

- Not yet flashed to the physical Cube Orange+. Existing rollback image remains
  `cubepilot_cubeorangeplus_VTOL_1003_optim_aero_hex.px4` (git_hash
  `99c40407ffd7ac184e2d7b4b293f36f10fe561ef`, provenance unknown per Section 3.1 of the plan).
- `X3_LPA`/`X3_LPW`/`X3_LPO`/`X3_ODATA_RATE` left at driver defaults (100 Hz, 45 Hz LPF) —
  not yet reviewed against qualification-gate findings.
