function [accel_vib_mps2, gyro_vib_radps] = ANELLO_X3_Vibration_LevelA_fcn(rotorAngVel_radps, enableVibration)
% PHASE 6C CANDIDATE C2 -- TONAL REBALANCE. K_fund x0.75, K_bpf x1.25 vs. production (Section 8 of
% PLAN_VIBRATION_MODEL_LEVEL_A_CRITICAL_REVIEW_AND_IMPROVEMENT.md, brief's original Case 3 spec).
% Only the four K_* constants differ from C1_control -- everything else (phases, directions,
% motorLocs, Ts, Nb) is identical to production. Direction of change (reduce fundamental, boost
% BPF) matches the brief's original diagnosis; picked as a moderate, clearly-differentiated
% candidate rather than Phase 6B's own screened "winner" (which coincided with the unmodified
% baseline under a metric later found to disagree with recorded HIL data by ~256x in the BPF band
% for reasons not yet resolved -- see the C1 control candidate's docstring). Compare this session's
% accel.csv/gyro.csv against C1's to see whether this rebalance direction helps in a way this
% session's own EKF/fit-set metrics agree with, independent of the open Phase 6B discrepancy.

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

% CANDIDATE C2 CHANGE: K_fund x0.75, K_bpf x1.25 vs. production's 5.464e-6/1.639e-5/1.366e-7/4.098e-7.
K_fund_accel = 5.464e-6 * 0.75;
K_bpf_accel  = 1.639e-5 * 1.25;
K_fund_gyro  = 1.366e-7 * 0.75;
K_bpf_gyro   = 4.098e-7 * 1.25;

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
