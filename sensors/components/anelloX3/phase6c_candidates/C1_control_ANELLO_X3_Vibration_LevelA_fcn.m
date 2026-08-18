function [accel_vib_mps2, gyro_vib_radps] = ANELLO_X3_Vibration_LevelA_fcn(rotorAngVel_radps, enableVibration)
% PHASE 6C CANDIDATE C1 -- FRESH CONTROL. Byte-identical to the current production
% ANELLO_X3_Vibration_LevelA_fcn.m as of 2026-08-03 (no parameter changes). Purpose: re-run the
% exact current working-tree model under a fresh, freshly-compiled HIL session, because the
% existing "repaired baseline" session (session_vibration_repaired_full_mission_01) predates the
% current working-tree state and its recorded accel.csv does not reproduce this exact model's own
% math when replayed offline (Phase 6B open issue -- see phase6b_offline_screening.py's module
% docstring and BASELINE_MANIFEST.md). This candidate exists to settle that discrepancy: if THIS
% session's recorded accel.csv matches the Phase 6B offline replay closely, the old baseline
% session was stale; if it doesn't, something else is still unexplained and needs more digging
% before trusting any Phase 6C comparison.
%
% See PLAN_HEXAROTOR_VEHICLE_PLANT_AUDIT_AND_VIBRATION_MODEL.md Section 5 for the full architecture.

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
    accel_dir = [uy; -ux; 1.0];
    accel_dir = accel_dir / norm(accel_dir);
    gyro_dir = [ux; uy; 1.0];
    gyro_dir = gyro_dir / norm(gyro_dir);

    accel_vib_mps2 = accel_vib_mps2 + w2 * (K_fund_accel * s1 + K_bpf_accel * s2) * accel_dir;
    gyro_vib_radps = gyro_vib_radps + w2 * (K_fund_gyro * s1 + K_bpf_gyro * s2) * gyro_dir;
end
end
