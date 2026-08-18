function [accel_vib_mps2, gyro_vib_radps, dbg] = ANELLO_X3_Vibration_LevelA_fcn_debug(rotorAngVel_radps, enableVibration)
% Debug/instrumentation variant of ANELLO_X3_Vibration_LevelA_fcn.m (Phase 0 of
% PLAN_VIBRATION_MODEL_LEVEL_A_CRITICAL_REVIEW_AND_IMPROVEMENT.md). NOT wired into ins_anello.slx --
% for offline/harness analysis only (struct outputs don't compile in a MATLAB Function block, see
% optimAeroPX4SIL/CLAUDE.md Section 3). accel_vib_mps2/gyro_vib_radps must stay bit-identical to the
% production function for the same inputs to floating-point precision (~1e-15, from summation-order
% differences -- fundamental/BPF terms are summed separately here so they can be reported
% individually, vs. combined in one expression in production; not literally bit-identical, see
% phase0_debug_harness.m); only dbg is new. Uses its own persistent `phase` (not shared with the
% production function), so calling this from a harness does not perturb production state and vice
% versa.
%
% dbg fields (all as of the CURRENT step, i.e. phase already advanced):
%   phase              (6x1) rad, per-motor phase after this step's update
%   omega              (6x1) rad/s, == rotorAngVel_radps (echoed for convenience)
%   accel_fund (3x6)   per-motor fundamental-only contribution to accel_vib_mps2, column i = motor i
%   accel_bpf  (3x6)   per-motor blade-pass-only contribution to accel_vib_mps2
%   gyro_fund  (3x6)   per-motor fundamental-only contribution to gyro_vib_radps
%   gyro_bpf   (3x6)   per-motor blade-pass-only contribution to gyro_vib_radps
%   accel_pre_filter, gyro_pre_filter (3x1) pre-filter output (== post-filter today; there is no
%     filter block yet in Level A -- kept so a future filter addition doesn't silently break this
%     instrumentation, per Phase 0's own goal of observability before further changes)

persistent phase
if isempty(phase)
    phase = (0:5)' * (2*pi/6);
end

if enableVibration <= 0
    accel_vib_mps2 = zeros(3,1);
    gyro_vib_radps = zeros(3,1);
    dbg.phase = phase;
    dbg.omega = rotorAngVel_radps;
    dbg.accel_fund = zeros(3,6);
    dbg.accel_bpf = zeros(3,6);
    dbg.gyro_fund = zeros(3,6);
    dbg.gyro_bpf = zeros(3,6);
    dbg.accel_pre_filter = accel_vib_mps2;
    dbg.gyro_pre_filter = gyro_vib_radps;
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
accel_fund = zeros(3,6);
accel_bpf  = zeros(3,6);
gyro_fund  = zeros(3,6);
gyro_bpf   = zeros(3,6);

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

    accel_fund(:,i) = w2 * K_fund_accel * s1 * accel_dir;
    accel_bpf(:,i)  = w2 * K_bpf_accel  * s2 * accel_dir;
    gyro_fund(:,i)  = w2 * K_fund_gyro  * s1 * gyro_dir;
    gyro_bpf(:,i)   = w2 * K_bpf_gyro   * s2 * gyro_dir;

    accel_vib_mps2 = accel_vib_mps2 + accel_fund(:,i) + accel_bpf(:,i);
    gyro_vib_radps = gyro_vib_radps + gyro_fund(:,i) + gyro_bpf(:,i);
end

dbg.phase = phase;
dbg.omega = rotorAngVel_radps;
dbg.accel_fund = accel_fund;
dbg.accel_bpf = accel_bpf;
dbg.gyro_fund = gyro_fund;
dbg.gyro_bpf = gyro_bpf;
dbg.accel_pre_filter = accel_vib_mps2;
dbg.gyro_pre_filter = gyro_vib_radps;
end
