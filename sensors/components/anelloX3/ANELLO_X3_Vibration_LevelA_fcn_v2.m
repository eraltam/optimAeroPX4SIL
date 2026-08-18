function [accel_vib_mps2, gyro_vib_radps] = ANELLO_X3_Vibration_LevelA_fcn_v2(rotorAngVel_radps, enableVibration, p)
% PHASE 7 — per-band direction/gain parameterization + optional broadband, built on top of
% ANELLO_X3_Vibration_LevelA_fcn.m (kept unmodified as the regression baseline -- see that file's
% own header and PLAN_VIBRATION_MODEL_LEVEL_A_CRITICAL_REVIEW_AND_IMPROVEMENT.md Phase 7).
%
% WHY per-band, not per-axis-only: the Phase 6C closing synthesis (2026-08-04) found the original
% single-scalar Ka_z direction gain (candidates C3/C4, Ka_z=2.0 applied to BOTH the fundamental and
% BPF harmonic through one shared accel_dir) could not be cleanly separated from a much larger
% phase-seed effect on the same accel-Z/BPF metric (candidate C5). This version gives the
% fundamental and blade-pass harmonics their own direction vectors (Ka_xy_fund/Ka_z_fund vs.
% Ka_xy_bpf/Ka_z_bpf, same idea for gyro's Kg_*), so a future tuning pass can push R_peak without
% being forced to move both harmonics' direction by the same factor.
%
% Broadband term (p.broadbandEnable) is C4's own quasi-broadband sinusoid bank, carried forward
% unmodified (see phase6c_candidates/C4_tonal_direction_broadband_ANELLO_X3_Vibration_LevelA_fcn.m
% for the full rationale/caveats -- still a fixed-sinusoid proxy, not the plan's final filtered-
% white-noise design) since the Phase 6C synthesis found it the single largest improvement of any
% candidate and recommended keeping it, not replacing it.
%
% p (struct, all fields optional -- see anelloVibrationLevelA2DefaultParams() for defaults):
%   Ka_xy_fund, Ka_z_fund, Ka_xy_bpf, Ka_z_bpf   -- accel direction gains, fundamental vs. BPF
%   Kg_xy_fund, Kg_z_fund, Kg_xy_bpf, Kg_z_bpf   -- gyro direction gains, fundamental vs. BPF
%   broadbandEnable (logical), bbAccelGain, bbGyroGain, bbFreqHz (6x1)
%   initialPhase (6x1, radians) -- selects a phase6b_precomputed_phase_vectors.csv column (e.g.
%   Case 2/C5's "seed1") instead of production's baseline_ordered vector. Only takes effect on this
%   function's FIRST call after a `clear` (persistent `phase` is only seeded once) -- see
%   phase8_sensitivity_sweep.m, which clears the function between every swept configuration for
%   exactly this reason.
%
% REGRESSION CONTRACT: calling this with anelloVibrationLevelA2DefaultParams() (all Ka/Kg=1,
% broadbandEnable=false) must reproduce ANELLO_X3_Vibration_LevelA_fcn.m's output to floating-point
% tolerance (~1e-15, NOT exactly bit-for-bit -- this version sums the fundamental/BPF terms as two
% separately-direction-scaled terms rather than production's single combined
% (K_fund*s1+K_bpf*s2)*accel_dir, a different but mathematically equivalent summation order needed
% so per-band direction can differ), for any input sequence, given the same persistent-state call
% history (one call per timestep, in order -- see check_phase7_v2_regression.m, confirmed max diff
% 8.9e-16 m/s^2 accel / 2.8e-17 rad/s gyro over a full real 250,001-sample trace). This is what
% makes it safe to sweep new parameter values without re-deriving the tonal base case from scratch.
%
% NOT yet wired into ins_anello.slx / any Simulink MATLAB Function block -- this file is called
% directly from MATLAB scripts (Phase 8's sensitivity sweep, the regression check above) with a
% plain struct input, which is fine outside a MATLAB Function block context (see
% optimAeroPX4SIL/CLAUDE.md Section 3 for why a struct-returning *function* call, not a struct
% *input*, is what breaks Simulink codegen in this MATLAB release). If this configuration is ever
% promoted to production, inline p's fields as local constants the same way
% ANELLO_X3_Vibration_LevelA_fcn.m already inlines its own K_* gains, per that file's own header.

if nargin < 3 || isempty(p)
    p = anelloVibrationLevelA2DefaultParams();
end

persistent phase
persistent bbPhase
if isempty(phase)
    if isfield(p, 'initialPhase') && ~isempty(p.initialPhase)
        phase = p.initialPhase(:);
    else
        phase = (0:5)' * (2*pi/6);
    end
end
if isempty(bbPhase)
    if isfield(p, 'bbFreqHz') && ~isempty(p.bbFreqHz)
        bbPhase = zeros(numel(p.bbFreqHz), 1);
    else
        bbPhase = zeros(6, 1);
    end
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

    accel_dir_fund = [p.Ka_xy_fund*uy; -p.Ka_xy_fund*ux; p.Ka_z_fund];
    accel_dir_fund = accel_dir_fund / norm(accel_dir_fund);
    accel_dir_bpf  = [p.Ka_xy_bpf*uy;  -p.Ka_xy_bpf*ux;  p.Ka_z_bpf];
    accel_dir_bpf  = accel_dir_bpf / norm(accel_dir_bpf);

    gyro_dir_fund = [p.Kg_xy_fund*ux; p.Kg_xy_fund*uy; p.Kg_z_fund];
    gyro_dir_fund = gyro_dir_fund / norm(gyro_dir_fund);
    gyro_dir_bpf  = [p.Kg_xy_bpf*ux;  p.Kg_xy_bpf*uy;  p.Kg_z_bpf];
    gyro_dir_bpf  = gyro_dir_bpf / norm(gyro_dir_bpf);

    accel_vib_mps2 = accel_vib_mps2 + w2 * K_fund_accel * s1 * accel_dir_fund ...
                                     + w2 * K_bpf_accel  * s2 * accel_dir_bpf;
    gyro_vib_radps = gyro_vib_radps + w2 * K_fund_gyro * s1 * gyro_dir_fund ...
                                     + w2 * K_bpf_gyro  * s2 * gyro_dir_bpf;
end

if isfield(p, 'broadbandEnable') && p.broadbandEnable
    bbPhase = mod(bbPhase + 2*pi*p.bbFreqHz*Ts, 2*pi);

    meanOmega2 = mean(rotorAngVel_radps)^2;
    bbAccelSum = sum(sin(bbPhase)) * p.bbAccelGain * meanOmega2 / sqrt(numel(p.bbFreqHz));
    bbGyroSum  = sum(sin(bbPhase)) * p.bbGyroGain  * meanOmega2 / sqrt(numel(p.bbFreqHz));
    accel_vib_mps2 = accel_vib_mps2 + bbAccelSum * ones(3,1) / sqrt(3);
    gyro_vib_radps = gyro_vib_radps + bbGyroSum * ones(3,1) / sqrt(3);
end
end
