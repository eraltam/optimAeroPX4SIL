% This script initialzies the environment, including the wind parameters.
% Winds aloft
xWindInNED_mps = 0;
yWindInNED_mps = 0;
zWindInNED_mps = 0;

% Gusts
gustEnabled = false;
gustStartTime_sec = 5;
gustDuration_sec = 5; % when to start decreasing the gust
gustLength_m = [120 100 80];
gustAmplitude_mps = [3.5 3.5 3.0];

% Turbulence
turbulenceEnabled = false;
turbulenceStartTime_sec = 5;
turbulenceEndTime_sec = 10;
turbulenceWingspan_m = 10;
turbulenceMinAirspeed_mps = 1;

airDensityAtStd_kgpm3 = 1.225;

% Wind source selector for environment/components/Air.slx (see Air.slx's WindSourceSwitch
% and HIL_Ground_Truth_Fidelity_Plan.md Fase 2). 1 = synthetic (winds aloft + Discrete
% Wind Gust Model + Dryden turbulence, all above -- default, unchanged behavior).
% 2 = ULog replay (real North/East wind extracted from nav_hil_ground_truth.ulg by
% AnalysisWind/extract_ulog_wind.py, Down forced to 0 -- see loadULogWindForSimulink.m).
WIND_SOURCE = 2;

% Always loaded (even when WIND_SOURCE==1) because Air.slx's Multiport Switch computes
% both branches every step, same tradeoff already accepted for INS_VARIANT in
% sensors/components/wire_ins_variant.m.
windNED_ts = loadULogWindForSimulink();