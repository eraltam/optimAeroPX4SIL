repoRoot = "C:\Users\Usuario\AnelloSummer\optimAeroPX4SIL";
sessionId = string(getenv("C172_SESSION_ID"));
if strlength(sessionId) == 0
    sessionId = "c172js_20260729_1725_truthyaw_a80";
end

cd(repoRoot);
addpath(fullfile(repoRoot, "HILDiagnostics"));
addpath(fullfile(repoRoot, "HILDiagnostics", "automation"));
clear mex;
% Keep generated Simulink artifacts isolated from earlier plant variants.
% Reusing work/slprj after changing the JSBSim input vector can preserve the
% old inferred 4-element MATLAB Function width while the S-function expects 6.
runWork = fullfile(repoRoot, "work", char(sessionId));
if ~isfolder(runWork)
    mkdir(runWork);
end
Simulink.fileGenControl("set", ...
    "CacheFolder", runWork, ...
    "CodeGenFolder", runWork, ...
    "createDir", true);
initVehicleSIL( ...
    "launchFullSIL", false, ...
    "vehicleType", "c172pJSBSim", ...
    "controllerRuntime", "SITL", ...
    "visualizationType", "PassThrough");

% Use the mapping stored in the model. PX4 airframe 10044 assigns its output
% functions as channel 1 engine, channel 2 aileron, channel 3 elevator, and
% channel 4 rudder. The saved model's selectors were verified through the
% Simulink model tools as Selector1=3 (elevator), Selector2=2 (aileron), and
% Selector3=4 (rudder). Do not override these using the generic
% HIL_ACTUATOR_CONTROLS [roll,pitch,yaw,throttle] assumption: PX4 sends the
% configured actuator-output functions for this airframe.
load_system(fullfile(repoRoot, "vehicle", "c172pJSBSim", "components", ...
    "PX4OutputMappingC172pJSBSim.slx"));

% initVehicleSIL loads the plant copy embedded in VehicleSilSimulation. Keep
% that MATLAB Function synchronized with the six-channel JSBSim S-function
% interface (four flight controls plus left/right wheel brakes).
packerPath = "VehicleSilSimulation/vehicleSimulation/plant/c172pJSBSim/packJSBSimInputs";
sf = sfroot;
packerChart = sf.find("-isa", "Stateflow.EMChart", "Path", packerPath);
assert(~isempty(packerChart), "Could not find embedded JSBSim input packer");
packerChart.Script = fileread(fullfile(repoRoot, ...
    "vehicle", "c172pJSBSim", "components", "packJSBSimInputs.m"));

% Keep the embedded JSBSim output unpacker synchronized as well. Editing the
% source .m file alone does not update the MATLAB Function chart copied into
% VehicleSilSimulation by initVehicleSIL. This is required for acceleration
% corrections (including the rotating-frame omega-cross-velocity term) to
% reach the simulated IMU.
unpackerPath = "VehicleSilSimulation/vehicleSimulation/plant/c172pJSBSim/unpackJSBSimOutputs";
unpackerChart = sf.find("-isa", "Stateflow.EMChart", "Path", unpackerPath);
assert(~isempty(unpackerChart), "Could not find embedded JSBSim output unpacker");
unpackerChart.Script = fileread(fullfile(repoRoot, ...
    "vehicle", "c172pJSBSim", "components", "unpackJSBSimOutputs.m"));

% Preserve the existing plant safety limits while filtering isolated
% solver/transition spikes. Invalid or extreme states still abort
% immediately; ordinary limit crossings must persist for 0.5 seconds.
abortPath = "VehicleSilSimulation/CheckAbort";
abortChart = sf.find("-isa", "Stateflow.EMChart", "Path", abortPath);
assert(~isempty(abortChart), "Could not find CheckAbort MATLAB Function");
abortChart.Script = fileread(fullfile(repoRoot, ...
    "HILDiagnostics", "automation", "checkAbort.m"));

% The connector's optional 17-element truth port is grounded in the stock
% model. Route the existing synchronized groundTruthData vector into it.
groundLine = get_param("VehicleSilSimulation/PX4 Interface/Ground", "LineHandles");
delete_line(groundLine.Outport);
delete_block("VehicleSilSimulation/PX4 Interface/Ground");
add_block("simulink/Ports & Subsystems/In1", ...
    "VehicleSilSimulation/PX4 Interface/PX4 Truth In", "Port", "4");
