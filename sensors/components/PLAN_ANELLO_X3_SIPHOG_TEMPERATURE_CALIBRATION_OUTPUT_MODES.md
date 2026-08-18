# Plan: ANELLO X3 SiPhOG Temperature Calibration and Selectable Output Modes

**Status (2026-08-13, implementation pass):** Phases A-E implemented and verified per the
evidence in section 10 below, INCLUDING a live PX4 HIL flight on the real Cube Orange+
(`session_siphog_calib_verify_01`, 2026-08-13, standard 19-wp `mission.yaml`, `INS_VARIANT=2`
with the calibrated SiPhOG path live): `MAVSDK: MISSION SUCCESS`, armed/airborne/landed
normally, `ESTIMATOR_STATUS` innovation ratios all well under the 1.0 GPS-innovation-abort
threshold throughout (vel_ratio=0.167, pos_horiz_ratio=0.066, pos_vert_ratio=0.088,
mag_ratio=0.073 near mission end), `HIL_SENSOR`/`HIL_GPS` ran gap-free for the full 300s at a
steady 8ms step. **CONDITIONALLY CLOSED, not fully closed** -- open exceptions remain: Section
9's decision-gate defaults were ACCEPTED (not independently vendor/source-owner confirmed) per
explicit user direction on 2026-08-12; this one successful 19-wp flight is a smoke test, not the
full real-vs-sim fidelity comparison against actual bench/flight ground truth a promotion
decision would need. The full `VehicleSilSimulation.slx` compile still could not be verified via
scripted `update_diagram` in this session, due to an unrelated, pre-existing Windows Application
Control policy blocking `gps_msf.mexw64` -- the live HIL flight above is stronger evidence than
that scripted check would have been anyway, since it exercises the actual compiled/running
system, but the policy block itself is still unresolved.
**2026-08-13 follow-up:** the debug telemetry bus (section 7) is now genuinely logged, not just
terminated -- `ins_anello.slx`'s 4 new debug outputs (`raw_counts_by_fog`, `temp_clamp_flags`,
`ic_clamp_flags`, `count_saturation_flags`) have `DataLogging='on'` with custom names
(`anelloDebug_*`), confirmed populated in `logsout` from a direct `sim('ins_anello', ...)` call.
`vehicle.imu.anello.debugEnable` is NOT wired to gate this -- logging is currently unconditional
(cheap: 4 small vectors/sample), which is a simplification worth knowing about, not a defect.
Section 11.5's PSD/Allan-vs-production-baseline comparison is also now done:
`anelloX3SiPhOGNoisePSDAllanComparison.m` (zero-input, 600 s, Ts=0.004s, seed-matched old vs
new) -- PSD-derived ARW-equivalent and Allan deviation @ tau=1s are both exactly ratio=1.0000
old-vs-new on all 3 axes at the nominal (T_counts=0) operating point, i.e. the calibration
integration has NOT altered the validated stochastic noise model's PSD/Allan characteristics at
that point. Plots: `psd_comparison_old_vs_new.png`, `allan_comparison_old_vs_new.png`; numeric
results in `anelloX3SiPhOGNoisePSDAllanResults.mat`. This is still only checked at one
temperature-count operating point, not swept across the full domain the way the noise-std
sweep in the Phase C row above was -- a formal PSD/Allan sweep across temperature is not done.  
**Date:** 2026-08-12  
**Target unit:** `SN261200001124`  
**Production baseline:** `anelloX3/ANELLO_X3_IMU_fcn_SIL.m`, used by `ins_anello.slx` with `INS_VARIANT=2` at 250 Hz (`Ts = 0.004 s`).  
**Primary goal:** Extend the existing, validated ANELLO X3 SiPhOG simulator so one generated physical sensor sample can be observed as calibrated angular rate, uncalibrated angular rate, and optionally raw FOG counts.

## 1. Non-negotiable design rule

The simulator will generate one physical FOG observation per sample. All exposed outputs will be derived from that same observation:

```text
body angular-rate truth + one stochastic-noise realization + temperature counts
                                  |
                                  v
                         physical FOG model
                                  |
                                  v
                         raw FOG counts [3]
                          /        |        \
                         /         |         \
                raw debug   uncalibrated   calibrated
                               rate            rate
```

The output selector will run after all three views have been calculated. It must never call the random-number generator or update a stochastic state. Therefore, changing the selected output cannot create a different noise realization for the same physical sample.

