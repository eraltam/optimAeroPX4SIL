function results = runVehicleIntegrationMatrix(opts)
% runVehicleIntegrationMatrix Validate registered vehicles against Simulink and PX4.
%
% This is a bounded integration check for the multi-vehicle SIL registry. It does
% not start a live PX4 session unless runPx4Build is true, and even then it only
% builds the common px4_sitl_default binary and verifies that each vehicle target
% is registered in the local PX4 tree.

arguments
    opts.vehicleTypes string = string.empty
    opts.insVariants double = 1
    opts.runSimulinkUpdate (1,1) logical = true
    opts.runPlantSmoke (1,1) logical = false
    opts.runPx4Build (1,1) logical = false
    opts.wslDistro (1,1) string = "auto"
    opts.resultsDir (1,1) string = "work/vehicle_test_results"
end

repoRoot = fileparts(fileparts(mfilename("fullpath")));
if ~strcmpi(pwd, repoRoot)
    cd(repoRoot)
end

addpath("vehicle")
addpath("utilities")

registry = vehicleRegistry();
if isempty(opts.vehicleTypes)
    vehicleTypes = [registry.VehicleKey];
else
    vehicleTypes = opts.vehicleTypes;
end

resultsDir = char(opts.resultsDir);
if ~isfolder(resultsDir)
    mkdir(resultsDir)
end

results = struct( ...
    "timestamp", char(datetime("now", "Format", "yyyy-MM-dd HH:mm:ss")), ...
    "matlabRelease", version("-release"), ...
    "vehicles", [], ...
    "px4Build", struct("requested", opts.runPx4Build, "exitCode", NaN, "output", ""));

if opts.runPx4Build
    results.px4Build = runPx4SitlBuild(repoRoot, opts.wslDistro);
end

vehicleResults = repmat(emptyVehicleResult(), 0, 1);
for ii = 1:numel(vehicleTypes)
    vehicleType = vehicleTypes(ii);
    fprintf("\n=== Vehicle matrix: %s ===\n", vehicleType)
    vehicleResult = runOneVehicle(vehicleType, opts, repoRoot);
    vehicleResults(end + 1, 1) = vehicleResult; %#ok<AGROW>
    writeOneVehicleResult(resultsDir, vehicleResult)
end

results.vehicles = vehicleResults;
summaryFile = fullfile(resultsDir, "vehicle_integration_matrix_" + ...
    string(datetime("now", "Format", "yyyyMMdd_HHmmss")) + ".json");
writeJson(summaryFile, results)
fprintf("\nVehicle integration matrix written to %s\n", summaryFile)
end

function result = runOneVehicle(vehicleType, opts, repoRoot)
result = emptyVehicleResult();
result.vehicleKey = char(vehicleType);
timer = tic;

try
    vehicle = vehicleRegistry(vehicleType);
    result.vehicleKey = char(vehicle.VehicleKey);
    result.displayName = char(vehicle.DisplayName);
    result.px4Target = char(vehicle.PX4Target);
    result.buildCommand = char("make px4_sitl_default " + vehicle.PX4Target);
    result.px4TargetRegistered = isPx4TargetRegistered(repoRoot, vehicle.PX4Target);
    result.airframeRegistered = isAirframeRegistered(repoRoot, vehicle.PX4Target);
    result.airframeAutostartConsistent = isAirframeAutostartConsistent(repoRoot, vehicle.PX4Target);

    initVehicleSIL( ...
        "launchFullSIL", false, ...
        "vehicleType", vehicle.VehicleKey, ...
        "controllerRuntime", "SITL", ...
        "visualizationType", "Matlab", ...
        "launchPreflightVisualization", false);

    if opts.runSimulinkUpdate
        result.insVariantResults = runInsVariantUpdates(opts.insVariants);
    end

    if opts.runPlantSmoke
        smokeTimer = tic;
        sim("VehiclePlant", "StopTime", "1");
        result.plantSmokePassed = true;
        result.plantSmokeDuration_s = toc(smokeTimer);
    end

    result.passed = result.px4TargetRegistered && result.airframeRegistered && ...
        result.airframeAutostartConsistent && ...
        all([result.insVariantResults.passed]) && ...
        (~opts.runPlantSmoke || result.plantSmokePassed);
catch ME
    result.passed = false;
    result.errorText = getReport(ME, "extended", "hyperlinks", "off");
end

result.duration_s = toc(timer);
try
    bdclose("all")
catch
end
end

function variantResults = runInsVariantUpdates(insVariants)
variantResults = repmat(struct("insVariant", NaN, "passed", false, ...
    "duration_s", NaN, "errorText", ""), 0, 1);

for jj = 1:numel(insVariants)
    variant = insVariants(jj);
    item = struct("insVariant", variant, "passed", false, ...
        "duration_s", NaN, "errorText", "");
    timer = tic;
    try
        assignin("base", "INS_VARIANT", variant);
        set_param("VehicleSilSimulation", "SimulationCommand", "update");
        item.passed = true;
    catch ME
        item.errorText = getReport(ME, "extended", "hyperlinks", "off");
    end
    item.duration_s = toc(timer);
    variantResults(end + 1, 1) = item; %#ok<AGROW>
