%RUN_CURRENT_MISSION_FROM_ENV Stable no-quoting entry point for detached MATLAB -batch runs.
repoRoot = fileparts(mfilename("fullpath"));
cd(repoRoot);
addpath(fullfile(repoRoot, "HILDiagnostics", "automation"));

vehicleType = string(getenv("OPTIMAERO_VEHICLE"));
sessionId = string(getenv("OPTIMAERO_SESSION_ID"));
visualizationType = string(getenv("OPTIMAERO_VISUALIZATION"));
stopTimeText = string(getenv("OPTIMAERO_STOP_TIME"));
insVariantText = string(getenv("OPTIMAERO_INS_VARIANT"));
assert(strlength(vehicleType) > 0 && strlength(sessionId) > 0 && ...
    strlength(visualizationType) > 0, "Missing OPTIMAERO mission environment variables.");
stopTime = str2double(stopTimeText);
if isnan(stopTime); stopTime = 18000; end
insVariant = str2double(insVariantText);
if isnan(insVariant); insVariant = 2; end

run_jsbsim_mission_batch(vehicleType, sessionId, visualizationType, stopTime, insVariant);
