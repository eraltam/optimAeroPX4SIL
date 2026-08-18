function [accel_meas, gyro_meas, mag_meas, accel_bias, gyro_bias, mag_bias, ...
          raw_counts_by_fog, temp_clamp_flags, ic_clamp_flags, count_saturation_flags] = ...
         ANELLO_X3_IMU_fcn_SIL(accel_true, gyro_true, mag_true, T, temperature_counts, outputMode, reset)
% ANELLO_X3_IMU_FCN_SIL  Datasheet-parametric IMU sensor model, SIL sample-rate variant.
%
% Same algorithm and parameter VALUES as anelloX3Params_SIL.m (Ts = 0.004 s, matching
% optimAeroPX4SIL's fixed step), but with every parameter inlined as a local constant
% instead of read from a struct returned by anelloX3Params_SIL().
%
% Noise model synced 2026-07-14 with newIMU_model/models/ANELLO_X3_IMU_fcn.m +
% updateStochasticNoiseState.m (added there 2026-07-05): gyro and accel now also carry
% pink (1/f) noise, and gyro additionally carries rate-random-walk (RRW), on top of the
% white noise (ARW/VRW) and Gauss-Markov bias instability this file already had. Ported
% as inlined persistent-vector recursions (bias/pink/RRW kept as separate [3x1]
% persistents per sensor) rather than by calling updateStochasticNoiseState(cfg, state,
% ...) directly, to stay consistent with this file's existing no-struct-calls
% convention -- see the compile-bug note below. Mag is unchanged: the newIMU_model
% version also hardcodes mag's pink/RRW off (magCfg.enablePinkNoise/enableRandomWalk =
% false regardless of params), so there is nothing to port there.
%
% Why: MATLAB Function blocks in this MATLAB release (R2026a, MATLAB MCP toolchain)
% fail to statically infer the output type/size of a MATLAB Function block whose code
% calls a function returning a large nested struct (anelloX3Params_SIL has 4 nested
% sub-structs and ~30 fields) — Simulink reports a confusing, misattributed "port
% width/dimension" error on an unrelated port ('reset') instead of identifying the
% real cause. Confirmed by direct reproduction: this also breaks the already-validated
% newIMU_model/models/ANELLO_X3_IMU.slx in this same environment (see
% optimAeroPX4SIL/CLAUDE.md). A flat struct, or no struct at all, compiles fine — hence
% inlining here. anelloX3Params_SIL.m is kept as the documented source of these values
% for use in plain MATLAB scripts (non-Simulink); keep the two in sync by hand if the
% datasheet parameters ever change.
%
% GYRO PATH (2026-08-12): the gyroscope's generic deterministic scale-factor error and
% Celsius thermal term are REPLACED by the real per-unit SiPhOG IC/TC/SFvT calibration
% tables for SN261200001124 (ANELLO_X3_SiPhOG_CalibrationCore_fcn.m). The white/pink/RRW/
% bias-instability stochastic recursions below are UNCHANGED (same persistents, same
% randn() call order/count) -- only their sum is now routed through the calibration core
% instead of being added directly to a scaled/thermal-biased truth signal. See
% PLAN_ANELLO_X3_SIPHOG_TEMPERATURE_CALIBRATION_OUTPUT_MODES.md sections 3-4 and Phase C.
% Accel and mag are unaffected by this change.
%
% Inputs:
%   accel_true          [3x1]  true specific force, body frame [m/s^2]
%   gyro_true           [3x1]  true angular rate,   body frame [rad/s]
%   mag_true            [3x1]  true magnetic field,  body frame [Gauss]
%   T                   [1x1]  sensor temperature [deg C] (accel/mag thermal model only)
%   temperature_counts  [1x1]  SiPhOG internal temperature, in raw temperature counts (NOT
%                               Celsius -- see PLAN section 5). Used by the gyro TC/SFvT LUTs.
%   outputMode          [1x1]  uint8-compatible: 0=RAW_COUNTS (debug-only fallback, see below),
%                               1=UNCALIBRATED, 2=CALIBRATED (production default)
%   reset               [1x1]  nonzero -> reset all bias states to zero
%
% Outputs:
%   accel_meas, gyro_meas, mag_meas   [3x1]  measured signals
%   accel_bias, gyro_bias, mag_bias   [3x1]  current bias drift (for debug); gyro_bias is
%                                              bias-instability + pink + RRW (excludes white
%                                              noise), unchanged meaning from the prior version
%   raw_counts_by_fog                [3x1]  modeled raw FOG counts, FOG-indexed (debug/
%                                              telemetry only -- NEVER interpret as rad/s; see
%                                              PLAN section 6.2). outputMode=RAW_COUNTS does NOT
%                                              route counts onto gyro_meas; gyro_meas stays in
%                                              rad/s (falls back to CALIBRATED) in that mode.
%                                              Raw counts are always available here regardless
%                                              of outputMode.
%   temp_clamp_flags, ic_clamp_flags,
%   count_saturation_flags           [3x1]  logical diagnostic flags, FOG-indexed (see
%                                              ANELLO_X3_SiPhOG_CalibrationCore_fcn.m)