This is an extension of the current simulator, not a replacement. Accelerometer, magnetometer, vehicle kinematics, lever-arm correction, vibration integration, sample timing, and existing bus contracts remain unchanged unless a separately documented integration need requires a small change.

## 2. Sources inspected

### 2.1 Calibration procedure

The supplied procedure is currently:

```text
anelloX3/imu_temp_calib/SiPhOG Calibration.docx
```

The requested file name `angular rate calibration.pdf` is not present. The DOCX contains the SiPhOG single-axis angular-rate calibration procedure and is treated as the available reference. Before implementation, confirm that it is the intended revision or supply the named PDF if it contains additional or newer instructions.

Important findings from the procedure:

- The calibration is performed in sensor counts; temperature is also expressed in temperature counts.
- The sensor model is expressed as a temperature-dependent sensitivity, an IC rate response, a temperature-dependent count offset, and count-domain noise.
- Calibration order is TC offset removal, IC-Rate conversion, then IC-SFvT correction.
- Earth rate is accounted for iteratively while producing the TC/IC/SFvT calibration products. It is not evidence that the simulator should independently add Earth rate again.
- The final SiPhOG estimate is the rate projected onto each FOG input axis. Three FOG projections must then be fused/reconstructed into the X3/body-frame three-axis rate.

### 2.2 Calibration archive

The supplied archive is:

```text
anelloX3/imu_temp_calib/SN261200001124_cal_tables.zip
```

It contains all five requested data files:

1. `SN261200001124_IC_LUTs.py`
2. `SN261200001124_TC_LUTs.py`
3. `261200001124_sf_calibration_x_05_05_2026__08_09_51_sf_lut.json`
4. `261200001124_sf_calibration_y_05_04_2026__18_55_49_sf_lut.json`
5. `261200001124_sf_calibration_z_05_05_2026__12_01_57_sf_lut.json`

The implementation must not import Python or parse JSON inside a compiled Simulink MATLAB Function block. A deterministic build/import step will convert these authoritative sources to fixed-size MATLAB numeric arrays and record source hashes and unit metadata. Generated MATLAB data is a compiled representation, not a replacement calibration source.

### 2.3 Existing simulator and validation baseline

The current production path is:

```text
BodyStatesBus.angular velocity [rad/s]
    -> ANELLO_X3_IMU_Vehicle_fcn_SIL
       -> mounting transform (currently identity)
       -> ANELLO_X3_IMU_fcn_SIL
          -> gyro deterministic errors
          -> gyro stochastic states/noise
          -> gyro saturation
    -> ins_anello.slx
    -> GyroSensorBus x/y/z [rad/s]
```

Current gyro truth enters in `ANELLO_X3_IMU_Vehicle_fcn_SIL.m` as:

```matlab
omega_true = mp.R_sensor_body * omega;
```

The current stochastic gyro terms are generated once in `ANELLO_X3_IMU_fcn_SIL.m`:

- `gy_bias`: Gauss-Markov bias instability.
- `gy_pink`: first-order colored/pink approximation.
- `gy_rrw`: rate random walk (the requested red/random-walk-like component).
- `g_noise`: white ARW-derived noise.

The current deterministic gyro model also has:

- a generic constant `0.1%` scale-factor error;
- a generic linear Celsius thermal term, `5e-8 rad/s/degC`.

For the new FOG calibration path, those two generic deterministic terms must not be stacked on top of the real IC/SFvT/TC tables. The real LUT pipeline will replace them for the optical gyro path. The stochastic terms will be preserved. Accelerometer and magnetometer thermal models are outside this objective and remain unchanged.

The final real-versus-sim report confirms that the active production sensor is `ANELLO_X3_IMU_fcn_SIL.m`, running at 250 Hz. None of its N1/N2 noise candidates was promoted. The report also warns that its existing Celsius thermal analysis is exploratory and confounded; that fitted Celsius slope must not be substituted for the supplied count-domain LUTs.

A separate, not-yet-active candidate also exists: `ANELLO_X3_IMU_fcn_SIL_v2.m` replaces the scalar datasheet ARW/VRW coefficients with per-axis values fit to physical repeatability data (`STATIC_ACCEPTANCE_CRITERIA_PROPOSAL.md`, approved 2026-07-22). It is orthogonal to this SiPhOG calibration objective -- it only touches white-noise density, not IC/TC/SFvT -- but because Phase C below edits `ANELLO_X3_IMU_fcn_SIL.m` directly, that edit must preserve the file's current scalar ARW/VRW behavior and must not be silently reconciled with `_v2`. If `_v2` is promoted to production before or during this work, the calibration core integration in Phase C/D must be re-applied against it explicitly, as its own decision, not inferred.