end
end

function px4Build = runPx4SitlBuild(repoRoot, requestedDistro)
px4Build = struct("requested", true, "exitCode", NaN, "output", "");
px4Path = fullfile(repoRoot, "PX4-Autopilot");
if ~isfolder(px4Path)
    px4Build.exitCode = -1;
    px4Build.output = "PX4-Autopilot folder not found.";
    return
end

wslPath = replace(px4Path, "\", "/");
if startsWith(wslPath, "C:", "IgnoreCase", true)
    wslPath = "/mnt/c" + extractAfter(wslPath, 2);
end
distro = resolveWSLDistro(requestedDistro);
if strlength(distro) > 0
    wslCommand = sprintf('wsl -d "%s"', distro);
else
    wslCommand = 'wsl';
end
command = sprintf('%s bash -lc "cd ''%s'' && make px4_sitl_default"', wslCommand, wslPath);
[exitCode, output] = system(command);
px4Build.exitCode = exitCode;
px4Build.output = output;
end

function distro = resolveWSLDistro(requestedDistro)
distro = strtrim(string(requestedDistro));
if ~strcmpi(distro, "auto")
    return
end

[status, output] = system('wsl --list --quiet');
if status ~= 0
    distro = "";
    return
end

output = erase(string(output), char(0));
installed = strip(splitlines(output));
installed(installed == "") = [];
preferred = ["PX4Simulink", "Ubuntu-22.04"];
for ii = 1:numel(preferred)
    if any(strcmpi(installed, preferred(ii)))
        distro = preferred(ii);
        return
    end
end
distro = "";
end

function tf = isPx4TargetRegistered(repoRoot, px4Target)
cmakeFile = fullfile(repoRoot, "PX4-Autopilot", "src", "modules", ...
    "simulation", "simulator_mavlink", "CMakeLists.txt");
tf = contains(readTextIfExists(cmakeFile), string(px4Target));
end

function tf = isAirframeRegistered(repoRoot, px4Target)
airframesDir = fullfile(repoRoot, "PX4-Autopilot", "ROMFS", ...
    "px4fmu_common", "init.d-posix", "airframes");
files = dir(fullfile(airframesDir, "*_" + string(px4Target)));
airframesCmake = fullfile(airframesDir, "CMakeLists.txt");
tf = ~isempty(files) && contains(readTextIfExists(airframesCmake), files(1).name);
end

function tf = isAirframeAutostartConsistent(repoRoot, px4Target)
% The comment `@autostart N` inside an airframe file is documentation only --
% PX4's rcS resolves SYS_AUTOSTART from the filename prefix (see
% ROMFS/px4fmu_common/init.d-posix/rcS lines ~53-61), so a mismatch here can't
% cause a runtime autostart collision. It is still worth catching because it
% misleads anyone reading the file (see PLAN_CORRECCION_MULTIVEHICULO_SITL.md F8).
airframesDir = fullfile(repoRoot, "PX4-Autopilot", "ROMFS", ...
    "px4fmu_common", "init.d-posix", "airframes");
files = dir(fullfile(airframesDir, "*_" + string(px4Target)));
if isempty(files)
    tf = false;
    return
end

tokens = regexp(files(1).name, "^(\d+)_", "tokens", "once");
if isempty(tokens)
    tf = false;
    return
end
expectedId = str2double(tokens{1});

text = readTextIfExists(fullfile(airframesDir, files(1).name));
declared = regexp(text, "@autostart\s+(\d+)", "tokens", "once");
if isempty(declared)
    tf = false;
    return
end
declaredId = str2double(declared{1});

tf = declaredId == expectedId;
end

function text = readTextIfExists(filePath)
if isfile(filePath)
    text = string(fileread(filePath));
else
    text = "";
end
end

function result = emptyVehicleResult()
result = struct( ...
    "vehicleKey", "", ...
    "displayName", "", ...
    "px4Target", "", ...
    "buildCommand", "", ...
    "px4TargetRegistered", false, ...
    "airframeRegistered", false, ...
    "airframeAutostartConsistent", false, ...
    "insVariantResults", repmat(struct("insVariant", NaN, "passed", false, ...
        "duration_s", NaN, "errorText", ""), 0, 1), ...
    "plantSmokePassed", false, ...
    "plantSmokeDuration_s", NaN, ...
    "duration_s", NaN, ...
    "passed", false, ...
    "errorText", "");
end

function writeOneVehicleResult(resultsDir, vehicleResult)
fileName = sprintf("%s_matrix_%s.json", vehicleResult.vehicleKey, ...
    char(datetime("now", "Format", "yyyyMMdd_HHmmss")));
writeJson(fullfile(resultsDir, fileName), vehicleResult)
end

function writeJson(filePath, value)
fid = fopen(filePath, "w");
if fid < 0
    error("Unable to open %s for writing.", filePath)
end
cleanup = onCleanup(@() fclose(fid));
fprintf(fid, "%s", jsonencode(value, "PrettyPrint", true));
end
