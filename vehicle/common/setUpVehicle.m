% This script populates the base workspace with vehicle parameters for the specified vehicle. This script also includes
% the data describing the initial location of the vehicle in the ismulation.
switch lower(vehicleParams.type)
    case "f-16"
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
        % --- Mass configuration (fixed 2026-07-30, see
        % PLAN_HEXAROTOR_VEHICLE_PLANT_AUDIT_AND_VIBRATION_MODEL.md Section 3.2) ---
        % The prior value here (`180 * lbs2kg` = 81.65 kg, with inertia diag([11.8 11.8 23.5])
        % scaled for that same mass) was off by roughly 7-19x from every real named configuration
        % of this vehicle -- there is no record of where "180 lbs" came from; it does not match
        % this project's own documentation and was almost certainly a leftover template default.
        % Masses below are Class 1/2 (provided/measured, per the plan's parameter-confidence
        % table). If vehicleParams.massConfig is not set upstream (e.g. a caller that predates this
        % fix), default to the standard-battery + sensors/mounts case, since that is the "nominal
        % standard experimental configuration" this project actually flies.
        if ~isfield(vehicleParams, 'massConfig') || strlength(vehicleParams.massConfig) == 0
            vehicleParams.massConfig = "StandardBatterySensorsMounts";
        end
        switch lower(vehicleParams.massConfig)
            case "empty"
                vehicleParams.dryMass_kg = 4.200;
            case "standardbattery"
                vehicleParams.dryMass_kg = 6.725;
            case "longendurancebattery"
                vehicleParams.dryMass_kg = 7.200;
            case "standardbatterysensorsmounts"
                vehicleParams.dryMass_kg = 7.441;
            case "longendurancebatterysensorsmounts"
                vehicleParams.dryMass_kg = 7.916;
            otherwise
                error(char(["unknown vehicleParams.massConfig: " vehicleParams.massConfig ...
                    " (expected one of Empty, StandardBattery, LongEnduranceBattery, " ...
                    "StandardBatterySensorsMounts, LongEnduranceBatterySensorsMounts)"]))
        end

        % Rotor-center radius (0.4992 m) is Class 1 (provided spec value); armMassFraction and
        % bodyRadius_m are Class 3 engineering estimates -- see estimateHexInertia.m. This REPLACES
        % the old fixed diag([11.8 11.8 23.5]) kg*m^2, which was sized for the wrong (81.65 kg)
        % mass and is not reusable at any corrected mass by simple scaling.
        vehicleParams.aircraftInertialBody_kgm2 = estimateHexInertia(vehicleParams.dryMass_kg, 0.4992);

        % --- Ground-contact damper gains (fixed 2026-07-31, root-caused after a live HITL test
        % diverged with the corrected mass/inertia above) ---
        % hexGroundContact.slx has three simplified contact dampers (each with its own TODO
        % annotation admitting it's an approximation), all originally hardcoded and therefore
        % mass/inertia-INDEPENDENT by construction:
        %   1. computeGroundForces/Gain: torque = Gain*angularVelocity (was -100)
        %   2. groundForceX, groundForceY: force = Gain*velocity, ground friction (was -500 each)
        %   3. groundForceZ/Gain2: force = Gain*downwardVelocity, ground-penetration damper (was -5000)
        % All three were implicitly tuned for the OLD, wrong 81.65 kg / diag([11.8 11.8 23.5]) kg*m^2
        % configuration. Checked numerically (discrete stability factor |1 - gain*dt/(mass or
        % inertia)|, stepSize_s=0.004 s) at the corrected ~7.4 kg / ~0.59 kg*m^2 mean values:
        % the angular damper (#1) and friction dampers (#2) stay numerically stable either way
        % (factors 0.33 and 0.73, vs. 0.97 before -- overdamped, not divergent) -- but the
        % penetration damper (#3) does NOT: factor = 1.69 (>1, confirmed divergent), which is the
        % actual, verified root cause of the live-test failure (zero actuator commands the whole
        % session, yet ~9.8 km position drift, a false isVehicleAirborne trip, and abnormally
        % large accel variance -- all consistent with an unstable vertical contact force, not a
        % real flight). All three are recomputed here to preserve their OLD characteristic
        % rate/time-constant (gain/mass or gain/inertia) at whatever configuration is active, so
        % this class of bug can't silently reappear the next time the mass configuration changes --
        % not just patching the one that happened to be provably unstable this time.
        oldMass_ref_kg = 81.6466;  % 180 * lbs2kg, the prior (wrong) hardcoded mass
        oldMeanInertia_ref_kgm2 = mean([11.8, 11.8, 23.5]);
        vehicleParams.groundContact.angRateDampingGain = ...
            mean(diag(vehicleParams.aircraftInertialBody_kgm2)) * (100 / oldMeanInertia_ref_kgm2);
        vehicleParams.groundContact.linVelDampingGainXY = ...
            vehicleParams.dryMass_kg * (500 / oldMass_ref_kg);
        vehicleParams.groundContact.linVelDampingGainZ = ...
            vehicleParams.dryMass_kg * (5000 / oldMass_ref_kg);

        % CG is still assumed at the geometric center ([0;0;0]) -- Class 3 simplification, not a
        % measurement. The real CG must shift with battery/payload/sensor/mount position (brief
        % Section 4.2); component positions needed to compute that offset are not yet available
        % (see the plan's prioritized-measurement list). Revisit once measured/estimated component
        % positions exist -- do not silently invent an offset with false precision in the meantime.
        vehicleParams.aircraftCg_m = [0; 0; 0];
        vehicleParams.refCG_m = [0; 0; 0]; % reference cg
        vehicleParams.SRef_m2 = 0.5;
        aeroDataHex
        % TEMPORARY override for nav_hil_ground_truth.ulg replay: matches that log's actual
        % first valid GPS fix (fix_type>=3) so mission.yaml's reconstructed waypoints (from the
        % same flight) are physically reachable instead of far from home. Revert to the
        % Saba/Juancho E. Yrausquin Airport values below for any other hexarotor test.
        % NOTE: updated 2026-07-10 to match the *current* (shorter, ~538s) nav_hil_ground_truth.ulg
        % -- the previous override (37.336, -121.821) was for a different, since-replaced 2-hour
        % ground-truth log and no longer matches. Re-check this any time nav_hil_ground_truth.ulg
        % or mission.yaml change.
        referenceAltitude_m = 11.200000000000001;
        referenceLatitude_deg = 37.4128378;
        referenceLongitude_deg = -122.0001884;
        % --- Original Saba/Juancho E. Yrausquin Airport helipad values, restore when done: ---
        % referenceAltitude_m = 116 * ft2m;
        % referenceLatitude_deg = 17.64450;
        % referenceLongitude_deg = -63.21988;
    otherwise
        error(char(["unknown vehicle: " vehicleType]))
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