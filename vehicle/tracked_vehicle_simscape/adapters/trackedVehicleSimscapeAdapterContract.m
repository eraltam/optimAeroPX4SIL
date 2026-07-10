function contract = trackedVehicleSimscapeAdapterContract()
% trackedVehicleSimscapeAdapterContract Describe tracked vehicle adapter boundary.

contract = struct();
contract.vehicleKey = "tracked_vehicle_simscape";
contract.commandAdapter.inputs = [
    "ActuatorCommandBus.MotorsCommandBus or rover throttle/yaw equivalent"
    "PX4 left/right differential command after output mapping"
    ];
contract.commandAdapter.outputs = [
    "leftTrackForce_N"
    "rightTrackForce_N"
    "leftTrackSpeed_mps"
    "rightTrackSpeed_mps"
    ];
contract.stateAdapter.inputs = [
    "simscapeWorldPosition"
    "simscapeWorldVelocity"
    "simscapeBodyAttitude"
    "simscapeBodyRates"
    "simscapeBodyAcceleration"
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
    "World-to-NED position sign"
    "Forward body axis to FRD X"
    "Yaw sign under left/right differential turn"
    "Ground contact does not inject NaN at t=0"
    ];
end
