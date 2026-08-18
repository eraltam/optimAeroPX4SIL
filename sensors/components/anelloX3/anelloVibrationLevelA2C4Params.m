function p = anelloVibrationLevelA2C4Params()
% Params reproducing Phase 6C candidate C4 (direction Ka_z=2.0, applied identically to both the
% fundamental and BPF harmonic since C4 predates the per-band split -- see
% phase6c_candidates/C4_tonal_direction_broadband_ANELLO_X3_Vibration_LevelA_fcn.m) plus its
% broadband sinusoid bank. This is the Phase 6C closing synthesis's recommended carry-forward
% starting point for Phase 7's per-band tuning -- NOT a new, independently-derived configuration.
p = anelloVibrationLevelA2DefaultParams();
p.Ka_z_fund = 2.0;
p.Ka_z_bpf  = 2.0;
p.broadbandEnable = true;
end
