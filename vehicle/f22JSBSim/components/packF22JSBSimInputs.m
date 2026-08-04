function jsbsimInputs = packF22JSBSimInputs(posAileron_rad, posElevator_rad, posRudder_rad, cmdThrottle_unit, simTime_s, isVehicleAirborne, controllerArmed)
%#codegen
% Re-normalizes actuatorsF22JSBSim.slx's achieved (rate-limited) surface positions back to
% JSBSim's fcs/*-cmd-norm convention, using the same real deflection limits as
% setUpActuators.m's "f22jsbsim" case (sourced from f22.xml <flight_control> aerosurface_scale
% ranges: aileron +-0.436 rad/24.98 deg, elevator +-0.5236 rad/30 deg, rudder +-0.5236 rad/30 deg
% -- confirmed by direct grep of aircraft/f22/f22.xml's "Lateral"/"Pitch" channels, see
% PLAN_JSBSIM_SFUNCTION_F22_C130.md Phase A). Throttle is broadcast to both F119-PW-100 engine
% indices -- PX4's MAV_TYPE 1 airframe has only one throttle channel, no independent per-engine
% control in this pass. Output order matches f22JSBSim_io.xml <input> order exactly: aileron,
% elevator, rudder, throttle (bare, engine 0), throttle[1] (engine 1), gear-cmd-norm,
% left-brake-cmd-norm, right-brake-cmd-norm -- do not reorder without updating that file.
%
% PARKING BRAKE / GROUND-ROLL INTERLOCK (added 2026-08-01, PLAN_JSBSIM_SFUNCTION_F22_C130.md
% section 17): f22run-guarded (session_gtfix1) found the plant rolling and accelerating
% completely unsupervised for the ENTIRE ~178s PX4 spends on EKF-alignment/arm retries before
% this vehicle's first "armed" event -- idle turbine thrust (~1300 lbf/engine once the engine-IC
% transient below settles) with no brake command ever wired and only rolling_friction=0.02 lets
% the aircraft reach 32+ m/s before PX4 ever takes control, which is what actually corrupted the
% land-detector/TECS/runway-takeoff state machine downstream (not a sensor bug, not a JSBSim aero
% bug). Fix: hold full parking brake (both fcs/left-brake-cmd-norm and fcs/right-brake-cmd-norm
% at 1.0) AND force commanded throttle to 0 (a second, independent interlock -- this does not
% eliminate turbine idle thrust by itself, brakes carry that load) whenever controllerArmed is
% false. controllerArmed is threaded in from PX4 Interface's own existing (previously
% subsystem-local) "controllerArmed" Goto tag, now exposed as a real port -- see
% VehicleSilSimulation.slx/PX4 Interface and the plant/vehicleSimulation port chain.
%
% RELEASE CONDITION: PX4 Interface does not expose vehicle_status.nav_state or
% vehicle_attitude_setpoint over the HIL link this SIL uses (pixhawk_sil_connector is a
% source-unavailable binary S-Function whose output vector carries mapped servo/actuator values,
% not internal PX4 topics) -- true "active mode is takeoff/mission" and "valid takeoff attitude
% setpoint exists" checks from the user's original spec are therefore not directly observable in
% Simulink. Pragmatic, documented substitute: release once controllerArmed is true AND commanded
% throttle (BEFORE this function's own zero-while-disarmed override, i.e. the raw
% cmdThrottle_unit input) exceeds a small threshold -- PX4 only commands real throttle once it is
% actually trying to taxi/take off, so this approximates "a real takeoff/mission setpoint is
% driving the vehicle" using signals that actually exist here. Debounced 0.75s (persistent sample
% counter at this model's Ts, matches stepSize_s=0.004s/250Hz) before brakes actually release, per
% the user's 0.5-1s debounce request.
releaseThrottleThreshold_unit = 0.05;
debounceTime_s = 0.75;
Ts_s = 0.004;
debounceSamples = round(debounceTime_s / Ts_s);

