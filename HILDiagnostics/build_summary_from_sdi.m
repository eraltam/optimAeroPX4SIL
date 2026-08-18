%% build_summary_from_sdi.m
% Builds matlab_session.mat / matlab_summary.json for a HITL session that was run via
% direct set_param(...,'SimulationCommand','start') (async, to avoid a blocking sim()
% call tying up an interactive tool call for the full mission duration) rather than
% through run_hil_automated_session.m's sim(simIn) wrapper. Logged signals in that case
% land in the Simulation Data Inspector repository (Simulink.sdi), not in a base-workspace
% simOut/logsout variable, so this rebuilds the same summary shape run_hil_automated_session.m
% produces (see its buildSummary/collectInitialSummary functions) sourced from the SDI run
% instead, so build_manifest.py and friends can consume this session identically to the
% two sim(simIn)-based sessions.
%
% Usage: set `runID` and `sessionDir` below, then run.

function build_summary_from_sdi(runID, sessionDir)

if ~exist(sessionDir, 'dir')
    mkdir(sessionDir);
end

r = Simulink.sdi.getRun(runID);

summary = struct();
summary.created_utc = char(datetime('now', 'TimeZone', 'UTC', 'Format', 'yyyy-MM-dd''T''HH:mm:ss.SSS''Z'''));
summary.has_logsout = true;
summary.logged_signals = {};
summary.stop_time = [];

initialSummary = struct();
initialSummary.created_utc = summary.created_utc;
if evalin('base', "exist('aircraftInitial','var')")
    initialSummary.aircraftInitial = evalin('base', 'aircraftInitial');
end
if evalin('base', "exist('CONTROLLER_RUNTIME','var')")
    initialSummary.CONTROLLER_RUNTIME = evalin('base', 'CONTROLLER_RUNTIME');
end
if evalin('base', "exist('hitlParams','var')")
    initialSummary.hitlParams = evalin('base', 'hitlParams');
end
summary.initial = makeJsonSafe(initialSummary);

% ---- coordinates ----
out = struct('available', false);
try
    lat = getSig(r, 'SensorsBus_final.GPSSensorBus.lat_deg');
    lon = getSig(r, 'SensorsBus_final.GPSSensorBus.lon_deg');
    alt = getSig(r, 'SensorsBus_final.GPSSensorBus.alt_m');
    out.gps_initial = struct('lat_deg', lat.Data(1), 'lon_deg', lon.Data(1), 'alt_m', alt.Data(1));
    out.gps_final = struct('lat_deg', lat.Data(end), 'lon_deg', lon.Data(end), 'alt_m', alt.Data(end));

    posX = getSig(r, 'VehicleBus_final.BodyStatesBus.aircraftPosInNED_m(1)');
    posY = getSig(r, 'VehicleBus_final.BodyStatesBus.aircraftPosInNED_m(2)');
    posZ = getSig(r, 'VehicleBus_final.BodyStatesBus.aircraftPosInNED_m(3)');
    pos = [posX.Data(:), posY.Data(:), posZ.Data(:)];
    t = posX.Time;
    out.local_ned_initial_m = pos(1, :);
    out.local_ned_final_m = pos(end, :);
    displacement = vecnorm(pos - pos(1, :), 2, 2);
    out.max_position_displacement_from_initial_m = max(displacement);

    airborne = getSig(r, 'VehicleBus_final.VehicleConfigurationBus.isVehicleAirborne');
    out.was_airborne_at_any_point = any(logical(airborne.Data));
    out.airborne_at_end = logical(airborne.Data(end));

    summary.stop_time = struct('first', t(1), 'last', t(end), 'count', numel(t));
    out.available = true;
catch err
    out.error = err.message;
end
summary.coordinates = out;

