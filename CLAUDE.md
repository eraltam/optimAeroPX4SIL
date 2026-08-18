# optimAeroPX4SIL -- ANELLO X3 IMU Integration

**Status:** Implemented and validated by compile (both variants update_diagram clean). Not yet
validated against live PX4 SITL / EKF2.
**Date:** June 30, 2026
**Goal:** Replace/augment the generic INS sensor model used in `VehicleSilSimulation.slx` with an
ANELLO X3 IMU model derived from the sibling project `../newIMU_model/`, so the PX4 SITL loop can
receive datasheet-accurate ANELLO X3 IMU measurements instead of (or alongside) the generic
ICM-42688-P-like model in `sensors/components/ins.slx`.

This file documents the architecture of both repos, the exact signal/bus contract that had to be
honored, the bugs found and fixed along the way, and what's left for full PX4 validation. The
ANELLO sensor model itself was already built and validated in the sibling project -- see
`../newIMU_model/CLAUDE.md` (repo root) and `../newIMU_model/instruction.md` for its math,
parameters, and 8-test validation suite.

---

## 1. What changed in this repo

| File | Status | Role |
|---|---|---|
| `sensors/components/anelloX3/anelloX3Params_SIL.m` | new | ANELLO datasheet params, `Ts=0.004s`/250Hz (matches SIL `stepSize_s`) instead of the original 200Hz. Kept for documentation/reference only -- **not called from Simulink-facing code**, see section 3. |
| `sensors/components/anelloX3/anelloX3MountParams.m` | new | Copy of `newIMU_model/params/anelloX3MountParams.m` (`r_IMU_CG`, `R_sensor_body`, `gravitySign`). Small flat struct -- safe to call from a MATLAB Function block (unlike the params struct, see section 3). |
| `sensors/components/anelloX3/ANELLO_X3_IMU_fcn_SIL.m` | new | SIL-rate variant of `ANELLO_X3_IMU_fcn.m`. All datasheet parameters **inlined as local constants** rather than read from `anelloX3Params_SIL()` -- required to compile inside a MATLAB Function block, see section 3. |
| `sensors/components/anelloX3/ANELLO_X3_IMU_Vehicle_fcn_SIL.m` | new | SIL-rate variant of `ANELLO_X3_IMU_Vehicle_fcn.m`. Takes `R_bn` (nav->body) directly instead of `R_nb` + internal transpose, matching what `ins.slx`'s `DCM_be` already provides -- see section 4.1 for why. |
| `sensors/components/build_ins_anello.m` | new | Programmatic builder (Simulink API script, same pattern as `newIMU_model/models/build_ANELLO_X3_IMU.m`) for `ins_anello.slx`. Re-run this if the model needs to be rebuilt from scratch. |
| `sensors/components/ins_anello.slx` | new | Drop-in alternative to `ins.slx`: same interface (`EnvironmentBus`, `BodyStatesBus` in; `INSSensorBus` out), built around the ANELLO X3 datasheet model instead of the generic Aerospace IMU block. |
| `sensors/components/wire_ins_variant.m` | new | Script that replaces `sensors/sensors.slx`'s `ins` block with a selector subsystem (see section 5). Re-run if `sensors.slx` needs to be rebuilt. |
| `sensors/components/build_ins_ideal.m` | new (2026-07-21) | Programmatic builder for `ins_ideal.slx` -- the "D1 ideal IMU" variant from `HIL_TEST_STATUS_AND_NEXT_PRIORITIES.md` (repo root). Copies `ins.slx` and strips all sensor error (see section 4.7). |
| `sensors/components/ins_ideal.slx` | new (2026-07-21) | Drop-in third INS choice, `INS_VARIANT=3`: zero-error, zero-dynamics pass-through of plant truth, used as a baseline to separate "IMU noise-model effect" from "any sensor noise vs. none at all" in the existing pairwise (`fase_c7_c8`) HIL comparison. |
| `sensors/sensors.slx` | modified | `ins` is now a small subsystem containing `Generic` (`ins.slx`), `AnelloX3` (`ins_anello.slx`), and `Ideal` (`ins_ideal.slx`) Model blocks feeding a Multiport Switch selected by `INS_VARIANT`. |
| `sensors/setUpSensors.m` | modified | Added `INS_VARIANT = 2;` (1=Generic, 2=AnelloX3 default, 3=Ideal) plus a comment explaining the switch. |
| `sensors/components/ins.slx` | unchanged | Still the generic baseline, selectable via `INS_VARIANT=1`. |

