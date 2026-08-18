% This script sets up the actuator parameters for the selected vehicle. This includes
% parameters that define the motors, control surfaces, engines, etc.
actuatorDelay_s = 0.005;
plantSampleTime_s = 0.01;
parameters.engine.minThrottle = 0.01; %idle happens above this value

switch lower(vehicleParams.type)
    case "f-16"
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
        % Rotor diameter (1.016 m / 40") is a DIFFERENT quantity from the reference spec's "maximum
        % unfolded propeller envelope diameter" (1.4556 m, which includes arm/frame clearance for a
        % folding prop) -- not necessarily a contradiction, left unchanged pending reconciliation,
        % see PLAN_HEXAROTOR_VEHICLE_PLANT_AUDIT_AND_VIBRATION_MODEL.md Section 3.4/2.
        vehicleParams.rotor.diameter_m  = 1.016; % 40 inch rotor
        vehicleParams.rotor.maxRPM      = 3473;  % sole source of truth now -- the duplicate,
        % inconsistent `vehicleParams.maxRPM=3000` (previously set in setUpVehicle.m) has been
        % removed; this is the only maxRPM used anywhere (feeds cmdToRpmData below).
        vehicleParams.rotor.cmdToRpmData = [0 vehicleParams.rotor.maxRPM];
        vehicleParams.rotor.cmdToRpmBkpts_nd = [0 1];

        % --- Thrust/torque coefficients (fixed 2026-07-30, see
        % PLAN_HEXAROTOR_VEHICLE_PLANT_AUDIT_AND_VIBRATION_MODEL.md Section 3.3) ---
        % hexMotorModel.slx's actual thrust equation, traced from the compiled block diagram (NOT
        % assumed): thrust_N = kct_nd * diameter_m^4 * rotorAngVel_radps^2 * (airDensity/airDensityAtStd).
        % The OLD value (0.0024558) combined with that equation and this project's real maxRPM
        % (3473) gives an unrealistic total max thrust-to-weight ratio of ~28:1 at the corrected
        % 7.441 kg mass (hover would sit at only 18.7% throttle command) -- this was miscalibrated
        % independently of the mass bug fixed in setUpVehicle.m, most likely tuned under a
        % different assumed thrust equation than the one actually implemented here.
        %
        % STOPGAP ESTIMATE, Class 3 (engineering estimate, NOT thrust-stand data -- true kct_nd/
        % kcp_nd are Class 4, "unknown, needs measurement" per the plan's confidence table):
        % rescaled so hover sits at a 50% throttle command (total max T/W = 4.0:1, a plausible
        % value for a capable survey/mapping hexarotor, not a validated one). kcp_nd rescaled by
        % the same factor, preserving the torque/thrust ratio implied by the original (also
        % unverified) values rather than inventing a new one. Replace both with thrust-stand-
        % measured T(RPM)/Q(RPM) curves as soon as available (see the plan's prioritized-
        % measurement list, items 9-10).
        vehicleParams.rotor.kct_nd = 3.451589e-04;
        vehicleParams.rotor.kcp_nd = 2.150698e-05;

        % --- Rotor geometry (fixed 2026-07-30, see
        % PLAN_HEXAROTOR_VEHICLE_PLANT_AUDIT_AND_VIBRATION_MODEL.md Sections 2-3.1) ---
        % The previous array here was an idealized UNIT-RADIUS hexagon (all 6 rotors at exactly
        % 1.0 m) that does not match this project's actual flight-validated PX4 airframe
        % (SYS_AUTOSTART=1003 -> ROMFS/px4fmu_common/init.d/airframes/1003_optim_aero_hex.hil) --
        % it instead matches a DIFFERENT airframe (10021_optimAeroHex, "Palledrone"). Replaced below
        % with 1003's own CA_ROTOR0-5_PX/PY values directly (already in meters, FRD body frame,
        % same convention this array expects) -- this is the geometry actually flown for every real
        % HIL session referenced by this project (nav_hil_ground_truth.ulg,
        % four_way_wind_comparison's four sessions), so it is the correct source of truth for
        % "what does motor index N mean," not the reference brief's abstract Rotor0-5 table (which,
        % cross-checked position-by-position, uses a DIFFERENT index-to-position numbering than
        % PX4's own CA_ROTOR indices for this same physical vehicle -- do not use it directly here).
        %
        % NOTE: 1003's own CA_ROTOR values are themselves slightly asymmetric (rotors 0/1 at radius
        % 0.500 m, rotors 2-5 at radius 0.4970 m, vs. a single 0.4992 m for all six in the
        % reference spec) -- left AS FLOWN rather than "corrected" to the spec's uniform radius,
        % since it is not yet known whether that asymmetry is a real feature of the physical
        % airframe or a PX4-config approximation; resolve with a real tape-measure check (see the
        % plan's prioritized-measurement list), not a guess.
        %
        % Spin direction was re-derived, not assumed: for each PX4 CA_ROTOR position, the OLD
        % array's spin value at the matching physical position (unit-vector dot product = 1.0000,
        % unambiguous for all 6) was carried over. Numerically this reproduces the exact same
        % [1 0 1 0 0 1] sequence as before -- verified independently two ways: (1) it alternates
        % CW/CCW/CW/CCW/CW/CCW when the six rotors are sorted by physical angle around the hub
        % (required for yaw-torque cancellation in hover), and (2) it correlates cleanly with
        % 1003's own CA_ROTORx_KM sign (KM=-0.05 <-> CW, KM=+0.05 <-> CCW for all 6, no exceptions).
        % Still Class 4 (brief Section 3: "rotor spin directions... unknown, requires
        % measurement") in the sense that this is inferred/cross-checked, not physically verified
        % against the real hardware -- do not treat as final without a real spin-direction check.
        %
        % +X-Axis is out of the nose, +Y is out of starboard side, +Z is toward ground (FRD, unchanged)
        vehicleParams.rotor.motorLocs = [ 0.0000    0.5000 0;   % rotor0
                                          0.0000   -0.5000 0;   % rotor1
                                          0.4300   -0.2500 0;   % rotor2
                                         -0.4300    0.2500 0;   % rotor3
                                          0.4300    0.2500 0;   % rotor4
                                         -0.4300   -0.2500 0];  % rotor5
        vehicleParams.rotor.spinDirectionCW_isTrue = [1 0 1 0 0 1];

        % Using a generic 2nd order transfer function for an ESC and brushless motor
        vehicleParams.actuator.tfNumerator = 250000;
        vehicleParams.actuator.tfDenominator = [1 700 250000];

    otherwise
        error(char(["unknown vehicle: " vehicleType]))
end