function [accel_vib_mps2, gyro_vib_radps] = ANELLO_X3_Vibration_LevelA_fcn(rotorAngVel_radps, enableVibration)
% PHASE 6C CANDIDATE C5 -- REPRESENTATIVE PHASE SEED. Identical to C1_control (production tonal
% gains, production direction vectors) EXCEPT the initial phase0 vector, which uses
% phase6b_precomputed_phase_vectors.csv's "seed1" column (fixed, reproducible, generated offline
% with RandomState(1) -- NOT a runtime rng call in this compiled function, per the plan's own
% guardrail against runtime rng in the production path). Isolates phase-initial-condition
% sensitivity from every other change (tonal/direction/broadband all held at baseline).
%
% seed1 = [2.620227, 4.525932, 0.000719, 1.899612, 0.922094, 0.580181] rad (from
% phase6b_precomputed_phase_vectors.csv, RandomState(1).uniform(0, 2*pi, size=6) -- verified by
% direct read of that CSV, not retyped from memory).

persistent phase
if isempty(phase)
    % CANDIDATE C5 CHANGE: seed1 initial phase vector (see header) instead of production's evenly
    % spaced (0:5)'*(2*pi/6).
    phase = [2.620227; 4.525932; 0.000719; 1.899612; 0.922094; 0.580181];
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
