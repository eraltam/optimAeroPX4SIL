function contract = wheelLoaderSimscapeAdapterContract()
% wheelLoaderSimscapeAdapterContract Describe wheel-loader adapter boundary.

contract = struct();
contract.vehicleKey = "wheel_loader_simscape";
contract.commandAdapter.inputs = [
    "PX4 rover throttle output"
    "PX4 rover steering output"
    ];
contract.commandAdapter.outputs = [
    "wheelTorqueCommand_Nm"
    "steeringOrArticulationCommand_rad"
    "brakeCommand"
    ];
contract.stateAdapter.inputs = [
    "simscapeVehiclePosition"
    "simscapeVehicleVelocity"
    "simscapeVehicleAttitude"
    "simscapeVehicleBodyRates"
    "simscapeVehicleAcceleration"
    ];
contract.stateAdapter.outputs = [
    "VehicleBus.BodyStateBus.aircraftPosInNED_m"
    "VehicleBus.BodyStateBus.aircraftVelInNED_mps"
    "VehicleBus.BodyStateBus.aircraftEuler_rad"
    "VehicleBus.BodyStateBus.aircraftVelInBody_mps"
    "VehicleBus.BodyStateBus.aircraftAngVelInBody_radps"
    "VehicleBus.BodyStateBus.aircraftAccelInBody_mps2"
    ];
contract.frameChecks = [
    "Forward drive produces positive body X velocity"
    "Positive steering gives expected yaw sign"
    "Flat terrain initial contact is finite"
    "Hydraulic/bucket states disabled for first PX4 loop closure"
    ];
end
