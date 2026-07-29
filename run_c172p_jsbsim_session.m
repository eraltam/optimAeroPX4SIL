function run_c172p_jsbsim_session(sessionId)
%RUN_C172P_JSBSIM_SESSION Run and record one C172P JSBSim/PX4 SIL attempt.
arguments
    sessionId (1,1) string
end

plantRoot = "C:\Users\Usuario\AnelloSummer\optimAeroPX4SIL";
diagnosticsRoot = "C:\Users\Usuario\AnelloSummer\optimAeroPX4SIL\HILDiagnostics";
sessionDir = fullfile(diagnosticsRoot, "logs", "session_" + sessionId);
px4LogRoot = "\\wsl.localhost\Ubuntu-22.04\home\eraltam\PX4-Autopilot-optimAero\build\px4_sitl_default\rootfs\log";

if ~exist(sessionDir, "dir")
    mkdir(sessionDir);
end
diary(fullfile(sessionDir, "matlab_console.log"));
diary on;
cleanup = onCleanup(@() diary("off")); %#ok<NASGU>

fprintf("[%s] Starting C172P JSBSim session %s\n", datetime("now"), sessionId);
addpath(diagnosticsRoot);
cd(plantRoot);

try
    initVehicleSIL("launchFullSIL", false, ...
        "vehicleType", "c172pJSBSim", ...
        "controllerRuntime", "SITL", ...
        "visualizationType", "PassThrough");

    % initVehicleSIL rebuilds the project path; re-add diagnostics afterward.
    addpath(diagnosticsRoot);
    % Runs are launched sequentially by the mission harness. Reuse one
    % vehicle-specific build cache so a new recorded attempt does not spend
    % several minutes recompiling all 14 referenced models while PX4 and the
    % mission harness are already consuming their connection timeouts.
    % Do not overlap two C172 JSBSim sessions against this cache.
    cleanBuildRoot = fullfile(diagnosticsRoot, "simulink_build_c172pJSBSim");
    cacheFolder = fullfile(cleanBuildRoot, "cache");
    codeGenFolder = fullfile(cleanBuildRoot, "codegen");
    if ~exist(cacheFolder, "dir"); mkdir(cacheFolder); end
    if ~exist(codeGenFolder, "dir"); mkdir(codeGenFolder); end
    Simulink.fileGenControl("set", "CacheFolder", cacheFolder, ...
        "CodeGenFolder", codeGenFolder, "createDir", true);
    cs = evalin("base", "standardSILConfigurationParams");
    set_param(cs, "SignalLogging", "on", "SignalLoggingName", "logsout");
    open_system("VehicleSilSimulation.slx");
    simIn = Simulink.SimulationInput("VehicleSilSimulation");
    % The departure-corridor mission completes its sixth item and RTL
    % handoff by about 162 simulated seconds. End naturally after a short
    % evidence margin so simOut/logsout are preserved instead of terminating
    % PX4 externally after mission success.
    % JSBSim advances aircraft/PX4 time at 1/20 of this top-model time
    % (a38: StopTime 3000 yielded tout=150.684 s). Keep the plant alive for
    % about 2400 aircraft seconds so a transient waypoint orbit also has
    % time to recover before RTL, approach, landing, and disarm.
    simIn = setModelParameter(simIn, "StopTime", "48000", ...
        "SignalLogging", "on", "SignalLoggingName", "logsout", ...
        "ReturnWorkspaceOutputs", "on");
    fprintf("[%s] Entering synchronous simulation\n", datetime("now"));
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
    try
        if bdIsLoaded("VehicleSilSimulation") && ...
                string(get_param("VehicleSilSimulation", "SimulationStatus")) ~= "stopped"
            set_param("VehicleSilSimulation", "SimulationCommand", "stop");
            pause(2);
        end
    catch stopError
        fprintf(2, "Failed to stop model cleanly: %s\n", stopError.message);
    end
end

try
    saveLiveSILSession(sessionId, "px4LogRoot", px4LogRoot);
catch saveError
    fprintf(2, "[%s] Session save error:\n%s\n", datetime("now"), getReport(saveError, "extended"));
end
fprintf("[%s] Session %s finished\n", datetime("now"), sessionId);
end
