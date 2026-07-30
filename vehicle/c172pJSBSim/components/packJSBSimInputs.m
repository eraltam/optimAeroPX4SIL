function jsbsimInputs = packJSBSimInputs(posAileron_rad, posElevator_rad, posRudder_rad, cmdThrottle_unit)
%#codegen
% Re-normalizes actuatorsC172p.slx's achieved (rate-limited) surface positions back to JSBSim's
% fcs/*-cmd-norm convention, using the same real deflection limits as setUpActuators.m's "c172p"
% case (sourced from c172p.xml <flight_control> aerosurface_scale ranges: elevator 28 deg,
% aileron 20 deg, rudder 16 deg). Throttle is passed straight through (already 0..1 normalized,
% not rate-limited by actuatorsC172p). Apply both wheel brakes while the engine is at idle so the
% brake-less PX4 mapping cannot let the aircraft roll hundreds of metres before arming. Brakes
% release as soon as takeoff power is commanded and reapply after landing at idle. Output order
% matches c172pJSBSim_io.xml <input> exactly: aileron, elevator, rudder, throttle, left brake,
% right brake -- do not reorder without updating that file.
maxAilDefl_rad = 20 * pi/180;
maxElevatorDefl_rad = 28 * pi/180;
maxRudderDefl_rad = 16 * pi/180;

aileronCmd_norm = max(min(posAileron_rad / maxAilDefl_rad, 1), -1);
elevatorCmd_norm = max(min(posElevator_rad / maxElevatorDefl_rad, 1), -1);
rudderCmd_norm = max(min(posRudder_rad / maxRudderDefl_rad, 1), -1);
throttleCmd_norm = max(min(cmdThrottle_unit, 1), 0);
brakeCmd_norm = double(throttleCmd_norm < 0.05);

jsbsimInputs = [aileronCmd_norm; elevatorCmd_norm; rudderCmd_norm; throttleCmd_norm; ...
    brakeCmd_norm; brakeCmd_norm];
end
