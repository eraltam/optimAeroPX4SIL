function [Ameas, omegaMeas, magMeas, accel_bias, gyro_bias, mag_bias] = ...
         ANELLO_X3_IMU_Vehicle_fcn_SIL_v2(Ab, omega, omegaDot, g_body, mag_nav_G, R_bn_vec, T, reset, seedVal)
%ANELLO_X3_IMU_VEHICLE_FCN_SIL_V2  Vehicle-interface wrapper for the corrected-noise-density
%ANELLO candidate (STATIC_ACCEPTANCE_CRITERIA_PROPOSAL.md, approved 2026-07-22).
%
%  Identical to ANELLO_X3_IMU_Vehicle_fcn_SIL.m except it calls
%  ANELLO_X3_IMU_fcn_SIL_v2 (corrected ARW/VRW) instead of ANELLO_X3_IMU_fcn_SIL (datasheet
%  nominal). Vehicle interface / lever-arm / mounting math is UNCHANGED.
%
%  Inputs/outputs: identical to ANELLO_X3_IMU_Vehicle_fcn_SIL.m -- see that file for the
%  full parameter documentation.

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

%% Sensor errors (v2 candidate: corrected ARW/VRW)
[Ameas, omegaMeas, magMeas, accel_bias, gyro_bias, mag_bias] = ...
    ANELLO_X3_IMU_fcn_SIL_v2(f_imu_true, omega_true, mag_body_true, T, reset, seedVal);
end
