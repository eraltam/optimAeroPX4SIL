function sessionDir = saveLiveSILSession(sessionId, opts)
%SAVELIVESILSESSION Snapshot a live TCP-connector SIL run: MATLAB signals + matching PX4 ulog.
%
% For the live SimulationCommand='start'/'stop'-driven PX4 SITL loop (pixhawk_sil_connector,
% TCP port 4560) -- NOT the HITL/serial sim()-owning workflow in run_hil_automated_session.m,
% which already saves its own logsout/summary. Call this right after stopping (or while still
% running) VehicleSilSimulation to capture, in one HILDiagnostics/logs/session_<id>/ folder:
%   - every base-workspace ToWorkspace variable matching log_*, plus tout/logsout if present,
%     into matlab_signals.mat
%   - the newest PX4 .ulg found under PX4-Autopilot/build/px4_sitl_default/rootfs/log/**,
%     copied alongside as px4.ulg
% This reuses the session_<id> folder convention from run_hil_automated_session.m and
% HILDiagnostics/mavlink_system/session_paths.py -- point the Python mission harness at the
% same session with `--session-id <id>` (or it auto-picks up latest_session.txt written here)
% so its own param dumps/logs land in the same folder as the MATLAB signals and the ulog.
%
% Example:
%   saveLiveSILSession("2026-07-25_mission9")
%
% Caveat (open issue as of 2026-07-25): log_* ToWorkspace variables have been observed missing
% from the base workspace after a SimulationCommand='start'/'stop' cycle in at least one prior
% session, for a reason not yet root-caused (they populate reliably from sim()-driven runs).
% This function reports exactly what it found so that failure is visible immediately rather
% than producing a silently-empty .mat file.

arguments
    sessionId (1,1) string
    opts.px4LogRoot (1,1) string = ""
    opts.maxUlogAge_s (1,1) double = 7200
end

thisDir = string(fileparts(mfilename("fullpath")));
repoRoot = fileparts(thisDir);

if strlength(opts.px4LogRoot) == 0
    envLogRoot = string(getenv("PX4_LOG_ROOT"));
    candidateRoots = [
        envLogRoot
        fullfile(repoRoot, "PX4-Autopilot", "build", "px4_sitl_default", "rootfs", "log")
        "\\wsl.localhost\Ubuntu-22.04\home\eraltam\PX4-Autopilot-optimAero\build\px4_sitl_default\rootfs\log"
    ];
    existingRoot = find(strlength(candidateRoots) > 0 & isfolder(candidateRoots), 1);
    if isempty(existingRoot)
        opts.px4LogRoot = candidateRoots(2);
    else
        opts.px4LogRoot = candidateRoots(existingRoot);
    end
end

sessionName = "session_" + sanitizeSessionId(sessionId);
sessionDir = fullfile(thisDir, "logs", sessionName);
if ~exist(sessionDir, "dir")
    mkdir(sessionDir);
end
writeTextFile(fullfile(thisDir, "logs", "latest_session.txt"), sessionDir);
fprintf("Session directory: %s\n", sessionDir);

%% MATLAB signals
data = struct();
found = strings(0, 1);

if evalin("base", "exist('tout','var')")
    data.tout = evalin("base", "tout");
    found(end + 1) = "tout";
end
if evalin("base", "exist('logsout','var')")
    data.logsout = evalin("base", "logsout");
    found(end + 1) = "logsout";
end
if evalin("base", "exist('simOut','var')")
    data.simOut = evalin("base", "simOut");
    found(end + 1) = "simOut";
end
if evalin("base", "exist('runError','var')")
    data.runError = evalin("base", "runError");
    found(end + 1) = "runError";
end

baseVars = evalin("base", "who");
logVars = baseVars(startsWith(baseVars, "log_"));
for idx = 1:numel(logVars)
    name = logVars{idx};
    data.(name) = evalin("base", name);
    found(end + 1) = string(name); %#ok<AGROW>
end

data.sessionId = sessionId;
data.savedAt_utc = char(datetime("now", "TimeZone", "UTC", "Format", "yyyy-MM-dd'T'HH:mm:ss.SSS'Z'"));
data.matlabVersion = version;
data.matlabRelease = version("-release");
data.px4LogRoot = char(opts.px4LogRoot);

matPath = fullfile(sessionDir, "matlab_signals.mat");
save(matPath, "-struct", "data", "-v7.3");
matInfo = dir(matPath);
matVariables = whos("-file", matPath);
if isempty(matVariables) || matInfo.bytes < 1024
    error("saveLiveSILSession:InvalidMatArtifact", ...
        "MAT artifact validation failed: %s (%d bytes, %d variables)", ...
        matPath, matInfo.bytes, numel(matVariables));
end

if isempty(found)
    warning("saveLiveSILSession:NoSignals", ...
        "No tout/logsout/log_* variables found in the base workspace -- matlab_signals.mat " + ...
        "only has sessionId/savedAt_utc. See this function's caveat about the " + ...
        "SimulationCommand start/stop ToWorkspace gap.");
else
    fprintf("Saved %d MATLAB variable(s) to %s:\n", numel(found), matPath);
    fprintf("  %s\n", found);
end

%% PX4 ulog
ulogFiles = dir(fullfile(opts.px4LogRoot, "**", "*.ulg"));
if isempty(ulogFiles)
    warning("saveLiveSILSession:NoUlog", "No .ulg files found under %s", opts.px4LogRoot);
    return;
end

[~, newestIdx] = max([ulogFiles.datenum]);
newest = ulogFiles(newestIdx);
newestPath = fullfile(newest.folder, newest.name);
ageSeconds = seconds(datetime("now") - datetime(newest.datenum, "ConvertFrom", "datenum"));

if ageSeconds > opts.maxUlogAge_s
    warning("saveLiveSILSession:StaleUlog", ...
        "Newest ulog found (%s) is %.0f s old, past maxUlogAge_s=%.0f -- likely from a " + ...
        "different/earlier run, not copying. Pass a larger maxUlogAge_s if this run genuinely " + ...
        "took that long.", newestPath, ageSeconds, opts.maxUlogAge_s);
    return;
end

destPath = fullfile(sessionDir, "px4.ulg");
copyfile(newestPath, destPath);
destInfo = dir(destPath);
if isempty(destInfo) || destInfo.bytes == 0
    error("saveLiveSILSession:InvalidUlogArtifact", ...
        "ULG artifact validation failed after copying to %s", destPath);
end
fprintf("Copied ulog (%.0f s old): %s -> %s\n", ageSeconds, newestPath, destPath);

manifest = struct( ...
    "sessionId", char(sessionId), ...
    "savedAt_utc", data.savedAt_utc, ...
    "matlabFile", "matlab_signals.mat", ...
    "matlabBytes", matInfo.bytes, ...
    "matlabVariables", {string({matVariables.name})}, ...
    "ulogFile", "px4.ulg", ...
    "ulogBytes", destInfo.bytes, ...
    "ulogSource", newestPath, ...
    "ulogAgeSeconds", ageSeconds);
writeTextFile(fullfile(sessionDir, "artifacts.json"), jsonencode(manifest, PrettyPrint=true));
fprintf("Validated paired artifacts and wrote %s\n", ...
    fullfile(sessionDir, "artifacts.json"));
end

function sessionId = sanitizeSessionId(rawSessionId)
sessionId = string(regexprep(strtrim(char(rawSessionId)), "[^A-Za-z0-9_.-]", "_"));
if strlength(sessionId) == 0
    error("sessionId cannot be empty after sanitization.");
end
end

function writeTextFile(path, text)
fid = fopen(path, "w");
if fid < 0
    error("Could not open text output: %s", path);
end
cleanup = onCleanup(@() fclose(fid)); %#ok<NASGU>
fprintf(fid, "%s\n", char(text));
end
