function sessionDir = run_hil_automated_session(opts)
%RUN_HIL_AUTOMATED_SESSION Start a repeatable HITL Simulink run and save evidence.
%
% This script intentionally avoids editing nested model references. It loads the
% normal HITL workspace, marks known top-level signal/bus ports for logging by
% block path (verified against VehicleSilSimulation.slx directly, not guessed by
% line name), runs VehicleSilSimulation for a fixed duration, and saves MAT/JSON
% outputs with the numeric checks called for in
% HIL_MAVSDK_PYMAVLINK_AUTOMATED_DEBUG_PLAN.md section 6.2.

arguments
    opts.hitlSerialPort (1,1) string = "COM5"
    opts.hitlBaudRate (1,1) double = 921600
    opts.hitlQGCPort (1,1) double = 14550
    opts.vehicleType (1,1) string = "hexarotor"
    opts.visualizationType (1,1) string = "FlightGear"
    opts.stopTime_s (1,1) double = 200
    opts.clearSLCache (1,1) logical = false
    opts.sessionRoot (1,1) string = ""
    opts.sessionId (1,1) string = ""
    opts.preflightWindow_s (1,1) double = 10
end

repoRoot = fileparts(fileparts(mfilename("fullpath")));
if ~strcmpi(string(pwd), string(repoRoot))
    cd(repoRoot);
end

if strlength(opts.sessionRoot) == 0
    opts.sessionRoot = fullfile(repoRoot, "HILDiagnostics", "logs");
end

if strlength(opts.sessionId) > 0
    sessionName = "session_" + sanitizeSessionId(opts.sessionId);
else
    timestamp = string(datetime("now", "Format", "yyyyMMdd_HHmmss"));
    sessionName = "session_" + timestamp;
end

sessionDir = fullfile(opts.sessionRoot, sessionName);
if ~exist(sessionDir, "dir")
    mkdir(sessionDir);
end
writeTextFile(fullfile(opts.sessionRoot, "latest_session.txt"), sessionDir);

diaryFile = fullfile(sessionDir, "matlab_diary.txt");
diary(diaryFile);
cleanupDiary = onCleanup(@() diary("off"));

fprintf("HIL automated session directory: %s\n", sessionDir);
fprintf("Initializing VehicleSilSimulation in HITL mode...\n");

initVehicleSIL( ...
    "launchFullSIL", false, ...
    "vehicleType", opts.vehicleType, ...
    "visualizationType", opts.visualizationType, ...
    "controllerRuntime", "HITL", ...
    "hardwareTarget", "CubeOrangePlus", ...
    "hitlSerialPort", opts.hitlSerialPort, ...
    "hitlBaudRate", opts.hitlBaudRate, ...
    "hitlQGCPort", opts.hitlQGCPort, ...
    "clearSLCache", opts.clearSLCache);

modelName = "VehicleSilSimulation";
load_system(modelName);

fprintf("Configuring signal logging...\n");
configureLogging(modelName);

simIn = Simulink.SimulationInput(modelName);
simIn = simIn.setModelParameter("StopTime", num2str(opts.stopTime_s));
simIn = simIn.setModelParameter("SignalLogging", "on");
simIn = simIn.setModelParameter("SignalLoggingName", "logsout");
simIn = simIn.setModelParameter("ReturnWorkspaceOutputs", "on");

initialSummary = collectInitialSummary();
save(fullfile(sessionDir, "matlab_initial_workspace.mat"), "initialSummary");

fprintf("Running simulation for %.3f seconds...\n", opts.stopTime_s);
simOut = sim(simIn);

fprintf("Saving MATLAB outputs...\n");
save(fullfile(sessionDir, "matlab_session.mat"), "simOut", "initialSummary", "opts", "-v7.3");

summary = buildSummary(simOut, initialSummary, opts.preflightWindow_s);
writeJson(fullfile(sessionDir, "matlab_summary.json"), summary);

fprintf("HIL automated session complete: %s\n", sessionDir);
end

%% ------------------------------------------------------------------------
% Logging configuration
% ------------------------------------------------------------------------