## 3. Calibration data facts that affect the design

### 3.1 FOG input axes

The IC source gives these X3-frame sensor-axis rows:

```text
FOG 1: [-0.99997207, -0.00116392, -0.00738314]   approximately -X
FOG 2: [-0.00397688, -0.00283814, +0.99998806]   approximately +Z
FOG 3: [+0.00874953, -0.99995686, +0.00311948]   approximately -Y
```

Therefore, FOG1/FOG2/FOG3 must not be treated as X/Y/Z. Define the fixed matrix:

```text
U_fog = [u_fog1^T;
         u_fog2^T;
         u_fog3^T]
```

and project body/sensor truth as:

```text
q_true_dps = U_fog * omega_true_body_dps
```

After calibrating the three scalar FOG channels, reconstruct body rate using a numerically checked solve:

```text
omega_body_dps = U_fog \ q_fog_dps
```

Use the exact `sensAxes` rows, not a hand-coded sign/permutation. Add a test for matrix rank, condition number, and reconstruction error.

### 3.2 IC-Rate LUTs

The FOG IC tables are nonlinear, monotonic, and not uniformly spaced:

- FOG1: 43 points.
- FOG2: 41 points.
- FOG3: 43 points.
- Count extent: signed 24-bit endpoints `[-8388608, 8388607]`.
- Scaled-output extent differs by FOG and is about `+/-717` to `+/-751 deg/s`.

Define, per FOG:

```text
G_j(c)     = piecewise-linear IC LUT, counts -> projected rate [deg/s]
G_j^-1(q)  = inverse piecewise-linear IC LUT, projected rate [deg/s] -> counts
```

Do not replace `G_j` with a constant counts-to-rate factor.

### 3.3 TC LUTs

The FOG TC tables map temperature counts to count offset:

```text
b_j(T_counts) = TC_j(T_counts)  [FOG sensor counts]
```

The physical model adds `b_j`; the calibrated path subtracts the identical computed value. No separate generic FOG thermal drift will be active in the nominal model.

### 3.4 SFvT LUTs

Each JSON file has 17 nonuniform temperature-count points and a dimensionless `scale_factor` curve. The correction values are near, but not equal to, 1 and differ by axis.

Define:

```text
s_x(T), s_y(T), s_z(T)
```

with piecewise-linear interpolation. The exact mapping from the files labelled X/Y/Z to FOG1/FOG2/FOG3 is a decision gate in Section 9 because the metadata does not explicitly name the physical FOG. The likely geometric mapping is X -> FOG1, Y -> FOG3, Z -> FOG2, but the signs and test convention must be confirmed rather than inferred.

## 4. Proposed equations

All internal calibration calculations should use degrees per second because the IC LUT scaled outputs are in degrees per second. Convert the existing simulator input from rad/s exactly once at the boundary and convert selected engineering output back to rad/s exactly once before `GyroSensorBus`.

For FOG `j`, let:

```text
q_j       = u_j^T * omega_true_body_dps
n_j       = u_j^T * noise_body_dps
s_j(T)    = SFvT correction at temperature counts T
b_j(T)    = TC bias in counts at T
G_j       = IC counts-to-rate LUT
```

### 4.1 Nominal physical sample

The preferred noise-preserving formulation is:

```text
q_noisy_j          = q_j + n_j
rate_before_SFvT_j = q_noisy_j / s_j(T)
ideal_counts_j     = G_j^-1(rate_before_SFvT_j)
raw_counts_j       = ideal_counts_j + b_j(T)
```

This formulation converts the existing engineering-unit stochastic realization into the equivalent nonlinear IC count response. It ensures the calibrated output contains the same existing stochastic term instead of accidentally attenuating or regenerating it.

An alternative, closer to the document's explicit count-domain noise term, is:

```text
raw_counts_j = G_j^-1(q_j / s_j(T)) + b_j(T) + noise_counts_j
```

but it requires a documented rule for converting the current rad/s white/pink/RRW/bias states into count-domain amplitudes across a nonlinear IC curve and changing temperature. This choice must be confirmed at the implementation review. The first formulation is recommended for preserving the validated simulator's stochastic behavior.

### 4.2 Uncalibrated engineering output

Uncalibrated rate means the raw observation is interpreted using IC-Rate only, with no TC subtraction and no SFvT correction:

```text
q_uncal_j_dps       = G_j(raw_counts_j)
omega_uncal_body_dps = U_fog \ q_uncal_dps
```

