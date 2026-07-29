repoRoot = "C:\Users\Usuario\AnelloSummer\optimAeroPX4SIL";
sessionId = "c172js_20260729_1035_zigzag_a73";

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

% Use the synchronous API so MATLAB cannot return to the script and close
% the model before simulator_mavlink establishes its blocking TCP session.
simOut = sim("VehicleSilSimulation", ...
    "StopTime", "3600", ...
    "ReturnWorkspaceOutputs", "on");
assignin("base", "simOut", simOut);

addpath(fullfile(repoRoot, "HILDiagnostics"));
saveLiveSILSession(sessionId);
