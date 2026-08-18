function [accel_vib_mps2, gyro_vib_radps] = ANELLO_X3_Vibration_LevelA_fcn(rotorAngVel_radps, enableVibration)
% Level A phenomenological motor/rotor vibration model -- harmonics only, this first pass. See
% PLAN_HEXAROTOR_VEHICLE_PLANT_AUDIT_AND_VIBRATION_MODEL.md Section 5 for the full architecture
% (this is the "Rotor forcing generator" block, injected inside ins_anello.slx between the
% BodyStatesBus selector and the existing ANELLO_X3_IMU_Vehicle_fcn_SIL call).
%
% Adds a per-motor fundamental + blade-pass harmonic pair, phase-locked to each motor's own
% rotorAngVel_radps (already computed by hexMotorModel.slx, zero automatically when
% disarmed/RPM=0 since hexActuator.slx already zeros all motor commands on disarm), with axis
% coupling derived from each motor's real position -- deliberately NOT identical across motors or
% axes (brief restriction: do not assume identical phase/axis coupling across motors).
%
% NOT included in this pass (deferred, flagged as follow-up work, not silently omitted):
%   - Broadband stochastic forcing (the real-vs-sim gap has a broadband-floor component in
%     addition to tonal peaks -- see AnalysisIMU/dynamic_comparison tables/figures -- this function
%     only addresses the tonal/PSD-peak part of that gap).
%   - Level B structural/mount transfer functions.
%   - Ground-impact/ring-down transients.
%
% Every K_* gain below is a Class 3 engineering estimate (parameter-confidence table, plan
% Section 2), NOT measured -- editable here, intended to be replaced once a dedicated
% multi-setpoint hover session (plan Section 7) or thrust-stand data is available. N_b=2 (blade
% count) is inferred, not measured: 2x the corrected plant's hover rotor-rate estimate (~28.9 Hz at
% 50% throttle, using the also-corrected kct_nd in setUpActuators.m) lands close to the dominant
% real accel_norm PSD peak found in nav_hil_ground_truth.ulg's longest hover segment (56.54 Hz) --
% see AnalysisIMU/dynamic_comparison/scripts/vibration_spectral_analysis.py output. This is a
% plausible first estimate, not a confirmed measurement of the real propeller's blade count.
%
% Motor positions below are inlined (not read from vehicleParams.rotor.motorLocs) for the same
% reason ANELLO_X3_IMU_fcn_SIL.m inlines its own datasheet constants -- MATLAB Function blocks in
% this MATLAB release cannot reliably call functions that return large/param-struct values (see
% optimAeroPX4SIL/CLAUDE.md Section 3). KEEP THESE IN SYNC BY HAND with
% vehicle/common/setUpActuators.m's hexarotor case if that geometry ever changes.

persistent phase
if isempty(phase)
    % Distinct, deterministic per-motor initial phase offsets (60 deg apart), fixed 2026-07-31 --
    % previously all six motors shared phase=zeros(6,1), so at t=0 (and near any hover-symmetric
    % condition) all six contributions added COHERENTLY (in-phase) instead of the incoherent
    % addition the design intends (see header). Evenly spacing them removes that artifact without
    % needing per-motor tuning: each motor's own rotorAngVel_radps still drives its phase forward
    % every step, this only changes the t=0 starting point.
    phase = (0:5)' * (2*pi/6);
end

if enableVibration <= 0
    accel_vib_mps2 = zeros(3,1);
    gyro_vib_radps = zeros(3,1);
    return
end

Ts = 0.004;  % SIL step size (250 Hz), matches ANELLO_X3_IMU_fcn_SIL.m's own inlined Ts convention
Nb = 2;      % blade count -- ESTIMATE, see header

% Motor positions (FRD, meters) -- mirrors setUpActuators.m's corrected, PX4-1003-airframe-matched
% motorLocs (rotor0..5 order).
motorLocs = [ 0.0   0.5   0.0;
              0.0  -0.5   0.0;
              0.43 -0.25  0.0;
             -0.43  0.25  0.0;
              0.43  0.25  0.0;
             -0.43 -0.25  0.0];

% RECALIBRATED 2026-07-31 (see HIL_GPS_FUSION_AND_ACTUATOR_OUTPUT_FIX_2026-07-31.md section 5a):
% the previous 2026-07-30 calibration below assumed hover omega ~68 rad/s, but the corrected
% plant's actual 50%-throttle hover omega is ~181.8 rad/s (0.5 * vehicleParams.rotor.maxRPM(3473)
% * 2*pi/60 -- the SAME hover condition already used two paragraphs up for the Nb=2 blade-pass
% justification, ~28.9 Hz -- this file simply used two contradictory hover-omega values in two
% places). Since vibration amplitude scales as omega^2, the old gains at the real hover omega
% produced accel_vib norm RMS ~4.6 m/s^2 (measured directly, Monte Carlo, 5000 samples) --
% roughly 5x the intended target below, not the theoretical (181.8/68)^2~7.15x, because the old
% calibration's target RMS already baked in phase/geometry effects at ITS reference condition.
% Rescaled by measuring RMS directly at the real hover omega (5000-sample Monte Carlo, 2% per-
% motor omega jitter, WITH the distinct-phase fix above already applied) and solving for the
% scale factor that lands back on the same target: accel_vib norm RMS 0.88 m/s^2, gyro_vib norm
% RMS 0.022 rad/s -- both confirmed within 0.5% of target after rescaling (0.880/0.0220
% measured). Also re-verified: RMS/omega^2 constant to ~2.5% across the full 0-100% throttle
% range (clean quadratic scaling, no discretization surprises), motors-off (omega=0) gives exact
% zero, and blade-pass frequency at 100% throttle (115.8 Hz, Nb=2) sits at 92.6% of this
% function's own Nyquist limit (Ts=0.004s -> 125 Hz) -- a real, bounded margin, not a violation,
% but flagged here since it leaves little headroom for any future increase in Nb or max RPM.
% Still Class 3 (engineering estimate, NOT thrust-stand/regression-fit data) -- replace via
% proper calibration once a multi-setpoint hover session (plan Section 7) is available.
K_fund_accel = 5.464e-6;  % m/s^2 per (rad/s)^2 -- ESTIMATE, rescaled 2026-07-31 (was 2.0e-5)
K_bpf_accel  = 1.639e-5;  % m/s^2 per (rad/s)^2 -- ESTIMATE (dominant term), rescaled 2026-07-31 (was 6.0e-5)
K_fund_gyro  = 1.366e-7;  % rad/s per (rad/s)^2 -- ESTIMATE, rescaled 2026-07-31 (was 5.0e-7)
K_bpf_gyro   = 4.098e-7;  % rad/s per (rad/s)^2 -- ESTIMATE, rescaled 2026-07-31 (was 1.5e-6)

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
    % Tangential+vertical direction for accel coupling (reaction-force-like); radial+vertical mix
    % for gyro -- position-dependent, so each of the 6 motors couples differently into the sensed
    % axes rather than uniformly.
    accel_dir = [uy; -ux; 1.0];
    accel_dir = accel_dir / norm(accel_dir);
    gyro_dir = [ux; uy; 1.0];
    gyro_dir = gyro_dir / norm(gyro_dir);

    accel_vib_mps2 = accel_vib_mps2 + w2 * (K_fund_accel * s1 + K_bpf_accel * s2) * accel_dir;
    gyro_vib_radps = gyro_vib_radps + w2 * (K_fund_gyro * s1 + K_bpf_gyro * s2) * gyro_dir;
end
end
