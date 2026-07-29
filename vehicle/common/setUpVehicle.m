% This script populates the base workspace with vehicle parameters for the specified vehicle. This script also includes
% the data describing the initial location of the vehicle in the ismulation.
%
% NOTE: this switches on vehicleParams.type (the specific plant/vehicle key), not
% vehicleParams.baseVehicleType. Every non-F16/hexarotor vehicle in vehicle/vehicleRegistry.m
% currently has BaseVehicleType forced to "hexarotor" as a compile-time placeholder; switching on
% baseVehicleType silently gave every rover/tracked/marine vehicle the hexarotor's mass, inertia,
% CG and aero data. See PLAN_CORRECCION_MULTIVEHICULO_SITL.md (Fase 1 / F1) for the full writeup.
% Each vehicle needing real physics must get its own case here (or this script must be pointed at
% the per-vehicle vehicle/<key>/data/setUpVehicle_<key>.m files, which currently exist but are
% never called by initVehicleSIL.m -- confirmed dead code, see the same document).
setupVehicleType = vehicleParams.type;

switch lower(setupVehicleType)
    case "f16"
        vehicleParams.dryMass_kg = 20500 * lbs2kg;
        vehicleParams.aircraftInertialBody_kgm2 = [12820.61 0 1331.41; 0 75673.62 0; 0 0 85552.11];
        vehicleParams.bRef_m = 30 * ft2m;
        vehicleParams.SRef_m2 = 27.8709;
        vehicleParams.cRef_m = 11.32 * ft2m;% Mean Aero Chord
        vehicleParams.aircraftCg_m = [vehicleParams.cRef_m * 0.35; 0; 0]; 
        vehicleParams.refCG_m = [vehicleParams.cRef_m * 0.35; 0; 0]; % reference cg
        aeroDataF16   
        % set initial location to Juancho E. Yrausquin Airport, runway 12
        referenceAltitude_m = 125 * ft2m;
        referenceLatitude_deg = 17.645927; 
        referenceLongitude_deg = -63.222027;
    case "hexarotor"    
        vehicleParams.dryMass_kg = 180 * lbs2kg;
        vehicleParams.aircraftInertialBody_kgm2 = [11.8 0 0; 0 11.8 0;  0 0 23.5];
        vehicleParams.aircraftCg_m = [0; 0; 0]; 
        vehicleParams.refCG_m = [0; 0; 0]; % reference cg
        vehicleParams.maxRPM = 3000;
        vehicleParams.SRef_m2 = 0.5;
        aeroDataHex
        % set initial location to Juancho E. Yrausquin Airport, helipad
        referenceAltitude_m = 116 * ft2m;
        referenceLatitude_deg = 17.64450;
        referenceLongitude_deg = -63.21988;
    case "quadrotor"
        % LADAC-family multirotor (Fase 3 aerial track, see PLAN_CORRECCION_MULTIVEHICULO_SITL.md).
        % Reuses hexarotor's actual 6DOF rigid-body engine (hexActuator/hexMotorModel's For Each
        % subsystem is parametric in rotor count -- confirmed by inspection, not just assumed) --
        % this vehicle only needs its own rotor geometry/mass/inertia, not a rebuilt plant. Rotor
        % positions/spin directions taken directly from
        % PX4-Autopilot/.../airframes/10022_optimAeroQuad (CA_ROTORn_PX/PY/KM, X-quad layout,
        % arm length 1.0 m as literally given by the airframe file). Mass/inertia scaled from
        % hexarotor's own values by rotor-count ratio (4/6) -- order-of-magnitude placeholder for a
        % quad on the same physical scale as the existing hex, not a specific catalog vehicle.
        vehicleParams.dryMass_kg = (4/6) * 180 * lbs2kg;
        vehicleParams.aircraftInertialBody_kgm2 = (4/6) * [11.8 0 0; 0 11.8 0; 0 0 23.5];
        vehicleParams.aircraftCg_m = [0; 0; 0];
        vehicleParams.refCG_m = [0; 0; 0];
        vehicleParams.maxRPM = 3000;
        vehicleParams.SRef_m2 = 0.35;
        aeroDataHex
        referenceAltitude_m = 116 * ft2m;
        referenceLatitude_deg = 17.64450;
        referenceLongitude_deg = -63.21988;
    case "octarotor"
        % LADAC-family multirotor -- see quadrotor case above for the reuse rationale. PX4 airframe
        % (10024_optimAeroOcta) declares CA_ROTOR_COUNT=8 but leaves CA_ROTORn_PX/PY unset (uses
        % PX4's internal default octa-X geometry, not available to inspect from this repo) --
        % documented gap, same class as the F8 @autostart metadata mismatches. Uses a standard flat
        % octocopter-X layout (8 rotors at 45 deg spacing, arm length 1.0 m matching hex's scale,
        % alternating spin direction) as our own explicit, physically valid choice. Mass/inertia
        % scaled from hexarotor by rotor-count ratio (8/6).
        vehicleParams.dryMass_kg = (8/6) * 180 * lbs2kg;
        vehicleParams.aircraftInertialBody_kgm2 = (8/6) * [11.8 0 0; 0 11.8 0; 0 0 23.5];
        vehicleParams.aircraftCg_m = [0; 0; 0];
        vehicleParams.refCG_m = [0; 0; 0];
        vehicleParams.maxRPM = 3000;
        vehicleParams.SRef_m2 = 0.65;
        aeroDataHex
        referenceAltitude_m = 116 * ft2m;
        referenceLatitude_deg = 17.64450;
        referenceLongitude_deg = -63.21988;
    case "evtol"
        % LADAC-family aerial vehicle -- see quadrotor case above for the reuse rationale. PX4
        % airframe (10025_optimAeroEVTOL) is declared "@type VTOL Tiltrotor" but its actual
        % PWM_MAIN_FUNC1-6 assignment (101-106, all plain motor functions, no distinct tilt/pusher
        % actuator channel) only wires a 6-motor multirotor mixer -- true tiltrotor transition/
        % pusher-prop/fixed-wing aerodynamics are NOT modeled at this fidelity level, same
        % documented-gap pattern as the kinematic rovers' "no tire slip". Reuses hexarotor's exact
        % rotor geometry (no distinct CA_ROTORn_PX/PY given in the airframe file either) but with
        % its own mass/inertia -- a lighter electric VTOL demonstrator class, distinct from the
        % heavy-lift hex.
        vehicleParams.dryMass_kg = 45;
        vehicleParams.aircraftInertialBody_kgm2 = [6.5 0 0; 0 6.5 0; 0 0 13];
        vehicleParams.aircraftCg_m = [0; 0; 0];
        vehicleParams.refCG_m = [0; 0; 0];
        vehicleParams.maxRPM = 3000;
        vehicleParams.SRef_m2 = 0.5;
        aeroDataHex
        referenceAltitude_m = 116 * ft2m;
        referenceLatitude_deg = 17.64450;
        referenceLongitude_deg = -63.21988;
    case "evtol_simscape"
        % Simscape adapter variant of "evtol" -- see PLAN_CORRECCION_MULTIVEHICULO_SITL.md.
        % evtol_simscape.slx is a clone of evtol.slx (same hexarotor-engine Model Reference chain)
        % plus an unconnected "simscape_adapter_diagnostics" placeholder subsystem documented in
        % vehicle/evtol_simscape/README.md as future work -- real motor/propeller/powertrain
        % Simscape physics is blocked on an external source project
        % (C:\AnelloSummer\new_vehicles\eVTOL-Drone-Simscape) that does not exist on this machine.
        % Uses the exact same rigid-body geometry/mass/inertia as "evtol" since it is the same
        % airframe pending the future Simscape powertrain swap-in.
        vehicleParams.dryMass_kg = 45;
        vehicleParams.aircraftInertialBody_kgm2 = [6.5 0 0; 0 6.5 0; 0 0 13];
        vehicleParams.aircraftCg_m = [0; 0; 0];
        vehicleParams.refCG_m = [0; 0; 0];
        vehicleParams.maxRPM = 3000;
        vehicleParams.SRef_m2 = 0.5;
        aeroDataHex
        referenceAltitude_m = 116 * ft2m;
        referenceLatitude_deg = 17.64450;
        referenceLongitude_deg = -63.21988;
    case "vtol_tailsitter"
        % Twin-rotor tailsitter -- see setUpActuators.m's "vtol_tailsitter" case for the reuse
        % rationale. Real mass/inertia from LADAC-Examples/Plane/../Arkbird/
        % tailsitter_params_Arkbird.m (tailsitter.body.m/I) -- a genuine small tailsitter airframe,
        % not a placeholder. SRef_m2 is a minor drag-only term at this fidelity level (hover
        % regime only), estimated from the Arkbird wing span/chord in
        % simple_wing_params_Arkbird.m (b=0.86 m, c=0.2425 m).
        vehicleParams.dryMass_kg = 1.1;
        vehicleParams.aircraftInertialBody_kgm2 = [0.0179 0 0; 0 0.0113 0; 0 0 0.0287];
        vehicleParams.aircraftCg_m = [0; 0; 0];
        vehicleParams.refCG_m = [0; 0; 0];
        vehicleParams.maxRPM = 15000;
        vehicleParams.SRef_m2 = 0.86 * 0.2425;
        aeroDataHex
        referenceAltitude_m = 116 * ft2m;
        referenceLatitude_deg = 17.64450;
        referenceLongitude_deg = -63.21988;
    case "ship_semisub"
        % Semisubmersible platform -- real mass and dimensions from
        % MSS/CRAFT/SHIP/models/semisubModels/data_rig.m (mass=27,162,500 kg, pontoon length
        % L_p=84.6 m, radii of gyration r_x=30/r_y=32/r_z=37 m) -- see
        % PLAN_VEHICULOS_AEREOS_Y_MARINOS_SITL.md and setUpActuators.m's "ship_semisub" case for
        % why this reuses ship_surface's 2-command Nomoto architecture instead of a true
        % holonomic DP model.
        vehicleParams.dryMass_kg = 27162500;
        vehicleParams.aircraftInertialBody_kgm2 = [ ...
            vehicleParams.dryMass_kg*30^2 0 0; ...
            0 vehicleParams.dryMass_kg*32^2 0; ...
            0 0 vehicleParams.dryMass_kg*37^2];
        vehicleParams.aircraftCg_m = [0; 0; 0];
        vehicleParams.refCG_m = [0; 0; 0];
        vehicleParams.shipLength_m = 84.6;
        vehicleParams.maxRudderAngle_rad = 0.52;
        vehicleParams.maxSpeed_mps = 1.0;
        referenceAltitude_m = 0;
        referenceLatitude_deg = 17.64450;
        referenceLongitude_deg = -63.21988;
    case "ship_surface"
        % Single rudder+propeller surface ship (Nomoto-class maneuvering, subactuated -- see
        % PLAN_VEHICULOS_AEREOS_Y_MARINOS_SITL.md). Reference length L=175 m from
        % MSS/CRAFT/SHIP/models/container.m (the classic Son & Nomoto 1982 container-ship
        % maneuvering benchmark used throughout the marine control literature). Mass/inertia are
        % order-of-magnitude displacement estimates for this ship class (container.m itself only
        % gives non-dimensional hydrodynamic derivatives, not a dimensional mass) -- not a specific
        % catalog vessel, same spirit as this project's other rover/USV mass placeholders.
        vehicleParams.dryMass_kg = 45000e3;
        vehicleParams.aircraftInertialBody_kgm2 = [1e9 0 0; 0 8e10 0; 0 0 8e10];
        vehicleParams.aircraftCg_m = [0; 0; 0];
        vehicleParams.refCG_m = [0; 0; 0];
        vehicleParams.shipLength_m = 175;
        vehicleParams.maxRudderAngle_rad = 0.61;
        vehicleParams.maxSpeed_mps = 8.0;
        referenceAltitude_m = 0;
        referenceLatitude_deg = 17.64450;
        referenceLongitude_deg = -63.21988;
    case "uuv_dsrv"
        % Deep Submergence Rescue Vehicle (MSS/CRAFT/AUV/models/DSRV.m, Healey 1992, NPS) -- see
        % PLAN_VEHICULOS_AEREOS_Y_MARINOS_SITL.md. Real reference length L=5.0 m, cruise speed
        % U0=4.11 m/s (8 knots) from DSRV.m's own header. DSRV.m does not give a dimensional mass
        % (only normalized hydrodynamic derivatives like Iy=0.001925) -- 5000 kg is an
        % order-of-magnitude displacement estimate for a vehicle this size, comparable to
        % uuv_npsauv's real 5443 kg at a similar 5.3 m length, not measured. IMPORTANT: the real
        % DSRV is controlled by a single stern plane for depth/pitch at near-constant forward
        % speed, NOT by differential-thrust yaw steering -- this reuses uuv_subsea's horizontal-
        % plane simplification anyway (see setUpActuators.m's "uuv_dsrv" case for why that is an
        % explicitly larger fidelity gap here than for the other UUVs).
        vehicleParams.dryMass_kg = 5000;
        vehicleParams.aircraftInertialBody_kgm2 = [300 0 0; 0 5000 0; 0 0 5000];
        vehicleParams.aircraftCg_m = [0; 0; 0];
        vehicleParams.refCG_m = [0; 0; 0];
        vehicleParams.trackWidth_m = 0.8;
        vehicleParams.wheelRadius_m = 0.2;
        vehicleParams.maxSpeed_mps = 4.11;
        referenceAltitude_m = -10;
        referenceLatitude_deg = 17.64450;
        referenceLongitude_deg = -63.21988;
    case "uuv_npsauv"
        % Cruise-mission AUV (Naval Postgraduate School design, Healey & Lienard 1993) -- see
        % PLAN_VEHICULOS_AEREOS_Y_MARINOS_SITL.md. Real mass/length from
        % MSS/CRAFT/AUV/models/npsauv.m (L=5.3 m, mass=5443 kg) -- distinct mission class from
        % uuv_subsea's torpedo-shaped remus100 (inspection/cruise vs. general-purpose). Reuses the
        % same horizontal-plane-only differential-thrust kinematic simplification as uuv_subsea
        % (real npsauv has a single prop + rudder/stern-plane/bow-planes, not modeled here).
        vehicleParams.dryMass_kg = 5443;
        vehicleParams.aircraftInertialBody_kgm2 = [400 0 0; 0 6000 0; 0 0 6000];
        vehicleParams.aircraftCg_m = [0; 0; 0];
        vehicleParams.refCG_m = [0; 0; 0];
        vehicleParams.trackWidth_m = 0.6;
        vehicleParams.wheelRadius_m = 0.15;
        vehicleParams.maxSpeed_mps = 2.5;
        referenceAltitude_m = -10;
        referenceLatitude_deg = 17.64450;
        referenceLongitude_deg = -63.21988;
    case "ackermann_rover"
        % Small ground rover, kinematic-bicycle plant (Fase 3, PLAN_CORRECCION_MULTIVEHICULO_SITL.md).
        % Mass/inertia are reasonable-order-of-magnitude placeholders for a small UGV, not a specific
        % catalog vehicle -- refine once a real chassis is chosen. Wheelbase/steering limits match
        % PX4-Autopilot/.../airframes/10023_optimAeroAckermann (GND_WHEEL_BASE=2.0, GND_MAX_ANG=0.6).
        vehicleParams.dryMass_kg = 60;
        vehicleParams.aircraftInertialBody_kgm2 = [5 0 0; 0 15 0; 0 0 18];
        vehicleParams.aircraftCg_m = [0; 0; 0];
        vehicleParams.refCG_m = [0; 0; 0];
        vehicleParams.wheelBase_m = 2.0;
        vehicleParams.wheelRadius_m = 0.3;
        vehicleParams.maxSteerAngle_rad = 0.6;
        vehicleParams.maxSpeed_mps = 8.0;
        % Reuse the same reference location as the hexarotor (same airport, ground level) so the
        % rover starts from a known-good geodetic position for the WMM/gravity/GPS models.
        referenceAltitude_m = 116 * ft2m;
        referenceLatitude_deg = 17.64450;
        referenceLongitude_deg = -63.21988;
    case "ackermann_simscape"
        % Simscape adapter variant of "ackermann_rover" -- see PLAN_CORRECCION_MULTIVEHICULO_SITL.md.
        % ackermann_simscape.slx was a clone of the ORIGINAL hex-engine placeholder (Model Reference
        % to hexActuator.slx etc, wrong physics class entirely for a steer+throttle car) plus an
        % unconnected "simscape_adapter_diagnostics" network. Real tire/suspension Simscape physics
        % is blocked on an external source project
        % (C:\AnelloSummer\new_vehicles\Formula-Student-Vehicle-Simscape) that does not exist on
        % this machine. Rebuilt as a direct copy of ackermann_rover.slx's real kinematic-bicycle
        % plant (same AckermannKinematics MATLAB Function block) -- same car, pending the future
        % Simscape tire/suspension swap-in. Uses identical parameters to ackermann_rover.
        vehicleParams.dryMass_kg = 60;
        vehicleParams.aircraftInertialBody_kgm2 = [5 0 0; 0 15 0; 0 0 18];
        vehicleParams.aircraftCg_m = [0; 0; 0];
        vehicleParams.refCG_m = [0; 0; 0];
        vehicleParams.wheelBase_m = 2.0;
        vehicleParams.wheelRadius_m = 0.3;
        vehicleParams.maxSteerAngle_rad = 0.6;
        vehicleParams.maxSpeed_mps = 8.0;
        referenceAltitude_m = 116 * ft2m;
        referenceLatitude_deg = 17.64450;
        referenceLongitude_deg = -63.21988;
    case "differential_rover"
        % Small skid/differential-drive ground rover, kinematic plant (Fase 3,
        % PLAN_CORRECCION_MULTIVEHICULO_SITL.md). Mass/inertia are order-of-magnitude placeholders,
        % same spirit as ackermann_rover. Matches PX4-Autopilot/.../airframes/10026_optimAeroDiffRover
        % (twin-motor mixer: PWM_MAIN_FUNC1=101 left, FUNC2=102 right).
        vehicleParams.dryMass_kg = 50;
        vehicleParams.aircraftInertialBody_kgm2 = [4 0 0; 0 12 0; 0 0 14];
        vehicleParams.aircraftCg_m = [0; 0; 0];
        vehicleParams.refCG_m = [0; 0; 0];
        vehicleParams.trackWidth_m = 0.6;
        vehicleParams.wheelRadius_m = 0.15;
        vehicleParams.maxSpeed_mps = 5.0;
        referenceAltitude_m = 116 * ft2m;
        referenceLatitude_deg = 17.64450;
        referenceLongitude_deg = -63.21988;
    case "tracked_vehicle"
        % Skid-steer tracked vehicle (Fase 3, terrestrial track, see
        % PLAN_CORRECCION_MULTIVEHICULO_SITL.md). Matches
        % PX4-Autopilot/.../airframes/10027_optimAeroTracked (twin-motor mixer: PWM_MAIN_FUNC1=101
        % left track, FUNC2=102 right track -- same mixer class as differential_rover; a tracked
        % vehicle's low-speed kinematics ARE differential-drive-equivalent, left/right track speed
        % -> forward vel + yaw rate via an effective track width). tracked_vehicle.slx reuses
        % differential_rover.slx's exact DifferentialKinematics MATLAB Function block with new
        % constants for a heavier, slower vehicle (own case, own inlined constants -- see the same
        % documentation-only-struct-fields rationale as differential_rover). Real track/terrain
        % contact and slip are NOT modeled at this fidelity level -- see
        % Tracked-Vehicles-Simscape/Models/{Excavator_Tracks,Tractor_Tracks} for the full Simscape
        % multibody reference this could be upgraded to (deferred, real integration effort, same
        % class of future work as the _simscape adapter vehicles).
        vehicleParams.dryMass_kg = 2500;
        vehicleParams.aircraftInertialBody_kgm2 = [400 0 0; 0 1200 0; 0 0 1400];
        vehicleParams.aircraftCg_m = [0; 0; 0];
        vehicleParams.refCG_m = [0; 0; 0];
        vehicleParams.trackWidth_m = 1.5;
        vehicleParams.wheelRadius_m = 0.3;
        vehicleParams.maxSpeed_mps = 3.0;
        referenceAltitude_m = 116 * ft2m;
        referenceLatitude_deg = 17.64450;
        referenceLongitude_deg = -63.21988;
    case "tracked_vehicle_simscape"
        % Simscape adapter variant of "tracked_vehicle" -- see PLAN_CORRECCION_MULTIVEHICULO_SITL.md.
        % Same finding/treatment as the other _simscape adapters: was a hex-engine clone plus an
        % unconnected simscape_adapter_diagnostics network. Unlike ackermann_simscape/evtol_simscape/
        % wheel_loader_simscape, the referenced Simscape source repo DOES exist in this checkout --
        % Tracked-Vehicles-Simscape/Models/{Excavator_Tracks,Tractor_Tracks} at the repo root (the
        % README's path C:\AnelloSummer\new_vehicles\Tracked-Vehicles-Simscape is stale/wrong, not
        % missing) -- making this the most promising candidate for a real future Simscape multibody
        % port, but a full bus-contract/frame-conversion integration is still out of scope for this
        % pass (see SIMSCAPE_VEHICLE_INTEGRATION_PLAN.md). Rebuilt as a direct copy of
        % tracked_vehicle.slx's real kinematic plant in the meantime -- same vehicle, pending that
        % future swap-in.
        vehicleParams.dryMass_kg = 2500;
        vehicleParams.aircraftInertialBody_kgm2 = [400 0 0; 0 1200 0; 0 0 1400];
        vehicleParams.aircraftCg_m = [0; 0; 0];
        vehicleParams.refCG_m = [0; 0; 0];
        vehicleParams.trackWidth_m = 1.5;
        vehicleParams.wheelRadius_m = 0.3;
        vehicleParams.maxSpeed_mps = 3.0;
        referenceAltitude_m = 116 * ft2m;
        referenceLatitude_deg = 17.64450;
        referenceLongitude_deg = -63.21988;
    case "usv_surface"
        % Twin-thruster surface vessel (Fase 3, marine track, see
        % PLAN_CORRECCION_MULTIVEHICULO_SITL.md). Matches
        % PX4-Autopilot/.../airframes/10029_optimAeroUSV (twin-motor mixer: PWM_MAIN_FUNC1=101 left
        % thruster, FUNC2=102 right thruster -- same mixer class as differential_rover/
        % tracked_vehicle). usv_surface.slx reuses the exact same DifferentialKinematics MATLAB
        % Function block (surge speed + yaw rate from differential thrust is the same math as
        % differential-drive; a real MSS 3DOF surface-vessel model adds hydrodynamic added-mass/
        % damping/restoring terms this does NOT capture -- see MSS/CRAFT/USV and
        % MSS/SIMULINK/mssSimulinkDemos for the reference this could be upgraded to, deferred real
        % integration effort, same class of future work as the _simscape adapters). Stays at a
        % constant NED down position (no heave/pitch/roll modeled) -- reasonable for calm-water
        % low-speed PX4 loop-closure testing, not seakeeping.
        %
        % UPDATED 2026-07-18 (see PLAN_VEHICULOS_AEREOS_Y_MARINOS_SITL.md): mass/geometry replaced
        % with the real Maritime Robotics Otter USV from MSS/CRAFT/USV/models/otter.m (L=2.0 m,
        % already a genuine twin-propeller differential-thrust catamaran -- unlike ship_surface/
        % uuv_npsauv, no kinematic-architecture mismatch to document here, this vehicle's real
        % design matches the DifferentialKinematics simplification almost exactly). Dry mass ~55 kg
        % and hull (catamaran) separation ~1.08 m are the vendor-published Otter specs (otter.m
        % itself only gives non-dimensional/6x6 mass-matrix hydrodynamic terms, not a simple
        % dimensional payload-free mass).
        vehicleParams.dryMass_kg = 55;
        vehicleParams.aircraftInertialBody_kgm2 = [8 0 0; 0 18 0; 0 0 20];
        vehicleParams.aircraftCg_m = [0; 0; 0];
        vehicleParams.refCG_m = [0; 0; 0];
        vehicleParams.trackWidth_m = 1.08;
        vehicleParams.wheelRadius_m = 0.1;
        vehicleParams.maxSpeed_mps = 3.6;
        referenceAltitude_m = 0;
        referenceLatitude_deg = 17.64450;
        referenceLongitude_deg = -63.21988;
    case "uuv_subsea"
        % Twin-thruster subsea vehicle (Fase 3, marine track, see
        % PLAN_CORRECCION_MULTIVEHICULO_SITL.md). Matches
        % PX4-Autopilot/.../airframes/10030_optimAeroUUV (twin-motor mixer: PWM_MAIN_FUNC1=101 left
        % thruster, FUNC2=102 right thruster -- same mixer class as differential_rover/
        % tracked_vehicle/usv_surface). Same reuse rationale as usv_surface -- horizontal-plane
        % (yaw + surge) kinematics only. Depth/heave/pitch/roll are explicitly NOT modeled at this
        % fidelity level (vehicle stays at a constant reference depth, same simplification as
        % staying at "ground level" for the terrestrial rovers) -- see MSS/CRAFT (remus100-class
        % AUV models) for the full 6DOF reference this could be upgraded to, same deferred-future-
        % work class as the other documented gaps in this plan. Slower max speed and longer thruster
        % lag than usv_surface reflect greater added-mass/drag when fully submerged.
        vehicleParams.dryMass_kg = 300;
        vehicleParams.aircraftInertialBody_kgm2 = [50 0 0; 0 150 0; 0 0 170];
        vehicleParams.aircraftCg_m = [0; 0; 0];
        vehicleParams.refCG_m = [0; 0; 0];
        vehicleParams.trackWidth_m = 1.2;
        vehicleParams.wheelRadius_m = 0.08;
        vehicleParams.maxSpeed_mps = 2.0;
        referenceAltitude_m = -5;
        referenceLatitude_deg = 17.64450;
        referenceLongitude_deg = -63.21988;
    case "bicycle_rover"
        % Small single-track steer+throttle rover, kinematic-bicycle plant (Fase 3, same equations
        % as ackermann_rover -- see PLAN_CORRECCION_MULTIVEHICULO_SITL.md). Wheelbase/steering
        % limits match PX4-Autopilot/.../airframes/10034_optimAeroBicycle (GND_WHEEL_BASE=1.2,
        % GND_MAX_ANG=0.6).
        vehicleParams.dryMass_kg = 25;
        vehicleParams.aircraftInertialBody_kgm2 = [2 0 0; 0 6 0; 0 0 7];
        vehicleParams.aircraftCg_m = [0; 0; 0];
        vehicleParams.refCG_m = [0; 0; 0];
        vehicleParams.wheelBase_m = 1.2;
        vehicleParams.wheelRadius_m = 0.25;
        vehicleParams.maxSteerAngle_rad = 0.6;
        vehicleParams.maxSpeed_mps = 6.0;
        referenceAltitude_m = 116 * ft2m;
        referenceLatitude_deg = 17.64450;
        referenceLongitude_deg = -63.21988;
    case "wheel_loader"
        % Small construction wheel loader, steer+throttle kinematic-bicycle plant (Fase 3, same
        % equations as ackermann_rover -- see PLAN_CORRECCION_MULTIVEHICULO_SITL.md). PX4 airframe
        % (10028_optimAeroLoader) doesn't set GND_WHEEL_BASE/GND_MAX_ANG explicitly; values below
        % are reasonable placeholders for a compact loader, not a specific catalog machine.
        % Hydraulics/bucket are explicitly out of scope for this fidelity level (matches
        % SIMSCAPE_ADAPTER_HANDOFF.md: "steering plus wheel torque on flat terrain; defer
        % hydraulics/bucket").
        vehicleParams.dryMass_kg = 4500;
        vehicleParams.aircraftInertialBody_kgm2 = [1200 0 0; 0 3500 0; 0 0 4000];
        vehicleParams.aircraftCg_m = [0; 0; 0];
        vehicleParams.refCG_m = [0; 0; 0];
        vehicleParams.wheelBase_m = 2.5;
        vehicleParams.wheelRadius_m = 0.6;
        vehicleParams.maxSteerAngle_rad = 0.5;
        vehicleParams.maxSpeed_mps = 4.0;
        referenceAltitude_m = 116 * ft2m;
        referenceLatitude_deg = 17.64450;
        referenceLongitude_deg = -63.21988;
    case "wheel_loader_simscape"
        % Simscape adapter variant of "wheel_loader" -- see PLAN_CORRECCION_MULTIVEHICULO_SITL.md.
        % Same finding/treatment as ackermann_simscape/evtol_simscape: was a hex-engine clone plus
        % an unconnected simscape_adapter_diagnostics network; external source project
        % (C:\AnelloSummer\new_vehicles\Wheel-Loader-Simscape) does not exist on this machine.
        % Rebuilt as a direct copy of wheel_loader.slx's real kinematic-bicycle plant. Same
        % machine, pending future Simscape driveline/hydraulics swap-in.
        vehicleParams.dryMass_kg = 4500;
        vehicleParams.aircraftInertialBody_kgm2 = [1200 0 0; 0 3500 0; 0 0 4000];
        vehicleParams.aircraftCg_m = [0; 0; 0];
        vehicleParams.refCG_m = [0; 0; 0];
        vehicleParams.wheelBase_m = 2.5;
        vehicleParams.wheelRadius_m = 0.6;
        vehicleParams.maxSteerAngle_rad = 0.5;
        vehicleParams.maxSpeed_mps = 4.0;
        referenceAltitude_m = 116 * ft2m;
        referenceLatitude_deg = 17.64450;
        referenceLongitude_deg = -63.21988;
    case "unicycle_rover"
        % Small skid/differential-drive rover, same kinematic plant as differential_rover -- see
        % PLAN_CORRECCION_MULTIVEHICULO_SITL.md. Matches
        % PX4-Autopilot/.../airframes/10035_optimAeroUnicycle (twin-motor mixer:
        % PWM_MAIN_FUNC1=101 left, FUNC2=102 right -- same class as differential_rover despite the
        % "unicycle" name; PX4's rover mixer has no true single-wheel airframe type).
        vehicleParams.dryMass_kg = 15;
        vehicleParams.aircraftInertialBody_kgm2 = [1 0 0; 0 3 0; 0 0 3.5];
        vehicleParams.aircraftCg_m = [0; 0; 0];
        vehicleParams.refCG_m = [0; 0; 0];
        vehicleParams.trackWidth_m = 0.35;
        vehicleParams.wheelRadius_m = 0.1;
        vehicleParams.maxSpeed_mps = 3.0;
        referenceAltitude_m = 116 * ft2m;
        referenceLatitude_deg = 17.64450;
        referenceLongitude_deg = -63.21988;
    case "fixedwing_plane"
        % Generic light ala-fija (RC-class general-aviation-style) fixed-wing plant -- see
        % PLAN_VEHICULOS_AEREOS_Y_MARINOS_SITL.md Fase A. Real geometry/mass/inertia sourced from
        % LADAC-Examples/Plane/Funray (airplane_params_Funray.m, wing_params_FunrayMain.m),
        % distinct from F16's supersonic-fighter aero -- this is a low-speed, low-Reynolds-number
        % RC-class airframe. Diagonal-only inertia (off-diagonal Ixz=-0.008 kgm^2 in the real
        % Funray data is neglected here, same simplification convention as every other vehicle's
        % aircraftInertialBody_kgm2 in this switch).
        vehicleParams.dryMass_kg = 2.15;
        vehicleParams.aircraftInertialBody_kgm2 = [0.165 0 0; 0 0.177 0; 0 0 0.338];
        vehicleParams.aircraftCg_m = [0; 0; 0];
        vehicleParams.refCG_m = [0; 0; 0];
        % Wingspan b=2.0 m (wing_params_FunrayMain.m prm.b); wing area/mean chord computed by
        % trapezoidal integration of the same file's chord/eta_segments_wing station data
        % (half-span area 0.1853 m^2 doubled -> SRef_m2; SRef_m2/(b/2) -> cRef_m) -- see
        % PLAN_VEHICULOS_AEREOS_Y_MARINOS_SITL.md for the worked calculation.
        vehicleParams.bRef_m = 2.0;
        vehicleParams.SRef_m2 = 0.3706;
        vehicleParams.cRef_m = 0.1853;
        referenceAltitude_m = 116 * ft2m;
        referenceLatitude_deg = 17.64450;
        referenceLongitude_deg = -63.21988;
    case {"c172p", "c172pjsbsim"}
        % Cessna 172P. "c172pjsbsim" (PLAN_JSBSIM_SFUNCTION_HYBRID_C172P.md) shares this case
        % verbatim with the native "c172p" (PLAN_INCORPORACION_AERONAVES_JSBSIM_SITL.md) -- same
        % real airframe, same reference lat/lon/alt, deliberately kept identical for a fair
        % side-by-side comparison between the two plants. c172pjsbsim's own plant (JSBSim itself)
        % integrates mass/inertia/aero internally, so aircraftInertialBody_kgm2/SRef_m2/etc below
        % are only used for its vestigial MassPropertiesBus (diagnostics only, not real dynamics)
        % -- see that plan's Phase C.
        %
        % Cessna 172P (JSBSim reference aircraft) -- see PLAN_INCORPORACION_AERONAVES_JSBSIM_SITL.md
        % Fase A-C. Real geometry/mass/inertia/aero sourced directly from
        % jsbsim/aircraft/c172p/c172p.xml (a published, community-validated JSBSim dataset), not an
        % engineering estimate like fixedwing_plane's Funray-derived linear derivatives. Unit
        % conversions per the plan's §2.4 table: 1 LBS=0.453592 kg, 1 FT=0.3048 m,
        % 1 SLUG*FT2=1.35581795 kg*m2.
        %
        % Mass: emptywt=1500 LBS + both fuel tanks at nominal "contents" (100+100=200 LBS) =
        % 1700 LBS. The <mass_balance> pointmass list (pilot=180 LBS, 4x baggage/passenger=0 LBS)
        % is deliberately NOT summed in -- this SIL flies every vehicle autonomously under PX4, not
        % with an onboard human pilot, so the pilot pointmass is dropped rather than kept as a
        % placeholder occupant (see plan §2.1 Fase A.3 recommendation).
        vehicleParams.dryMass_kg = 1700 * lbs2kg;
        % ixx/iyy/izz from c172p.xml <mass_balance> (ixy/ixz/iyz given as ~0 in the source file);
        % diagonal-only inertia, same simplification convention as every other vehicle in this
        % switch (see fixedwing_plane's case above).
        vehicleParams.aircraftInertialBody_kgm2 = [948*1.35581795 0 0; 0 1346*1.35581795 0; 0 0 1967*1.35581795];
        vehicleParams.aircraftCg_m = [0; 0; 0];
        vehicleParams.refCG_m = [0; 0; 0];
        % <metrics>: wingspan=35.8 FT, wingarea=174 FT2, chord=4.9 FT.
        vehicleParams.bRef_m = 35.8 * ft2m;
        vehicleParams.SRef_m2 = 174 * (ft2m^2);
        vehicleParams.cRef_m = 4.9 * ft2m;
        referenceAltitude_m = 116 * ft2m;
        referenceLatitude_deg = 17.64450;
        referenceLongitude_deg = -63.21988;
    otherwise
        error("setUpVehicle:NoPhysicsDefined", char("No real vehicle physics (mass/inertia/CG/aero) " + ...
            "defined yet for vehicleParams.type='" + setupVehicleType + "'. This vehicle previously " + ...
            "compiled silently using the hexarotor's physics via baseVehicleType fallback -- see " + ...
            "PLAN_CORRECCION_MULTIVEHICULO_SITL.md Fase 1/3. Add a case for '" + lower(setupVehicleType) + ...
            "' in this switch (or wire in vehicle/" + setupVehicleType + "/data/setUpVehicle_" + ...
            setupVehicleType + ".m if it already has real data) before re-enabling this vehicle."))
end

if strcmpi(setupVehicleType, "c172pjsbsim")
    % JSBSim initializes from c172pJSBSimInit.xml rather than aircraftInitialPosInNED_m. Keep the
    % Simulink reference at the same 4.7 ft runway-rest height used by the aircraft's stock
    % reset00.xml. Zero feet places the aircraft reference point (and therefore part of the
    % landing gear) below the ground plane, producing violent contact impulses and hundreds of
    % metres of false GPS-height motion before EKF2 can arm.
    referenceAltitude_m = 4.7 * ft2m;
end

aircraftInitialPosInNED_m = [0, 0, -referenceAltitude_m];
aircraftInitialVelInBody_mps = [0, 0, 0];
aircraftInitialEuler_rad = [0, 0, 1.9199];

aircraftInitialBodyRates_radps = [0, 0, 0];
referenceDecimalYear_years = 2024; % World magnetic model doesn't work in 2025
aircraftInitial.lat_deg = referenceLatitude_deg;
aircraftInitial.lon_deg = referenceLongitude_deg;
aircraftInitial.alt_m = aircraftInitialPosInNED_m(3);
aircraftInitial.yawAngle_rad = aircraftInitialEuler_rad(3);
terrainHeightNED_m = aircraftInitialPosInNED_m(3); 
