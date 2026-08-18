function [Ameas, omegaMeas, magMeas, accel_bias, gyro_bias, mag_bias, ...
          raw_counts_by_fog, temp_clamp_flags, ic_clamp_flags, count_saturation_flags] = ...
         ANELLO_X3_IMU_Vehicle_fcn_SIL(Ab, omega, omegaDot, g_body, mag_nav_G, R_bn_vec, T, ...
                                        temperature_counts, outputMode, reset)
%ANELLO_X3_IMU_VEHICLE_FCN_SIL  Vehicle-interface sensor model wired to
%optimAeroPX4SIL's sensors/components/ins.slx signal contract.
%
%  This is a SIL-specific sibling of
%  newIMU_model/models/ANELLO_X3_IMU_Vehicle_fcn.m. It differs in exactly one
%  respect: it takes the nav->body rotation matrix (R_bn, "DCM_be" in this repo's
%  naming) directly, instead of the body->nav matrix (R_nb) the original function
%  takes and transposes internally. ins.slx already has DCM_be (nav->body) on hand
%  from BodyStatesBus, so this avoids an extra transpose at the call site that would
%  be easy to get backwards (see optimAeroPX4SIL/CLAUDE.md section 4.1).
%
%  2026-08-12: passes temperature_counts and outputMode straight through to
%  ANELLO_X3_IMU_fcn_SIL.m's gyro SiPhOG calibration path, and forwards its raw-count/
%  clamp-diagnostic debug outputs. No vehicle-kinematics or frame handling changed --
%  see PLAN_ANELLO_X3_SIPHOG_TEMPERATURE_CALIBRATION_OUTPUT_MODES.md section 6.4.
%
%  Inputs:
%    Ab                  [3x1]  CG acceleration, body frame [m/s^2]  ("aircraftAccelbe_mps2")
%    omega               [3x1]  angular velocity, body frame [rad/s]
%    omegaDot            [3x1]  angular acceleration, body frame [rad/s^2]
%    g_body              [3x1]  gravity vector in body frame [m/s^2]  (= DCM_be * [0;0;gravityScalar_mps2])
%    mag_nav_G           [3x1]  magnetic field in navigation frame [Gauss] (nT->G conversion done by caller)
%    R_bn_vec            [9x1]  rotation matrix nav->body ("DCM_be"), column-major
%    T                   [1x1]  temperature [deg C] (accel/mag thermal model only)
%    temperature_counts  [1x1]  SiPhOG internal temperature, raw counts (NOT Celsius; PLAN section 5)
%    outputMode          [1x1]  uint8-compatible: 0=RAW_COUNTS, 1=UNCALIBRATED, 2=CALIBRATED (default)
%    reset               [1x1]  1 = reset persistent bias states
%
%  Outputs:
%    Ameas, omegaMeas, magMeas         [3x1] measured signals
%    accel_bias, gyro_bias, mag_bias   [3x1] current bias drift (debug)
%    raw_counts_by_fog                [3x1] modeled raw FOG counts, FOG-indexed (debug only --
%                                             never route onto a rad/s bus, PLAN section 6.2)
%    temp_clamp_flags, ic_clamp_flags,
%    count_saturation_flags           [3x1] logical diagnostic flags, FOG-indexed

persistent mp;
if isempty(mp); mp = anelloX3MountParams(); end
if reset ~= 0; mp = anelloX3MountParams(); end

%% Vehicle Interface
r    = mp.r_IMU_CG;
R_bn = reshape(R_bn_vec, [3,3]);

% Lever-arm correction
A_IMU = Ab + cross(omegaDot, r) + cross(omega, cross(omega, r));

% Specific force (accelerometer measurement before sensor errors)
f_imu_true = mp.R_sensor_body * A_IMU + mp.gravitySign .* g_body;

% Gyro input (angular rate, possibly rotated by mounting)
omega_true = mp.R_sensor_body * omega;

% Magnetic field: navigation frame -> body frame (R_bn already nav->body)
mag_body_true = mp.R_sensor_body * R_bn * mag_nav_G;

%% Sensor errors (SIL sample-rate variant of Phase 1 function)
[Ameas, omegaMeas, magMeas, accel_bias, gyro_bias, mag_bias, ...
    raw_counts_by_fog, temp_clamp_flags, ic_clamp_flags, count_saturation_flags] = ...
    ANELLO_X3_IMU_fcn_SIL(f_imu_true, omega_true, mag_body_true, T, temperature_counts, outputMode, reset);
end
