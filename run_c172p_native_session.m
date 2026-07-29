function run_c172p_native_session(sessionId)
%RUN_C172P_NATIVE_SESSION Run and record the native C172/PX4 SIL plant.
arguments
    sessionId (1,1) string
end

plantRoot = "C:\Users\Usuario\AnelloSummer\optimAeroPX4SIL";
diagnosticsRoot = fullfile(plantRoot, "HILDiagnostics");
sessionDir = fullfile(diagnosticsRoot, "logs", "session_" + sessionId);
px4LogRoot = "\\wsl.localhost\Ubuntu-22.04\home\eraltam\PX4-Autopilot-optimAero\build\px4_sitl_default\rootfs\log";

if ~exist(sessionDir, "dir"); mkdir(sessionDir); end
diary(fullfile(sessionDir, "matlab_console.log"));
diary on;
cleanup = onCleanup(@() diary("off")); %#ok<NASGU>

fprintf("[%s] Starting native C172P session %s\n", datetime("now"), sessionId);
cd(plantRoot);

try
    initVehicleSIL("launchFullSIL", false, ...
        "vehicleType", "c172p", ...
        "controllerRuntime", "SITL", ...
        "visualizationType", "PassThrough");
    addpath(diagnosticsRoot);

    buildRoot = fullfile(diagnosticsRoot, "simulink_build_c172pNative");
    cacheFolder = fullfile(buildRoot, "cache");
    codeGenFolder = fullfile(buildRoot, "codegen");
    if ~exist(cacheFolder, "dir"); mkdir(cacheFolder); end
    if ~exist(codeGenFolder, "dir"); mkdir(codeGenFolder); end
    Simulink.fileGenControl("set", "CacheFolder", cacheFolder, ...
        "CodeGenFolder", codeGenFolder, "createDir", true);

    cs = evalin("base", "standardSILConfigurationParams");
    set_param(cs, "SignalLogging", "on", "SignalLoggingName", "logsout");
    open_system("VehicleSilSimulation.slx");
    simIn = Simulink.SimulationInput("VehicleSilSimulation");
    simIn = setModelParameter(simIn, "StopTime", "1800", ...
        "SignalLogging", "on", "SignalLoggingName", "logsout", ...
        "ReturnWorkspaceOutputs", "on");
    fprintf("[%s] Entering synchronous native-plant simulation\n", datetime("now"));
    simOut = sim(simIn);
    assignin("base", "simOut", simOut);
    if isprop(simOut, "tout") || any(string(simOut.who) == "tout")
        assignin("base", "tout", simOut.tout);
    end
    if any(string(simOut.who) == "logsout")
        assignin("base", "logsout", simOut.logsout);
    end
catch runError
    fprintf(2, "[%s] MATLAB run error:\n%s\n", datetime("now"), getReport(runError, "extended"));
end

try
    saveLiveSILSession(sessionId, "px4LogRoot", px4LogRoot);
catch saveError
    fprintf(2, "[%s] Session save error:\n%s\n", datetime("now"), getReport(saveError, "extended"));
end
fprintf("[%s] Native C172P session %s finished\n", datetime("now"), sessionId);
end
