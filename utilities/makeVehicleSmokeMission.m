function mission = makeVehicleSmokeMission(vehicleType)
% makeVehicleSmokeMission Create a deterministic waypoint smoke mission.

vehicle = vehicleRegistry(vehicleType);
mission = struct();
mission.vehicleKey = vehicle.VehicleKey;
mission.controllerType = vehicle.ControllerType;
mission.supportsWaypoints = vehicle.SupportsWaypoints;

switch lower(vehicle.ControllerType)
    case "fixedwing"
        mission.profile = "fixedwing";
        mission.waypoints_m = [0 0 -50; 80 0 -50; 80 80 -50; 0 80 -50];
        mission.tolerance_m = 10;
    case "rover"
        mission.profile = "ground_rover";
        mission.waypoints_m = [0 0 0; 10 0 0; 10 10 0; 0 10 0; 0 0 0];
        mission.tolerance_m = 2;
    case "boat"
        mission.profile = "usv_surface";
        mission.waypoints_m = [0 0 0; 20 0 0; 10 18 0; 0 0 0];
        mission.tolerance_m = 3;
    case "sub"
        mission.profile = "uuv_subsea";
        mission.waypoints_m = [0 0 -2; 10 0 -4; 10 10 -4; 0 10 -2; 0 0 0];
        mission.tolerance_m = 3;
    otherwise
        mission.profile = "air_multirotor";
        mission.waypoints_m = [0 0 -10; 20 0 -10; 20 20 -10; 0 20 -10; 0 0 -10; 0 0 0];
        mission.tolerance_m = 2;
end
end