function targets = loggingTargets()
% Block path (relative to model root) + output port index + logging name.
% Verified directly against VehicleSilSimulation.slx via model_read/get_param
% (see HIL_MAVSDK_PYMAVLINK_AUTOMATED_DEBUG_PLAN.md section 6.1) -- these are
% real block paths, not guessed Simulink line names.
targets = struct("relPath", {}, "portIndex", {}, "loggingName", {});
targets(end+1) = struct("relPath", "vehicleSimulation", "portIndex", 1, "loggingName", "SensorsBus_final");
targets(end+1) = struct("relPath", "vehicleSimulation", "portIndex", 2, "loggingName", "VehicleBus_final");
targets(end+1) = struct("relPath", "Memory", "portIndex", 1, "loggingName", "ActuatorCommandBus_final");
targets(end+1) = struct("relPath", "CONTROLLER_RUNTIME_sel", "portIndex", 1, "loggingName", "ControllerRuntimeSelector");
targets(end+1) = struct("relPath", "PX4 HITL Interface/Select_Controls", "portIndex", 1, "loggingName", "PX4Controls");
targets(end+1) = struct("relPath", "PX4 HITL Interface/HIL Sensor", "portIndex", 1, "loggingName", "HIL_SENSOR_bytes");
targets(end+1) = struct("relPath", "PX4 HITL Interface/HIL_GPS", "portIndex", 1, "loggingName", "HIL_GPS_bytes");
targets(end+1) = struct("relPath", "PX4 HITL Interface/Compute Timestamp", "portIndex", 1, "loggingName", "PX4Timestamp");
end

function configureLogging(modelName)
set_param(modelName, "SignalLogging", "on");
targets = loggingTargets();
for idx = 1:numel(targets)
    blockPath = char(modelName) + "/" + targets(idx).relPath;
    markBlockOutputForLogging(char(blockPath), targets(idx).portIndex, targets(idx).loggingName);
end
end

function markBlockOutputForLogging(blockPath, portIndex, loggingName)
try
    handle = get_param(blockPath, "Handle");
    ph = get_param(handle, "PortHandles");
    if portIndex > numel(ph.Outport)
        fprintf("Logging note: %s has no output port %d\n", blockPath, portIndex);
        return;
    end
    portHandle = ph.Outport(portIndex);
    set_param(portHandle, "DataLogging", "on");
    set_param(portHandle, "DataLoggingNameMode", "Custom");
    set_param(portHandle, "DataLoggingName", char(loggingName));
    fprintf("Logging enabled: %s (port %d) -> ""%s""\n", blockPath, portIndex, loggingName);
catch err
    fprintf("Logging warning for %s: %s\n", blockPath, err.message);
end
end

%% ------------------------------------------------------------------------
% Initial workspace snapshot
% ------------------------------------------------------------------------

function initialSummary = collectInitialSummary()
initialSummary = struct();
initialSummary.created_utc = char(datetime("now", "TimeZone", "UTC", "Format", "yyyy-MM-dd'T'HH:mm:ss.SSS'Z'"));

if evalin("base", "exist('aircraftInitial','var')")
    aircraftInitial = evalin("base", "aircraftInitial");
    initialSummary.aircraftInitial = aircraftInitial;
end

if evalin("base", "exist('CONTROLLER_RUNTIME','var')")
    initialSummary.CONTROLLER_RUNTIME = evalin("base", "CONTROLLER_RUNTIME");
end

if evalin("base", "exist('hitlParams','var')")
    initialSummary.hitlParams = evalin("base", "hitlParams");
end
end

%% ------------------------------------------------------------------------
% Post-run summary (HIL_MAVSDK_PYMAVLINK_AUTOMATED_DEBUG_PLAN.md section 6.2)
% ------------------------------------------------------------------------

function summary = buildSummary(simOut, initialSummary, preflightWindow_s)
summary = struct();
summary.created_utc = char(datetime("now", "TimeZone", "UTC", "Format", "yyyy-MM-dd'T'HH:mm:ss.SSS'Z'"));
summary.initial = makeJsonSafe(initialSummary);
summary.has_logsout = false;
summary.logged_signals = {};

logsout = [];
try
    logsout = simOut.logsout;
    summary.has_logsout = true;
    names = strings(1, logsout.numElements);
    for idx = 1:logsout.numElements
        names(idx) = string(logsout.get(idx).Name);
    end
    summary.logged_signals = cellstr(names);
catch err
    summary.logsout_error = err.message;
end

summary.stop_time = getSimOutField(simOut, "tout");

if summary.has_logsout
    summary.coordinates = buildCoordinateSummary(logsout, preflightWindow_s);
    summary.actuator = buildActuatorSummary(logsout);
    summary.px4_controls = buildPx4ControlsSummary(logsout);
    summary.imu_noise = buildImuSummary(logsout);
    summary.altitude_disagreement = buildAltitudeDisagreement(logsout);
    summary.controller_runtime = buildScalarSeriesSummary(logsout, "ControllerRuntimeSelector");
    summary.hil_sensor_payload = buildHilPayloadSummary(logsout, "HIL_SENSOR_bytes");
    summary.hil_gps_payload = buildHilPayloadSummary(logsout, "HIL_GPS_bytes");
    summary.hil_timestamp = buildTimestampMonotonicity(logsout);