---

## 2. Original signal chain (unchanged structurally)

```
VehicleSilSimulation.slx
  |- vehicleSimulation [Model Ref -> VehiclePlant.slx]
       |- (inside) sensors.slx
            |- gps  [Model Ref -> sensors/components/gps.slx]
            |- ins  [now a selector subsystem -- see section 5]
            |- adc  [Model Ref -> sensors/components/adc.slx]
            |- BusCreator -> SensorsBus { GPSSensorBus, INSSensorBus, ADCSensorBus }
  |- PX4 Interface (in: SensorsBus, VehicleBus, EnvironmentBus)
       |- S-Function: pixhawk_sil_connector
```

`sensors.slx` inputs: `EnvironmentBus`, `VehicleBus`. It `Goto`/`From`-routes pieces of those buses
into `gps`, `ins`, `adc`. The `EnvironmentBus` port type flowing into `ins`/`ins_anello` is
actually the full top-level `EnvironmentBus` (with `EarthEnvironment`/`AirEnvironment`/
`TerrainEnvironment` sub-buses) -- `ins.slx`'s Bus Selector blocks reach into it via dotted paths
like `EarthEnvironment.worldMagneticFieldInNED_nT`, not a pre-selected sub-bus.

### `sensors/components/ins.slx` internals (generic baseline, unchanged)

```
Inputs:  EnvironmentBus (Bus: EnvironmentBus), BodyStatesBus (Bus: BodyStateBus)
Outputs: INSSensorBus

BodyStatesBus -> BusSelector -> {DCM_be, aircraftAccelbe_mps2, aircraftAngVelInBody_radps, aircraftAngAccelInBody_radps2}
EnvironmentBus -> BusSelector -> {EarthEnvironment.worldMagneticFieldInNED_nT, EarthEnvironment.gravityScalar_mps2}

g_body = DCM_be * [0; 0; gravityScalar_mps2]      % NED gravity rotated into body frame

[accel_meas, gyro_meas] = "Three-axis Inertial Measurement Unit" (aerolibnav lib block)
    u1 = aircraftAccelbe_mps2      (Ab)
    u2 = aircraftAngVelInBody_radps (omega)
    u3 = aircraftAngAccelInBody_radps2 (omegaDot)
    u4 = center_of_gravity = CONSTANT [0 0 0]      % no lever arm modeled
    u5 = g_body

mag_body_nT = DCM_be * (worldMagneticFieldInNED_nT * 1e-5)     % nT -> Gauss
mag_meas    = mag_body_nT + 0.01 * BandLimitedWhiteNoise        % magNoise_gauss block

INSSensorBus = { GyroSensorBus, MagSensorBus, AccelSensorBus }   % each device_id/x/y/z/temperature_degC
  device_id: accel=1310988, gyro=1310988, mag=197388
  temperature_degC: hardcoded Constant = 15 for all three sensors
```

### `sensors/components/ins_anello.slx` internals (new ANELLO X3 path)

```
Inputs:  EnvironmentBus (Bus: EnvironmentBus), BodyStatesBus (Bus: BodyStateBus)
Outputs: INSSensorBus

Same BusSelector extraction as ins.slx, plus:
  AirEnvironment.airTemperature_K  -> Bias(-273.15) -> temperature_degC
                                       (used as the T input for all three sensor models --
                                        ins.slx never wired this through, it used a Constant=15)

g_body = DCM_be * [0; 0; gravityScalar_mps2]   % same as ins.slx

[Ameas, omegaMeas, magMeas, accel_bias, gyro_bias, mag_bias] =
    ANELLO_X3_IMU_Vehicle_fcn_SIL(Ab, omega, omegaDot, g_body, mag_nav_G, DCM_be(:), T, reset=0)

  where mag_nav_G = worldMagneticFieldInNED_nT * 1e-5   (nT -> Gauss, same conversion as ins.slx)

Demux each of Ameas/omegaMeas/magMeas into x/y/z, pack into
AccelSensorBus/GyroSensorBus/MagSensorBus with the SAME device_id constants as ins.slx
(1310988/1310988/197388), then INSSensorBus = {Gyro, Mag, Accel} (element order matches
signals/BusDefinition.m).
```

---

## 3. Bug found: MATLAB Function blocks can't compile a call to `anelloX3Params()`-shaped functions

