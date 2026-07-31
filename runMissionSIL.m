function runMissionSIL(sessionId, opts)
%RUNMISSIONSIL Set up and start a c172pJSBSim SIL run against an already-running PX4 + harness.
%
% Run HILDiagnostics\start_mission.ps1 first (WSL relay + PX4 + Windows harness), wait for it to
% report PX4 is up, then call this with the SAME session id.
%
% runMissionSIL(sessionId) starts a run with PassThrough (no external) visualization.
%
% Name-value options:
%   visualizationType - "PassThrough" (default), "FlightGear", or "Matlab"
%   stopTime          - top-model StopTime, default 48000 (plenty past any real mission length)
%
% Examples:
%   runMissionSIL("run1")
%   runMissionSIL("run2", "visualizationType", "FlightGear")
%
% Between runs (same MATLAB session): call `clearSF` (or `clear functions; clear all;` if
% clearSF isn't on the path), Ctrl+C the PX4 and harness windows, then re-run start_mission.ps1
% and this function with a new session id.

arguments
    sessionId (1,1) string
    opts.visualizationType (1,1) string {mustBeMember(opts.visualizationType, ["PassThrough","FlightGear","Matlab"])} = "PassThrough"
    opts.stopTime (1,1) double = 48000
end

plantRoot = "C:\Users\Edison Altamirano\SITLV2\AnelloSummer\optimAeroPX4SIL";
diagnosticsRoot = fullfile(plantRoot, "HILDiagnostics");
addpath(diagnosticsRoot);
cd(plantRoot);

initVehicleSIL("launchFullSIL", false, "vehicleType", "c172pJSBSim", ...
    "controllerRuntime", "SITL", "visualizationType", opts.visualizationType);
addpath(diagnosticsRoot);

% Reuse one build cache across runs so a fresh session doesn't recompile all referenced models
% every time (first run after a reboot/clearSF is still a cold compile; later ones are fast).
cleanBuildRoot = fullfile(diagnosticsRoot, "simulink_build_c172pJSBSim");
cacheFolder = fullfile(cleanBuildRoot, "cache");
codeGenFolder = fullfile(cleanBuildRoot, "codegen");
if ~exist(cacheFolder, "dir"); mkdir(cacheFolder); end
if ~exist(codeGenFolder, "dir"); mkdir(codeGenFolder); end
Simulink.fileGenControl("set", "CacheFolder", cacheFolder, "CodeGenFolder", codeGenFolder, "createDir", true);

cs = evalin("base", "standardSILConfigurationParams");
set_param(cs, "SignalLogging", "on", "SignalLoggingName", "logsout");
set_param(cs, "StopTime", num2str(opts.stopTime));

open_system("VehicleSilSimulation.slx");
set_param("VehicleSilSimulation", "SimulationCommand", "start");

fprintf("[%s] Session '%s' started (visualization=%s).\n", datetime("now"), sessionId, opts.visualizationType);
fprintf("Check the harness/PX4 windows for arm/takeoff/mission progress.\n");
fprintf("Poll status with: get_param(""VehicleSilSimulation"",""SimulationStatus"")\n");
end