This output contains both TC offset and SFvT variation, plus the same stochastic observation. It is distinct from raw counts.

### 4.3 Calibrated engineering output

```text
tc_corrected_counts_j = raw_counts_j - b_j(T)
q_ic_j_dps             = G_j(tc_corrected_counts_j)
q_cal_j_dps            = s_j(T) * q_ic_j_dps
omega_cal_body_dps     = U_fog \ q_cal_dps
```

With noise disabled and inputs inside the LUT domain:

```text
omega_cal_body_dps approximately equals omega_true_body_dps
```

With noise enabled:

```text
omega_cal_body_dps approximately equals omega_true_body_dps + existing stochastic noise
```

Calibration will not reset, refilter, subtract, or independently regenerate white, pink, RRW/red, or bias-instability states.

### 4.4 Saturation and quantization order

Raw FOG count saturation belongs after adding TC bias and before any calibration output consumes the raw sample:

```text
raw_counts = clamp(raw_counts, -8388608, 8388607)
```

Whether to round to integer digital counts is unresolved. Initial tests should retain double-precision fractional counts so interpolation round trips can be isolated from quantization error. If raw protocol fidelity requires integer counts, add a separate, explicitly configurable quantization stage and quantify its effect. Do not enable it silently.

## 5. Temperature representation and source

The current model receives `AirEnvironment.airTemperature_K`, converts it to Celsius, and uses that value for the legacy generic sensor thermal terms and the PX4 bus `temperature_degC` field. There is no verified mapping from that Celsius value to SiPhOG temperature counts.

Therefore:

- Add `temperature_counts` as a distinct input to the new gyro calibration core.
- Do not use Celsius as a lookup coordinate.
- Keep the existing Celsius signal for the bus and for unchanged accelerometer/magnetometer behavior.
- Do not claim ambient air temperature equals internal SiPhOG temperature.
- Expose/log the exact `temperature_counts` used by TC and SFvT.

Initial integration should support:

1. Direct constant temperature counts for normal SIL/HIL operation.
2. A direct time-varying temperature-count input in a dedicated calibration/validation harness.
3. Full-vehicle time-varying counts only after choosing a code-generation-safe signal source that does not invent a Celsius conversion.

A future verified `temperature_C -> temperature_counts` transfer may be added as an optional source, with its provenance and validity range documented separately.

## 6. Proposed API and integration

### 6.1 Output modes

Use an explicit Simulink-compatible enum if it compiles cleanly in this project; otherwise use a documented `uint8` with named constants:

```text
RAW_COUNTS   = 0   (debug/standalone only by default)
UNCALIBRATED = 1
CALIBRATED   = 2   (default; production-safe)
```

Recommended configuration shape:

```matlab
vehicle.imu.anello.outputMode = AnelloIMUOutputMode.CALIBRATED;
vehicle.imu.anello.temperatureCounts = <explicit count value>;
vehicle.imu.anello.debugEnable = false;
vehicle.imu.anello.residualThermalEnable = false;
```

`residualThermalEnable` must remain false in the initial implementation. No residual model should be added until separately specified and validated.

### 6.2 Raw-count safety

`GyroSensorBus.x/y/z` is consumed by PX4 as body angular rate in rad/s. Putting FOG counts into that bus would create a unit and frame violation. Therefore:

- Always make `raw_counts_fog1_fog2_fog3` available as debug output/telemetry.
- Allow `RAW_COUNTS` as the selected output only in the standalone core or a dedicated raw-count harness.
- In the production `ins_anello.slx` path, either reject `RAW_COUNTS` at compile/start time or keep `gyro_meas` in engineering units and expose raw counts only through a separate debug port/logging signal.

This preserves the existing public PX4 interface while still delivering raw sensor access.

### 6.3 Core implementation shape

Prefer small code-generation-safe functions with fixed-size numeric inputs/outputs:

```text
anelloX3SiPhOGCalibrationData_SN261200001124.m
    fixed numeric LUT arrays, lengths, sensAxes, metadata checks

anelloInterp1Clamped.m
    nonuniform piecewise-linear interpolation + endpoint flag

anelloInverseMonotonicLUT.m
    monotonic inverse interpolation + endpoint flag

ANELLO_X3_SiPhOG_CalibrationCore_fcn.m
    truth/noise projection -> one raw sample -> uncalibrated/calibrated views
```

Do not return a large nested parameter struct from a function called by a MATLAB Function block. This repository has already documented an R2026a type-inference failure for that pattern. Use fixed-size arrays and flat scalar/vector outputs.

