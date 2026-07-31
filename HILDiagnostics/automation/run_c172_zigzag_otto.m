% run_c172_zigzag_otto.m
% Reconstruction of run_c172_zigzag_a76.m for THIS machine (was C:\Users\Usuario).
% Phase 1: PassThrough visualization, ANELLO INS (INS_VARIANT=2), C172 JSBSim wide-zigzag.
% Headless-friendly: load_system (not open_system) + try/catch so saveLiveSILSession always runs.

repoRoot = "C:\Users\Edison Altamirano\SITLV2\AnelloSummer\optimAeroPX4SIL";
sessionId = getenv("OTTO_SESSION_ID");
if strlength(sessionId) == 0
    sessionId = "c172js_20260729_zigzag_pt_a77";
end
visType = getenv("OTTO_VIS_TYPE");
if strlength(visType) == 0
    visType = "PassThrough";   % Phase 1
end

fprintf("[otto] repoRoot=%s\n[otto] sessionId=%s\n[otto] visualizationType=%s\n", ...
    repoRoot, sessionId, visType);

cd(repoRoot);
addpath(fullfile(repoRoot, "HILDiagnostics"));
clear mex; %#ok<CLMEX>

initVehicleSIL( ...
    "launchFullSIL", false, ...
    "vehicleType", "c172pJSBSim", ...
    "controllerRuntime", "SITL", ...
    "visualizationType", visType);

% Select ANELLO X3 INS path (1=Generic, 2=AnelloX3).
assignin("base", "INS_VARIANT", 2);
assignin("base", "CONTROLLER_RUNTIME", 1);

cs = evalin("base", "standardSILConfigurationParams");
set_param(cs, "SignalLogging", "on");
set_param("VehicleSilSimulation", "StopTime", "18000");
load_system("VehicleSilSimulation");

fprintf("[otto] starting sim -- pixhawk_sil_connector will listen on TCP 4560 shortly...\n");
simErr = [];
try
    simOut = sim("VehicleSilSimulation", ...
        "StopTime", "18000", ...
        "ReturnWorkspaceOutputs", "on");
    assignin("base", "simOut", simOut);
    fprintf("[otto] sim returned normally.\n");
catch simErr
    fprintf(2, "[otto] sim ended with error (expected on PX4 shutdown / TCP close): %s\n", simErr.message);
end

addpath(fullfile(repoRoot, "HILDiagnostics"));
sessionDir = saveLiveSILSession(sessionId);
fprintf("[otto] session saved to: %s\n", sessionDir);

if ~isempty(simErr)
    fprintf("[otto] (sim error above was tolerated so the session could be saved)\n");
end
