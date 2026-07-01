function mp = anelloX3MountParams()
% ANELLOX3MOUNTPARAMS  IMU mounting parameters relative to vehicle CG.
% Copied unmodified from newIMU_model/params/anelloX3MountParams.m.
  mp.r_IMU_CG      = [0.10; 0.02; -0.03];   % [m] IMU offset from CG, body frame
  mp.R_sensor_body = eye(3);                % sensor frame == body frame (no mounting misalignment)
  mp.gravitySign   = -1;                    % f_imu = A_IMU + gravitySign * g_body
end
