function run_jsbsim_mission_batch(vehicleType, sessionId, visualizationType, stopTime, insVariant)
%RUN_JSBSIM_MISSION_BATCH Blocking SIL runner with ANELLO and reproducible artifacts.
arguments
    vehicleType (1,1) string {mustBeMember(vehicleType,["c172pJSBSim","c130JSBSim","f22JSBSim"])}
    sessionId (1,1) string
    visualizationType (1,1) string {mustBeMember(visualizationType,["PassThrough","Matlab","FlightGear"])} = "PassThrough"
    stopTime (1,1) double = 18000
    insVariant (1,1) double {mustBeMember(insVariant,[1 2])} = 2
end

repoRoot = fileparts(fileparts(fileparts(mfilename("fullpath"))));
sessionStartedDatenum = now;
diagnosticsRoot = fullfile(repoRoot, "HILDiagnostics");
sessionDir = fullfile(diagnosticsRoot, "logs", "session_" + sessionId);
if ~exist(sessionDir, "dir"); mkdir(sessionDir); end
missionLockDir = fullfile(diagnosticsRoot, ".mission.lock");
missionLockCleanup = onCleanup(@() releaseMissionLock(missionLockDir, sessionId)); %#ok<NASGU>

cd(repoRoot);
addpath(diagnosticsRoot, fullfile(diagnosticsRoot, "automation"));
diary(fullfile(sessionDir, "matlab_console.log"));
diaryCleanup = onCleanup(@() diary("off")); %#ok<NASGU>

clear mex;

initVehicleSIL("launchFullSIL", false, "vehicleType", vehicleType, ...
    "controllerRuntime", "SITL", "visualizationType", visualizationType);
% initVehicleSIL rebuilds the project path; restore diagnostics helpers used after sim().
addpath(diagnosticsRoot, fullfile(diagnosticsRoot, "automation"));
% initVehicleSIL loads the executable VehiclePlant/VehicleSilSimulation
% variant copies. Synchronize the selected vehicle's reviewed unpack source
% into the standalone and both embedded copies after variant initialization.
sync_jsbsim_unpack_chart(vehicleType);

% initVehicleSIL/setUpSensors establishes defaults, so override after initialization and verify.
assignin("base", "INS_VARIANT", insVariant);
assert(evalin("base", "INS_VARIANT") == insVariant, "Requested INS_VARIANT was not selected.");
assignin("base", "CONTROLLER_RUNTIME", 1);

buildRoot = fullfile(diagnosticsRoot, "simulink_build_" + vehicleType);
cacheFolder = fullfile(buildRoot, "cache");
codeGenFolder = fullfile(buildRoot, "codegen");
if ~exist(cacheFolder, "dir"); mkdir(cacheFolder); end
if ~exist(codeGenFolder, "dir"); mkdir(codeGenFolder); end
Simulink.fileGenControl("set", "CacheFolder", cacheFolder, "CodeGenFolder", codeGenFolder, "createDir", true);

cs = evalin("base", "standardSILConfigurationParams");
set_param(cs, "SignalLogging", "on", "SignalLoggingName", "logsout");
open_system("VehicleSilSimulation.slx");

fprintf("SESSION_START id=%s vehicle=%s visualization=%s INS_VARIANT=%g\n", ...
    sessionId, vehicleType, visualizationType, evalin("base", "INS_VARIANT"));
try
    simArgs = {"StopTime", num2str(stopTime), "ReturnWorkspaceOutputs", "on"};
    % The MATLAB animation generally benefits from accelerator mode. Keep the F-22 in
    % normal mode, however: its contact dynamics diverged in accelerator at touchdown
    % (roll reached -84 deg and the model stopped while still armed), while the otherwise
    % identical normal-mode mission completed LAND with roll bounded to about +/-31 deg.
    % PassThrough and FlightGear retain their established normal-mode behavior.
    if strcmpi(visualizationType, "Matlab") && ~strcmpi(vehicleType, "f22JSBSim")
        simArgs = [simArgs, {"SimulationMode", "accelerator"}]; %#ok<AGROW>
    end
    simOut = sim("VehicleSilSimulation", simArgs{:});
    assignin("base", "simOut", simOut);
    if isprop(simOut, "logsout") || any(strcmp(simOut.who, "logsout"))
        assignin("base", "logsout", simOut.logsout);
    end
    if any(strcmp(simOut.who, "tout"))
        assignin("base", "tout", simOut.tout);
    end
catch err
    fprintf(2, "SIMULATION_TERMINATED: %s\n", getReport(err, "extended", "hyperlinks", "off"));
    assignin("base", "simulationError", err);
end

saveLiveSILSession(sessionId, "maxUlogAge_s", 43200, ...
    "notBeforeDatenum", sessionStartedDatenum);
save(fullfile(sessionDir, "run_metadata.mat"), "vehicleType", "sessionId", ...
    "visualizationType", "stopTime");
fprintf("SESSION_END id=%s\n", sessionId);
end

function releaseMissionLock(lockDir, sessionId)
% Only release the lock acquired for this session; never remove another run's lock.
ownerFile = fullfile(lockDir, "owner.txt");
if ~isfolder(lockDir) || ~isfile(ownerFile)
    return;
end
ownerText = string(fileread(ownerFile));
if contains(ownerText, "session=" + sessionId + newline) || ...
        endsWith(strtrim(ownerText), "session=" + sessionId)
    rmdir(lockDir, "s");
end
end
