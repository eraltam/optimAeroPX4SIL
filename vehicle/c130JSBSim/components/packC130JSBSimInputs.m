function jsbsimInputs = packC130JSBSimInputs(posAileron_rad, posElevator_rad, posRudder_rad, cmdThrottle_unit)
%#codegen
% Re-normalizes actuatorsC130JSBSim.slx's achieved (rate-limited) surface positions back to
% JSBSim's fcs/*-cmd-norm convention, using the same real deflection limits as
% setUpActuators.m's "c130jsbsim" case (sourced from C130.xml <flight_control> aerosurface_scale
% ranges: elevator -0.35/+0.3 rad, aileron/rudder +-0.35 rad -- symmetric +-0.35 rad/20.05 deg used
% here for all three, same "store/use the larger magnitude" convention as setUpActuators.m).
% Throttle is broadcast to all 4 T56 engine indices -- PX4's MAV_TYPE 1 airframe has only one
% throttle channel, no independent per-engine control in this pass (see
% PLAN_JSBSIM_SFUNCTION_F22_C130.md Phase B). Output order matches c130JSBSim_io.xml <input> order
% exactly: aileron, elevator, rudder, throttle[0], throttle[1], throttle[2], throttle[3],
% gear-cmd-norm, left brake, right brake -- do not reorder without updating that file.
% gear-cmd-norm is a hardcoded 1.0
% (gear permanently down, never retracts) -- see c130JSBSim_io.xml's header comment for why this
% is required (unlike c172p's fixed gear, C130's gear is retractable and defaults to 0/up if left
% undriven, which would remove ground-contact support entirely at simulation start).
maxAilDefl_rad = 20.05 * pi/180;
maxElevatorDefl_rad = 20.05 * pi/180;
maxRudderDefl_rad = 20.05 * pi/180;

aileronCmd_norm = max(min(posAileron_rad / maxAilDefl_rad, 1), -1);
elevatorCmd_norm = max(min(posElevator_rad / maxElevatorDefl_rad, 1), -1);
rudderCmd_norm = max(min(posRudder_rad / maxRudderDefl_rad, 1), -1);
throttleCmd_norm = max(min(cmdThrottle_unit, 1), 0);
gearCmd_norm = 1;
% Running T56 engines produce enough idle thrust to move the aircraft while
% PX4/MAVSDK are still connecting. Hold both main-wheel brakes at idle and
% release them as soon as PX4 commands meaningful takeoff power. Reapplying
% this rule after touchdown also lets PX4 confirm landed/disarmed.
brakeCmd_norm = double(throttleCmd_norm < 0.05);

jsbsimInputs = [aileronCmd_norm; elevatorCmd_norm; rudderCmd_norm; ...
                throttleCmd_norm; throttleCmd_norm; throttleCmd_norm; throttleCmd_norm; ...
                gearCmd_norm; brakeCmd_norm; brakeCmd_norm];
end
