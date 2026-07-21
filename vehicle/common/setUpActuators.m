% This script sets up the actuator parameters for the selected vehicle. This includes
% parameters that define the motors, control surfaces, engines, etc.
%
% NOTE: switches on vehicleParams.type (the specific plant/vehicle key), not
% vehicleParams.baseVehicleType -- see the matching note in vehicle/common/setUpVehicle.m and
% PLAN_CORRECCION_MULTIVEHICULO_SITL.md (Fase 1 / F1).
actuatorDelay_s = 0.005;
plantSampleTime_s = 0.01;
parameters.engine.minThrottle = 0.01; %idle happens above this value

setupVehicleType = vehicleParams.type;

switch lower(setupVehicleType)
    case "f16"
        % Max deflection of control surfaces
        maxAilDefl_deg = 20;
        maxRudderDefl_deg = 30;
        maxElevatorDefl_deg = 25;
        elevatorDeflRateLimit_degps = 60;
        aileronDeflRateLimit_degps = 80;
        rudderDeflRateLimit_degps = 120;
        tauElevator_s = 0.0495;
        tauAilerons_s = 0.0495;
        tauRudder_s = 0.0495;
        F16EngineData

    case "hexarotor"
        % Engine based on U15II KV100 T-Motor
        vehicleParams.rotor.diameter_m  = 1.016; % 40 inch rotor
        vehicleParams.rotor.maxRPM      = 3473;
        vehicleParams.rotor.cmdToRpmData = [0 vehicleParams.rotor.maxRPM];
        vehicleParams.rotor.cmdToRpmBkpts_nd = [0 1];
        vehicleParams.rotor.kct_nd =  0.002455798004676;
        vehicleParams.rotor.kcp_nd = 1.530217019534773e-04; 
        % Mimic PX4 spin direction and motor number for hexarotor
        % NOTE: +X-Axis is out of the nose, +Y is out of starboard side, +Z is toward ground
        vehicleParams.rotor.motorLocs = [ 0.0000    1.0000 0;
                                          0.0000   -1.0000 0;
                                          0.8660   -0.5000 0;
                                         -0.8660    0.5000 0;
                                          0.8660    0.5000 0;
                                         -0.8660   -0.5000 0];
        vehicleParams.rotor.spinDirectionCW_isTrue = [1 0 1 0 0 1]; 

        % Using a generic 2nd order transfer function for an ESC and brushless motor
        vehicleParams.actuator.tfNumerator = 250000;
        vehicleParams.actuator.tfDenominator = [1 700 250000];

    case "quadrotor"
        % Same motor/prop/ESC coefficients as hexarotor (generic T-Motor-class characteristics, not
        % mass-specific) -- only the rotor count and geometry differ. X-quad layout matching
        % PX4-Autopilot/.../airframes/10022_optimAeroQuad (CA_ROTORn_PX/PY/KM), arm length 1.0 m.
        % See setUpVehicle.m's "quadrotor" case for the full rationale.
        vehicleParams.rotor.diameter_m  = 1.016;
        vehicleParams.rotor.maxRPM      = 3473;
        vehicleParams.rotor.cmdToRpmData = [0 vehicleParams.rotor.maxRPM];
        vehicleParams.rotor.cmdToRpmBkpts_nd = [0 1];
        vehicleParams.rotor.kct_nd =  0.002455798004676;
        vehicleParams.rotor.kcp_nd = 1.530217019534773e-04;
        vehicleParams.rotor.motorLocs = [ 0.7071  0.7071 0;
                                          -0.7071 -0.7071 0;
                                           0.7071 -0.7071 0;
                                          -0.7071  0.7071 0];
        vehicleParams.rotor.spinDirectionCW_isTrue = [1 1 0 0];

        vehicleParams.actuator.tfNumerator = 250000;
        vehicleParams.actuator.tfDenominator = [1 700 250000];

    case "octarotor"
        % Same motor/prop/ESC coefficients as hexarotor. Flat octocopter-X layout (8 rotors at 45
        % deg spacing, alternating spin direction, arm length 1.0 m matching hex's scale) -- our own
        % explicit choice since PX4-Autopilot/.../airframes/10024_optimAeroOcta does not specify
        % CA_ROTORn_PX/PY. See setUpVehicle.m's "octarotor" case for the full rationale.
        vehicleParams.rotor.diameter_m  = 1.016;
        vehicleParams.rotor.maxRPM      = 3473;
        vehicleParams.rotor.cmdToRpmData = [0 vehicleParams.rotor.maxRPM];
        vehicleParams.rotor.cmdToRpmBkpts_nd = [0 1];
        vehicleParams.rotor.kct_nd =  0.002455798004676;
        vehicleParams.rotor.kcp_nd = 1.530217019534773e-04;
        vehicleParams.rotor.motorLocs = [ 1.0000  0.0000 0;
                                           0.7071  0.7071 0;
                                           0.0000  1.0000 0;
                                          -0.7071  0.7071 0;
                                          -1.0000  0.0000 0;
                                          -0.7071 -0.7071 0;
                                           0.0000 -1.0000 0;
                                           0.7071 -0.7071 0];
        vehicleParams.rotor.spinDirectionCW_isTrue = [1 0 1 0 1 0 1 0];

        vehicleParams.actuator.tfNumerator = 250000;
        vehicleParams.actuator.tfDenominator = [1 700 250000];

    case "evtol"
        % Same motor/prop/ESC coefficients and rotor geometry as hexarotor -- PX4-Autopilot/.../
        % airframes/10025_optimAeroEVTOL declares CA_ROTOR_COUNT=6 with no distinct CA_ROTORn_PX/PY,
        % and its PWM_MAIN_FUNC1-6 assignment only wires a plain 6-motor mixer (no tilt/pusher
        % channel) -- see setUpVehicle.m's "evtol" case for the full rationale on why true tiltrotor
        % physics is out of scope at this fidelity level.
        vehicleParams.rotor.diameter_m  = 1.016;
        vehicleParams.rotor.maxRPM      = 3473;
        vehicleParams.rotor.cmdToRpmData = [0 vehicleParams.rotor.maxRPM];
        vehicleParams.rotor.cmdToRpmBkpts_nd = [0 1];
        vehicleParams.rotor.kct_nd =  0.002455798004676;
        vehicleParams.rotor.kcp_nd = 1.530217019534773e-04;
        vehicleParams.rotor.motorLocs = [ 0.0000    1.0000 0;
                                          0.0000   -1.0000 0;
                                          0.8660   -0.5000 0;
                                         -0.8660    0.5000 0;
                                          0.8660    0.5000 0;
                                         -0.8660   -0.5000 0];
        vehicleParams.rotor.spinDirectionCW_isTrue = [1 0 1 0 0 1];

        vehicleParams.actuator.tfNumerator = 250000;
        vehicleParams.actuator.tfDenominator = [1 700 250000];

    case "evtol_simscape"
        % Same rotor geometry/motor coefficients as "evtol" -- see setUpVehicle.m's
        % "evtol_simscape" case for the full rationale.
        vehicleParams.rotor.diameter_m  = 1.016;
        vehicleParams.rotor.maxRPM      = 3473;
        vehicleParams.rotor.cmdToRpmData = [0 vehicleParams.rotor.maxRPM];
        vehicleParams.rotor.cmdToRpmBkpts_nd = [0 1];
        vehicleParams.rotor.kct_nd =  0.002455798004676;
        vehicleParams.rotor.kcp_nd = 1.530217019534773e-04;
        vehicleParams.rotor.motorLocs = [ 0.0000    1.0000 0;
                                          0.0000   -1.0000 0;
                                          0.8660   -0.5000 0;
                                         -0.8660    0.5000 0;
                                          0.8660    0.5000 0;
                                         -0.8660   -0.5000 0];
        vehicleParams.rotor.spinDirectionCW_isTrue = [1 0 1 0 0 1];

        vehicleParams.actuator.tfNumerator = 250000;
        vehicleParams.actuator.tfDenominator = [1 700 250000];

    case "vtol_tailsitter"
        % Twin-rotor tailsitter (Fase aerial track, see PLAN_VEHICULOS_AEREOS_Y_MARINOS_SITL.md).
        % Reuses hexarotor's exact rigid-body-with-N-rotors engine (same rationale as
        % quadrotor/octarotor/evtol above) -- only rotor count/geometry/coefficients differ. Prop
        % "7x4SF" (7 in diameter) and twin-motor layout from
        % LADAC-Examples/modules/ladac-examples-data/LADAC_params/Arkbird/tailsitter_params_Arkbird.m
        % (propPos_c, propDir). kct_nd/kcp_nd are an order-of-magnitude fit (not vendor-validated) to
        % produce roughly 1.5x this vehicle's own weight (1.1 kg) in combined static thrust at
        % maxRPM -- same methodology as the hexarotor/fixedwing_plane rotor coefficients, scaled
        % down for this much smaller/lighter airframe. This models only the twin-rotor HOVER
        % regime -- true tailsitter wing-borne cruise/transition aerodynamics is explicitly out of
        % scope at this fidelity level (same documented gap as evtol's tiltrotor note).
        vehicleParams.rotor.diameter_m  = 7 * 0.0254;
        vehicleParams.rotor.maxRPM      = 15000;
        vehicleParams.rotor.cmdToRpmData = [0 vehicleParams.rotor.maxRPM];
        vehicleParams.rotor.cmdToRpmBkpts_nd = [0 1];
        vehicleParams.rotor.kct_nd = 3.28e-6;
        vehicleParams.rotor.kcp_nd = 2.03e-7;
        vehicleParams.rotor.motorLocs = [-0.115 -0.165 0;
                                          -0.115  0.165 0];
        vehicleParams.rotor.spinDirectionCW_isTrue = [1 0];

        vehicleParams.actuator.tfNumerator = 250000;
        vehicleParams.actuator.tfDenominator = [1 700 250000];

    case "ship_semisub"
        % Semisubmersible platform (MSS/CRAFT/SHIP/models/semisubModels/data_rig.m) -- see
        % PLAN_VEHICULOS_AEREOS_Y_MARINOS_SITL.md. Real vehicle is dynamic-positioning-capable
        % (independent surge/sway/yaw thrust via azimuth thrusters) -- NOT modeled here as a true
        % holonomic 3-DOF vehicle because PX4's rover/boat control modes (GND_*) only ever
        % generate steer+throttle-style commands, never an independent sway command, so a real
        % holonomic actuation scheme could never actually be driven by this SIL's own PX4 side.
        % Reuses ship_surface's exact Nomoto-class 2-command architecture instead, with the
        % platform's own REAL surge/yaw time constants from data_rig.m (T_x=100s, T_n=80s) used
        % directly as tauThrottle_s/nomotoT (T_y=200s, the sway response, has no equivalent in
        % this 2-DOF representation and is simply not used).
        vehicleParams.tauSteer_s = 5.0;
        vehicleParams.tauThrottle_s = 100.0;
        vehicleParams.nomotoK_radps_per_rad = 0.0059;
        vehicleParams.nomotoT_s = 80.0;

    case "ship_surface"
        % Nomoto-class rudder+propeller ship (subactuated, single rudder/single shaft) -- see
        % PLAN_VEHICULOS_AEREOS_Y_MARINOS_SITL.md. tauSteer_s/tauThrottle_s here are steering-gear
        % and engine-spool lag time constants (documentation/reference only -- the
        % NomotoShipKinematics MATLAB Function block inside ship_surface.slx inlines these same
        % values as local constants, same rationale as ackermann_rover's own case). Nomoto K/T
        % (yaw-rate gain/time-constant) are standard order-of-magnitude approximations
        % (T ~ 2*L/V, K ~ 0.5*V/L) for a 175 m ship at ~8 m/s service speed (see
        % vehicle/common/setUpVehicle.m's "ship_surface" case for L/V sourcing) -- not measured
        % from container.m's own hydrodynamic derivatives, which would require porting its full
        % nonlinear MMG model (out of scope at this fidelity level).
        vehicleParams.tauSteer_s = 3.0;
        vehicleParams.tauThrottle_s = 30.0;
        vehicleParams.nomotoK_radps_per_rad = 0.0229;
        vehicleParams.nomotoT_s = 43.75;

    case "uuv_dsrv"
        % Left/right thruster lag time constant -- same differential-thrust kinematic
        % simplification as uuv_subsea/uuv_npsauv. Real DSRV (Deep Submergence Rescue Vehicle,
        % MSS/CRAFT/AUV/models/DSRV.m) is actually a depth/pitch-control vehicle (single stern
        % plane, near-constant forward speed) -- horizontal yaw steering via differential thrust
        % is NOT how the real vehicle is controlled, an even larger fidelity gap than
        % uuv_subsea/uuv_npsauv's own remus100/npsauv simplifications. Documented explicitly, not
        % silently assumed -- see PLAN_VEHICULOS_AEREOS_Y_MARINOS_SITL.md.
        vehicleParams.tauMotor_s = 2.5;

    case "uuv_npsauv"
        % Left/right thruster lag time constant -- same differential-thrust kinematic
        % simplification already used by uuv_subsea (real single-prop+control-surfaces npsauv
        % design is not modeled at this fidelity level, same class of gap as uuv_subsea's own
        % remus100-vs-differential-thrust simplification) -- see
        % PLAN_VEHICULOS_AEREOS_Y_MARINOS_SITL.md.
        vehicleParams.tauMotor_s = 2.0;

    case "ackermann_rover"
        % Steering servo / drive motor lag time constants for the kinematic-bicycle plant (Fase 3).
        % Documentation/reference only -- the AckermannKinematics MATLAB Function block inside
        % ackermann_rover.slx inlines these same values as local constants rather than reading this
        % struct field, to avoid the MATLAB Function + struct compile issue documented in
        % optimAeroPX4SIL/CLAUDE.md section 3. Keep the two in sync by hand if changed.
        vehicleParams.tauSteer_s = 0.15;
        vehicleParams.tauThrottle_s = 0.5;

    case "ackermann_simscape"
        % Same time constants as ackermann_rover -- see setUpVehicle.m's "ackermann_simscape" case
        % for the full rationale (same car, pending future Simscape tire/suspension swap-in).
        vehicleParams.tauSteer_s = 0.15;
        vehicleParams.tauThrottle_s = 0.5;

    case "differential_rover"
        % Left/right drive motor lag time constant for the differential-drive kinematic plant
        % (Fase 3). Documentation/reference only -- same rationale as ackermann_rover above.
        vehicleParams.tauMotor_s = 0.4;

    case "tracked_vehicle"
        % Left/right track motor lag time constant -- see setUpVehicle.m's "tracked_vehicle" case
        % for the full rationale. Slower than differential_rover (heavier vehicle).
        vehicleParams.tauMotor_s = 0.6;

    case "tracked_vehicle_simscape"
        % Same time constant as tracked_vehicle -- see setUpVehicle.m's "tracked_vehicle_simscape"
        % case.
        vehicleParams.tauMotor_s = 0.6;

    case "usv_surface"
        % Left/right thruster lag time constant -- see setUpVehicle.m's "usv_surface" case.
        vehicleParams.tauMotor_s = 1.0;

    case "uuv_subsea"
        % Left/right thruster lag time constant -- see setUpVehicle.m's "uuv_subsea" case (slower
        % than usv_surface due to greater added-mass/drag when fully submerged).
        vehicleParams.tauMotor_s = 1.5;

    case "bicycle_rover"
        % Steering servo / drive motor lag time constants for the kinematic-bicycle plant (Fase 3).
        % Documentation/reference only -- same rationale as ackermann_rover above.
        vehicleParams.tauSteer_s = 0.15;
        vehicleParams.tauThrottle_s = 0.4;

    case "wheel_loader"
        % Steering / drive motor lag time constants for the kinematic-bicycle plant (Fase 3).
        % Heavier machine, slower actuators than the small rovers. Documentation/reference only --
        % same rationale as ackermann_rover above.
        vehicleParams.tauSteer_s = 0.4;
        vehicleParams.tauThrottle_s = 0.8;

    case "wheel_loader_simscape"
        % Same time constants as wheel_loader -- see setUpVehicle.m's "wheel_loader_simscape" case.
        vehicleParams.tauSteer_s = 0.4;
        vehicleParams.tauThrottle_s = 0.8;

    case "unicycle_rover"
        % Left/right drive motor lag time constant for the differential-drive kinematic plant
        % (Fase 3). Documentation/reference only -- same rationale as differential_rover.
        vehicleParams.tauMotor_s = 0.3;

    case "fixedwing_plane"
        % Generic light ala-fija (RC-class) actuator dynamics -- see
        % PLAN_VEHICULOS_AEREOS_Y_MARINOS_SITL.md. actuators.slx is a direct clone of F16's
        % actuators.slx (same servoAileron/servoElevator/servoRudder Model References into
        % servo.slx, same Rate Limiter + failureInjection topology), so this case defines the
        % exact same workspace variable names as the "f16" case above, just with real numbers
        % sourced from LADAC-Examples/Plane/Funray (KST DS215MG HV servos:
        % actuatorsPt2_params_Funray.m naturalFrequency=200 rad/s, dampingRatio=0.866,
        % deflectionRateMax=60deg/0.08s=750 deg/s, delay=0.0075s; per-surface limits from
        % airplane_params_Funray.m: aileron +-28deg, elevator +-21deg, rudder +-27deg).
        % servo.slx's TransferFcn is a first-order lag (not a true 2nd-order natural-frequency/
        % damping model), so tau below approximates Funray's real 2nd-order servo response time
        % (~1/naturalFrequency plus the 7.5ms transport delay) rather than replicating it exactly.
        maxAilDefl_deg = 28;
        maxRudderDefl_deg = 27;
        maxElevatorDefl_deg = 21;
        elevatorDeflRateLimit_degps = 750;
        aileronDeflRateLimit_degps = 750;
        rudderDeflRateLimit_degps = 750;
        tauElevator_s = 0.02;
        tauAilerons_s = 0.02;
        tauRudder_s = 0.02;

        % Single tractor propeller + electric motor (enginePropFixedwing.slx) -- same
        % thrust-coefficient style as the multirotor rotor.kct_nd/kcp_nd fields above, but for one
        % forward-facing prop instead of N lifting rotors. 9x6 prop class (9 in diameter, matching
        % LADAC-Examples/Plane/EasyGlider's own '9x6' propeller choice for this aircraft weight
        % class -- see params_propeller_map_based_EasyGlider.m). kct_nd/kcp_nd are an
        % order-of-magnitude fit (not vendor-validated) chosen so static thrust at maxRPM is about
        % 1.5x the aircraft's own weight (2.15 kg Funray-class airframe) -- see setUpVehicle.m's
        % "fixedwing_plane" case for the airframe mass.
        vehicleParams.prop.diameter_m = 9 * 0.0254;
        vehicleParams.prop.maxRPM = 9000;
        vehicleParams.prop.cmdToRpmData = [0 vehicleParams.prop.maxRPM];
        vehicleParams.prop.cmdToRpmBkpts_nd = [0 1];
        vehicleParams.prop.kct_nd = 3.6e-5;
        vehicleParams.prop.kcp_nd = 2.24e-6;

    case "c172p"
        % Cessna 172P (JSBSim reference aircraft) -- see PLAN_INCORPORACION_AERONAVES_JSBSIM_SITL.md
        % Fase A-C. actuatorsC172p.slx is a direct clone of F16's/fixedwing_plane's actuators.slx
        % (same servo/rate-limiter/failure-injection topology), so this case defines the exact same
        % workspace variable names as the "f16"/"fixedwing_plane" cases above.
        %
        % Max deflections from c172p.xml <flight_control> aerosurface_scale ranges (real JSBSim
        % FCS data, not an estimate): elevator -28/+23 deg, aileron -20/+15 deg, rudder -16/+16
        % deg -- this switch only stores one symmetric magnitude per surface (same convention as
        % F16/fixedwing_plane), so the larger of the two bounds is used.
        maxElevatorDefl_deg = 28;
        maxAilDefl_deg = 20;
        maxRudderDefl_deg = 16;
        % JSBSim's own FCS models these surfaces as ideal/instantaneous (no rate limit or lag given
        % in c172p.xml -- ailerons/elevator/rudder are driven directly by fcs/*-cmd-norm with no
        % actuator dynamics block). Since this SIL flies the aircraft via PX4-commanded autopilot
        % servos (not a human pilot on cables), a rate limit/lag IS needed -- these are an explicit
        % engineering estimate for GA-class autopilot servos (slower/higher-torque than the tiny
        % RC-class servos fixedwing_plane uses), not sourced from c172p.xml.
        elevatorDeflRateLimit_degps = 60;
        aileronDeflRateLimit_degps = 60;
        rudderDeflRateLimit_degps = 60;
        tauElevator_s = 0.1;
        tauAilerons_s = 0.1;
        tauRudder_s = 0.1;

        % Single tractor propeller + real Lycoming IO-320 piston engine
        % (enginePistonPropC172p.slx) -- unlike fixedwing_plane's enginePropFixedwing.slx (a
        % constant-kct_nd/kcp_nd electric-motor fit), this engine model uses the REAL fixed-pitch
        % propeller thrust/power coefficient tables from jsbsim/engine/prop_75in2f.xml (C_THRUST/
        % C_POWER vs advance ratio J), evaluated each step against the vehicle's actual airspeed --
        % see plan §2.3/build_enginePistonPropC172p.m. Values below are documentation/reference
        % only, same convention as fixedwing_plane's own case -- the MATLAB Function chart inside
        % enginePistonPropC172p.slx inlines these same constants as local literals (see
        % optimAeroPX4SIL/CLAUDE.md section 3 on why MATLAB Function blocks in this project read
        % inlined literals, not vehicleParams struct fields). Keep the two copies in sync by hand.
        vehicleParams.prop.diameter_m = 75 * 0.0254;      % jsbsim/engine/prop_75in2f.xml <diameter>
        vehicleParams.prop.maxRPM = 2700;                 % jsbsim/engine/eng_io320.xml <maxrpm>
        vehicleParams.prop.idleRPM = 550;                 % jsbsim/engine/eng_io320.xml <idlerpm>
        vehicleParams.prop.maxPower_W = 160 * 745.7;       % jsbsim/engine/eng_io320.xml <maxhp>=160
        vehicleParams.prop.tauEngine_s = 1.0;              % piston spool-up estimate, slower than
                                                            % fixedwing_plane's electric motor (0.15s)
        % C_THRUST(J)/C_POWER(J) tables, copied verbatim from the ACTIVE (non-commented)
        % <table name="C_THRUST"/"C_POWER"> in jsbsim/engine/prop_75in2f.xml. NOTE: the two real
        % JSBSim tables do NOT share the same J breakpoint set (C_THRUST stops at J=2.3, C_POWER at
        % J=2.4, both before the final constant-extrapolation point at J=5.0) -- kept as two
        % independent breakpoint arrays rather than forcing them to match, to avoid silently
        % dropping/misaligning a real data point.
        vehicleParams.prop.J_bkpts_CT_nd = [0.0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2 1.3 1.4 1.5 1.6 1.7 1.8 1.9 2.0 2.1 2.2 2.3 5.0];
        vehicleParams.prop.CT_data       = [0.073 0.073 0.072 0.071 0.069 0.066 0.062 0.055 0.045 0.034 0.024 0.013 -0.006 -0.013 -0.024 -0.034 -0.045 -0.055 -0.062 -0.066 -0.069 -0.071 -0.072 -0.073 -0.073];
        vehicleParams.prop.J_bkpts_CP_nd = [0.0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2 1.3 1.4 1.5 1.6 1.7 1.8 1.9 2.0 2.1 2.2 2.3 2.4 5.0];
        vehicleParams.prop.CP_data       = [0.0660 0.0700 0.0700 0.0660 0.0600 0.0530 0.0501 0.0469 0.0426 0.0360 0.0282 0.0191 0.0155 0.0191 0.0282 0.0360 0.0426 0.0469 0.0501 0.0516 0.0525 0.0525 0.0522 0.0511 0.0504 0.0493];

    otherwise
        error("setUpActuators:NoActuatorModelDefined", char("No real actuator model (motor/ESC/" + ...
            "control-surface dynamics) defined yet for vehicleParams.type='" + setupVehicleType + ...
            "'. This vehicle previously compiled silently using the hexarotor's actuator parameters " + ...
            "via baseVehicleType fallback -- see PLAN_CORRECCION_MULTIVEHICULO_SITL.md Fase 1/3. Add " + ...
            "a case for '" + lower(setupVehicleType) + "' in this switch before re-enabling this vehicle."))
end