**This is the most important finding from this work and affects the original, already-"validated"
`newIMU_model/models/ANELLO_X3_IMU.slx`, not just this integration.**

In this MATLAB release (R2026a), a MATLAB Function block / Stateflow chart that calls a function
returning a large nested struct (`anelloX3Params()` / `anelloX3Params_SIL()` -- 9 top-level fields,
4 of which are themselves sub-structs with ~10-15 fields each, mixing scalars/vectors/matrices)
fails Simulink's `update_diagram` / compile-time type inference. The error is generic and
misleading:

```
Simulink is unable to determine sizes and/or types of the outputs for block '.../wrap' due to
errors in the block body... Error in port widths or dimensions. 'Output Port 1' of '.../reset' is
a one dimensional vector with 1 elements.
```

It blames an unrelated port (`reset` in our case) rather than the actual struct-returning call.

**Reproduced and confirmed directly against `newIMU_model/models/ANELLO_X3_IMU.slx` itself**
(`set_param('ANELLO_X3_IMU','SimulationCommand','update')` fails with the same error, pointing at
`ANELLO_X3_IMU/Accelerometer_Model/reset`). This means that model -- despite the "Phase 1 Complete,
8/8 tests passing" status in `newIMU_model/CLAUDE.md` -- currently cannot pass a Simulink diagram
update/compile in this MATLAB environment. The Phase 1 test scripts
(`newIMU_model/scripts/run_*_test.m`) almost certainly validated `ANELLO_X3_IMU_fcn.m` by calling
it directly as a plain MATLAB function, never actually compiling the `.slx` model -- which is why
this was never caught. **Recommend filing this as a follow-up item against `newIMU_model`** (not
fixed here, out of scope for this repo) -- either flatten `anelloX3Params.m`'s usage inside
`ANELLO_X3_IMU_fcn.m` the same way done here, or confirm the model actually compiles/sims correctly
under whatever MATLAB version it was originally validated with.

**Bisection done to isolate the cause** (see conversation history / re-derivable by running the
same experiment): a function returning a small flat struct (2-3 fields, no nesting) compiles fine
in a MATLAB Function block. The large nested `anelloX3Params()`-shaped struct does not, regardless
of whether the call result is stored in a `persistent` variable or not, and regardless of
`coder.extrinsic` being declared or not. `anelloX3MountParams()` (3 fields, no nesting) compiles
fine and is used as-is.

**Fix applied here:** `ANELLO_X3_IMU_fcn_SIL.m` does not call `anelloX3Params_SIL()` at all --
every constant is inlined as a local variable, computed with the same formulas. This is the
file that actually executes inside `ins_anello.slx`'s MATLAB Function block.
`anelloX3Params_SIL.m` is kept only as human-readable documentation of those values (e.g. for use
in plain MATLAB test scripts outside Simulink) -- **keep the two in sync by hand** if the ANELLO
datasheet parameters ever change.

---

## 4. Design decisions made during implementation

### 4.1 Rotation convention

`ins.slx`'s `DCM_be` is already the nav->body rotation (it's used directly as
`g_body = DCM_be * g_nav` and `mag_body = DCM_be * mag_nav`). The original
`ANELLO_X3_IMU_Vehicle_fcn.m` (in `newIMU_model`) takes `R_nb` (body->nav) and transposes
internally to get `R_bn`. Rather than transpose `DCM_be` at the call site (a footgun -- easy to
get backwards and silently invert the magnetometer rotation), `ANELLO_X3_IMU_Vehicle_fcn_SIL.m`
was written to take `R_bn` directly. `ins_anello.slx`'s wrapper passes `DCM_be(:)` straight in,
no transpose, by construction.

### 4.2 Lever arm / mounting location

Kept `anelloX3MountParams.m`'s real values (`r_IMU_CG = [0.10; 0.02; -0.03]`, not `[0,0,0]`) rather
than matching `ins.slx`'s hardcoded zero lever arm. This means `ins_anello` and `ins` will diverge
under angular acceleration/rate even with identical noise seeds -- expected and desired, since
modeling the real mount offset is the point.

### 4.3 Sample time

Created `anelloX3Params_SIL.m` with `Ts=0.004s`/250Hz instead of the original 200Hz, matching
`stepSize_s` from `initVehicleSIL.m`. Since the noise/bias-drift formulas are written generically
in terms of `Ts`/`fs` (not hardcoded to 200Hz), this keeps the model datasheet-accurate at the
SIL's actual call rate without touching the underlying physics. (As noted in section 3, this file's
values are now only used for documentation since the MATLAB Function block inlines them directly,
but the inlined constants in `ANELLO_X3_IMU_fcn_SIL.m` use the same `Ts=0.004` basis.)

