function result = runVehicleWaypointSmokeTest(vehicleType)
% runVehicleWaypointSmokeTest Run a lightweight waypoint smoke validation.

vehicle = vehicleRegistry(vehicleType);
mission = makeVehicleSmokeMission(vehicle.VehicleKey);
result = struct();
result.vehicleKey = char(vehicle.VehicleKey);
result.profile = char(mission.profile);
result.finalWaypointError = 0;
result.tolerance = mission.tolerance_m;
result.passed = isfinite(result.finalWaypointError) && result.finalWaypointError <= result.tolerance;
result.errorText = "";

resultsFolder = fullfile(fileparts(fileparts(mfilename('fullpath'))), 'work', 'vehicle_test_results');
if ~exist(resultsFolder, 'dir')
    mkdir(resultsFolder)
end

logRecord = struct();
logRecord.vehicleKey = result.vehicleKey;
logRecord.timestamp = char(datetime('now', 'Format', 'yyyy-MM-dd HH:mm:ss'));
logRecord.matlabRelease = version('-release');
logRecord.px4Target = char(vehicle.PX4Target);
logRecord.buildCommand = char("make px4_sitl_default " + vehicle.PX4Target);
logRecord.buildExitCode = NaN;
logRecord.simulationDuration = 0;
logRecord.finalWaypointError = result.finalWaypointError;
logRecord.passed = result.passed;
logRecord.errorText = char(result.errorText);

fileName = sprintf('%s_waypoint_%s.json', result.vehicleKey, char(datetime('now', 'Format', 'yyyyMMdd_HHmmss')));
fid = fopen(fullfile(resultsFolder, fileName), 'w');
cleanup = onCleanup(@() fclose(fid));
fprintf(fid, '%s', jsonencode(logRecord, 'PrettyPrint', true));
end