**Correction (implementation, 2026-08-12):** this rule directly conflicts with treating `anelloX3SiPhOGCalibrationData_SN261200001124.m` (a nested struct: `cal.ic.*`, `cal.tc.*`, `cal.sfvt.*`) as something `ANELLO_X3_SiPhOG_CalibrationCore_fcn.m` can call. It cannot, for the same reason `ANELLO_X3_IMU_fcn_SIL.m` does not call `anelloX3Params_SIL()` (see `optimAeroPX4SIL/CLAUDE.md` section 3). `anelloX3SiPhOGCalibrationData_SN261200001124.m` is therefore documentation/test-only status, exactly like `anelloX3Params_SIL.m` -- used by `anelloX3SiPhOGCalibrationImporterTest.m` (plain script, not compiled into a block) to freeze and parity-check the source values. `ANELLO_X3_SiPhOG_CalibrationCore_fcn.m` inlines the same literal LUT arrays as local constants directly, and the two must be kept in sync by hand if the archive is ever regenerated -- same maintenance burden already accepted for `anelloX3Params_SIL.m` / `ANELLO_X3_IMU_fcn_SIL.m`.

### 6.4 Existing files expected to change during implementation

Likely modifications:

- `anelloX3/ANELLO_X3_IMU_fcn_SIL.m`
  - Preserve stochastic state recursions and random-call count/order as far as practical.
  - Separate white, pink, RRW/red, and bias-instability vectors for debug.
  - Replace generic optical-gyro deterministic scale/thermal equations with the calibration core.
  - Calculate all output views before selection.
- `anelloX3/ANELLO_X3_IMU_Vehicle_fcn_SIL.m`
  - Pass temperature counts and output mode through without altering vehicle kinematics or frame handling.
- `build_ins_anello.m` and regenerated `ins_anello.slx`
  - Wire configuration, preserve current two model inports and `INSSensorBus` output if possible, and add safe debug logging.
- `setUpSensors.m`
  - Add explicit default calibrated mode, direct temperature-count configuration, debug flag, and disabled residual-thermal flag.
- `anelloX3/anelloX3Params_SIL.m`
  - Document that generic gyro Celsius drift and constant scale error are legacy-only when LUT calibration is active.

New calibration-data/core/test files should live under `sensors/components/anelloX3`, with authoritative source material retained under `imu_temp_calib`.

## 7. Debug telemetry contract

When debug is enabled, expose fixed-size vectors/scalars without printing every sample:

```text
omega_true_body_radps[3]
temperature_counts
sfvt_by_fog[3]
tc_bias_counts_by_fog[3]
ideal_counts_by_fog[3]
raw_counts_by_fog[3]
tc_corrected_counts_by_fog[3]
ic_rate_dps_by_fog[3]
uncalibrated_body_radps[3]
calibrated_body_radps[3]

white_noise_body_radps[3]
pink_noise_body_radps[3]
rrw_red_noise_body_radps[3]
bias_instability_body_radps[3]
total_stochastic_noise_body_radps[3]

count_saturation_flags[3]
temperature_clamp_flags[3]
ic_clamp_flags[3]
```

Do not label `gy_bias` as red noise: it is the existing Gauss-Markov bias-instability state. Report RRW as the red/random-walk-like term and keep bias instability separate.

## 8. Interpolation and limit policy

The current ANELLO simulator has no LUT interpolation policy to reuse. Use:

- Piecewise-linear interpolation on the actual nonuniform knot coordinates.
- No assumption of uniform spacing.
- No uncontrolled extrapolation.
- Clamp outside the declared LUT endpoints and set an explicit diagnostic flag.
- Saturate FOG raw counts to the signed 24-bit IC range.
- Validate all independent axes for strictly increasing abscissas and monotonic IC outputs before simulation.

The temperature LUTs include signed 16-bit endpoint knots (`-32768`, `32767`) whose values appear to act as end caps. They are authoritative knots, but validation plots must distinguish the dense observed/calibrated interior region from the synthetic/full-range endpoints so large endpoint behavior is not misinterpreted.

## 9. Decision gates and unresolved ambiguities

No production code should be changed until the following are resolved or explicitly accepted:

