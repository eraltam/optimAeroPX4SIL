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

    otherwise
        error("setUpActuators:NoActuatorModelDefined", char("No real actuator model (motor/ESC/" + ...
            "control-surface dynamics) defined yet for vehicleParams.type='" + setupVehicleType + ...
            "'. This vehicle previously compiled silently using the hexarotor's actuator parameters " + ...
            "via baseVehicleType fallback -- see PLAN_CORRECCION_MULTIVEHICULO_SITL.md Fase 1/3. Add " + ...
            "a case for '" + lower(setupVehicleType) + "' in this switch before re-enabling this vehicle."))
end
