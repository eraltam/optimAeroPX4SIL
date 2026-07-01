function [accel_meas, gyro_meas, mag_meas, accel_bias, gyro_bias, mag_bias] = ...
         ANELLO_X3_IMU_fcn_SIL(accel_true, gyro_true, mag_true, T, reset)
% ANELLO_X3_IMU_FCN_SIL  Datasheet-parametric IMU sensor model, SIL sample-rate variant.
%
% Same algorithm and parameter VALUES as anelloX3Params_SIL.m (Ts = 0.004 s, matching
% optimAeroPX4SIL's fixed step), but with every parameter inlined as a local constant
% instead of read from a struct returned by anelloX3Params_SIL().
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
% Inputs:
%   accel_true  [3x1]  true specific force, body frame [m/s^2]
%   gyro_true   [3x1]  true angular rate,   body frame [rad/s]
%   mag_true    [3x1]  true magnetic field,  body frame [Gauss]
%   T           [1x1]  sensor temperature [deg C]
%   reset       [1x1]  nonzero -> reset all bias states to zero
%
% Outputs:
%   accel_meas, gyro_meas, mag_meas   [3x1]  measured signals
%   accel_bias, gyro_bias, mag_bias   [3x1]  current bias drift (for debug)

%#codegen

persistent ga_bias  gy_bias  mg_bias;

if isempty(ga_bias)
    ga_bias  = zeros(3,1);
    gy_bias  = zeros(3,1);
    mg_bias  = zeros(3,1);
end

% ---- Optional reset --------------------------------------------------
if reset ~= 0
    ga_bias  = zeros(3,1);
    gy_bias  = zeros(3,1);
    mg_bias  = zeros(3,1);
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
%  GYROSCOPE MODEL  (optical SiPhOG)
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

g_scaled = gyro_misalignment * diag(1.0 + gyro_scaleFactor) * gyro_true;
g_tbias  = gyro_tempCoeff_radps_C .* (T - T0) .* ones(3,1);
g_noise  = gyro_sigma_radps .* randn(3,1);
g_raw    = g_scaled + gyro_staticBias + g_tbias + gy_bias + g_noise;

gyro_meas = min(max(g_raw, -gyro_range_radps .* ones(3,1)), gyro_range_radps .* ones(3,1));
gyro_bias = gy_bias;

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

a_scaled = accel_misalignment * diag(1.0 + accel_scaleFactor) * accel_true;
a_tbias  = accel_tempCoeff_mps2_C .* (T - T0) .* ones(3,1);
a_noise  = accel_sigma_mps2 .* randn(3,1);
a_raw    = a_scaled + accel_staticBias + a_tbias + ga_bias + a_noise;

accel_meas = min(max(a_raw, -accel_range_mps2 .* ones(3,1)), accel_range_mps2 .* ones(3,1));
accel_bias = ga_bias;

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