1. **Procedure revision:** Confirm `SiPhOG Calibration.docx` is the intended substitute for the missing `angular rate calibration.pdf`.
2. **SFvT-to-FOG mapping:** Confirm whether JSON X/Y/Z maps geometrically as X->FOG1, Y->FOG3, Z->FOG2, and whether sign is already absorbed by IC/sensor-axis conventions.
3. **Temperature-count source:** Select how full-vehicle simulations receive time-varying internal SiPhOG temperature counts. There is no verified Celsius conversion today.
4. **Raw output interface:** Confirm raw counts remain debug-only for `ins_anello.slx`; placing them on `GyroSensorBus` is unsafe because that bus is rad/s in body axes.
5. **Noise-domain convention:** Approve the recommended engineering-noise-preserving inverse-IC formulation, or provide count-domain noise calibration parameters.
6. **Quantization:** Decide whether raw output is fractional modeled counts or rounded signed-24-bit digital counts.
7. **Earth-rate truth convention:** Confirm `aircraftAngVelInBody_radps` is the intended truth input and that no separate Earth-rate term is to be added. The calibration tables appear to have Earth-rate handled during their generation.
8. **Sensor-axis frame:** Confirm `real_fog_*_sensAxes` rows are expressed in the same X3 sensor frame used by `R_sensor_body`. Current mounting is identity, but the frame contract should be documented before future non-identity mounts.
9. **Legacy deterministic scale error:** Confirm that the old generic 0.1% optical gyro scale error is replaced by real IC/SFvT behavior, not retained as an unexplained residual.

Recommended defaults if the source owners confirm no contrary convention:

- Linear interpolation with endpoint clamp and flags.
- X->FOG1, Y->FOG3, Z->FOG2 mapping based on `sensAxes` geometry.
- Raw counts as debug only in the PX4-connected model.
- Fractional modeled counts internally; optional explicit quantization later.
- No separate Earth-rate injection.
- No generic FOG Celsius thermal term or 0.1% scale term in LUT-active modes.

## 10. Implementation phases

### Phase A: Freeze data and conventions

1. Record SHA-256 hashes of the DOCX, ZIP, and each archive member.
2. Confirm unit number, point counts, coordinate units, monotonicity, and endpoint ranges.
3. Resolve Section 9 decisions and record answers in this file or an implementation record.
4. Create generated fixed-size MATLAB calibration arrays with traceable source comments.
5. Add an importer test that compares generated arrays to every authoritative archive value.

**Gate:** No simulator changes until source parity and axis/SF mapping are approved.

### Phase B: Calibration math in isolation

1. Implement forward and inverse IC interpolation.
2. Implement TC and SFvT interpolation.
3. Implement projection through `U_fog` and reconstruction with the checked matrix solve.
4. Implement one-sample physical/raw/uncalibrated/calibrated core with externally supplied noise.
5. Add saturation/clamp diagnostics.

**Gate:** Knot round trips, axis reconstruction, and zero-noise temperature sweep pass.

### Phase C: Preserve and connect stochastic model

1. Refactor the existing gyro section just enough to name its four stochastic components.
2. Generate those components once per timestep using the existing state equations and call order.
3. Pass their single total realization into the calibration core.
4. Ensure output selection occurs after the physical raw sample and both rate views exist.
5. Verify selected mode does not change persistent-state advancement or random draws.

**Gate:** Same-sample/noise tests pass and calibrated noise PSD/Allan behavior remains statistically consistent with the production baseline.

### Phase D: Simulink/configuration integration

1. Add calibrated/uncalibrated selection with calibrated as the default.
2. Add direct temperature-count configuration.
3. Add debug signals without changing the PX4 gyro-bus units.
4. Regenerate `ins_anello.slx` from `build_ins_anello.m`; do not hand-edit only the binary model.
5. Compile/update the model in the supported MATLAB version and confirm 250 Hz timing.

**Gate:** `ins_anello.slx` compiles, bus dimensions/units are unchanged, and legacy accel/mag paths regress cleanly.

### Phase E: Validation and evidence

Run all tests in Section 11 and generate the required plot and numeric summary. Freeze configuration, random seed, source hashes, and output artifacts.

### Implementation status (2026-08-12)

