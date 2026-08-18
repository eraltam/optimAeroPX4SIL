% This script holds the parameters for the GPS, accelerometer, gyro, and barometer sensors. 
% GPS data used in the simulation
parameters.gps.vertPositionAccuracy_m = 40;
parameters.gps.horzPositionAccuracy_m = 30;
parameters.gps.vertPositionAccuracy_m = 0;
parameters.gps.velocityAccuracy_mps = 0.05;
parameters.gps.decayFactor_nd = 0.999; % 0 -> white noise, 1 -> random walk

% specific accel and gyro not known, so use parameters from a representative
% device "ICM-42688-P"
parameters.accel.naturalFrequency_radps = 190;
parameters.accel.dampingRatio_nd = 0.707;
parameters.accel.scaleFactCrossCoupling_nd = [1 0 0; 0 1 0; 0 0 1;];
parameters.accel.measurementBias_mps2 = [0 0 0];
parameters.accel.lowerUpperLimits_mps2 = [-10000 -10000 -10000 10000 10000 10000];
parameters.accel.noisePower_mps2 = [0.0000001 0.0000001 0.0000001];
parameters.gyro.naturalFrequency_radps = 190;
parameters.gyro.dampingRatio_nd = 0.707;
parameters.gyro.scaleFactCrossCoupling_nd = [1 0 0; 0 1 0; 0 0 1;];
parameters.gyro.measurementBias_radps = [0 0 0];
parameters.gyro.gSensitiveBias_radps = [0 0 0];
parameters.gyro.lowerUpperLimits_radps = [-10000 -10000 -10000 10000 10000 10000];
parameters.gyro.noisePower_radps = [0.0000001 0.0000001 0.0000001];
parameters.mag.noisePower_gauss = [0.000001 0.000001 0.000001];
parameters.baro.noisePower_Pa = [0.001];
parameters.thermo.noisePower_degC = [0.001];

% INS source selector for sensors/ins (see sensors/components/wire_ins_variant.m,
% sensors/components/build_ins_ideal.m, and optimAeroPX4SIL/CLAUDE.md).
% 1 = Generic (above ICM-42688-P-like model, default — regression baseline)
% 2 = AnelloX3 (datasheet-accurate ANELLO X3 IMU model from sensors/components/ins_anello.slx)
% 3 = Ideal (sensors/components/ins_ideal.slx — zero-error, zero-dynamics pass-through of
%     plant truth; the "D1" baseline from HIL_TEST_STATUS_AND_NEXT_PRIORITIES.md, used to
%     separate "IMU noise-model effect" from "any sensor noise vs. none at all")
INS_VARIANT = 2;

% ANELLO X3 SiPhOG output-mode / temperature-count configuration, read by
% sensors/components/ins_anello.slx's TemperatureCounts_const/OutputMode_const blocks (see
% sensors/components/build_ins_anello.m and PLAN_ANELLO_X3_SIPHOG_TEMPERATURE_CALIBRATION_
% OUTPUT_MODES.md section 6.1). Flat base-workspace variables, matching this file's existing
% INS_VARIANT convention, not the plan's illustrative vehicle.imu.anello.* struct shape.
%   0 = RAW_COUNTS (debug fallback -- gyro_meas still falls back to CALIBRATED, see
%       ANELLO_X3_IMU_fcn_SIL.m; raw counts are computed every step but only reachable via the
%       debug outputs, which this build currently just terminates -- see build_ins_anello.m's
%       "OPEN ITEM" comment)
%   1 = UNCALIBRATED
%   2 = CALIBRATED (production default)
anelloOutputMode = uint8(2);

% Direct constant SiPhOG internal temperature, in raw temperature counts (NOT Celsius --
% PLAN section 5). 0 counts is near the middle of the valid TC/SFvT domain for this unit
% (SN261200001124); it is NOT derived from AirEnvironment.airTemperature_K, since there is no
% verified Celsius->counts transfer function for this sensor yet.
anelloSiPhOGTemperatureCounts = 0;

% residualThermalEnable MUST stay false -- no residual thermal error model has been specified
% or validated (PLAN section 6.1). debugEnable is documented here but NOT wired to anything yet
% (open item, see build_ins_anello.m); both are recorded now so the config surface matches the
% plan's documented API even though the debug logging path itself is still a placeholder.
vehicle.imu.anello.debugEnable = false;
vehicle.imu.anello.residualThermalEnable = false;

% Level A motor/rotor vibration model, injected inside ins_anello.slx only (see
% sensors/components/add_vibration_to_ins_anello.m and
% PLAN_HEXAROTOR_VEHICLE_PLANT_AUDIT_AND_VIBRATION_MODEL.md Section 5). Defaults OFF: this is a
% new, first-pass addition to a model used for real HIL flights -- any existing script/session
% that doesn't explicitly opt in must see byte-identical ANELLO behavior to before this feature
% existed. Set vehicle.vibration.enable = 1 to turn it on for a run.
vehicle.vibration.enable = 1;  % Re-enabled 2026-07-31 after the gain-scaling and phase-coherence
% bugs (found during the A/B control test the same day -- see
% HIL_GPS_FUSION_AND_ACTUATOR_OUTPUT_FIX_2026-07-31.md section 5a) were both fixed in
% ANELLO_X3_Vibration_LevelA_fcn.m and validated OFFLINE (motors-off exact zero, hover accel/gyro
% RMS within 0.1% of the 0.88 m/s^2 / 0.022 rad/s target, RMS/omega^2 constant to ~2.5% across the
% full throttle range, blade-pass frequency at 92.6% of the function's own Nyquist limit at max
% RPM). Root cause was two contradictory hover-omega assumptions in the same file (~68 rad/s used
% for the K_fund/K_bpf gain calibration vs. ~181.8 rad/s used a few lines above it for the Nb=2
% blade-pass justification), which inflated injected accel/gyro RMS by ~how far the old gains
% overshot the real hover condition, plus all 6 rotor phases sharing one identical initial value
% (zeros(6,1)) causing coherent instead of incoherent addition. NOT YET re-validated against the
% real Cube over HIL -- do that (19-wp mission first, per the recovery-sequence plan) before
% trusting this for the actual vibration-vs-ground-truth comparison. The
% session_ab_test_no_vibration_01 / session_ab_test_no_vibration_full_mission_02 results (and
% their onboard .ulg logs) remain the immutable vibration-disabled control/rollback baseline
% regardless of how this flag is set going forward.