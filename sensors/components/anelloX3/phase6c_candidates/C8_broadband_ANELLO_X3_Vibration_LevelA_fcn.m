function [accel_vib_mps2, gyro_vib_radps] = C8_broadband_ANELLO_X3_Vibration_LevelA_fcn(rotorAngVel_radps, enableVibration)
% C8_BROADBAND -- M2.1 candidate: adds a filtered broadband stochastic component on top of
% PRODUCTION's tonal vibration model (fundamental + BPF), per
% PLAN_MISION_CIERRE_FIDELIDAD_IMU_SIPHOG_M0_M5.md M2.1:
%
%   vibration_total = vibration_tonal + vibration_broadband
%   vibration_broadband = G1*BP_0_5(noise1) + G2*BP_5_15(noise2) + G3*BP_18_25(noise3)
%                         + G4*BP_30_45(noise4)
%
% Deliberately NOT built by shadowing production's function name the way C7 does (C7 fully
% REPLACES production's tonal generation so it can be file-copied over production for a live HIL
% run). C8 only ADDS a term on top, so it is a genuinely distinct function that calls production's
% ANELLO_X3_Vibration_LevelA_fcn directly for the tonal part -- no path-shadowing needed, and it
% can never silently drift out of sync with production's tonal model since it isn't a copy of it.
% If this candidate is ever promoted to a real HIL run, give it production's exact function name
% at that time (an M6 promotion step, not done here).
%
% Fit basis (TRAIN only -- session_vibration_repaired_full_mission_01, all 5 regimes pooled,
% Production_SIL as the reference "current sim" baseline): one shared gain per band per sensor
% (NOT per-axis, per the plan's "evitar demasiados parametros por eje" rule), solved from
% needed_added_power = max(0, real_band_power - Production_SIL_band_power), averaged over axis and
% regime, divided by each band filter's own output variance under unit-variance white-noise input
% (measured empirically, 2e6-sample offline calibration -- see
% AnalysisIMU/final_real_vs_sim/tables/M2_C8_target_broadband_power.csv for the target powers).
% Independent noise realization per axis (no cross-axis coupling introduced deliberately -- that is
% M2.3/C9's separate problem, not this one's).
%
% Filters: 2nd-order Butterworth (4th-order effective for the 3 band-pass bands, since a band-pass
% Butterworth of order N has 2N poles), fs=250 Hz, coefficients precomputed offline
% (scipy.signal.butter) and hardcoded here -- same "no runtime filter design, no struct calls"
% convention as ANELLO_X3_IMU_fcn_SIL.m, for the same MATLAB-Function-block compile-safety reason.
% Implemented as a direct-form-II-transposed per-sample recursion with persistent state per
% axis/band/sensor (24 independent single-input-single-output filter instances: 4 bands x 3 axes
% x 2 sensors).
%
% Inputs/outputs: identical contract to ANELLO_X3_Vibration_LevelA_fcn.

persistent w_lp_a w_bp1_a w_bp2_a w_bp3_a;   % accel filter states, [order x 3 axes]
persistent w_lp_g w_bp1_g w_bp2_g w_bp3_g;   % gyro filter states

if isempty(w_lp_a)
    w_lp_a  = zeros(2, 3); w_bp1_a = zeros(4, 3); w_bp2_a = zeros(4, 3); w_bp3_a = zeros(4, 3);
    w_lp_g  = zeros(2, 3); w_bp1_g = zeros(4, 3); w_bp2_g = zeros(4, 3); w_bp3_g = zeros(4, 3);
end

% ---- Tonal part: production's own model, called directly (not shadowed) --------------------
[accel_tonal, gyro_tonal] = ANELLO_X3_Vibration_LevelA_fcn(rotorAngVel_radps, enableVibration);

if enableVibration == 0
    accel_vib_mps2 = accel_tonal;
    gyro_vib_radps = gyro_tonal;
    return;
end

% ---- Filter coefficients (fs=250 Hz, precomputed via scipy.signal.butter) -------------------
b_lp  = [0.0036216815, 0.0072433630, 0.0036216815];       a_lp  = [-1.8226949252,  0.8371816513];
b_bp1 = [0.0133592000, 0.0, -0.0267184001, 0.0, 0.0133592000];
a_bp1 = [-3.5609474528, 4.8388639737, -2.9769296154, 0.7008967812];
b_bp2 = [0.0068678657, 0.0, -0.0137357315, 0.0, 0.0068678657];
a_bp2 = [-3.2301588846, 4.3697378167, -2.8509340829, 0.7797394590];
b_bp3 = [0.0278597661, 0.0, -0.0557195322, 0.0, 0.0278597661];
a_bp3 = [-2.0796728736, 2.5720148550, -1.5853111255, 0.5869195081];

% ---- Gains -----------------------------------------------------------------------------------
% First pass used an independent per-band closed-form fit (gain^2 = target/own-band-unit-variance)
% and overshot gyro's 5-15/18-25/30-45 Hz bands by 40-150x once actually run -- these 4 filters
% are not perfectly band-isolated (only 2nd/4th-order Butterworth), so a large 0-5Hz gain leaks
% non-negligible power into the higher bands. Attempted fix #2 (measuring the full 4x4 cross-band
% "leakage matrix" L and solving G^2 = L^-1 * target) made accel's fit noticeably BETTER (its 4
% targets are comparable magnitude, well-conditioned) but made gyro WORSE: gyro's targets span
% >10,000x across bands (0.021 down to 2e-6), so L's own Monte-Carlo measurement noise on its small
% off-diagonal entries gets amplified by the inversion into a meaningless correction for the tiny
% bands. Final choice for gyro: fit ONLY the dominant, well-conditioned 0-5Hz band; leave bands
% 5-15/18-25/30-45 at gain 0 (no broadband injection there at all) rather than chase targets too
% small to fit reliably through this filter bank -- those 3 bands keep whatever small pre-existing
% mismatch production/C7 already have (not a regression, since nothing here changes them). Accel
% keeps the leakage-corrected 4-band fit (verified: D3 broadband error 22.8dB -> ~4.4dB, no
% regression on tonal bands). See AnalysisIMU/final_real_vs_sim/tables/M2_C8_leakage_matrix.npy.
G_accel = [2.2177146100, 0.5014011500, 1.1786216400, 0.7244736800];   % [0-5, 5-15, 18-25, 30-45] Hz
G_gyro  = [0.7933651000, 0.0000000000, 0.0000000000, 0.0000000000];
%
% DECISION (2026-08-07, confirmed with project owner): even at filter order 8 (tested, not just
% order 2/4 used here), the 0-5Hz lowpass's stopband leakage into 5-15Hz is still ~5% of its own
% in-band power -- at the gain needed to close gyro's real, large 0-5Hz gap (real=0.038 rad^2/s^2
% vs production's sim=0.018), that leakage alone is ~30x gyro's TRUE signal in the 5-15/18-25/
% 30-45 Hz bands (which are themselves tiny in absolute terms, 1e-6 to 4e-5 rad^2/s^2). No
% single-noise-source-per-band filter design can close the large 0-5Hz gap without this leakage,
% given how much smaller gyro's true higher-band content is. Decision: accept the 0-5Hz fit and
% the resulting dB-scale "regression" on gyro's 3 tiny higher bands -- it is real but physically
% negligible (absolute power stays in the 1e-4 rad^2/s^2 range or below, far under any band gyro
% actually cares about), not chased further. Accel's 4-band fit is unaffected by this decision.