| Phase | Status | Evidence |
|---|---|---|
| A (freeze data/conventions) | Done | `anelloX3SiPhOGCalibrationData_SN261200001124.m` + `anelloX3SiPhOGCalibrationImporterTest.m`: 30/30 array parity checks vs raw archive, all LUTs monotonic, `U_fog` rank 3 / cond 1.01. SHA-256 hashes recorded in the data file header. |
| B (calibration math in isolation) | Done | `anelloInterp1Clamped.m`, `anelloInverseMonotonicLUT.m`, `ANELLO_X3_SiPhOG_CalibrationCore_fcn.m`, `anelloX3SiPhOGCalibrationCoreTest.m`: exact knot round-trips, axis reconstruction to ~1e-14 deg/s (zero noise), TC add/subtract identity, temp sweep noise-off calibrated error 0.0000 deg/s. |
| C (preserve/connect stochastic model) | Done | `ANELLO_X3_IMU_fcn_SIL.m` integration + `anelloX3IMUfcnSILRegressionTest.m` (diffed against `ANELLO_X3_IMU_fcn_SIL_OLD_baseline.m`, the last-committed pre-change version): accel_meas/mag_meas/gyro_bias bit-identical old vs new (confirms unchanged randn() order/count); calibrated gyro_meas noise std ratio 1.0000x vs pre-change at nominal temp, ~1.1-1.14x the raw ARW spec consistently from -25000 to +25000 temperature counts. |
| D (Simulink/config integration) | Done, with one open item | `ANELLO_X3_IMU_Vehicle_fcn_SIL.m`, `build_ins_anello.m`, `setUpSensors.m` updated; `ins_anello.slx` regenerated. **Caught and fixed during this pass:** rebuilding from `build_ins_anello.m` via `new_system` silently discarded the separately-applied Level-A vibration integration (`add_vibration_to_ins_anello.m`) that was baked into the previously-committed file and is relied on for live HIL flights (`vehicle.vibration.enable=1` default) -- re-ran that script to restore it, confirmed via `model_overview`. Also found and fixed a real Simulink Model-Reference port-width bug unrelated to the calibration logic itself: the new `RotorAngVel_radps` inport's auto (`-1`) `PortDimensions` resolved to width 1 when compiled standalone (no external driver) and that stale interface conflicted with the true 6-wide requirement when referenced from `sensors.slx`; fixed by setting `PortDimensions='6'` explicitly. `model_check` (unconnected ports/lines, Stateflow lint) and `update_diagram` both pass on `ins_anello.slx` standalone and on `sensors.slx` for `INS_VARIANT` = 1, 2, and 3. **Open item:** full-chain `VehicleSilSimulation.slx` compile could not be verified -- blocked by a pre-existing, unrelated Windows Application Control policy on `work/gps_msf.mexw64` (GPS submodel, untouched by this work). Debug telemetry (raw counts / clamp flags) is wired to `Terminator` blocks, not logged, matching the pre-existing treatment of `accel_bias`/`gyro_bias`/`mag_bias` -- not a regression, but not the full logging contract in section 7 either. |
| E (validation/evidence) | Done, including a live HIL flight | `anelloX3SiPhOGValidation.m`: 90 deg/s single-axis and [30,-45,90] deg/s combined cases, noise-off and noise-on, full temperature-count sweep (-30000 to +30000). Noise-off calibrated error: exactly 0.0000 deg/s both cases (all clamp/saturation counters at 0 -- sweep stayed in-domain). Noise-off uncalibrated peak-to-peak thermal movement: up to 9.85 deg/s (X axis, 90 dps case). Noise-on calibrated: mean error ~-0.0002 deg/s, std ~0.013 deg/s, zero clamps/saturations. Plots: `validation_plot_case_90dps_singleaxis.png`, `validation_plot_case_combined_30_m45_90.png`; raw results in `anelloX3SiPhOGValidationResults.mat`. **Live HIL** (run by the user, 2026-08-13, real Cube Orange+, standard 19-wp `mission.yaml`, `HILDiagnostics/logs/session_siphog_calib_verify_01/`): `MAVSDK: MISSION SUCCESS`, armed at t=30.3s, airborne throughout, landed/disarmed normally, GPS-innovation ratios well within the abort threshold, gap-free `HIL_SENSOR`/`HIL_GPS` stream for the full run. This is a smoke test confirming the integration flies cleanly end to end on real hardware -- it is NOT the full real-vs-sim/PSD/Allan fidelity comparison (`AnalysisIMU/...`) that a promotion-to-default decision would need; that remains open.

## 11. Required tests

### 11.1 LUT unit tests

- Exact recovery of every IC knot in forward interpolation.
- Exact recovery of every IC knot in inverse interpolation.
- Forward(inverse(rate)) and inverse(forward(count)) dense-grid tolerances.
- Exact recovery of every TC and SFvT knot.
- Nonuniform-spacing tests.
- Endpoint clamp and diagnostic-flag tests.
- Rejection/test failure for non-monotonic IC abscissas or outputs.

### 11.2 Axis tests

Test body inputs in deg/s:

