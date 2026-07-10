function contract = ackermannSimscapeAdapterContract()
% ackermannSimscapeAdapterContract Describe automotive adapter boundary.

contract = struct();
contract.vehicleKey = "ackermann_simscape";
contract.commandAdapter.inputs = [
    "PX4 rover throttle output"
    "PX4 rover steering output"
    ];
contract.commandAdapter.outputs = [
    "driveTorqueCommand_Nm"
    "steeringAngleCommand_rad"
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
    "Positive throttle moves forward in body X"
    "Positive steering yaw sign matches PX4 rover controller"
    "Vehicle remains finite on flat terrain at t=0"
    "Low-speed waypoint tracking before tire/suspension fidelity"
    ];
end