noise_a = randn(3, 4);   % [axis x band], independent per axis and band
noise_g = randn(3, 4);

bb_accel = zeros(3, 1);
bb_gyro  = zeros(3, 1);
for ax = 1:3
    [y, w_lp_a(:, ax)]  = df2t_step(b_lp,  a_lp,  noise_a(ax, 1), w_lp_a(:, ax));  bb_accel(ax) = bb_accel(ax) + G_accel(1) * y;
    [y, w_bp1_a(:, ax)] = df2t_step(b_bp1, a_bp1, noise_a(ax, 2), w_bp1_a(:, ax)); bb_accel(ax) = bb_accel(ax) + G_accel(2) * y;
    [y, w_bp2_a(:, ax)] = df2t_step(b_bp2, a_bp2, noise_a(ax, 3), w_bp2_a(:, ax)); bb_accel(ax) = bb_accel(ax) + G_accel(3) * y;
    [y, w_bp3_a(:, ax)] = df2t_step(b_bp3, a_bp3, noise_a(ax, 4), w_bp3_a(:, ax)); bb_accel(ax) = bb_accel(ax) + G_accel(4) * y;

    [y, w_lp_g(:, ax)]  = df2t_step(b_lp,  a_lp,  noise_g(ax, 1), w_lp_g(:, ax));  bb_gyro(ax) = bb_gyro(ax) + G_gyro(1) * y;
    [y, w_bp1_g(:, ax)] = df2t_step(b_bp1, a_bp1, noise_g(ax, 2), w_bp1_g(:, ax)); bb_gyro(ax) = bb_gyro(ax) + G_gyro(2) * y;
    [y, w_bp2_g(:, ax)] = df2t_step(b_bp2, a_bp2, noise_g(ax, 3), w_bp2_g(:, ax)); bb_gyro(ax) = bb_gyro(ax) + G_gyro(3) * y;
    [y, w_bp3_g(:, ax)] = df2t_step(b_bp3, a_bp3, noise_g(ax, 4), w_bp3_g(:, ax)); bb_gyro(ax) = bb_gyro(ax) + G_gyro(4) * y;
end

accel_vib_mps2 = accel_tonal + bb_accel;
gyro_vib_radps = gyro_tonal + bb_gyro;

end

function [y, w_new] = df2t_step(b, a, x, w)
% Direct-form-II-transposed single-sample update. b has N+1 coeffs, a has N coeffs (a0=1 implicit,
% not stored), w is the [N x 1] state vector.
N = numel(a);
y = b(1) * x + w(1);
w_new = zeros(N, 1);
for i = 1:N-1
    w_new(i) = b(i+1) * x - a(i) * y + w(i+1);
end
w_new(N) = b(N+1) * x - a(N) * y;
end