### 4.4 Temperature input

`ins_anello.slx` wires `EnvironmentBus.AirEnvironment.airTemperature_K` (converted to degC) into
the ANELLO temperature-bias model -- more correct than `ins.slx`'s hardcoded `Constant=15`, and it
turned out to be a one-block addition since the full `EnvironmentBus` (not just the
`EarthEnvironment` sub-bus) was already flowing into the INS submodel's input port.

### 4.5 `reset` input

Tied to `Constant 0` inside `ins_anello.slx` (no reset capability exposed at the `sensors.slx`
level) -- matches `ins.slx`'s implicit "never reset" behavior.

### 4.6 Integration architecture: selector subsystem, not a true Variant Subsystem

The original plan (section 5 of the prior revision of this doc) called for a Simulink **Variant
Subsystem** block. This was attempted first but abandoned: in this MATLAB release, a
`simulink/Ports & Subsystems/Variant Subsystem` block requires every variant choice's port set
(names, count) to match the parent exactly, the GUI offers a "synchronize ports" one-click fix-it
action for mismatches, but **no equivalent programmatic API could be found** to perform that sync
headlessly (`Simulink.SubSystem.convertToVariant` does not exist in this release; no other
candidate API was found in `methods('Simulink.SubSystem')`/`methods('Simulink.VariantManager')`).

**What's implemented instead:** `sensors/ins` is now a plain Subsystem containing two **Model**
blocks (`Generic` -> `ins.slx`, `AnelloX3` -> `ins_anello.slx`), both computed every step, feeding
a **Multiport Switch** selected by the base-workspace double `INS_VARIANT` (1 or 2). Functionally
equivalent to a variant for this use case (one variable controls which INS feeds PX4); the only
real cost is that both INS models compute every step instead of only the active one -- negligible,
both are lightweight. If a true Variant Subsystem is wanted later (e.g. for code-gen exclusion of
the unused branch), revisit with a newer/different MATLAB release or build it interactively via the
GUI's fix-it action rather than headlessly.

### 4.7 `ins_ideal.slx` (D1 "ideal IMU" variant, added 2026-07-21)

The HIL validation plan's Block D1 calls for a mission run with an "ideal IMU" -- a raw pass-through
of plant truth with zero sensor error -- as a baseline to separate "IMU noise-model effect" from
"any sensor noise vs. none at all" (see `HIL_TEST_STATUS_AND_NEXT_PRIORITIES.md`, repo root,
sections 3-5). Rather than re-deriving the specific-force/gravity-mixing formula from scratch (a
sign-convention footgun -- see section 4.1), `ins_ideal.slx` is built by **copying `ins.slx` file-for-file**
(`copyfile` + `load_system`, not `new_system(...,'Model',...)` -- that API expects a subsystem
block handle, not a whole model to duplicate) and then, on the copied `Three-axis Inertial
Measurement Unit` block:
- `dtype_a`/`dtype_g` (2nd-order sensor dynamics for accel/gyro) -> `off`, so there is no lag/filter
  settling time -- confirmed by direct comparison (see below): Generic's accel.z at t=3 samples
  after a step to rest was still settling (-8.77 m/s^2 vs. true -9.80665), Ideal already reads the
  exact value.
- `i_rand` (internal white-noise generator) -> `off`
- `a_bias`/`g_bias`/`g_sens` -> `[0 0 0]`, `a_sf_cc`/`g_sf_cc` -> `eye(3)`, `a_sat`/`g_sat` widened to
  `+-1e6` -- all inlined as literals rather than references to `setUpSensors.m`'s `parameters`
  struct, so this variant stays "ideal" even if Generic's own parameters change later.
- The ad hoc magnetometer noise (`ins.slx`'s `Gain` block feeding the Band-Limited White Noise into
  the mag Add junction) is set to `1e-4` (not `0` -- see the live-HITL finding in section 5: a truly
  zero mag noise made PX4 flag the sensor as stuck) rather than removed structurally, to keep the
  block diagram/port structure identical to `ins.slx`.

