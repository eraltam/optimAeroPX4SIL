function contract = evtolSimscapeAdapterContract()
% evtolSimscapeAdapterContract Describe the required Simscape-to-SIL boundary.

contract = struct();
contract.vehicleKey = "evtol_simscape";
contract.commandAdapter.inputs = [
    "ActuatorCommandBus.MotorsCommandBus.motor1"
    "ActuatorCommandBus.MotorsCommandBus.motor2"
    "ActuatorCommandBus.MotorsCommandBus.motor3"
    "ActuatorCommandBus.MotorsCommandBus.motor4"
    "ActuatorCommandBus.MotorsCommandBus.motor5"
    "ActuatorCommandBus.MotorsCommandBus.motor6"
    ];
contract.commandAdapter.outputs = [
    "VehicleHarness.R1.Act.Wref"
    "VehicleHarness.R2.Act.Wref"
    "VehicleHarness.R3.Act.Wref"
    "VehicleHarness.R4.Act.Wref"
    "VehicleHarness.Tilt.Tilt1"
    "VehicleHarness.Tilt.Tilt2"
    ];
contract.stateAdapter.inputs = [
    "Vehicle_A_St.Xe"
    "Vehicle_A_St.Ve"
    "Vehicle_A_St.Euler"
    "Vehicle_A_St.Vb"
    "Vehicle_A_St.pqr"
    "Vehicle_A_St.Abb"
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
    "ENU/Earth velocity to NED velocity"
    "Body frame to FRD"
    "Gravity sign at rest"
    "Yaw sign"
    "Rotor thrust positive direction"
    ];
end
