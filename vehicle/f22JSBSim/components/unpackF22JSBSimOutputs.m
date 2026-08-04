function [aircraftVelInNED_mps, aircraftPosInNED_m, aircraftEulerAngles_rad, ...
          aircraftVelInBody_mps, aircraftAngVelInBody_radps, aircraftAngAccelInBody_radps2, ...
          aircraftAccelInBody_mps2, DCM_be, aircraftAccelbe_mps2, ...
          airspeedInBody_mps, alpha_rad, beta_rad, isVehicleAirborne] = ...
          unpackF22JSBSimOutputs(jsbsimState, jsbsimAero, gravityScalar_mps2, ...
          referenceLatitude_deg, referenceLongitude_deg) %#codegen
% Converts JSBSim_SFunction's raw English-unit "State"/"Aero" output vectors (see
% f22JSBSim_io.xml for the exact property order this function assumes) into the SI
% BodyStateBus/AirDataBus fields the rest of this SIL expects. Same conversions as
% c172pJSBSim's/c130JSBSim's own unpack*JSBSimOutputs.m (aircraft-independent physics -- see
% PLAN_JSBSIM_SFUNCTION_HYBRID_C172P.md §2 for the original derivation of each one); only the
% function/file name differs per PLAN_JSBSIM_SFUNCTION_F22_C130.md Phase C.
%
% jsbsimState order (19): lat-gc-deg, long-gc-deg, h-sl-ft, phi-rad, theta-rad, psi-rad, u-fps,
%   v-fps, w-fps, p-rad_sec, q-rad_sec, r-rad_sec, udot-ft_sec2, vdot-ft_sec2, wdot-ft_sec2,
%   pdot-rad_sec2, qdot-rad_sec2, rdot-rad_sec2, h-agl-ft
% jsbsimAero order (5): alpha-rad, beta-rad, u-aero-fps, v-aero-fps, w-aero-fps
ft2m = 0.3048;

lat_deg     = jsbsimState(1);
lon_deg     = jsbsimState(2);
h_sl_ft     = jsbsimState(3);
phi_rad     = jsbsimState(4);
theta_rad   = jsbsimState(5);
psi_rad     = jsbsimState(6);
u_fps       = jsbsimState(7);
v_fps       = jsbsimState(8);
w_fps       = jsbsimState(9);
p_radps     = jsbsimState(10);
q_radps     = jsbsimState(11);
r_radps     = jsbsimState(12);
udot_ftps2  = jsbsimState(13);
vdot_ftps2  = jsbsimState(14);
wdot_ftps2  = jsbsimState(15);
pdot_radps2 = jsbsimState(16);
qdot_radps2 = jsbsimState(17);
rdot_radps2 = jsbsimState(18);
h_agl_ft    = jsbsimState(19);

alpha_rad = jsbsimAero(1);
beta_rad  = jsbsimAero(2);
u_aero_fps = jsbsimAero(3);
v_aero_fps = jsbsimAero(4);
w_aero_fps = jsbsimAero(5);

% Nav (NED) -> body DCM, standard 3-2-1 Euler sequence (Aerospace Toolbox, codegen-compatible).
DCM_be = angle2dcm(psi_rad, theta_rad, phi_rad);

% Local flat-Earth tangent-plane position, anchored at this vehicle's own reference lat/lon (same
% point setUpVehicle.m's "f22jsbsim" case and f22JSBSimInit.xml use), with zDown absolute
% (-h_sl_ft*ft2m), matching every other vehicle's aircraftInitialPosInNED_m convention. Hand-coded
% spherical equirectangular approximation instead of Aerospace Toolbox's lla2flat -- lla2flat
% breaks Simulink's MATLAB Function block type inference in this MATLAB release (confirmed by
% isolated reproduction during c172pJSBSim's own Phase C, same bug class documented in
% optimAeroPX4SIL/CLAUDE.md section 3). At the sub-100km scale this SIL operates at, the
% spherical/WGS84 difference is negligible.
earthRadius_m = 6371000;
dLat_rad = (lat_deg - referenceLatitude_deg) * pi/180;
dLon_rad = (lon_deg - referenceLongitude_deg) * pi/180;
xNorth_m = dLat_rad * earthRadius_m;
yEast_m = dLon_rad * earthRadius_m * cos(referenceLatitude_deg * pi/180);
zDown_m = -h_sl_ft * ft2m;
aircraftPosInNED_m = [xNorth_m; yEast_m; zDown_m];

aircraftVelInBody_mps = [u_fps; v_fps; w_fps] * ft2m;
aircraftVelInNED_mps = DCM_be' * aircraftVelInBody_mps;

aircraftAngVelInBody_radps = [p_radps; q_radps; r_radps];
aircraftAngAccelInBody_radps2 = [pdot_radps2; qdot_radps2; rdot_radps2];

