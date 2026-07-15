function windNED_ts = loadULogWindForSimulink()
% LOADULOGWINDFORSIMULINK  Build the earth-fixed NED wind timeseries used by
% environment/components/Air.slx's WIND_SOURCE==2 path (ULog-replayed wind), from the
% ground-truth wind extracted by AnalysisWind/extract_ulog_wind.py.
%
% Non-destructive counterpart to AnalysisWind/load_wind_for_simulink.m (that script
% clears the workspace and opens a plot -- not safe to call from setUpEnvironment.m on
% every model load). If the extracted MAT file is not present, returns an all-zero
% 10-second timeseries so Air.slx still compiles with WIND_SOURCE==1 (Dryden/gust, the
% default) even when the ULog wind path hasn't been extracted yet.
%
% IMPORTANT: this signal is in the earth-fixed NED frame (matches Air.slx's
% windSpeedInNED_mps), not body frame.

matFile = fullfile('C:', 'AS', 'AnalysisWind', 'wind_ulog_output', 'wind_ulog_data.mat');

if ~isfile(matFile)
    warning('loadULogWindForSimulink:missingFile', ...
        ['%s not found -- run AnalysisWind/extract_ulog_wind.py first. ', ...
         'Falling back to a zero-wind placeholder timeseries (only affects WIND_SOURCE==2).'], matFile);
    windNED_ts = timeseries(zeros(2,3), [0 10]);
    windNED_ts.Name = 'PX4 ULog Wind NED (PLACEHOLDER -- extract_ulog_wind.py not run)';
    return;
end

S = load(matFile);

windTime_s  = double(S.simulink.time_s(:));
windNED_m_s = double(S.simulink.wind_ned_m_s);

% Remove the initial ULog offset so the replay starts at t = 0.
windTime_s = windTime_s - windTime_s(1);

windNED_ts = timeseries(windNED_m_s, windTime_s);
windNED_ts.Name = 'PX4 ULog Wind NED';
windNED_ts.DataInfo.Units = 'm/s';

end
