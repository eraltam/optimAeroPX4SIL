function [accel_vib_mps2, gyro_vib_radps] = C9_gyrocoupling_ANELLO_X3_Vibration_LevelA_fcn(rotorAngVel_radps, enableVibration)
% C9_GYROCOUPLING -- M2.3 candidate: corrects gyro's tonal-band PRINCIPAL DIRECTION only, on top
% of C8 (M2.1's broadband candidate). Calls C8 directly (same wrapper convention C8 itself uses
% to call production -- no shadowing, never drifts out of sync), then rotates ONLY the 50-65 Hz
% tonal content of gyro_vib_radps toward real's principal direction. accel_vib_mps2 is passed
% through from C8 completely unmodified -- this candidate touches gyro direction only, per the
% plan's explicit rule that direction is a separate problem from power (M2.3 vs M2.1) and per the
% reviewer's instruction not to fix a direction by changing power.
%
% 27-31Hz DELIBERATELY NOT CORRECTED (decided with project owner, 2026-08-07): a per-band rotation
% was fit for 27-31Hz too (see git history / M2_3_C9_GYRO_COUPLING.md for the full result) and gave
% a large angle improvement (e.g. giros: 82.6 -> 29.6 deg) but failed the reviewer's explicit joint
% angle-AND-identifiability gate in 2 of 5 regimes (giros, hover -- eigenvalue ratio dropping below
% the 1.3 confidence floor after rotation, including in `giros`, the highest-weight cell used to
% fit it). Root cause: an orthonormal rotation preserves eigenvalue *difference* exactly, not
% *ratio*, once an additive isotropic noise floor is present in the band -- ratio is not
% rotation-invariant in that case, contrary to this file's own earlier assumption. Decision: leave
% 27-31Hz exactly as C8/production produces it (no correction, no regression, no identifiability
% caveat) rather than accept a partially-out-of-gate fix. 50-65Hz passed the joint gate cleanly in
% all 5 regimes and is the only band corrected here.
%
% WHY A ROTATION, AND WHY PER-BAND, NOT ONE SHARED ROTATION OR A FREE 3x3:
% An orthonormal rotation R preserves ||R*v|| = ||v|| exactly, so applying it to a band-limited
% component cannot change that band's total power -- D3's per-band power gate is satisfied by
% construction for gyro's tonal bands, not approximately (verified numerically after
% implementation, not just assumed from the math -- see M2_3_C9_GYRO_COUPLING.md). A free 3x3 (9
% DOF) was deliberately avoided per the reviewer's instruction; an SO(3) rotation is 3 DOF.
%
% A single ROTATION SHARED across both tonal bands was fit first and tried: weighted-mean angle
% improved (76.2 -> 35.4 deg) but was inconsistent -- one regime (hover, 27-31Hz) got WORSE
% (55 -> 76 deg) because the fit is dominated by the higher-weight 50-65Hz cells, and a single
% rotation cannot serve two bands whose real coupling geometry differs (plausible: fundamental
% rotor imbalance vs. blade-pass aerodynamic coupling are different physical mechanisms). Fitting
% one rotation PER BAND separately gave a clearly better and more consistent result (27-31Hz:
% 76.8 -> 45.3 deg weighted, one regime worse but the dominant giros cell drops 83 -> 19 deg;
% 50-65Hz: 76.0 -> 24.0 deg weighted, ALL 4 usable cells improve). Confirmed with project owner
% before implementing the extra filtering this requires.
%
% FIT BASIS (TRAIN only -- session_vibration_repaired_full_mission_01, C8 as baseline for gyro
% tonal content since C8 does not touch it):
%   - only (regime, band) cells with eigenvalue ratio >= 1.3 for BOTH real and sim, AND >= 2
%     segments on both sides, were used to fit R (7/10 candidate cells passed; descenso excluded
%     for both bands for having only 1 real segment, vuelo_recto/27-31hz excluded for sim
%     eigenvalue ratio 1.17 < 1.3);
%   - each usable cell weighted by min(eigenvalue_ratio_real, eigenvalue_ratio_sim) / bootstrap_
%     CI_width_deg, so well-identified, narrow-CI cells (e.g. hover/50-65hz: ratio 6.3, CI 12.6 deg)
%     dominate the fit over thin/uncertain ones (e.g. ascenso: only 2-4 segments, wide CI);
%   - solved via weighted Kabsch/Wahba (scipy Rotation.align_vectors), sign ambiguity resolved by
%     flipping each pair's sim vector to match real's sign before fitting;
%   - see AnalysisIMU/final_real_vs_sim/scripts/m2_fit_c9_gyro_rotation.py and
%     tables/M2_C9_fit_cells.csv for the full per-cell derivation.
%
% Inputs/outputs: identical contract to ANELLO_X3_Vibration_LevelA_fcn / C8's wrapper.

persistent w_5065;   % gyro-only isolation-filter state, [order x 3 axes]
if isempty(w_5065)
    w_5065 = zeros(4, 3);
end

[accel_vib_mps2, gyro_c8] = C8_broadband_ANELLO_X3_Vibration_LevelA_fcn(rotorAngVel_radps, enableVibration);

if enableVibration == 0
    gyro_vib_radps = gyro_c8;
    return;
end

% ---- Isolation filter (fs=250 Hz, precomputed via scipy.signal.butter) ----------------------
b_5065 = [0.0278597661, 0.0, -0.0557195322, 0.0, 0.0278597661];
a_5065 = [-0.4434478834, 1.5253364381, -0.3380353093, 0.5869195081];

% ---- Fitted rotation (50-65Hz only, SO(3), see derivation above) ----------------------------
R_5065 = [ 0.3118237837,  0.5329830977,  0.7865716404; ...
          -0.5456064221,  0.7781944404, -0.3110097185; ...
          -0.7778686008, -0.3321783113,  0.5334585358];
D_5065 = R_5065 - eye(3);

comp_5065 = zeros(3, 1);
for ax = 1:3
    [y2, w_5065(:, ax)] = df2t_step(b_5065, a_5065, gyro_c8(ax), w_5065(:, ax));
    comp_5065(ax) = y2;
end

gyro_vib_radps = gyro_c8 + D_5065 * comp_5065;

end

function [y, w_new] = df2t_step(b, a, x, w)
% Direct-form-II-transposed single-sample update -- identical structure to C8's, kept local here
% rather than shared, matching this codebase's existing convention of not calling functions that
% return non-trivial values across MATLAB Function block boundaries (see
% ANELLO_X3_IMU_fcn_SIL.m's header for the underlying compile-safety reason).
N = numel(a);
y = b(1) * x + w(1);
w_new = zeros(N, 1);
for i = 1:N-1
    w_new(i) = b(i+1) * x - a(i) * y + w(i+1);
end
w_new(N) = b(N+1) * x - a(N) * y;
end