add_line("VehicleSilSimulation", "Rate Transition2/1", ...
    "PX4 Interface/4", "autorouting", "on");
add_block("simulink/Signal Routing/Bus Selector", ...
    "VehicleSilSimulation/PX4 Interface/PX4 Truth Selector", ...
    "OutputSignals", ...
    "signal2.aircraftEulerAngles_rad,signal2.aircraftAngVelInBody_radps,signal2.aircraftVelInNED_mps");
add_block("simulink/Math Operations/Gain", ...
    "VehicleSilSimulation/PX4 Interface/PX4 Truth Velocity cmps", ...
    "Gain", "100");
add_block("simulink/Sources/Constant", ...
    "VehicleSilSimulation/PX4 Interface/PX4 Truth Zero3", ...
    "Value", "zeros(3,1)");
add_block("simulink/Sources/Constant", ...
    "VehicleSilSimulation/PX4 Interface/PX4 Truth Zero5", ...
    "Value", "zeros(5,1)");
add_block("simulink/Signal Routing/Mux", ...
    "VehicleSilSimulation/PX4 Interface/PX4 Truth Vector", ...
    "Inputs", "5");
add_line("VehicleSilSimulation/PX4 Interface", "PX4 Truth In/1", ...
    "PX4 Truth Selector/1", "autorouting", "on");
add_line("VehicleSilSimulation/PX4 Interface", "PX4 Truth Selector/1", ...
    "PX4 Truth Vector/1", "autorouting", "on");
add_line("VehicleSilSimulation/PX4 Interface", "PX4 Truth Selector/2", ...
    "PX4 Truth Vector/2", "autorouting", "on");
add_line("VehicleSilSimulation/PX4 Interface", "PX4 Truth Zero3/1", ...
    "PX4 Truth Vector/3", "autorouting", "on");
add_line("VehicleSilSimulation/PX4 Interface", "PX4 Truth Selector/3", ...
    "PX4 Truth Velocity cmps/1", "autorouting", "on");
add_line("VehicleSilSimulation/PX4 Interface", "PX4 Truth Velocity cmps/1", ...
    "PX4 Truth Vector/4", "autorouting", "on");
add_line("VehicleSilSimulation/PX4 Interface", "PX4 Truth Zero5/1", ...
    "PX4 Truth Vector/5", "autorouting", "on");
add_line("VehicleSilSimulation/PX4 Interface", "PX4 Truth Vector/1", ...
    "S-Function/10", "autorouting", "on");

assignin("base", "CONTROLLER_RUNTIME", 1);
cs = evalin("base", "standardSILConfigurationParams");
set_param(cs, "SignalLogging", "on");
% Batch execution currently advances substantially faster than wall time even
% with pacing enabled. Keep a generous simulation-time horizon so a valid
% fixed-wing mission is not truncated while it is physically in progress.
set_param("VehicleSilSimulation", "StopTime", "180000");
set_param("VehicleSilSimulation", "EnablePacing", "on");
set_param("VehicleSilSimulation", "PacingRate", "0.5");

runException = [];
successMarker = fullfile(repoRoot, "HILDiagnostics", "logs", ...
    "session_" + sessionId, "mission_success.marker");
if isfile(successMarker)
    delete(successMarker);
end
stopTimer = timer( ...
    "ExecutionMode", "fixedSpacing", ...
    "Period", 1.0, ...
    "TimerFcn", @(~,~) stopSILWhenMarker("VehicleSilSimulation", successMarker));
start(stopTimer);
try
    simOut = sim("VehicleSilSimulation", ...
        "StopTime", "180000", ...
        "ReturnWorkspaceOutputs", "on");
    assignin("base", "simOut", simOut);
catch caughtException
    runException = caughtException;
    runError = struct( ...
        "identifier", caughtException.identifier, ...
        "message", caughtException.message, ...
        "report", getReport(caughtException, "extended", "hyperlinks", "off"));
    assignin("base", "runError", runError);
end
stop(stopTimer);
delete(stopTimer);

addpath(fullfile(repoRoot, "HILDiagnostics"));
saveLiveSILSession(sessionId);
if ~isempty(runException)
    rethrow(runException);
end