% ---- actuator ----
out = struct('available', false);
try
    armedTs = getSig(r, 'ActuatorCommandBus_final.controllerArmed');
    armed = armedTs.Data; t = armedTs.Time;

    leafNames = {'ActuatorCommandBus_final.ServosCommandBus.MotorCommandHexBus.cmdRotor1_nd', ...
        'ActuatorCommandBus_final.ServosCommandBus.MotorCommandHexBus.cmdRotor2_nd', ...
        'ActuatorCommandBus_final.ServosCommandBus.MotorCommandHexBus.cmdRotor3_nd', ...
        'ActuatorCommandBus_final.ServosCommandBus.MotorCommandHexBus.cmdRotor4_nd', ...
        'ActuatorCommandBus_final.ServosCommandBus.MotorCommandHexBus.cmdRotor5_nd', ...
        'ActuatorCommandBus_final.ServosCommandBus.MotorCommandHexBus.cmdRotor6_nd', ...
        'ActuatorCommandBus_final.EngineCommandBus.cmdThrottle_unit'};
    leaves = [];
    for i = 1:numel(leafNames)
        s = getSig(r, leafNames{i});
        leaves = [leaves, s.Data(:)]; %#ok<AGROW>
    end
    magnitude = vecnorm(leaves, 2, 2);
    out.mean_command_magnitude = mean(magnitude);
    out.max_command_magnitude = max(magnitude);

    armedIdx = find(armed ~= 0, 1, 'first');
    if isempty(armedIdx)
        out.armed_observed = false;
        out.actuator_nonzero_after_arm = false;
    else
        out.armed_observed = true;
        out.time_first_armed_s = t(armedIdx);
        n = min(numel(magnitude), numel(armed) - armedIdx + 1);
        out.actuator_nonzero_after_arm = any(magnitude(armedIdx:armedIdx + n - 1) > 1e-6);
    end
    out.available = true;
catch err
    out.error = err.message;
end
summary.actuator = out;

% ---- IMU noise summary ----
out = struct('available', false);
try
    gx = getSig(r, 'SensorsBus_final.INSSensorBus.GyroSensorBus.x_radps').Data;
    gy = getSig(r, 'SensorsBus_final.INSSensorBus.GyroSensorBus.y_radps').Data;
    gz = getSig(r, 'SensorsBus_final.INSSensorBus.GyroSensorBus.z_radps').Data;
    ax = getSig(r, 'SensorsBus_final.INSSensorBus.AccelSensorBus.x_mps2').Data;
    ay = getSig(r, 'SensorsBus_final.INSSensorBus.AccelSensorBus.y_mps2').Data;
    az = getSig(r, 'SensorsBus_final.INSSensorBus.AccelSensorBus.z_mps2').Data;
    out.gyro_mean_radps = [mean(gx), mean(gy), mean(gz)];
    out.gyro_var_radps2 = [var(gx), var(gy), var(gz)];
    out.accel_mean_mps2 = [mean(ax), mean(ay), mean(az)];
    out.accel_var_mps2_2 = [var(ax), var(ay), var(az)];
    out.available = true;
catch err
    out.error = err.message;
end
summary.imu_noise = out;

% ---- controller runtime selector ----
out = struct('available', false);
try
    ts = getSig(r, 'ControllerRuntimeSelector');
    data = ts.Data(:);
    out.initial = data(1); out.final = data(end); out.unique_values = unique(data)';
    out.available = true;
catch err
    out.error = err.message;
end
summary.controller_runtime = out;

save(fullfile(sessionDir, 'matlab_session.mat'), 'summary', 'initialSummary', '-v7.3');
fid = fopen(fullfile(sessionDir, 'matlab_summary.json'), 'w');
fprintf(fid, '%s\n', jsonencode(makeJsonSafe(summary), 'PrettyPrint', true));
fclose(fid);
fprintf('Wrote %s\n', fullfile(sessionDir, 'matlab_summary.json'));
fprintf('Wrote %s\n', fullfile(sessionDir, 'matlab_session.mat'));

end

function ts = getSig(r, name)
sig = r.getSignalsByName(name);
if isempty(sig)
    error('Signal not found: %s', name);
end
ts = sig.Values;
end

function out = makeJsonSafe(in)
if isstruct(in)
    out = struct();
    fields = fieldnames(in);
    for idx = 1:numel(fields)
        field = fields{idx};
        out.(field) = makeJsonSafe(in.(field));
    end
elseif iscell(in)
    out = cellfun(@makeJsonSafe, in, 'UniformOutput', false);
elseif isstring(in)
    out = cellstr(in);
elseif isnumeric(in) || islogical(in)
    if numel(in) <= 32
        out = in;
    else
        out = struct('class', class(in), 'size', size(in), 'first', in(1), 'last', in(end));
    end
elseif ischar(in)
    out = in;
else
    out = char(string(class(in)));
end
end
