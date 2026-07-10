function params = parameters_quadrotor()
% parameters_quadrotor Compatibility parameters for the first quadrotor SIL smoke model.
params = struct();
params.vehicleKey = "quadrotor";
params.baseVehicleType = "hexarotor";
params.description = "Quadrotor registry entry using the stable multirotor compatibility plant.";
params.actuatorCount = 4;
params.waypointTolerance_m = 2;
end
