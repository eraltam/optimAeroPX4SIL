function params = parameters_ackermann_simscape()
% parameters_ackermann_simscape Metadata for Ackermann Simscape adapter.

params = struct();
params.vehicleKey = "ackermann_simscape";
params.baseVehicleType = "hexarotor";
params.description = "Ackermann Simscape adapter entry using the stable SIL Ackermann boundary.";
params.sourceRoot = "C:\AnelloSummer\new_vehicles\Formula-Student-Vehicle-Simscape";
params.secondarySourceRoot = "C:\AnelloSummer\new_vehicles\Optimize-Vehicle-Design-with-AI-and-Simscape";
params.px4Target = "optimAeroAckermann";
params.fastBaselineVehicle = "ackermann_rover";

params.adapter.status = "simscape_adapter_model";
params.adapter.model = "ackermann_simscape";
params.adapter.commandInput = "ActuatorCommandBus";
params.adapter.stateOutput = "VehicleBus";
params.adapter.frameConvention = "PX4-facing boundary remains NED/FRD.";

params.sourceAreas = [
    "Libraries\Event\Scene\Plane_Grid"
    "Libraries\Event\Scene\Skidpad"
    "Formula Student vehicle libraries"
    "Optimize-Vehicle-Design-with-AI-and-Simscape"
    ];

params.vehicle.commandMode = "steering_angle_plus_drive_torque";
params.vehicle.steeringLimit_rad = 0.6;
params.waypointTolerance_m = 2;
end