end
end

function ts = getBusLeaf(signalObj, dottedPath)
% Walk a logged bus signal's nested struct by dotted field path down to the
% leaf timeseries. With no path, the signal itself must already be a
% non-bus (plain numeric) logged signal, whose .Values is the timeseries.
node = signalObj.Values;
if nargin >= 2 && strlength(string(dottedPath)) > 0
    parts = strsplit(char(dottedPath), ".");
    for k = 1:numel(parts)
        node = node.(parts{k});
    end
end
ts = node;
end

function out = concatLeaves(a, b)
if isempty(a)
    out = b;
    return;
end
if isempty(b)
    out = a;
    return;
end
n = min(size(a, 1), size(b, 1));
out = [a(1:n, :), b(1:n, :)];
end

function out = collectNumericLeaves(node)
% Recursively flatten a logged (sub-)bus struct into an [N x K] matrix of
% all leaf timeseries Data, concatenated column-wise. Leaves are assumed to
% share the same time base as siblings under the same logged port.
out = [];
if isa(node, "timeseries")
    data = node.Data;
    if isvector(data)
        data = data(:);
    end
    out = data;
    return;
end
if isstruct(node)
    fields = fieldnames(node);
    for k = 1:numel(fields)
        out = concatLeaves(out, collectNumericLeaves(node.(fields{k})));
    end
end
end

function out = buildCoordinateSummary(logsout, preflightWindow_s)
out = struct("available", false);
try
    sb = logsout.get("SensorsBus_final");
    lat = getBusLeaf(sb, "GPSSensorBus.lat_deg").Data;
    lon = getBusLeaf(sb, "GPSSensorBus.lon_deg").Data;
    alt = getBusLeaf(sb, "GPSSensorBus.alt_m").Data;
    out.gps_initial = struct("lat_deg", lat(1), "lon_deg", lon(1), "alt_m", alt(1));
    out.gps_final = struct("lat_deg", lat(end), "lon_deg", lon(end), "alt_m", alt(end));

    vb = logsout.get("VehicleBus_final");
    posTs = getBusLeaf(vb, "BodyStatesBus.aircraftPosInNED_m");
    pos = posTs.Data;
    t = posTs.Time;
    out.local_ned_initial_m = pos(1, :);
    out.local_ned_final_m = pos(end, :);
    displacement = vecnorm(pos - pos(1, :), 2, 2);
    out.max_position_displacement_from_initial_m = max(displacement);

    preflightMask = t <= (t(1) + preflightWindow_s);
    velTs = getBusLeaf(vb, "BodyStatesBus.aircraftVelInNED_mps");
    velMag = vecnorm(velTs.Data, 2, 2);
    if any(preflightMask)
        out.max_abs_velocity_during_preflight_mps = max(velMag(preflightMask));
    end

    airborne = getBusLeaf(vb, "VehicleConfigurationBus.isVehicleAirborne").Data;
    out.was_airborne_at_any_point = any(logical(airborne));
    out.airborne_at_end = logical(airborne(end));

    out.available = true;
catch err
    out.error = err.message;
end
end

function out = buildActuatorSummary(logsout)
out = struct("available", false);
try
    acb = logsout.get("ActuatorCommandBus_final");
    armedTs = getBusLeaf(acb, "controllerArmed");
    armed = armedTs.Data;
    t = armedTs.Time;

    servoLeaves = collectNumericLeaves(getBusLeaf(acb, "ServosCommandBus"));
    engineLeaves = collectNumericLeaves(getBusLeaf(acb, "EngineCommandBus"));
    leaves = concatLeaves(servoLeaves, engineLeaves);
    if isempty(leaves)
        error("no numeric leaves found under ActuatorCommandBus_final");
    end
    magnitude = vecnorm(leaves, 2, 2);

    out.mean_command_magnitude = mean(magnitude);
    out.max_command_magnitude = max(magnitude);

    armedIdx = find(armed ~= 0, 1, "first");
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
end

function out = buildPx4ControlsSummary(logsout)
out = struct("available", false);
try
    ts = getBusLeaf(logsout.get("PX4Controls"));
    data = ts.Data;
    if isvector(data)
        data = data(:);
    end
    mag = vecnorm(data, 2, 2);
    out.mean_magnitude = mean(mag);
    out.max_magnitude = max(mag);
    out.nonzero_at_any_point = any(mag > 1e-6);
    out.available = true;
catch err
    out.error = err.message;