```text
[90, 0, 0]
[0, 90, 0]
[0, 0, 90]
[30, -45, 90]
```

For each, verify projection via exact `sensAxes`, per-FOG LUT selection, sign, and body reconstruction. Do not validate using a hard-coded FOG permutation alone.

### 11.3 TC add/subtract test

For several temperatures and rates, verify:

```text
raw_counts - TC(T) == ideal_counts
```

within floating-point tolerance when noise and count saturation are disabled.

### 11.4 Temperature sweep, noise disabled

- Hold body truth constant at representative rates, including `90 deg/s` and the four three-axis vectors above.
- Sweep direct temperature counts through each axis's common valid range and separately exercise full endpoints.
- Disable white, pink, RRW/red, and bias-instability updates through a test-only configuration that does not alter production defaults.
- Require calibrated output to recover truth within a tolerance derived from interpolation/numerical error.
- Require uncalibrated output to exhibit the TC/SFvT curve rather than remain artificially flat.

### 11.5 Temperature sweep, noise enabled

- Use a fixed recorded seed/configuration.
- Confirm uncalibrated output contains thermal variation plus stochastic noise.
- Confirm calibrated output remains centered near truth but retains nonzero white, pink, RRW/red, and bias-instability content.
- Compare calibrated noise PSD and Allan deviation with the current production model so deterministic calibration work does not accidentally rewrite the validated noise model.

### 11.6 Same physical sample/noise realization

For every timestep, produce and log all views in one call:

```text
raw_counts
uncalibrated_rate
calibrated_rate
noise components
```

Assertions:

- Selection among views performs no random draw.
- Re-selecting a stored observation produces bit-identical views.
- With the same initial stochastic state and input sequence, mode changes do not alter subsequent raw samples.
- TC subtraction from the stored raw sample, followed by IC and SFvT, reproduces the stored calibrated result.

### 11.7 Regression tests

- Accelerometer output/state behavior unchanged.
- Magnetometer output/state behavior unchanged.
- Vehicle lever-arm and `R_sensor_body` logic unchanged.
- Production sample time remains `0.004 s`.
- Gyro range/saturation is enforced in the count-domain physical model and yields safe rad/s bus output.
- Default mode is calibrated.
- `INS_VARIANT=2` and existing model reference/bus compilation still work.

## 12. Validation plot and acceptance evidence

Create a reproducible script that writes both machine-readable results and a plot with:

- Ground-truth body angular rate.
- Uncalibrated body angular rate.
- Calibrated body angular rate.
- Temperature counts on a secondary axis or aligned subplot.

At minimum, include `90 deg/s` single-axis and `[30, -45, 90] deg/s` combined-axis cases. Produce noise-off and noise-on variants.

Report quantitative metrics per axis:

```text
max/rms calibrated error with noise disabled
uncalibrated peak-to-peak thermal movement
calibrated mean error with noise enabled
calibrated standard deviation with noise enabled
noise-component RMS values
number of temperature/IC clamps
number of raw-count saturations
```

Primary acceptance criterion:

```text
fixed truth + changing temperature counts

UNCALIBRATED: deterministic mean moves with TC/SFvT temperature behavior
CALIBRATED:   deterministic mean stays near truth
              existing stochastic noise remains present
```

## 13. Deliverables after implementation

1. Short architecture description and final signal-flow diagram.
2. List of modified/generated files and source hashes.
3. Exact implemented equations and unit conversions.
4. TC, IC, SFvT interpolation and limit policies.
5. Confirmed FOG/SF/body-axis mapping.
6. Output-mode and temperature-count configuration API.
7. Debug telemetry definition.
8. LUT, axis, same-sample, regression, and Simulink compile tests.
9. Noise-off and noise-on temperature-sweep results.
10. Ground-truth/uncalibrated/calibrated/temperature plots.
11. Quantitative acceptance table.
12. Remaining assumptions or unresolved calibration-source ambiguities.

## 14. Explicit out of scope for the first implementation

- Rewriting the ANELLO simulator from scratch.
- Retuning white, pink, RRW/red, or Gauss-Markov noise parameters.
- Adding a generic FOG thermal drift on top of TC/SFvT.
- Fitting a Celsius-to-temperature-count conversion without authoritative data.
- Treating ambient air temperature as internal SiPhOG temperature.
- Changing accelerometer or magnetometer calibration.
- Adding residual thermal error or hysteresis by default.
- Promoting N1/N2 noise or vibration candidates.
- Sending raw counts to PX4 through a bus documented as rad/s.

