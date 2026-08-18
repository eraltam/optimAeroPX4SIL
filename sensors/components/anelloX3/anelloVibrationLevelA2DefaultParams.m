function p = anelloVibrationLevelA2DefaultParams()
% Default parameter struct for ANELLO_X3_Vibration_LevelA_fcn_v2.m -- all direction gains at 1.0
% (fundamental == BPF == production's single shared accel_dir/gyro_dir), broadband disabled. Passing
% this struct must reproduce ANELLO_X3_Vibration_LevelA_fcn.m's output exactly -- see that file's
% regression contract and check_phase7_v2_regression.m.
p.Ka_xy_fund = 1.0;
p.Ka_z_fund  = 1.0;
p.Ka_xy_bpf  = 1.0;
p.Ka_z_bpf   = 1.0;
p.Kg_xy_fund = 1.0;
p.Kg_z_fund  = 1.0;
p.Kg_xy_bpf  = 1.0;
p.Kg_z_bpf   = 1.0;
p.broadbandEnable = false;
p.bbAccelGain = 6.0e-6;
p.bbGyroGain  = 1.5e-7;
p.bbFreqHz = [8; 13; 19; 24; 33; 41];
p.initialPhase = (0:5)' * (2*pi/6);  % production's "baseline_ordered" column
end