% ONE-WAY LATCH (added after session_gtfix3, PLAN_JSBSIM_SFUNCTION_F22_C130.md section 18):
% gtfix3's LateralDebug log caught the exact mechanism -- without a latch, brakesReleased was
% recomputed fresh every step from releaseConditionCounter, which resets to 0 on the very next
% sample where cmdThrottle_unit dips at/below the threshold (release requires 0.75s sustained,
% but re-engage was a single-sample trip, no debounce in that direction). PX4 cut throttle to
% ~0 right at rotation (t=25.0-25.27s), which snapped full parking brake back on
% (fcs/left-brake-cmd-norm=fcs/right-brake-cmd-norm=1) for t=25.268-27.936s -- spanning the exact
% moment the aircraft actually left the ground (altitude climbing from t~26s onward, gear still
% commanded down). Reapplying full brake torque during that ground/air transition, when left/right
% main gear compression is momentarily asymmetric, injected a differential drag moment that seeded
% the roll/yaw divergence observed starting t=27.0-27.5s (fcs/aileron-cmd-norm and moments/l-aero-
% lbsft both diverging sharply, well before gear/gear-pos-norm even started retracting at t=28.0).
% Plant-only sign test (f22_lateral_probe.xml) had already cleared aileron/rudder sign and
% aero-derivative defects, and gear-pos-norm stayed at 1.000 through the divergence onset in both
% this run and the earlier session_gtfix2, ruling out the gear-retraction kinematic snap itself as
% the trigger -- it's specifically the brake re-engagement. Fix: once brakes have legitimately
% released (throttle sustained above threshold for the full debounce while armed), never re-engage
% for the rest of the flight -- same one-way latch pattern as gearRetracted below. A real parking
% brake wouldn't be stomped back on mid-rotation just because throttle blipped low for a moment.
persistent releaseConditionCounter brakesEverReleased
if isempty(releaseConditionCounter)
    releaseConditionCounter = int32(0);
end
if isempty(brakesEverReleased)
    brakesEverReleased = false;
end
releaseConditionNow = controllerArmed && (cmdThrottle_unit > releaseThrottleThreshold_unit);
if releaseConditionNow
    releaseConditionCounter = min(releaseConditionCounter + 1, debounceSamples);
else
    releaseConditionCounter = int32(0);
end
if ~brakesEverReleased && releaseConditionCounter >= debounceSamples
    brakesEverReleased = true;
end
brakesReleased = brakesEverReleased;

if ~controllerArmed
    cmdThrottle_unit = 0;