%#codegen

persistent ga_bias  gy_bias  mg_bias;
persistent gy_pink  gy_rrw   ga_pink;

if isempty(ga_bias)
    ga_bias  = zeros(3,1);
    gy_bias  = zeros(3,1);
    mg_bias  = zeros(3,1);
    gy_pink  = zeros(3,1);
    gy_rrw   = zeros(3,1);
    ga_pink  = zeros(3,1);
end

% ---- Optional reset --------------------------------------------------
if reset ~= 0
    ga_bias  = zeros(3,1);
    gy_bias  = zeros(3,1);
    mg_bias  = zeros(3,1);
    gy_pink  = zeros(3,1);
    gy_rrw   = zeros(3,1);
    ga_pink  = zeros(3,1);
    if isempty(accel_true)          % reset-only call, return dummy outputs
        accel_meas = zeros(3,1);  gyro_meas = zeros(3,1);  mag_meas = zeros(3,1);
        accel_bias = zeros(3,1);  gyro_bias = zeros(3,1);  mag_bias = zeros(3,1);
        raw_counts_by_fog = zeros(3,1); temp_clamp_flags = false(3,1);
        ic_clamp_flags = false(3,1); count_saturation_flags = false(3,1);
        return;
    end
end

%% ---- Timing / constants (anelloX3Params_SIL.m, Ts=0.004s / fs=250Hz) -----
Ts  = 0.004;
fs  = 1 / Ts;
d2r = pi / 180;
T0  = 25.0;

% =====================================================================
%  GYROSCOPE MODEL  (optical SiPhOG)
% =====================================================================
gyro_bias_radps     = 0.5 * d2r / 3600;                  % bias instability, < 0.5 deg/hr
gyro_ARW_radps_sqrts = 0.05 * d2r / 60;                  % ARW, < 0.05 deg/sqrt(hr)
gyro_sigma_radps    = gyro_ARW_radps_sqrts * sqrt(fs);
gyro_range_radps    = 400 * d2r;
% NOTE: the generic 0.1% scale-factor error and 5e-8 rad/s/degC Celsius thermal term that
% used to appear here are REPLACED by the real IC/TC/SFvT calibration tables below -- see
% anelloX3Params_SIL.m's "LEGACY-ONLY" note on p.gyro.scaleFactorError/tempCoeff_radps_C.

Tc_g       = 300.0;                              % correlation time [s]
alpha_g    = exp(-Ts / Tc_g);
sig_eta_g  = gyro_bias_radps * sqrt(1.0 - alpha_g^2);
gy_bias    = alpha_g .* gy_bias + sig_eta_g .* randn(3,1);

% Pink (1/f) noise -- from inertial_nav_sim custom_mems B, same first-order colored
% recursion as updateStochasticNoiseState.m's enablePinkNoise branch.
gyro_pinkNoise_radps = 1.0e-4 * d2r;              % inertialNavSim_B_degps = 1.0e-4 deg/s
pinkTc_g   = max(10.0 * Ts, 10.0);
alphaPink_g = exp(-Ts / pinkTc_g);
gy_pink = alphaPink_g .* gy_pink + gyro_pinkNoise_radps * sqrt(max(0.0, 1.0 - alphaPink_g^2)) .* randn(3,1);

% Rate random walk (RRW) -- from inertial_nav_sim custom_mems K.
gyro_rrw_radps_sqrts = 5.0e-6 * d2r;               % inertialNavSim_K_degps_sqrts = 5.0e-6 deg/s/sqrt(s)
gy_rrw = gy_rrw + gyro_rrw_radps_sqrts * sqrt(Ts) .* randn(3,1);

% White noise (ARW) -- same call, same position in the draw order as before this change, so
% the total random-draw count/order per timestep is unchanged from the pre-calibration model.
g_noise  = gyro_sigma_radps .* randn(3,1);

gy_stochastic = gy_bias + gy_pink + gy_rrw;
gy_totalNoise_radps = gy_stochastic + g_noise;