% JSBSim's accelerations/{u,v,w}dot-ft_sec2 are TOTAL body-axis acceleration at the CG (includes
% gravity's contribution via the equations of motion) -- i.e. the same "raw inertial acceleration"
% quantity every other vehicle's plant supplies for this field. `aircraftAccelbe_mps2` must stay
% the RAW acceleration here, NOT specific force: both ins.slx's "Three-axis Inertial Measurement
% Unit" block (aerolibnav) and ANELLO_X3_IMU_Vehicle_fcn_SIL.m already subtract gravity themselves
% (their own g_body/Gb input) to turn raw acceleration into specific force. Subtracting gravity a
% second time here would double-count it. This is also why no gravity_body term appears below --
% "raw inertial acceleration" (accelInertial_body) IS this function's contract, not specific force;
% subtracting gravity here would violate that contract and double-count it downstream, exactly the
% failure mode the comment above already warns about.
%
% CENTRIPETAL/CORIOLIS CORRECTION (2026-08-01, ported from the c130JSBSim fix -- see
% PLAN_JSBSIM_SFUNCTION_F22_C130.md section 14.1/15 for the full derivation and c130JSBSim's own
% confirmed-working live-test evidence): udot/vdot/wdot as JSBSim computes them (FGPropagate:
% vUVWdot = vForces/mass - vPQR x vUVW) are the component-wise derivative of the body velocity
% vector AS SEEN IN THE ROTATING BODY FRAME (dV/dt|_body) -- NOT the true inertial acceleration
% expressed in body axes. By the rotating-frame transport theorem (Stevens & Lewis "Aircraft
% Control and Simulation"):
%   accelInertial_body = dV/dt|_body + omega_body x velocity_body
% Convert velocity and its derivative to SI FIRST, then take the cross product (omega_body is
% already rad/s, dimensionless x 1/time, no conversion needed) -- this is a first-principles
% physics bug independent of any particular airframe, confirmed and fixed for c130JSBSim first;
% same missing term here. Applying it before attributing F-22's own pitch-runaway divergence to
% aero data (aero/coefficient/Cmalpha/Cmq, PLAN_JSBSIM_SFUNCTION_F22_C130.md section 13.6) --
% that divergence was isolated via Simulink's own JSBSim-groundtruth state (not an EKF/estimator
% artifact), so this fix is not expected to eliminate it by itself, but it changes what the
% simulated accelerometer feeds to PX4's own attitude/rate estimator during rotation, which can
% feed back into the real control response -- must be ruled out before concluding aero-table work
% is the only remaining lever.
omega_body_radps = [p_radps; q_radps; r_radps];
velocity_body_mps = [u_fps; v_fps; w_fps] * ft2m;
accelBodyFrame_mps2 = [udot_ftps2; vdot_ftps2; wdot_ftps2] * ft2m;
omegaCrossV_mps2 = [omega_body_radps(2)*velocity_body_mps(3) - omega_body_radps(3)*velocity_body_mps(2); ...
                    omega_body_radps(3)*velocity_body_mps(1) - omega_body_radps(1)*velocity_body_mps(3); ...
                    omega_body_radps(1)*velocity_body_mps(2) - omega_body_radps(2)*velocity_body_mps(1)];
aircraftAccelInBody_mps2 = accelBodyFrame_mps2 + omegaCrossV_mps2;
aircraftAccelbe_mps2 = aircraftAccelInBody_mps2;

aircraftEulerAngles_rad = [phi_rad; theta_rad; psi_rad];

% Airmass-relative body-axis velocity (wind-inclusive) for AirDataBus -- NOT the inertial u/v/w.
airspeedInBody_mps = [u_aero_fps; v_aero_fps; w_aero_fps] * ft2m;

% Threshold intentionally much higher than C130's/C172p's 0.1m convention -- f22JSBSimInit.xml's
% corrected IC altitude means F-22 sits ~5.5ft (1.68m) AGL at rest on its own extended gear (a real
% geometry fact, confirmed by a standalone plant settle test: 5.44-5.56ft), so a 0.1m threshold is
% ALWAYS true even parked, unarmed, on the runway. f22run13's groundtruth (vehicle_local_position_
% groundtruth) showed this directly: the one-way gearRetracted latch in packF22JSBSimInputs.m fired
% the instant simTime_s crossed 5s (isVehicleAirborne having been spuriously true from t=0), and the
% still-parked, still-disarmed aircraft free-fell at a clean 9.8 m/s^2 once its gear support was
% pulled out from under it. 15ft/4.572m is comfortably above the ~5.5ft at-rest height (with margin
% for gear-compression settling noise) while still being low enough to trip promptly once genuinely
% climbing out after a real takeoff.
isVehicleAirborne = (h_agl_ft * ft2m) > 4.572;
end
