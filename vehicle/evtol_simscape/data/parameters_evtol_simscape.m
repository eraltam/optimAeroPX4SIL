function params = parameters_evtol_simscape()
% parameters_evtol_simscape Metadata for the eVTOL Simscape adapter entry.

params = struct();
params.vehicleKey = "evtol_simscape";
params.baseVehicleType = "hexarotor";
params.description = "eVTOL Simscape adapter entry using the stable SIL eVTOL boundary.";
params.sourceProject = "C:\AnelloSummer\new_vehicles\eVTOL-Drone-Simscape\eVTOLDroneSimscape.prj";
params.sourceRoot = "C:\AnelloSummer\new_vehicles\eVTOL-Drone-Simscape";
params.px4Target = "optimAeroEVTOL";
params.fastBaselineVehicle = "evtol";

params.adapter.status = "simscape_adapter_model";
params.adapter.model = "evtol_simscape";
params.adapter.commandInput = "ActuatorCommandBus.MotorsCommandBus";
params.adapter.stateOutput = "VehicleBus";
params.adapter.frameConvention = "PX4-facing boundary remains NED/FRD.";

params.sourceModels = [
    "Components\Vehicle\AirframePowertrain.slx"
    "Components\Vehicle\AirframePowertrainMotorSDL.slx"
    "Components\Motor\PropulsionMotor.slx"
    "Components\Propeller\PropellerSimscape.slx"
    "Components\Propeller\PropulsionSystem.slx"
    "Components\Battery\BatteryPlantModel_Simple.slx"
    "Model\EVTOLTiltrotor.slx"
    ];

params.battery.nominalVoltage_V = 22.5;
params.battery.internalResistance_ohm = 0.001;
params.battery.capacity_Ah = 10;
params.battery.initialPackSOC = 0.75;
params.battery.seriesCells = 6;
params.battery.parallelCells = 1;
params.battery.gravimetricDensity_WhPerKg = 180;

params.rotor.initialWref_radps = 107.320163057312;
params.rotor.count = 4;
params.rotor.tiltRotorCount = 2;
params.waypointTolerance_m = 3;
end
