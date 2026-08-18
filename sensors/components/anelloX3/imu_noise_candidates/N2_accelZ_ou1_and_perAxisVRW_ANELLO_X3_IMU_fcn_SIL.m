function [accel_meas, gyro_meas, mag_meas, accel_bias, gyro_bias, mag_bias] = ...
         N2_accelZ_ou1_and_perAxisVRW_ANELLO_X3_IMU_fcn_SIL(accel_true, gyro_true, mag_true, T, reset)
% N2_ACCELZ_OU1_AND_PERAXISVRW_ANELLO_X3_IMU_FCN_SIL  Candidate N2: builds on N1
% (N1_accelVRW_scale1p307_ANELLO_X3_IMU_fcn_SIL.m) with an accel_z-SPECIFIC correction, identified
% from a follow-up residual-classification pass (PLAN_CIERRE_IMU_SIPHOG_REAL_VS_SIM_FIGURAS_TEX.md
% P1, "make the sim similar to the physical" follow-up, 2026-08-06).
%
% ONLY CHANGES from N1:
%   1. accel_VRW_mps_sqrts becomes a per-axis [3x1] vector: x/y keep N1's scale (1.3072x
%      production); z gets a LARGER scale (1.4758x production), fitted directly (not derived from
%      the cruder flat-PSD-ratio method N1 used).
%   2. accel_bias_mps2 (the Gauss-Markov "ou1"/bias-instability sigma) becomes per-axis: x/y keep
%      the original UNSCALED value; z gets a ~4.06x larger value, fitted directly.
%   3. Pink noise ("ou2") is explicitly DECOUPLED from accel_bias_mps2 and kept at the ORIGINAL
%      unscaled scalar value for ALL axes including z -- see derivation step 4 below for why.
% Gyro and mag are untouched (byte-identical to N1/production).
%
% Derivation (recorded here for auditability, not re-derived at runtime):
%   1. After N1 (flat white-noise-level fix), x/y showed NO further residual (real/N1 PSD ratio
%      ~0.94-1.08x across 0.1-15 Hz in both 3h and 6h) -- left unchanged. accel_z still showed a
%      LOW-FREQUENCY-DOMINANT residual (ratio ~1.46-1.56x at 0.01-0.1 Hz, decaying to ~1.06x by
%      5-15 Hz, consistent in both 3h and 6h) -- the signature of a missing/undersized colored
%      (OU/bias-instability) component, not a further white-level issue.
%   2. Per the plan's explicit caution ("usar como maximo un OU minimo... no promoverlo
%      automaticamente desde gyro-Z"), a joint white+OU1+OU2 model was fit against real accel_z
%      Allan deviation (script: AnalysisIMU/final_real_vs_sim/scripts/fit_accel_z_ou.py), reusing
%      the exact methodology already validated for gyro_z in
%      vibration_full_mission_comparison/scripts/phase10_item3_stochastic_identification.py.
%   3. A synthetic-recovery-first gate is mandatory in that script. The FIRST attempt (5 free
%      parameters: white_sigma, ou1_sigma, ou1_tc, ou2_sigma, ou2_tc) FAILED the gate -- the two OU
%      time constants were not identifiable from Allan data alone (recovered/true ratios 0.24 and
%      0.43), the exact non-identifiability failure mode this project has already documented for
%      2-OU models. Per that finding, the fit was REDONE constrained to 3 free parameters
%      (white_sigma, ou1_sigma, ou2_sigma; ou1_tc=200s and ou2_tc=10s held FIXED at the existing
%      production values, not re-fit). This constrained version PASSED the synthetic recovery gate
%      cleanly (all 5 params, including the 2 fixed ones trivially, recovered within
%      0.5x-2.0x of truth).
%   4. Real-data fit against siphog_6h accel_z (common rate, 50 Hz), then independently re-fit on
%      1h and 3h to check cross-duration stability (frozen practice, same as gyro_z's Item 3):
%        white_sigma [common-rate]: 6h=0.005180, 3h=0.005255, 1h=0.005055 -- stable across all three.
%        ou1_sigma:                  6h=0.000788, 3h=0.000806, 1h=0.001962 -- 3h/6h agree closely
%                                     (within 2.3%); 1h is a ~2.5x outlier, consistent with the
%                                     independently-diagnosed 1h accel_x/y contamination
%                                     (FINDING_1H_ACCEL_X_STEP_EVENT.md) -- 1h excluded from the
%                                     promoted value for the same reason N1 excluded it.
%        ou2_sigma (pink):           6h=0.000128 (nonzero); 3h=0.0000100, 1h=0.0000100 -- PINNED AT
%                                     THE FIT'S LOWER BOUND for 1h and 3h, i.e. not robustly
%                                     identified at those durations. NOT adopted -- pink noise is
%                                     left at its original, unscaled value for all axes. Promoting
%                                     an estimate that only two of three durations can even resolve
%                                     away from a bound would repeat exactly the kind of
%                                     single-fit-without-cross-duration-check mistake this
%                                     project's own OU-identification discipline exists to catch.
%   5. Promoted values = average of the 3h and 6h fits only (1h excluded, per step 4):
%        white_sigma_z [common rate, 50 Hz] = mean(0.005255, 0.005180) = 0.0052176
%          -> accel_VRW_mps_sqrts_z = 0.0052176 / sqrt(50) = 7.3788e-4 m/s^2/sqrt(Hz)
%          -> vs. production (0.03/sqrt(3600) = 5.0e-4): scale = 1.4758x
%          -> vs. N1's shared 1.3072x: an ADDITIONAL 1.1290x beyond N1, for z only.
%        ou1_sigma_z = mean(0.000806, 0.000788) = 7.9717e-4 m/s^2
%          -> vs. production/N1 accel_bias_mps2 (20e-6*9.80665 = 1.9613e-4): scale = 4.0645x.
%   6. RMSE improvement (log10 Allan deviation, vs. real siphog_6h accel_z): N1-baseline-equivalent
%      params ident RMSE 0.212 -> fitted 0.025 (8.5x better); holdout (tau>200s) RMSE 0.580 -> 0.111
%      (5.2x better). Full numbers: AnalysisIMU/final_real_vs_sim/tables/accel_z_ou_fit_summary.csv.
%
% Everything else below is copy-pasted from N1/production -- see those files' headers for the
% general model documentation, not repeated here.

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
%  ACCELEROMETER MODEL  (MEMS) -- per-axis VRW and bias-instability (z != x,y), see header
% =====================================================================
ACCEL_VRW_SCALE_XY = 1.3071572874513513;   % N1 value, unchanged for x/y
ACCEL_VRW_SCALE_Z  = 1.4757575542300887;   % new, z-specific (fit_accel_z_ou.py, 3h+6h average)
ACCEL_BIAS_SCALE_XY = 1.0;                 % unchanged for x/y
ACCEL_BIAS_SCALE_Z  = 4.064451037392974;   % new, z-specific (fit_accel_z_ou.py, 3h+6h average)

g0 = 9.80665;
accel_bias_mps2_base = 20e-6 * g0;                       % original datasheet bias-instability, 20 ug
accel_VRW_mps_sqrts_base = 0.03 / sqrt(3600);            % original datasheet VRW

accel_VRW_mps_sqrts = [accel_VRW_mps_sqrts_base * ACCEL_VRW_SCALE_XY; ...
                        accel_VRW_mps_sqrts_base * ACCEL_VRW_SCALE_XY; ...
                        accel_VRW_mps_sqrts_base * ACCEL_VRW_SCALE_Z];        % [3x1], per-axis
accel_sigma_mps2    = accel_VRW_mps_sqrts * sqrt(fs);                        % [3x1]

accel_bias_mps2_ou1 = [accel_bias_mps2_base * ACCEL_BIAS_SCALE_XY; ...
                        accel_bias_mps2_base * ACCEL_BIAS_SCALE_XY; ...
                        accel_bias_mps2_base * ACCEL_BIAS_SCALE_Z];          % [3x1], ou1 only

accel_range_mps2    = 16 * g0;
accel_tempCoeff_mps2_C = 1e-5;
accel_staticBias    = [0;0;0];
accel_scaleFactor   = [0;0;0];
accel_misalignment  = eye(3);

Tc_a      = 200.0;
alpha_a   = exp(-Ts / Tc_a);
sig_eta_a = accel_bias_mps2_ou1 .* sqrt(1.0 - alpha_a^2);     % [3x1] now, was scalar in N1/production
ga_bias   = alpha_a .* ga_bias + sig_eta_a .* randn(3,1);

% Pink (1/f) noise -- DELIBERATELY kept at the ORIGINAL unscaled scalar for ALL axes (including z)
% -- the z-specific pink/ou2 fit was pinned at its lower bound for 1h/3h (not robustly identified
% across durations), so it is NOT adopted here. This is why pink is decoupled from
% accel_bias_mps2_ou1 above (which IS scaled for z) -- scaling both from the same variable would
% have silently carried the z bias-instability fix into pink too, which the evidence does not
% support.
pinkTc_a   = max(10.0 * Ts, 10.0);
alphaPink_a = exp(-Ts / pinkTc_a);
ga_pink = alphaPink_a .* ga_pink + accel_bias_mps2_base * sqrt(max(0.0, 1.0 - alphaPink_a^2)) .* randn(3,1);

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
