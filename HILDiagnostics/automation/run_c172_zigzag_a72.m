repoRoot = "C:\Users\Usuario\AnelloSummer\optimAeroPX4SIL";
sessionId = "c172js_20260729_1025_zigzag_a72";

cd(repoRoot);
addpath(fullfile(repoRoot, "HILDiagnostics"));
clear mex;
initVehicleSIL( ...
    "launchFullSIL", false, ...
    "vehicleType", "c172pJSBSim", ...
    "controllerRuntime", "SITL", ...
    "visualizationType", "PassThrough");

assignin("base", "CONTROLLER_RUNTIME", 1);
cs = evalin("base", "standardSILConfigurationParams");
set_param(cs, "SignalLogging", "on");
set_param("VehicleSilSimulation", "StopTime", "3600");
open_system("VehicleSilSimulation");
set_param("VehicleSilSimulation", "SimulationCommand", "start");

% SimulationCommand is asynchronous. Do not mistake the initial queued
% "stopped" state for completion and close the model before its first step.
startDeadline = tic;
while strcmp(get_param("VehicleSilSimulation", "SimulationStatus"), "stopped")
    if toc(startDeadline) > 120
        error("VehicleSilSimulation did not leave the stopped state within 120 seconds.");
    end
    pause(0.1);
end

while ~strcmp(get_param("VehicleSilSimulation", "SimulationStatus"), "stopped")
    pause(1);
end

addpath(fullfile(repoRoot, "HILDiagnostics"));
saveLiveSILSession(sessionId);