% One physical FOG sample -> raw counts + both engineering views, from the SAME noise
% realization computed above. No random draws or state updates happen inside the core, so
% selecting a view below can never change gy_bias/gy_pink/gy_rrw or the noise realization
% for this sample (PLAN section 1's non-negotiable design rule).
[raw_counts_by_fog, ~, ~, omega_uncal_body_radps, omega_cal_body_radps, ~, ~, ...
    count_saturation_flags, temp_clamp_flags, ic_clamp_flags] = ...
    ANELLO_X3_SiPhOG_CalibrationCore_fcn(gyro_true, gy_totalNoise_radps, temperature_counts);

% Output-mode selection (PLAN section 6.1). RAW_COUNTS (0) is intentionally NOT routed onto
% gyro_meas -- GyroSensorBus is documented rad/s, body axes; putting FOG counts there would be
% a unit/frame violation (PLAN section 6.2). raw_counts_by_fog above is always populated
% regardless of outputMode, so debug/standalone consumers still get raw counts in RAW_COUNTS
% mode; gyro_meas itself falls back to the calibrated view in that mode.
if outputMode == 1
    g_selected = omega_uncal_body_radps;
else
    g_selected = omega_cal_body_radps;     % CALIBRATED (2, default) and RAW_COUNTS (0) fallback
end

gyro_meas = min(max(g_selected, -gyro_range_radps .* ones(3,1)), gyro_range_radps .* ones(3,1));
gyro_bias = gy_stochastic;

% =====================================================================
%  ACCELEROMETER MODEL  (MEMS)
% =====================================================================
g0 = 9.80665;
accel_bias_mps2     = 20e-6 * g0;                        % bias instability, 20 ug
accel_VRW_mps_sqrts = 0.03 / sqrt(3600);                 % VRW, < 0.03 m/s/sqrt(hr)
accel_sigma_mps2    = accel_VRW_mps_sqrts * sqrt(fs);
accel_range_mps2    = 16 * g0;
accel_tempCoeff_mps2_C = 1e-5;
accel_staticBias    = [0;0;0];
accel_scaleFactor   = [0;0;0];
accel_misalignment  = eye(3);

Tc_a      = 200.0;
alpha_a   = exp(-Ts / Tc_a);
sig_eta_a = accel_bias_mps2 * sqrt(1.0 - alpha_a^2);
ga_bias   = alpha_a .* ga_bias + sig_eta_a .* randn(3,1);

% Pink (1/f) noise -- newIMU_model uses the bias-instability magnitude itself as the
% pink-noise sigma (p.accel.pinkNoise_mps2 = p.accel.bias_mps2). Velocity random walk is
% disabled in that source config (enableVelocityRandomWalk=false), so no RRW term here.
pinkTc_a   = max(10.0 * Ts, 10.0);
alphaPink_a = exp(-Ts / pinkTc_a);
ga_pink = alphaPink_a .* ga_pink + accel_bias_mps2 * sqrt(max(0.0, 1.0 - alphaPink_a^2)) .* randn(3,1);

ga_stochastic = ga_bias + ga_pink;

a_scaled = accel_misalignment * diag(1.0 + accel_scaleFactor) * accel_true;
a_tbias  = accel_tempCoeff_mps2_C .* (T - T0) .* ones(3,1);
a_noise  = accel_sigma_mps2 .* randn(3,1);
a_raw    = a_scaled + accel_staticBias + a_tbias + ga_stochastic + a_noise;

accel_meas = min(max(a_raw, -accel_range_mps2 .* ones(3,1)), accel_range_mps2 .* ones(3,1));
accel_bias = ga_stochastic;

% =====================================================================
%  MAGNETOMETER MODEL
% =====================================================================
mag_sigma_G        = 0.4e-3;                             % noise RMS, 0.4 mG
mag_range_G        = 8;
mag_tempCoeff_G_C  = 1e-6;
mag_staticBias     = [0;0;0];
mag_scaleFactor    = [0;0;0];
mag_misalignment   = eye(3);

Tc_m      = 500.0;
alpha_m   = exp(-Ts / Tc_m);
sig_eta_m = mag_sigma_G * sqrt(1.0 - alpha_m^2);
mg_bias   = alpha_m .* mg_bias + sig_eta_m .* randn(3,1);

m_scaled = mag_misalignment * diag(1.0 + mag_scaleFactor) * mag_true;
m_tbias  = mag_tempCoeff_G_C .* (T - T0) .* ones(3,1);
m_noise  = mag_sigma_G .* randn(3,1);
m_raw    = m_scaled + mag_staticBias + m_tbias + mg_bias + m_noise;

mag_meas  = min(max(m_raw, -mag_range_G .* ones(3,1)), mag_range_G .* ones(3,1));
mag_bias  = mg_bias;

end