Because the block and wiring are otherwise byte-identical to `ins.slx`, the gravity-sign and
specific-force convention is guaranteed identical to Generic by construction -- no independent
derivation needed. Verified numerically (throwaway harness, static rest condition, DCM_be=I,
gravityScalar=9.80665, mag=[200 0 400] nT): Ideal reports `accel.z = -9.80665` exactly and
`mag = [0.002, 0, 0.004]` G exactly (bare nT->Gauss conversion of the input, no noise), gyro exactly
zero on all axes -- matching Generic's sign/units convention with the noise and dynamics-settling
error subtracted out.

Wired into `sensors/sensors.slx`'s `ins` selector subsystem as a third `Model` block (`Ideal` ->
`ins_ideal.slx`) feeding a third data port on the existing `INS_Select` Multiport Switch (`Inputs`
bumped from `2` to `3`); `INS_VARIANT==3` selects it, following the same one-based-contiguous
indexing already used for `1`/`2`. `sensors/setUpSensors.m` documents the new value but the default
is left at `2` (AnelloX3) -- switching to `INS_VARIANT=3` is a deliberate per-run choice for D1
testing, not a new default.

---

## 5. Validation performed

- `model_check` (unconnected ports/lines, Stateflow lint): both `ins_anello.slx` and `sensors.slx`
  report healthy/no issues.
- `update_diagram` compile check on `ins_anello.slx` standalone: **passes**.
- `update_diagram` compile check on `sensors.slx` standalone: passes once the full
  `initVehicleSIL`-equivalent workspace is loaded (bus objects, `parameters`, `stepSize_s`,
  `aircraftInitial`, etc. -- `sensors.slx` was never designed to compile in isolation from the rest
  of the SIL init sequence; this is pre-existing, not introduced here).
- `update_diagram` compile check on the **full** `VehicleSilSimulation.slx`, after running
  `initVehicleSIL("launchFullSIL", false, "vehicleType", "hexarotor")`: **passes for all three**
  `INS_VARIANT=1` (Generic), `INS_VARIANT=2` (AnelloX3), **and** `INS_VARIANT=3` (Ideal, added
  2026-07-21 -- see section 4.7).
- `ins_ideal.slx` additionally checked with a static-rest numeric comparison against `ins.slx`
  (throwaway harness, not committed): confirms matching gravity sign/units and zero residual gyro,
  with noise and dynamics-settling error absent as designed -- see section 4.7.
