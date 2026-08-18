function [accel_vib_mps2, gyro_vib_radps] = ANELLO_X3_Vibration_LevelA_fcn(rotorAngVel_radps, enableVibration)
% PHASE 6C CANDIDATE C3 -- DIRECTION ADJUSTMENT ONLY. Ka_z/Ka_xy=2.0 on the accel direction vector
% only (gyro direction unchanged -- Phase 4's PCA evidence was specifically about ACCEL Z-dominance
% in the 18-31Hz bands; gyro's own PCA Z-fractions were low-confidence/inconsistent, so gyro
% direction is left at baseline here, per PLAN Section 8/Phase 6B scope).
%
% UPDATED 2026-08-04 (post R_peak_real correction, see
% ../../../../AnalysisIMU/vibration_full_mission_comparison/ERRATUM_R_PEAK_REAL_CORRECTION_2026-08-04.md):
% the K_fund x0.75/K_bpf x1.25 tonal rebalance this candidate previously inherited from C2 is REMOVED
% -- C2 was dropped from the Phase 6C hardware disposition (it ranked worst of all screened levers
% once R_peak_real's gravity/DC-in-norm bug was fixed; the corrected target is ~0.446, not 1.776).
% K_fund_accel/K_bpf_accel/K_fund_gyro/K_bpf_gyro below are back to production's unchanged values.
% Ka_z=2.0 is retained and is now this disposition's chosen direction value -- picked over the
% offline screening's best-R_peak-match Ka_z=3.0 because 3.0 overshoots the real BPF-band Z-fraction
% target (0.843-0.902) and regresses the already-close fundamental-band Z-fraction (sim 0.95-0.99 ->
% 0.949), which the frozen acceptance criteria (Section 2c) prohibit. Ka_z=2.0 lands inside the BPF
% target range (uniform Z-fraction 0.894) with less fundamental-band regression. R_peak_real_corrected
% (~0.446) is one metric among several gating criteria for this candidate's HIL result -- not an
% optimization target that overrides mission success, EKF safety, clipping, innovation-gate
% behavior, or preservation of already-close bands.
%
% Ka_z/Ka_xy=2.0 is a MODEST, independently-screened candidate value from the {1,1.5,2,3} set --
% explicitly NOT derived from Phase 4's ~0.99 PCA Z-fraction directly (vehicle-level modal
% direction does not uniquely identify each motor's own coupling vector; see plan Phase 6B intro).
% Per-band Ka_xy/Ka_z tuning (a genuinely different value per band, not one scalar for both
% fundamental and BPF) is deferred to Phase 7 as planned -- this candidate is still a single scalar.
%
% accel_dir construction changes from production's [uy; -ux; 1.0] to [uy; -ux; Ka_z] (Ka_xy=1 kept
% at 1.0 here, only Ka_z scaled) before normalization -- this is the Ka_xy/Ka_z parameterization
% from Phase 7 of the plan, introduced here directly for this HIL candidate rather than as a
% separate regression-tested production change (that formal Phase 7 change happens later, only if
% this candidate's HIL result supports it).

persistent phase
if isempty(phase)
    phase = (0:5)' * (2*pi/6);
end

if enableVibration <= 0
    accel_vib_mps2 = zeros(3,1);
    gyro_vib_radps = zeros(3,1);
    return
end

Ts = 0.004;
Nb = 2;

motorLocs = [ 0.0   0.5   0.0;
              0.0  -0.5   0.0;
              0.43 -0.25  0.0;
             -0.43  0.25  0.0;
              0.43  0.25  0.0;
             -0.43 -0.25  0.0];

K_fund_accel = 5.464e-6;
K_bpf_accel  = 1.639e-5;
K_fund_gyro  = 1.366e-7;
K_bpf_gyro   = 4.098e-7;

% CANDIDATE C3 CHANGE: accel direction Z-scale only (gyro direction unchanged from production).
Ka_z = 2.0;

phase = mod(phase + rotorAngVel_radps * Ts, 2*pi);

accel_vib_mps2 = zeros(3,1);
gyro_vib_radps = zeros(3,1);

for i = 1:6
    w2 = rotorAngVel_radps(i)^2;
    s1 = sin(phase(i));
    s2 = sin(Nb*phase(i));

    r = motorLocs(i, 1:2);
    rn = norm(r);
    if rn > 1e-6
        ux = r(1) / rn;
        uy = r(2) / rn;
    else
        ux = 0.0;
        uy = 0.0;
    end
    accel_dir = [uy; -ux; Ka_z];
    accel_dir = accel_dir / norm(accel_dir);
    gyro_dir = [ux; uy; 1.0];
    gyro_dir = gyro_dir / norm(gyro_dir);

    accel_vib_mps2 = accel_vib_mps2 + w2 * (K_fund_accel * s1 + K_bpf_accel * s2) * accel_dir;
    gyro_vib_radps = gyro_vib_radps + w2 * (K_fund_gyro * s1 + K_bpf_gyro * s2) * gyro_dir;
end
end
