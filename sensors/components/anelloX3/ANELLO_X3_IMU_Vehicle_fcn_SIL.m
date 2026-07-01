function [Ameas, omegaMeas, magMeas, accel_bias, gyro_bias, mag_bias] = ...
         ANELLO_X3_IMU_Vehicle_fcn_SIL(Ab, omega, omegaDot, g_body, mag_nav_G, R_bn_vec, T, reset)
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
%  Inputs:
%    Ab        [3x1]  CG acceleration, body frame [m/s^2]   ("aircraftAccelbe_mps2")
%    omega     [3x1]  angular velocity, body frame [rad/s]
%    omegaDot  [3x1]  angular acceleration, body frame [rad/s^2]
%    g_body    [3x1]  gravity vector in body frame [m/s^2]  (= DCM_be * [0;0;gravityScalar_mps2])
%    mag_nav_G [3x1]  magnetic field in navigation frame [Gauss] (nT->G conversion done by caller)
%    R_bn_vec  [9x1]  rotation matrix nav->body ("DCM_be"), column-major
%    T         [1x1]  temperature [deg C]
%    reset     [1x1]  1 = reset persistent bias states
%
%  Outputs:
%    Ameas, omegaMeas, magMeas         [3x1] measured signals
%    accel_bias, gyro_bias, mag_bias   [3x1] current bias drift (debug)

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
[Ameas, omegaMeas, magMeas, accel_bias, gyro_bias, mag_bias] = ...
    ANELLO_X3_IMU_fcn_SIL(f_imu_true, omega_true, mag_body_true, T, reset);
end