- **`INS_VARIANT=3` flown live against the real Cube Orange+ over HITL (2026-07-22)**: not just a
  compile check -- `session_ideal_verify_02` flew the full 19-waypoint `mission.yaml` (byte-identical
  to the two baseline sessions', SHA-256-verified) end to end: armed, took off, all 19 waypoints
  reached, landed, `console.log` shows `MISSION SUCCESS`. Registered as `hil_ideal_success` in
  `AnalysisIMU/three_way_comparison/scripts/build_manifest.py` and folded into a new three-way
  pairwise RMSE/Pearson-r comparison (`fase12_three_way_pairwise_effects.py`) against
  `hil_generic_success`/`hil_anello_success` -- see `HIL_TEST_STATUS_AND_NEXT_PRIORITIES.md` section
  5 item 4 for the (mixed, not over-interpreted) result.
  - **First attempt failed and revealed a real finding, not just a bug in this integration**: a
    genuinely zero-noise magnetometer (`Gain=0`) produced ~37% bit-identical consecutive samples,
    which PX4's sensor-health monitor reads as a stuck sensor (`MAG #0 failed: STALE!`), triggering a
    failsafe/auto-land ~53s into the mission. Fixed by setting the mag gain to `1e-4` instead of `0`
    (still ~100x smaller than Generic's, negligible for any noise analysis) -- confirmed both
    numerically (200-step harness, zero exact-repeat diffs) and by the successful retry. **Practical
    implication for anyone building a "zero-error" sensor model for live PX4 HITL**: literal zero
    noise is not flyable -- PX4 needs some minimal per-sample dither to consider a sensor "alive."
    accel/gyro showed the identical ~37% repeat rate (same disabled dynamics/noise) but were never
    flagged, so this specific staleness check is evidently mag-specific or has a much stricter
    threshold there than for accel/gyro.

**Not yet done** (next phase, needs an interactive session / real PX4 connection -- out of scope
for this automated pass):
1. Actually running a simulation (not just compiling) with `INS_VARIANT=2` to sanity-check signal
   magnitudes (the gravity-sign test from `newIMU_model/instruction.md` Phase 2.13 Test 1: vehicle
   at rest, level, confirm `accel_meas` shows gravity with the sign PX4 expects).
2. Live PX4 SITL validation per `newIMU_model/instruction.md` section 2.13 Tests 7-9: MAVLink
   `listener sensor_accel/sensor_gyro/sensor_mag`, `vehicle_imu`, `estimator_status`, and the
   HIL_SENSOR inspector in QGroundControl, with `INS_VARIANT=2`.
3. Decide whether `INS_VARIANT` should default to `2` (AnelloX3) once validated, currently defaults
   to `1` (Generic) in `sensors/setUpSensors.m` for safety/regression-baseline reasons.
4. File/fix the MATLAB Function block struct-compile bug in `newIMU_model` (section 3).
5. **Run an actual D1 HIL/mission session with `INS_VARIANT=3`** (real PX4 hardware or SITL loop,
   not just a compile check) and fold its output into `AnalysisIMU/three_way_comparison/scripts/fase_c7_c8_pairwise_effects.py`
   as a third arm, per `HIL_TEST_STATUS_AND_NEXT_PRIORITIES.md` section 5 item 4. Only the Simulink
   model side (this section) is done so far -- the session capture and analysis fold-in are
   still open.

---

## 6. Key file map

| File | Role |
|---|---|
| `sensors/sensors.slx` | Top-level sensor subsystem; routes to gps/ins/adc, builds `SensorsBus` |
| `sensors/components/ins.slx` | Generic INS baseline (Aerospace block + ad hoc mag), `INS_VARIANT=1` |
| `sensors/components/ins_anello.slx` | ANELLO X3 datasheet INS, `INS_VARIANT=2` |
| `sensors/components/ins_ideal.slx` | Zero-error pass-through INS (D1 baseline), `INS_VARIANT=3` |
| `sensors/components/build_ins_anello.m` | Rebuild script for `ins_anello.slx` |
| `sensors/components/build_ins_ideal.m` | Rebuild script for `ins_ideal.slx` |
| `sensors/components/wire_ins_variant.m` | Rebuild script for `sensors.slx`'s `ins` selector (built the `INS_VARIANT=1/2` selector; the `INS_VARIANT=3` port was added directly via `model_edit`, not a rerun of this script -- update it by hand if `sensors.slx` is ever rebuilt from scratch) |
| `sensors/components/anelloX3/` | ANELLO X3 MATLAB functions/params, SIL-rate variants (see section 3 for why they're not 1:1 copies of `newIMU_model`) |
| `sensors/setUpSensors.m` | Generic sensor parameters + `INS_VARIANT` switch |
| `signals/BusDefinition.m` | Authoritative bus contracts (`AccelSensorBus`, `GyroSensorBus`, `MagSensorBus`, `INSSensorBus`, `SensorsBus`, `EnvironmentBus`, `BodyStateBus`) |
| `initVehicleSIL.m` | Path setup, step size (`stepSize_s = 0.004`), launches `VehicleSilSimulation.slx` |
| `../newIMU_model/models/ANELLO_X3_IMU_Vehicle_Interface.slx` | Original (standalone-project) source model -- not directly reused; SIL-specific copies built instead, see section 1 |
| `../newIMU_model/instruction.md` | Full build plan/spec for both Phase 1 (sensor) and Phase 2 (vehicle interface) |

---

## 7. Relevant MATLAB Agentic Toolkit skills for this work

- `model-based-design-core:building-simulink-models` -- structural edits to `ins_anello.slx`/`sensors.slx`
- `model-based-design-core:testing-simulink-models` -- Gherkin-based tests, next phase
- `model-based-design-core:simulating-simulink-models` -- running `VehicleSilSimulation.slx`
- `robotics-and-autonomous-systems:matlab-connect-mavlink` -- for the PX4 SITL validation step (section 5.2)
- `matlab-core:matlab-debugging` / `matlab-core:matlab-testing` -- general support

Also available via the `mcp__matlab__*` tools directly (`model_read`, `model_edit`, `model_check`,
`model_overview`, `model_query_params`, `evaluate_matlab_code`, `run_matlab_file`) for
inspecting/editing/running both `.slx` files and MATLAB scripts without leaving this session. Note:
if a MATLAB session has its path reset (e.g. via `restoredefaultpath`), the `model_*` tools break
with `Unrecognized function or variable 'model_check'` etc. -- re-add
`C:\Users\<user>\.claude\plugins\cache\matlab-agentic-toolkits\model-based-design-core\<version>\tools`
(via `addpath(genpath(...))`) to recover.
