function p = anelloX3Params_SIL()
% ANELLOX3PARAMS_SIL  ANELLO X3 IMU parameters for the optimAeroPX4SIL integration.
%
% Identical to newIMU_model/params/anelloX3Params.m except Ts/fs are set to the
% VehicleSilSimulation fixed step (stepSize_s = 0.004 s, see initVehicleSIL.m) instead
% of the datasheet's native 200 Hz output rate. All ARW/VRW noise sigmas and the
% Gauss-Markov bias-drift coefficients are formulas in terms of p.Ts/p.fs, so they
% rescale automatically and stay datasheet-accurate at the SIL's actual call rate.
%
% Source: ANELLO X3 IMU Datasheet rev 1.17, February 2026.

%% ---- Timing --------------------------------------------------------
p.Ts  = 0.004;          % sample period [s] — matches optimAeroPX4SIL stepSize_s
p.fs  = 1 / p.Ts;       % sample rate   [Hz]

%% ---- Constants -----------------------------------------------------
p.g0  = 9.80665;        % standard gravity [m/s^2]
p.d2r = pi / 180;       % deg -> rad
p.T0  = 25.0;           % nominal temperature [°C]

%% =====================================================================
%% GYROSCOPE  (optical SiPhOG)
%% =====================================================================
p.gyro.range_dps        = 400;
p.gyro.range_radps      = 400 * p.d2r;

p.gyro.biasInstability_dph = 0.5;
p.gyro.bias_radps       = 0.5 * p.d2r / 3600;

p.gyro.ARW_dpsqrt_hr    = 0.05;
p.gyro.ARW_radps_sqrts  = 0.05 * p.d2r / 60;
p.gyro.sigma_radps      = p.gyro.ARW_radps_sqrts * sqrt(p.fs);
p.gyro.noisePower       = p.gyro.sigma_radps^2;

% Stochastic model controls (synced 2026-07-14 with newIMU_model/params/anelloX3Params.m)
p.gyro.noiseProfileSource = 'inertial_nav_sim custom_mems';
p.gyro.inertialNavSim_B_degps = 1.0e-4;          % custom_mems B [deg/s]
p.gyro.inertialNavSim_K_degps_sqrts = 5.0e-6;    % custom_mems K [deg/s/sqrt(s)]
p.gyro.enableWhiteNoise = true;
p.gyro.enableBiasInstability = true;
p.gyro.enablePinkNoise = true;
p.gyro.enableRateRandomWalk = true;
p.gyro.enableQuantization = false;
p.gyro.biasCorrTime_s = 300.0;
p.gyro.pinkNoise_radps = p.gyro.inertialNavSim_B_degps * p.d2r;
p.gyro.rateRandomWalk_radps_sqrts = p.gyro.inertialNavSim_K_degps_sqrts * p.d2r;
p.gyro.quantizationStep_radps = 0.0;

p.gyro.scaleFactorError    = 0.001;
p.gyro.scaleFactorOverTemp = 0.001;

p.gyro.bandwidth_Hz     = 100;
p.gyro.wn_radps         = 2 * pi * 100;
p.gyro.zeta             = 0.707;

p.gyro.tempCoeff_radps_C = 5e-8;

p.gyro.staticBias    = [0; 0; 0];
p.gyro.scaleFactor   = [p.gyro.scaleFactorError;
                         p.gyro.scaleFactorError;
                         p.gyro.scaleFactorError];
p.gyro.misalignment  = eye(3);

%% =====================================================================
%% ACCELEROMETER  (MEMS, 3-axis)
%% =====================================================================
p.accel.range_g         = 16;
p.accel.range_mps2      = 16 * p.g0;

p.accel.biasInstability_ug = 20;
p.accel.bias_mps2       = 20e-6 * p.g0;

p.accel.VRW_mps_sqrt_hr = 0.03;
p.accel.VRW_mps_sqrts   = 0.03 / sqrt(3600);
p.accel.sigma_mps2      = p.accel.VRW_mps_sqrts * sqrt(p.fs);
p.accel.noisePower      = p.accel.sigma_mps2^2;

% Stochastic model controls (synced 2026-07-14 with newIMU_model/params/anelloX3Params.m)
p.accel.enableWhiteNoise = true;
p.accel.enableBiasInstability = true;
p.accel.enablePinkNoise = true;
p.accel.enableVelocityRandomWalk = false;
p.accel.enableQuantization = false;
p.accel.biasCorrTime_s = 200.0;
p.accel.pinkNoise_mps2 = p.accel.bias_mps2;
p.accel.velocityRandomWalk_mps2_sqrts = 0.0;
p.accel.quantizationStep_mps2 = 0.0;

p.accel.wn_radps        = 2 * pi * 100;
p.accel.zeta            = 0.707;

p.accel.tempCoeff_mps2_C = 1e-5;

p.accel.staticBias   = [0; 0; 0];
p.accel.scaleFactor  = [0; 0; 0];
p.accel.misalignment = eye(3);

%% =====================================================================
%% MAGNETOMETER
%% =====================================================================
p.mag.range_G           = 8;

p.mag.noiseRMS_mG       = 0.4;
p.mag.sigma_G           = 0.4e-3;
p.mag.noisePower_G2     = (0.4e-3)^2;

% Stochastic model controls (synced 2026-07-14). Pink noise/RRW are NOT ported to mag in
% ANELLO_X3_IMU_fcn_SIL.m -- newIMU_model's ANELLO_X3_IMU_fcn.m also hardcodes mag's
% magCfg.enablePinkNoise/enableRandomWalk to false regardless of these flags, so mag stays
% bias (Gauss-Markov) + white noise only in both implementations.
p.mag.enableWhiteNoise = true;
p.mag.enableBiasInstability = true;
p.mag.enableQuantization = false;
p.mag.biasCorrTime_s = 500.0;
p.mag.bias_G = p.mag.sigma_G;
p.mag.quantizationStep_G = 0.0;

p.mag.tempCoeff_G_C     = 1e-6;

p.mag.staticBias    = [0; 0; 0];
p.mag.scaleFactor   = [0; 0; 0];
p.mag.misalignment  = eye(3);

%% =====================================================================
%% TEMPERATURE RANGE
%% =====================================================================
p.temp.min_C            = -10;
p.temp.max_C            = 50;
p.temp.extMin_C         = -20;
p.temp.extMax_C         = 70;

end
