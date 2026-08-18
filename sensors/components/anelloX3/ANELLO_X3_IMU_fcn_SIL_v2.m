function [accel_meas, gyro_meas, mag_meas, accel_bias, gyro_bias, mag_bias] = ...
         ANELLO_X3_IMU_fcn_SIL_v2(accel_true, gyro_true, mag_true, T, reset, seedVal)
% ANELLO_X3_IMU_FCN_SIL_V2  Corrected-noise-density candidate, versioned per
% STATIC_ACCEPTANCE_CRITERIA_PROPOSAL.md (approved 2026-07-22).
%
% Identical to ANELLO_X3_IMU_fcn_SIL.m in every respect EXCEPT the white-noise (ARW/VRW)
% coefficients, which are set PER AXIS to each axis's own physical three-capture
% repeatability-window mean (AnalysisIMU/static_comparison/tables/step5_repeatability_
% physical.csv, corrected sqrt(tau)-conversion values), replacing the single scalar
% datasheet value with a 3x1 vector:
%
%   gyro ARW  [x;y;z] deg/sqrt(hr):  0.05 (datasheet, scalar) -> [0.289316; 0.273180; 0.266705]
%   accel VRW [x;y;z] m/s/sqrt(hr):  0.03 (datasheet, scalar) -> [0.083592; 0.102087; 0.081214]
%
% A single scalar (even the x/y/z arithmetic mean, first tried and rejected here) cannot
% simultaneously satisfy every axis's own mean+/-2*observed-SD acceptance band -- the mean
% scalar for gyro (0.2764) exceeds gyro_z's own band upper bound (0.275882), and the mean
% scalar for accel (0.0890) exceeds accel_z's own band upper bound (0.086328). Using a
% per-axis vector instead is barely more complex (the existing ".* randn(3,1)" elementwise
% multiply already supports a 3x1 vector exactly as it did a scalar) and trivially sits at
% the center of each axis's own band, since the target IS that axis's own historical mean.
%
% Bias instability (Gauss-Markov), pink noise, rate-random-walk, scale factor, temperature
% coefficient, and range/saturation are UNCHANGED from the datasheet/current model -- the
% approved acceptance criteria gate only the white-noise ARW/VRW coefficient and the
% short-tau Allan slope (STATIC_ACCEPTANCE_CRITERIA_PROPOSAL.md Section 3); everything else
% remains required descriptive evidence, not a gate, and is deliberately left untouched here
% rather than changed without a corresponding approved criterion.
%
% This is a NEW, SEPARATELY VERSIONED file. ANELLO_X3_IMU_fcn_SIL.m (the current model) is
% NOT modified and remains the active default.
%
%#codegen

persistent ga_bias  gy_bias  mg_bias;
persistent gy_pink  gy_rrw   ga_pink;

if isempty(ga_bias)
    % Explicit RNG seeding, held-out validation protocol (STATIC_ACCEPTANCE_CRITERIA_
    % PROPOSAL.md Section 7.2): calling rng() from OUTSIDE this chart (base workspace
    % before sim(), or the model's InitFcn) was tested and confirmed NOT to affect this
    % compiled Stateflow chart's own randn() draws -- three "different-seed" runs produced
    % bit-identical output both ways. Seeding from WITHIN the chart's own execution context,
    % on its natural one-time first-call branch, is the only method confirmed to work.
    rng(seedVal, 'twister');
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
%  GYROSCOPE MODEL  (optical SiPhOG) -- CANDIDATE v2: ARW corrected to physical per-axis means
% =====================================================================
gyro_bias_radps     = 0.5 * d2r / 3600;                  % bias instability -- UNCHANGED
% CORRECTED, per-axis (was a single scalar 0.05 deg/sqrt(hr)): a single scalar cannot
% simultaneously satisfy every axis's own mean+/-2*observed-SD band (STATIC_ACCEPTANCE_
% CRITERIA_PROPOSAL.md Section 3) -- e.g. the x/y/z arithmetic-mean scalar (0.2764) exceeds
% gyro_z's own upper bound (0.275882). Using each axis's own physical repeatability-window
% mean instead trivially sits at the center of that axis's own band. This is a per-axis
% VECTOR, not a scalar -- the existing ".* randn(3,1)" elementwise syntax already supports it
% without any other structural change.
gyro_ARW_radps_sqrts = [0.289316; 0.273180; 0.266705] * d2r / 60;   % [x;y;z], physical means
gyro_sigma_radps    = gyro_ARW_radps_sqrts .* sqrt(fs);
gyro_scaleFactorErr = 0.001;                             % 0.1 % -- UNCHANGED
gyro_range_radps    = 400 * d2r;
gyro_tempCoeff_radps_C = 5e-8;
gyro_staticBias     = [0;0;0];
gyro_misalignment   = eye(3);
gyro_scaleFactor    = [gyro_scaleFactorErr; gyro_scaleFactorErr; gyro_scaleFactorErr];

