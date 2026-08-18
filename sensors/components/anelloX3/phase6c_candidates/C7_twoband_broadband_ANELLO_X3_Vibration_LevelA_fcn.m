function [accel_vib_mps2, gyro_vib_radps] = ANELLO_X3_Vibration_LevelA_fcn(rotorAngVel_radps, enableVibration)
% PHASE 6C CANDIDATE C7 -- "TWO-BAND QUASI-BROADBAND", production direction UNCHANGED.
%
% Supersedes C4 as the Phase 6C/Item 4 recommended candidate (2026-08-05). C3/C4's Ka_z=2.0
% direction change is DROPPED here: it was chosen against a power-fraction proxy metric
% (Pz/(Px+Py+Pz) in narrow 55-59Hz bins), not the pre-registered PCA target (true covariance
% principal-eigenvector Z-component, 50-65Hz, computed on the frozen fit-set split -- see
% dynamic_comparison/scripts/pca_direction_analysis.py). Re-evaluated against the CORRECT PCA
% metric, Ka_z=1.0 (i.e. unmodified production direction) already lands close to the real
% 50-65Hz target (~0.87-0.92 vs. real 0.841/0.902 hover/vuelo_recto fit-set values), while Ka_z=2.0
% over-verticalizes it (~0.99). See AnalysisIMU/vibration_full_mission_comparison/
% FROZEN_CANDIDATE_MANIFEST_V2_PRE_PHASE9.md for the full evidence trail.
%
% Also fixes a second bug found in the SAME PCA re-audit: C4's original 6-tone broadband bank
% (bbFreqHz=[8;13;19;24;33;41]) applies the SAME scalar realization to x/y/z via a fixed
% [1,1,1]/sqrt(3) direction -- a rank-1 covariance contribution whose own principal-eigenvector
% Z-component is exactly 1/sqrt(3)=0.577. Its 19Hz/24Hz tones sit inside the 18-25Hz PCA guardrail
% band (real target ~0.99, already close without any broadband), and adding that isotropic energy
% there collapsed the simulated 18-25Hz Z-fraction from ~0.98 to ~0.59 -- a real, mechanistically
% exact regression, not noise. Fixed by dropping the 19Hz/24Hz tones entirely, keeping only
% [8;13;33;41]Hz (which sit inside the 5-15Hz/30-45Hz bands broadband was actually meant to fix,
% per Phase 6A's own finding that sim under-shoots real there) -- nothing remains inside 18-25Hz to
% interact with that guardrail. Gains rescaled by sqrt(4/6) so each SURVIVING tone's own amplitude
% is unchanged (the sum-of-tones formula divides by sqrt(numel(bbFreqHz)); dropping 2 of 6 tones
% without rescaling would silently amplify the remaining 4 by sqrt(6/4)).
%
% Paired gate test (5 traces x 6 phase seeds x {broadband OFF, this candidate ON}, identical phase
% within each pair, all direction gains at 1.0) confirms the two-band term itself does not regress
% 18-25/27-31/50-65Hz relative to the same-phase/same-trace control (mean paired delta -0.007/+0.024/
% -0.041, all tightly clustered around zero) while reliably closing the 5-15/30-45Hz gap in every
% single one of the 30 pairs (mean -10.5dB, worst pair still -6.7dB) -- not a single-seed fluke.

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
% NOTE: no Ka_z direction override here -- accel_dir/gyro_dir below use production's original
% [uy;-ux;1]/[ux;uy;1] construction unchanged, unlike C3/C4's Ka_z=2.0.

persistent bbPhase
if isempty(bbPhase)
    bbPhase = zeros(4,1);  % one running phase per quasi-broadband tone (4, not 6 -- see header)
end
bbFreqHz = [8; 13; 33; 41];             % 19Hz/24Hz DROPPED -- see header
bbAccelGain = 6.0e-6 * sqrt(4/6);        % = 4.899e-6 m/s^2 per (rad/s)^2 -- rescaled, same per-tone amplitude as C4
bbGyroGain  = 1.5e-7 * sqrt(4/6);        % = 1.225e-7 rad/s per (rad/s)^2 -- rescaled, same per-tone amplitude as C4

phase = mod(phase + rotorAngVel_radps * Ts, 2*pi);
bbPhase = mod(bbPhase + 2*pi*bbFreqHz*Ts, 2*pi);

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

meanOmega2 = mean(rotorAngVel_radps)^2;
bbAccelSum = sum(sin(bbPhase)) * bbAccelGain * meanOmega2 / sqrt(numel(bbFreqHz));
bbGyroSum  = sum(sin(bbPhase)) * bbGyroGain  * meanOmega2 / sqrt(numel(bbFreqHz));
accel_vib_mps2 = accel_vib_mps2 + bbAccelSum * ones(3,1) / sqrt(3);
gyro_vib_radps = gyro_vib_radps + bbGyroSum * ones(3,1) / sqrt(3);
end
