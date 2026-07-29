repoRoot = "C:\Users\Usuario\AnelloSummer\optimAeroPX4SIL";
sessionId = "c172js_20260729_1000_zigzag_a70";

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
set_param("VehicleSilSimulation", "StopTime", "3600");

% Without a connected PX4 lockstep client, the model can race through the
% stop time before TCP 4560 remains open long enough for WSL to attach.
% Ten-times-real-time pacing provides a stable connection window; once PX4
% connects, the simulator_mavlink lockstep exchange controls progression.
set_param("VehicleSilSimulation", "EnablePacing", "on");
set_param("VehicleSilSimulation", "PacingRate", "10");
set_param("VehicleSilSimulation", "SimulationCommand", "start");

while ~strcmp(get_param("VehicleSilSimulation", "SimulationStatus"), "stopped")
    pause(1);
end

addpath(fullfile(repoRoot, "HILDiagnostics"));
saveLiveSILSession(sessionId);
