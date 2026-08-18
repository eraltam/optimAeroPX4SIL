function [accel_vib_mps2, gyro_vib_radps] = ANELLO_X3_Vibration_LevelA_fcn(rotorAngVel_radps, enableVibration)
% PHASE 6C CANDIDATE C6 -- TORQUE-BASED GYRO. Accel path IDENTICAL to C1_control (production K_*,
% production direction, unchanged) -- isolates the gyro-generation-mechanism question from every
% other change, directly comparable to C1. Gyro output replaced with tau_i = r_i x F_i ->
% calibrated torque-to-rate transfer, per PLAN_VIBRATION_MODEL_LEVEL_A_CRITICAL_REVIEW_AND_
% IMPROVEMENT.md Phase 6 Case 6 / Phase 6B Stage 5.
%
% Units, end to end:
%   F_i [N] = accel-model tonal amplitude [m/s^2] x vehicle mass [kg]
%             (7.441 kg, setUpVehicle.m hexarotor "standard batteries+sensors+mounts" config --
%             Class 3 estimate; PLAN_HEXAROTOR_VEHICLE_PLANT_AUDIT_AND_VIBRATION_MODEL.md flagged
%             mass as needing verification for THIS specific vehicle -- if that verification
%             changes the mass, H_g_gain below must be re-derived, not just the mass constant.)
%   r_i [m]  = motor position relative to CG (motorLocs, in-plane, z=0 approximation -- ignores
%              any vertical offset between motor plane and CG, a known simplification)
%   tau_i = r_i x F_i  ->  [m] x [N] = [N*m]  (cross product, dimensionally correct)
%   tau_total = sum_i(tau_i)  [N*m]
%   gyro_vib_from_torque [rad/s] = H_g_gain [(rad/s)/(N*m)] x tau_total [N*m]
%
% H_g_gain = 6.7428e-3 (rad/s)/(N*m) -- CALIBRATED (not first-principles) by matching this
% candidate's RMS to the CURRENT PRODUCTION model's own gyro_vib RMS over the repaired-baseline
% session's recorded rotorAngVel_radps trace, hover windows only (Phase 6B Stage 5, verified
% against the actual production MATLAB function directly, not affected by the separate Phase 6B
% open issue about accel.csv's spectral content -- this calibration compares two OFFLINE-COMPUTED
% quantities, both derived from the verified-correct model, not from the possibly-stale recorded
% sensor output). This is NOT a first-principles inertia-based derivation (that would require
% omega_dot = I^-1*tau plus integration and a real inertia-tensor estimate -- deferred to Level B).
% Dimensional sanity check (Phase 6B): PASS (finite, positive gain from finite, nonzero torque).
% Still Class 3 (engineering estimate) -- recalibrate against this candidate's OWN HIL result before
% treating it as anything more than a first offline-screened value.

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

% CANDIDATE C6 CHANGE: vehicle mass + torque-to-rate calibrated gain (see header) replace
% K_fund_gyro/K_bpf_gyro entirely -- gyro is now generated via torque, not the phenomenological
% per-motor gyro_dir construction.
vehicleMass_kg = 7.441;
H_g_gain = 6.7428e-3;  % (rad/s)/(N*m)

phase = mod(phase + rotorAngVel_radps * Ts, 2*pi);

accel_vib_mps2 = zeros(3,1);
tau_total_Nm = zeros(3,1);

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

    accel_amp = w2 * (K_fund_accel * s1 + K_bpf_accel * s2);
    accel_vib_mps2 = accel_vib_mps2 + accel_amp * accel_dir;

    F_i_N = accel_amp * vehicleMass_kg * accel_dir;  % [N], same direction as the accel contribution
    tau_i_Nm = cross(motorLocs(i, :)', F_i_N);        % [m] x [N] = [N*m]
    tau_total_Nm = tau_total_Nm + tau_i_Nm;
end

gyro_vib_radps = H_g_gain * tau_total_Nm;
end