end
end

function out = buildImuSummary(logsout)
out = struct("available", false);
try
    sb = logsout.get("SensorsBus_final");
    gx = getBusLeaf(sb, "INSSensorBus.GyroSensorBus.x_radps").Data;
    gy = getBusLeaf(sb, "INSSensorBus.GyroSensorBus.y_radps").Data;
    gz = getBusLeaf(sb, "INSSensorBus.GyroSensorBus.z_radps").Data;
    ax = getBusLeaf(sb, "INSSensorBus.AccelSensorBus.x_mps2").Data;
    ay = getBusLeaf(sb, "INSSensorBus.AccelSensorBus.y_mps2").Data;
    az = getBusLeaf(sb, "INSSensorBus.AccelSensorBus.z_mps2").Data;

    out.gyro_mean_radps = [mean(gx), mean(gy), mean(gz)];
    out.gyro_var_radps2 = [var(gx), var(gy), var(gz)];
    out.accel_mean_mps2 = [mean(ax), mean(ay), mean(az)];
    out.accel_var_mps2_2 = [var(ax), var(ay), var(az)];
    out.available = true;
catch err
    out.error = err.message;
end
end

function out = buildAltitudeDisagreement(logsout)
out = struct("available", false);
try
    sb = logsout.get("SensorsBus_final");
    gpsAlt = getBusLeaf(sb, "GPSSensorBus.alt_m").Data;
    baroAlt = getBusLeaf(sb, "ADCSensorBus.BaroSensorBus.altitude_m").Data;
    n = min(numel(gpsAlt), numel(baroAlt));
    diffVals = gpsAlt(1:n) - baroAlt(1:n);
    out.mean_diff_m = mean(diffVals);
    out.std_diff_m = std(diffVals);
    out.max_abs_diff_m = max(abs(diffVals));
    out.available = true;
catch err
    out.error = err.message;
end
end

function out = buildScalarSeriesSummary(logsout, name)
out = struct("available", false);
try
    ts = getBusLeaf(logsout.get(name));
    data = ts.Data(:);
    out.initial = data(1);
    out.final = data(end);
    out.unique_values = unique(data)';
    out.available = true;
catch err
    out.error = err.message;
end
end

function out = buildHilPayloadSummary(logsout, name)
out = struct("available", false);
try
    ts = getBusLeaf(logsout.get(name));
    out.sample_count = numel(ts.Time);
    out.first_time_s = ts.Time(1);
    out.last_time_s = ts.Time(end);
    out.bytes_per_sample = size(ts.Data, 2);
    out.available = true;
catch err
    out.error = err.message;
end
end

function out = buildTimestampMonotonicity(logsout)
out = struct("available", false);
try
    ts = getBusLeaf(logsout.get("PX4Timestamp"));
    data = ts.Data(:);
    diffs = diff(data);
    out.is_monotonic_nondecreasing = all(diffs >= 0);
    out.min_step = min(diffs);
    out.max_step = max(diffs);
    out.available = true;
catch err
    out.error = err.message;
end
end

function value = getSimOutField(simOut, fieldName)
try
    rawValue = simOut.(fieldName);
    if isnumeric(rawValue)
        if isempty(rawValue)
            value = [];
        else
            value = struct("first", rawValue(1), "last", rawValue(end), "count", numel(rawValue));
        end
    else
        value = string(class(rawValue));
    end
catch
    value = [];
end
end

function writeJson(path, data)
fid = fopen(path, "w");
if fid < 0
    error("Could not open JSON output: %s", path);
end
cleanup = onCleanup(@() fclose(fid));
fprintf(fid, "%s\n", jsonencode(makeJsonSafe(data), "PrettyPrint", true));
end

function writeTextFile(path, text)
fid = fopen(path, "w");
if fid < 0
    error("Could not open text output: %s", path);
end
cleanup = onCleanup(@() fclose(fid));
fprintf(fid, "%s\n", char(text));
end

function sessionId = sanitizeSessionId(rawSessionId)
sessionId = string(regexprep(strtrim(char(rawSessionId)), "[^A-Za-z0-9_.-]", "_"));
if strlength(sessionId) == 0
    error("sessionId cannot be empty after sanitization.");
end
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
    out = cellfun(@makeJsonSafe, in, "UniformOutput", false);
elseif isstring(in)
    out = cellstr(in);
elseif isnumeric(in) || islogical(in)
    if numel(in) <= 32
        out = in;
    else
        out = struct("class", class(in), "size", size(in), "first", in(1), "last", in(end));
    end
elseif ischar(in)
    out = in;
else
    out = char(string(class(in)));
end
end
