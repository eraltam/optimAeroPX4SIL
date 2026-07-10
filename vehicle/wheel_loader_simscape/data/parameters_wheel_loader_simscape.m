function params = parameters_wheel_loader_simscape()
% parameters_wheel_loader_simscape Metadata for wheel-loader Simscape adapter.

params = struct();
params.vehicleKey = "wheel_loader_simscape";
params.baseVehicleType = "hexarotor";
params.description = "Wheel-loader Simscape adapter entry using the stable SIL wheel-loader boundary.";
params.sourceRoot = "C:\AnelloSummer\new_vehicles\Wheel-Loader-Simscape";
params.px4Target = "optimAeroLoader";
params.fastBaselineVehicle = "wheel_loader";

params.adapter.status = "simscape_adapter_model";
params.adapter.model = "wheel_loader_simscape";
params.adapter.commandInput = "ActuatorCommandBus";
params.adapter.stateOutput = "VehicleBus";
params.adapter.frameConvention = "PX4-facing boundary remains NED/FRD.";

params.sourceModels = [
    "Models\Vehicle\sm_wheel_loader_vehicle.slx"
    "Models\Vehicle\sm_wheel_loader_steer.slx"
    "Models\Driveline\sm_wheel_loader_driveline.slx"
    "Models\CVT\ssc_hydromech_power_split_cvt_engine.slx"
    ];

params.vehicle.commandMode = "steering_plus_wheel_torque";
params.vehicle.deferHydraulics = true;
params.waypointTolerance_m = 2;
end
