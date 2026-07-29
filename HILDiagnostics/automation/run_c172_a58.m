repoRoot = "C:\Users\Usuario\AnelloSummer\optimAeroPX4SIL";
sessionId = "c172js_20260729_0551_ordered_a58";

cd(repoRoot);
addpath(fullfile(repoRoot, "HILDiagnostics"));
clear mex;
initVehicleSIL( ...
    "launchFullSIL", false, ...
    "vehicleType", "c172pJSBSim", ...
    "controllerRuntime", "SITL", ...
    "visualizationType", "PassThrough");

cs = evalin("base", "standardSILConfiguraitonParams");
set_param(cs, "SignalLogging", "on");
set_param("VehicleSilSimulation", "StopTime", "1200");
set_param("VehicleSilSimulation", "SimulationCommand", "start");

while ~strcmp(get_param("VehicleSilSimulation", "SimulationStatus"), "stopped")
    pause(1);
end

saveLiveSILSession(sessionId);
