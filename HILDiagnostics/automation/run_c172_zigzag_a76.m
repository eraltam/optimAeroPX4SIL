repoRoot = "C:\Users\Usuario\AnelloSummer\optimAeroPX4SIL";
sessionId = "c172js_20260729_1045_zigzag_heading_a76";

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
set_param("VehicleSilSimulation", "StopTime", "18000");
open_system("VehicleSilSimulation");

simOut = sim("VehicleSilSimulation", ...
    "StopTime", "18000", ...
    "ReturnWorkspaceOutputs", "on");
assignin("base", "simOut", simOut);

addpath(fullfile(repoRoot, "HILDiagnostics"));
saveLiveSILSession(sessionId);
