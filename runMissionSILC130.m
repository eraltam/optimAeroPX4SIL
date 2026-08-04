function runMissionSILC130(sessionId, opts)
%RUNMISSIONSILC130 Set up and start a c130JSBSim SIL run against an already-running PX4 + harness.
%
% Clone of runMissionSIL.m (c172pJSBSim), pointed at vehicleType="c130JSBSim" and its own build
% cache -- see PLAN_JSBSIM_SFUNCTION_F22_C130.md Phase F.
%
% Run HILDiagnostics\start_mission_c130.ps1 first (WSL relay + PX4 + Windows harness), wait for
% it to report PX4 is up, then call this with the SAME session id.
%
% runMissionSILC130(sessionId) starts a run with PassThrough (no external) visualization.
%
% Name-value options:
%   visualizationType - "PassThrough" (default), "FlightGear", or "Matlab"
%   stopTime          - top-model StopTime, default 48000 (plenty past any real mission length)
%
% Examples:
%   runMissionSILC130("run1")
%   runMissionSILC130("run2", "visualizationType", "FlightGear")
%
% Between runs (same MATLAB session): call `clearSF` (or `clear functions; clear all;` if
% clearSF isn't on the path), Ctrl+C the PX4 and harness windows, then re-run
% start_mission_c130.ps1 and this function with a new session id.

arguments
    sessionId (1,1) string
    opts.visualizationType (1,1) string {mustBeMember(opts.visualizationType, ["PassThrough","FlightGear","Matlab"])} = "PassThrough"
    opts.stopTime (1,1) double = 48000
end

plantRoot = "C:\Users\Edison Altamirano\SITLV2\AnelloSummer\optimAeroPX4SIL";
diagnosticsRoot = fullfile(plantRoot, "HILDiagnostics");
addpath(diagnosticsRoot);
cd(plantRoot);

initVehicleSIL("launchFullSIL", false, "vehicleType", "c130JSBSim", ...
    "controllerRuntime", "SITL", "visualizationType", opts.visualizationType);
addpath(diagnosticsRoot);

% Reuse one build cache across runs so a fresh session doesn't recompile all referenced models
% every time (first run after a reboot/clearSF is still a cold compile; later ones are fast).
cleanBuildRoot = fullfile(diagnosticsRoot, "simulink_build_c130JSBSim");
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
