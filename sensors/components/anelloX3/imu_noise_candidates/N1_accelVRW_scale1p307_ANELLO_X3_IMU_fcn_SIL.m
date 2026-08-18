function [accel_meas, gyro_meas, mag_meas, accel_bias, gyro_bias, mag_bias] = ...
         N1_accelVRW_scale1p307_ANELLO_X3_IMU_fcn_SIL(accel_true, gyro_true, mag_true, T, reset)
% N1_ACCELVRW_SCALE1P307_ANELLO_X3_IMU_FCN_SIL  Candidate N1: accel VRW scaled by a factor
% identified from the P1 static real-vs-sim comparison (PLAN_CIERRE_IMU_SIPHOG_REAL_VS_SIM_FIGURAS_TEX.md).
%
% ONLY CHANGE from ANELLO_X3_IMU_fcn_SIL.m: line "accel_VRW_mps_sqrts = 0.03 / sqrt(3600);" is
% scaled by ACCEL_VRW_SCALE = 1.3071572874513513. Every other line, including all gyro and mag
% terms, is byte-identical to ANELLO_X3_IMU_fcn_SIL.m. This is a candidate, NOT production -- see
% model_manifest.json (P0.2 gate) before treating this as the active model.
%
% Derivation of ACCEL_VRW_SCALE (recorded here for auditability, not re-derived at runtime):
%   1. P1's static real-vs-sim comparison found the real/sim PSD ratio for accel is FLAT across
%      0.1-15 Hz (not decaying with frequency) in BOTH the 3h and 6h static_a1_siphog captures --
%      the signature of a plain white-noise-LEVEL (VRW) mismatch, not a missing colored/OU
%      process (which would show a decaying, not flat, ratio).
%   2. The 1h capture's accel_x/accel_y std is an outlier (13.5x and 2.6x sim, vs. the 3h/6h
%      captures' consistent ~1.3-1.4x) -- diagnosed as a session-specific transient/artifact, NOT a
%      reproducible sensor property, and excluded from this fit. accel_z did not show this 1h
%      anomaly and its PSD ratio was consistent across all three durations, but this fit still
%      only uses 3h/6h for a uniform, defensible basis across axes.
%   3. Per-axis flat-band PSD ratios (0.1-15 Hz, median), pooled 3h+6h:
%        accel_x: 1.625 (3h), 1.685 (6h)
%        accel_y: 1.603 (3h), 1.733 (6h)
%        accel_z: 1.825 (3h), 1.838 (6h)
%      median of all six = 1.7086601741371747
%      ACCEL_VRW_SCALE = sqrt(1.7086601741371747) = 1.3071572874513513
%      (PSD ratio -> sigma-scale via sqrt, since PSD ~ sigma^2 for white noise.)
%   4. A single shared scale factor across x/y/z (not a per-axis fit) is deliberate -- the three
%      per-axis values were reasonably consistent (1.29-1.35x), and the plan explicitly warns
%      against promoting an axis-specific fix without checking it generalizes (same principle as
%      the project's existing gyro-Z ou1_sigma caution). This is NOT a new stochastic process
%      (no OU/pink term added) -- exactly what the flat-PSD evidence supports and what
%      comparison_config.json's P1 tuning policy allows without extra scrutiny ("Conservar
%      white_sigma/ARW/VRW identificados de forma robusta").
%
% Everything else below is copy-pasted from ANELLO_X3_IMU_fcn_SIL.m -- see that file's own header
% for the general model documentation (persistent-state recursion structure, MATLAB Function block
% compile-bug workaround, etc.), not repeated here.

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
        return;
    end
end

%% ---- Timing / constants (anelloX3Params_SIL.m, Ts=0.004s / fs=250Hz) -----
Ts  = 0.004;
fs  = 1 / Ts;
d2r = pi / 180;
T0  = 25.0;

% =====================================================================
%  GYROSCOPE MODEL  (optical SiPhOG) -- UNCHANGED from ANELLO_X3_IMU_fcn_SIL.m
% =====================================================================
gyro_bias_radps     = 0.5 * d2r / 3600;                  % bias instability, < 0.5 deg/hr
gyro_ARW_radps_sqrts = 0.05 * d2r / 60;                  % ARW, < 0.05 deg/sqrt(hr)
gyro_sigma_radps    = gyro_ARW_radps_sqrts * sqrt(fs);
gyro_scaleFactorErr = 0.001;                             % 0.1 %
gyro_range_radps    = 400 * d2r;
gyro_tempCoeff_radps_C = 5e-8;
gyro_staticBias     = [0;0;0];
gyro_misalignment   = eye(3);
gyro_scaleFactor    = [gyro_scaleFactorErr; gyro_scaleFactorErr; gyro_scaleFactorErr];

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

gy_stochastic = gy_bias + gy_pink + gy_rrw;

g_scaled = gyro_misalignment * diag(1.0 + gyro_scaleFactor) * gyro_true;
g_tbias  = gyro_tempCoeff_radps_C .* (T - T0) .* ones(3,1);
g_noise  = gyro_sigma_radps .* randn(3,1);
g_raw    = g_scaled + gyro_staticBias + g_tbias + gy_stochastic + g_noise;

gyro_meas = min(max(g_raw, -gyro_range_radps .* ones(3,1)), gyro_range_radps .* ones(3,1));
gyro_bias = gy_stochastic;

% =====================================================================
%  ACCELEROMETER MODEL  (MEMS) -- accel_VRW_mps_sqrts scaled, see header derivation
% =====================================================================
ACCEL_VRW_SCALE = 1.3071572874513513;   % see header derivation, P1 flat-PSD fit (3h+6h, all axes)

g0 = 9.80665;
accel_bias_mps2     = 20e-6 * g0;                        % bias instability, 20 ug -- UNCHANGED
accel_VRW_mps_sqrts = (0.03 / sqrt(3600)) * ACCEL_VRW_SCALE;   % VRW, scaled from datasheet spec
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

% Pink (1/f) noise -- UNCHANGED: newIMU_model uses the bias-instability magnitude itself as the
% pink-noise sigma (p.accel.pinkNoise_mps2 = p.accel.bias_mps2). Velocity random walk is
% disabled in that source config (enableVelocityRandomWalk=false), so no RRW term here.
% Deliberately NOT scaled or replaced by an OU term -- the flat-PSD evidence (header) points to a
% pure white-noise-level fix, not a colored-noise gap.
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
%  MAGNETOMETER MODEL -- UNCHANGED from ANELLO_X3_IMU_fcn_SIL.m
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
