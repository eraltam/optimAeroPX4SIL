function [aircraftVelInNED_mps, aircraftPosInNED_m, aircraftEulerAngles_rad, ...
          aircraftVelInBody_mps, aircraftAngVelInBody_radps, aircraftAngAccelInBody_radps2, ...
          aircraftAccelInBody_mps2, DCM_be, aircraftAccelbe_mps2, ...
          airspeedInBody_mps, alpha_rad, beta_rad, isVehicleAirborne] = ...
          unpackJSBSimOutputs(jsbsimState, jsbsimAero, gravityScalar_mps2, ...
          referenceLatitude_deg, referenceLongitude_deg) %#codegen
% Converts JSBSim_SFunction's raw English-unit "State"/"Aero" output vectors (see
% aircraft/c172p/c172pJSBSim_io.xml for the exact property order this function assumes) into the SI
% BodyStateBus/AirDataBus fields the rest of this SIL expects. See
% PLAN_JSBSIM_SFUNCTION_HYBRID_C172P.md Phase B/C for why each conversion was chosen.
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
% point setUpVehicle.m's "c172pJSBSim" case and c172pJSBSimInit.xml use), with zDown absolute
% (-h_sl_ft*ft2m), matching every other vehicle's aircraftInitialPosInNED_m convention (NED origin
% at sea level / height 0, not at the reference altitude). Hand-coded spherical equirectangular
% approximation instead of Aerospace Toolbox's lla2flat -- lla2flat breaks Simulink's MATLAB
% Function block type inference in this MATLAB release (confirmed by isolated reproduction; same
% bug class as the one already documented in optimAeroPX4SIL/CLAUDE.md section 3 for a different
% toolbox call). At the sub-100km scale this SIL operates at, the spherical/WGS84 difference is
% negligible and this SIL has no other geodetic-accuracy requirement anywhere else.
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

% Both INS implementations form specific force downstream as f = Ab - g_body. JSBSim's
% accelerations/{u,v,w}dot are derivatives in the rotating body frame, so convert them to inertial
% acceleration expressed in body axes by adding omega x V. Do not add or subtract gravity here:
% a stationary ground run proves that this transport-corrected value is approximately zero, after
% which the INS correctly reports specific force ~= -g. Adding gravity here produced ~0 g at rest;
% omitting the transport correction in a stale embedded chart produced ~-2 g. This matches the
% validated reliability implementation in commit 05badb3.
%
% udot/vdot/wdot alone is dV/dt as seen in the ROTATING BODY FRAME (JSBSim's FGPropagate: vUVWdot =
% vForces/mass - vPQR x vUVW), not the true inertial acceleration expressed in body axes. By the
% rotating-frame transport theorem, a_inertial_body = dV/dt|_body + omega x V. This term is exactly
% zero in straight, non-rotating flight (why this was never caught by earlier c172pJSBSim
% validation) and grows with p/q/r otherwise -- including during ground handling, where idle thrust
% with no parking-brake model can produce nonzero body rates while still disarmed. Same fix already
% applied to unpackC130JSBSimOutputs.m/unpackF22JSBSimOutputs.m (PLAN_JSBSIM_SFUNCTION_F22_C130.md
% section 14.1/15); c172p was flagged there as still carrying this bug and not yet fixed.
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

isVehicleAirborne = (h_agl_ft * ft2m) > 0.1;
end