end
%
% gear-cmd-norm retracts once BOTH isVehicleAirborne (Memory-delayed h-agl-ft feedback from
% unpackF22JSBSimOutputs.m) is true AND simTime_s (Clock feedback) exceeds a 5s safety floor -- see
% PLAN_JSBSIM_SFUNCTION_F22_C130.md section 12 for the full history of why this specific condition
% was chosen; short version: f22.xml's "Pitch" channel has a "fcs/gear-down-q-limit" switch that
% caps commanded pitch-rate authority to 30% of normal while gear/gear-pos-norm > 0, and this
% airframe is documented as "10% unstable at subsonic" -- 30% authority alone was confirmed
% (f22run9: softened PX4 demand gains, gear permanently down, NO transition at all) to be
% insufficient for genuine STABILITY, not merely a demand/tracking mismatch -- the aircraft
% diverged into the same uncontrolled nose-up pitch runaway regardless of how gently PX4 asked for
% pitch rate. This rules out "PX4 demanding too much" as the sole cause and means gear MUST
% eventually retract to reach the full authority this airframe's own FCS was designed around.
% Earlier attempts at retraction alone (f22run6/f22run7/f22run8) each hit a different problem
% (spurious premature retraction at t=0; retraction while still parked/unarmed; a violent
% integrator-windup-release snap at the 30%->100% step transition after 22+ seconds of
% authority-limited flight) -- this version combines dynamic retraction with the SAME softened
% FW_P_RMAX_POS/FW_T_CLMB_R_SP/FW_P_LIM_MAX demand gains from the f22run9 attempt, so that
% whatever time is spent in the 30%-limited regime before liftoff accumulates much less
% integrator windup than the original unsoftened gains did, minimizing the severity of the
% eventual authority-restoration transition rather than eliminating the need for it.
%
% gearRetracted LATCH (added after f22run10): isVehicleAirborne is a raw instantaneous h-agl-ft>0.1m
% threshold (see unpackF22JSBSimOutputs.m), only delayed by a single step through the Memory block --
% NOT debounced. f22run10's groundtruth (vehicle_local_position_groundtruth in the resulting .ulg)
% showed the aircraft sitting still until simTime_s crossed 5s, then bouncing violently (z
% oscillating ~0.7-1.6m, vz swinging +-3 m/s) for ~17s before finally launching uncontrolled -- the
% bounce onset lines up exactly with the 5s floor arming the retract condition. Root cause: with
% zero gear-traverse-time (the same C130 lesson applied proactively in f22.xml), any ground-contact
% bounce that nudges h-agl-ft back and forth across the 0.1m threshold makes isVehicleAirborne (and
% therefore gearCmd_norm) chatter every step; gear position then snaps instantly on every chatter,
% injecting a contact-force impulse each time -- a resonant feedback loop that eventually threw the
% aircraft airborne rather than letting it settle. Fix: gear retraction is now a one-way persistent
% latch -- once the airborne+5s condition is true for a single step, gearRetracted stays true.
% A separate landing-extension latch below may subsequently command the gear down, but never lets
% it chatter back up after touchdown. This eliminates the takeoff chatter without
% reintroducing either of the f22run6/f22run7 premature-retraction regressions, since the latch only
% ever transitions 0->1, never back.
maxAilDefl_rad = 0.436;
maxElevatorDefl_rad = 0.5236;
maxRudderDefl_rad = 0.5236;

aileronCmd_norm = max(min(posAileron_rad / maxAilDefl_rad, 1), -1);
elevatorCmd_norm = max(min(posElevator_rad / maxElevatorDefl_rad, 1), -1);
rudderCmd_norm = max(min(posRudder_rad / maxRudderDefl_rad, 1), -1);
throttleCmd_norm = max(min(cmdThrottle_unit, 1), 0);

persistent gearRetracted gearExtendedForLanding
if isempty(gearRetracted)
    gearRetracted = false;
end
if isempty(gearExtendedForLanding)
    gearExtendedForLanding = false;
end
if ~gearRetracted && isVehicleAirborne && simTime_s > 5
    gearRetracted = true;
end
% Once a real flight has retracted the gear, the next low-AGL/contact
% indication is the only unambiguous landing cue available through this SIL
% interface. Latch the gear down at that point. The F-22 JSBSim gear has
% zero traverse time, so restoring contact geometry on this first sample
% prevents the aircraft from falling through the terrain during RTL. The
% latch also prevents the old gear chatter if the aircraft bounces.
if gearRetracted && ~gearExtendedForLanding && ~isVehicleAirborne
    gearExtendedForLanding = true;
end
gearCmd_norm = double(~gearRetracted || gearExtendedForLanding);

% Reapply symmetric wheel braking only after the landing-extension latch has
% observed the first post-flight ground contact.  brakesEverReleased must
% remain one-way during takeoff (see above), but leaving both brakes at zero
% forever made the F-22 roll/bounce beyond the 1000 s mission horizon after a
% valid NAV_LAND touchdown.  The landing latch is unavailable until the gear
% has first retracted in real flight, so this cannot reintroduce parking-brake
% chatter during rotation.  Symmetric commands also avoid a yaw impulse.
landingBrakeApplied = gearExtendedForLanding;
if ~brakesReleased || landingBrakeApplied
    leftBrakeCmd_norm = 1;
    rightBrakeCmd_norm = 1;
else
    leftBrakeCmd_norm = 0;
    rightBrakeCmd_norm = 0;
end

jsbsimInputs = [aileronCmd_norm; elevatorCmd_norm; rudderCmd_norm; ...
                throttleCmd_norm; throttleCmd_norm; ...
                gearCmd_norm; leftBrakeCmd_norm; rightBrakeCmd_norm];
end
