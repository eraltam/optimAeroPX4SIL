function params = parameters_ackermann_rover()
% parameters_ackermann_rover Compatibility parameters for the first rover SIL smoke model.
params = struct();
params.vehicleKey = "ackermann_rover";
params.baseVehicleType = "hexarotor";
params.description = "Ackermann rover registry entry using the stable multirotor compatibility plant.";
params.steeringLimit_rad = 0.6;
params.waypointTolerance_m = 2;
end