Tc_g       = 300.0;                              % correlation time [s] -- UNCHANGED
alpha_g    = exp(-Ts / Tc_g);
sig_eta_g  = gyro_bias_radps * sqrt(1.0 - alpha_g^2);
gy_bias    = alpha_g .* gy_bias + sig_eta_g .* randn(3,1);

% Pink (1/f) noise -- UNCHANGED from ANELLO_X3_IMU_fcn_SIL.m
gyro_pinkNoise_radps = 1.0e-4 * d2r;
pinkTc_g   = max(10.0 * Ts, 10.0);
alphaPink_g = exp(-Ts / pinkTc_g);
gy_pink = alphaPink_g .* gy_pink + gyro_pinkNoise_radps * sqrt(max(0.0, 1.0 - alphaPink_g^2)) .* randn(3,1);

% Rate random walk (RRW) -- UNCHANGED
gyro_rrw_radps_sqrts = 5.0e-6 * d2r;
gy_rrw = gy_rrw + gyro_rrw_radps_sqrts * sqrt(Ts) .* randn(3,1);

gy_stochastic = gy_bias + gy_pink + gy_rrw;

g_scaled = gyro_misalignment * diag(1.0 + gyro_scaleFactor) * gyro_true;
g_tbias  = gyro_tempCoeff_radps_C .* (T - T0) .* ones(3,1);
g_noise  = gyro_sigma_radps .* randn(3,1);
g_raw    = g_scaled + gyro_staticBias + g_tbias + gy_stochastic + g_noise;

gyro_meas = min(max(g_raw, -gyro_range_radps .* ones(3,1)), gyro_range_radps .* ones(3,1));
gyro_bias = gy_stochastic;

% =====================================================================
%  ACCELEROMETER MODEL  (MEMS) -- CANDIDATE v2: VRW corrected to physical per-axis means
% =====================================================================
g0 = 9.80665;
accel_bias_mps2     = 20e-6 * g0;                        % bias instability -- UNCHANGED
% CORRECTED, per-axis (was a single scalar 0.03 m/s/sqrt(hr)): the x/y/z arithmetic-mean
% scalar (0.0890) exceeds accel_z's own upper bound (0.086328) -- same reasoning as gyro
% above. Per-axis vector, same ".* randn(3,1)" syntax, no other structural change.
accel_VRW_mps_sqrts = [0.083592; 0.102087; 0.081214] / sqrt(3600);  % [x;y;z], physical means
accel_sigma_mps2    = accel_VRW_mps_sqrts .* sqrt(fs);
accel_range_mps2    = 16 * g0;
accel_tempCoeff_mps2_C = 1e-5;
accel_staticBias    = [0;0;0];
accel_scaleFactor   = [0;0;0];
accel_misalignment  = eye(3);

Tc_a      = 200.0;                                       % UNCHANGED
alpha_a   = exp(-Ts / Tc_a);
sig_eta_a = accel_bias_mps2 * sqrt(1.0 - alpha_a^2);
ga_bias   = alpha_a .* ga_bias + sig_eta_a .* randn(3,1);

% Pink (1/f) noise -- UNCHANGED
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
%  MAGNETOMETER MODEL -- UNCHANGED (not in scope of the approved static gate)
% =====================================================================
mag_sigma_G        = 0.4e-3;
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
