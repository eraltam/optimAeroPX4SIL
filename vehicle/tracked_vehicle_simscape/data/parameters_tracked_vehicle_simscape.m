function params = parameters_tracked_vehicle_simscape()
% parameters_tracked_vehicle_simscape Metadata for tracked Simscape adapter.

params = struct();
params.vehicleKey = "tracked_vehicle_simscape";
params.baseVehicleType = "hexarotor";
params.description = "Tracked vehicle Simscape adapter entry using the stable SIL tracked boundary.";
params.sourceRoot = "C:\AnelloSummer\new_vehicles\Tracked-Vehicles-Simscape";
params.px4Target = "optimAeroTracked";
params.fastBaselineVehicle = "tracked_vehicle";

params.adapter.status = "simscape_adapter_model";
params.adapter.model = "tracked_vehicle_simscape";
params.adapter.commandInput = "ActuatorCommandBus";
params.adapter.stateOutput = "VehicleBus";
params.adapter.frameConvention = "PX4-facing boundary remains NED/FRD.";

params.sourceModels = [
    "Libraries\Belt\sm_trackV_lib_belt_track_s45.slx"
    "Libraries\Chain\sm_trackV_lib_chain_track_s46.slx"
    "Libraries\Scene\sm_trackV_lib_terrain_contact_s46.slx"
    "Libraries\Chassis\sm_excv_chassis_lib.slx"
    ];

params.track.count = 2;
params.track.commandMode = "left_right_track_force";
params.waypointTolerance_m = 2;
end
