repoRoot = "C:\Users\Usuario\AnelloSummer\optimAeroPX4SIL";
sessionId = "c172js_20260729_1015_zigzag_a71";

cd(repoRoot);
addpath(fullfile(repoRoot, "HILDiagnostics"));
clear mex;
initVehicleSIL( ...
    "launchFullSIL", false, ...
    "vehicleType", "c172pJSBSim", ...
    "controllerRuntime", "SITL", ...
    "visualizationType", "PassThrough");

% The SITL connector's Enable gate and Variant Source evaluate this symbol
% from the base workspace after initVehicleSIL has returned.
assignin("base", "CONTROLLER_RUNTIME", 1);

cs = evalin("base", "standardSILConfigurationParams");
set_param(cs, "SignalLogging", "on");
set_param("VehicleSilSimulation", "StopTime", "3600");
open_system("VehicleSilSimulation");
set_param("VehicleSilSimulation", "SimulationCommand", "start");

while ~strcmp(get_param("VehicleSilSimulation", "SimulationStatus"), "stopped")
    pause(1);
end

addpath(fullfile(repoRoot, "HILDiagnostics"));
saveLiveSILSession(sessionId);
