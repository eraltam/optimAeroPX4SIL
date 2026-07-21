% servosCommandBusDefinition: This function defines the servo command bus signals. If you're importing your
% own vehicle design, replace the servosCommandCustomAircraftBusDefinition with your own vehicle servo command
% bus definition.
%
% NOTE: switches on the specific vehicle key (vehicleParams.type from the caller), not
% baseVehicleType -- see PLAN_CORRECCION_MULTIVEHICULO_SITL.md Fase 1/2.
function ServosCommandBusDefinition(vehicleType)
% Bus object: ServosCommandBus

switch lower(vehicleType)
    case "f16"
        % F16 servo command bus definition
        ServosCommandF16BusDefinition
        elems(1) = Simulink.BusElement;
        elems(1).Name = 'ServosCommandF16Bus';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'Bus: ServosCommandF16Bus';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        ServosCommandBus = Simulink.Bus;
        ServosCommandBus.HeaderFile = '';
        ServosCommandBus.Description = '';
        ServosCommandBus.DataScope = 'Auto';
        ServosCommandBus.Alignment = -1;
        ServosCommandBus.PreserveElementDimensions = 0;
        ServosCommandBus.Elements = elems;
        clear elems;
        assignin('base', 'ServosCommandBus', ServosCommandBus);
    case "hexarotor"
        MotorCommandHexBusDefinition
        elems(1) = Simulink.BusElement;
        elems(1).Name = 'MotorCommandHexBus';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'Bus: MotorCommandHexBus';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        ServosCommandBus = Simulink.Bus;
        ServosCommandBus.HeaderFile = '';
        ServosCommandBus.Description = '';
        ServosCommandBus.DataScope = 'Auto';
        ServosCommandBus.Alignment = -1;
        ServosCommandBus.PreserveElementDimensions = 0;
        ServosCommandBus.Elements = elems;
        assignin('base', 'ServosCommandBus', ServosCommandBus);
    case {"quadrotor", "octarotor", "evtol", "evtol_simscape", "vtol_tailsitter"}
        % LADAC-family multirotor servo command bus (Fase 3 aerial track, see
        % PLAN_CORRECCION_MULTIVEHICULO_SITL.md). Reuses the exact same global bus TYPE NAME
        % 'MotorCommandHexBus' as hexarotor -- hexActuator.slx/hexMotorModel.slx (shared, untouched)
        % select/type-pin that literal name, and since it is redefined fresh per active vehicle just
        % like ServosBus/FailureBus/PropulsionBus already are, giving it N=4/8/6/2 elements here is
        % safe and does not affect hexarotor's own compile (which redefines it again with N=6 when
        % hexarotor is the active vehicle). This is what lets quadrotor/octarotor/evtol/
        % vtol_tailsitter reuse hexarotor's proven rigid-body-with-N-rotors physics with zero edits
        % to shared .slx files. vtol_tailsitter (PLAN_VEHICULOS_AEREOS_Y_MARINOS_SITL.md) models
        % only the twin-rotor hover regime this way -- true tailsitter wing-borne cruise/transition
        % aerodynamics is explicitly NOT modeled at this fidelity level, same documented-gap
        % pattern as evtol's "no tiltrotor transition" note in setUpVehicle.m.
        switch lower(vehicleType)
            case "quadrotor"
                rotorCount = 4;
            case "octarotor"
                rotorCount = 8;
            case {"evtol", "evtol_simscape"}
                rotorCount = 6;
            case "vtol_tailsitter"
                rotorCount = 2;
        end
        clear rotorElems;
        for ii = 1:rotorCount
            rotorElems(ii) = Simulink.BusElement;
            rotorElems(ii).Name = sprintf('cmdRotor%d_nd', ii);
            rotorElems(ii).Dimensions = 1;
            rotorElems(ii).DimensionsMode = 'Fixed';
            rotorElems(ii).DataType = 'double';
            rotorElems(ii).Complexity = 'real';
            rotorElems(ii).Min = [];
            rotorElems(ii).Max = [];
            rotorElems(ii).DocUnits = '';
            rotorElems(ii).Description = '';
        end
        MotorCommandHexBus = Simulink.Bus;
        MotorCommandHexBus.HeaderFile = '';
        MotorCommandHexBus.Description = '';
        MotorCommandHexBus.DataScope = 'Auto';
        MotorCommandHexBus.Alignment = -1;
        MotorCommandHexBus.PreserveElementDimensions = 0;
        MotorCommandHexBus.Elements = rotorElems;
        clear rotorElems;
        assignin('base', 'MotorCommandHexBus', MotorCommandHexBus);

        elems(1) = Simulink.BusElement;
        elems(1).Name = 'MotorCommandHexBus';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'Bus: MotorCommandHexBus';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        ServosCommandBus = Simulink.Bus;
        ServosCommandBus.HeaderFile = '';
        ServosCommandBus.Description = '';
        ServosCommandBus.DataScope = 'Auto';
        ServosCommandBus.Alignment = -1;
        ServosCommandBus.PreserveElementDimensions = 0;
        ServosCommandBus.Elements = elems;
        clear elems;
        assignin('base', 'ServosCommandBus', ServosCommandBus);
    case {"ship_surface", "ship_semisub"}
        % Single rudder+propeller ship servo command bus -- same field names/shape as
        % ackermann_rover's ServosCommandAckermannRoverBus (cmdSteering_nd/cmdThrottle_nd
        % reinterpreted as rudder/throttle) so ship_surface.slx can reuse ackermann_rover.slx's
        % block diagram verbatim -- see PLAN_VEHICULOS_AEREOS_Y_MARINOS_SITL.md.
        clear innerElems;
        innerElems(1) = Simulink.BusElement;
        innerElems(1).Name = 'cmdSteering_nd';
        innerElems(1).Dimensions = 1;
        innerElems(1).DimensionsMode = 'Fixed';
        innerElems(1).DataType = 'double';
        innerElems(1).Complexity = 'real';
        innerElems(1).Min = [];
        innerElems(1).Max = [];
        innerElems(1).DocUnits = '';
        innerElems(1).Description = 'commanded rudder, nd [-1,1]';

        innerElems(2) = Simulink.BusElement;
        innerElems(2).Name = 'cmdThrottle_nd';
        innerElems(2).Dimensions = 1;
        innerElems(2).DimensionsMode = 'Fixed';
        innerElems(2).DataType = 'double';
        innerElems(2).Complexity = 'real';
        innerElems(2).Min = [];
        innerElems(2).Max = [];
        innerElems(2).DocUnits = '';
        innerElems(2).Description = 'commanded throttle, nd [0,1]';

        ServosCommandShipSurfaceBus = Simulink.Bus;
        ServosCommandShipSurfaceBus.HeaderFile = '';
        ServosCommandShipSurfaceBus.Description = '';
        ServosCommandShipSurfaceBus.DataScope = 'Auto';
        ServosCommandShipSurfaceBus.Alignment = -1;
        ServosCommandShipSurfaceBus.PreserveElementDimensions = 0;
        ServosCommandShipSurfaceBus.Elements = innerElems;
        clear innerElems;
        assignin('base', 'ServosCommandShipSurfaceBus', ServosCommandShipSurfaceBus);

        elems(1) = Simulink.BusElement;
        elems(1).Name = 'ServosCommandShipSurfaceBus';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'Bus: ServosCommandShipSurfaceBus';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        ServosCommandBus = Simulink.Bus;
        ServosCommandBus.HeaderFile = '';
        ServosCommandBus.Description = '';
        ServosCommandBus.DataScope = 'Auto';
        ServosCommandBus.Alignment = -1;
        ServosCommandBus.PreserveElementDimensions = 0;
        ServosCommandBus.Elements = elems;
        clear elems;
        assignin('base', 'ServosCommandBus', ServosCommandBus);
    case {"ackermann_rover", "ackermann_simscape"}
        % Ackermann steer+throttle rover servo command bus definition -- see
        % PLAN_CORRECCION_MULTIVEHICULO_SITL.md Fase 2. Inlined directly (not called out to
        % signals/ackermann_rover/ServosCommandAckermannRoverBusDefinition.m as a separate function)
        % because this MATLAB session would not resolve a brand-new .m file by name via a bare
        % call/feval even after addpath+rehash, only via run() -- see the same class of "looks wired
        % in but isn't" trap documented for setUpVehicle_<key>.m in Fase 1.
        clear innerElems;
        innerElems(1) = Simulink.BusElement;
        innerElems(1).Name = 'cmdSteering_nd';
        innerElems(1).Dimensions = 1;
        innerElems(1).DimensionsMode = 'Fixed';
        innerElems(1).DataType = 'double';
        innerElems(1).Complexity = 'real';
        innerElems(1).Min = [];
        innerElems(1).Max = [];
        innerElems(1).DocUnits = '';
        innerElems(1).Description = '';

        innerElems(2) = Simulink.BusElement;
        innerElems(2).Name = 'cmdThrottle_nd';
        innerElems(2).Dimensions = 1;
        innerElems(2).DimensionsMode = 'Fixed';
        innerElems(2).DataType = 'double';
        innerElems(2).Complexity = 'real';
        innerElems(2).Min = [];
        innerElems(2).Max = [];
        innerElems(2).DocUnits = '';
        innerElems(2).Description = '';

        ServosCommandAckermannRoverBus = Simulink.Bus;
        ServosCommandAckermannRoverBus.HeaderFile = '';
        ServosCommandAckermannRoverBus.Description = '';
        ServosCommandAckermannRoverBus.DataScope = 'Auto';
        ServosCommandAckermannRoverBus.Alignment = -1;
        ServosCommandAckermannRoverBus.PreserveElementDimensions = 0;
        ServosCommandAckermannRoverBus.Elements = innerElems;
        clear innerElems;
        assignin('base', 'ServosCommandAckermannRoverBus', ServosCommandAckermannRoverBus);

        elems(1) = Simulink.BusElement;
        elems(1).Name = 'ServosCommandAckermannRoverBus';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'Bus: ServosCommandAckermannRoverBus';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        ServosCommandBus = Simulink.Bus;
        ServosCommandBus.HeaderFile = '';
        ServosCommandBus.Description = '';
        ServosCommandBus.DataScope = 'Auto';
        ServosCommandBus.Alignment = -1;
        ServosCommandBus.PreserveElementDimensions = 0;
        ServosCommandBus.Elements = elems;
        assignin('base', 'ServosCommandBus', ServosCommandBus);
    case {"differential_rover", "tracked_vehicle", "tracked_vehicle_simscape", "usv_surface", "uuv_subsea", "uuv_npsauv", "uuv_dsrv"}
        % Differential/twin-motor rover servo command bus definition -- see
        % PLAN_CORRECCION_MULTIVEHICULO_SITL.md Fase 2/3. Inlined for the same reason as
        % ackermann_rover above.
        clear innerElems;
        innerElems(1) = Simulink.BusElement;
        innerElems(1).Name = 'cmdLeftMotor_nd';
        innerElems(1).Dimensions = 1;
        innerElems(1).DimensionsMode = 'Fixed';
        innerElems(1).DataType = 'double';
        innerElems(1).Complexity = 'real';
        innerElems(1).Min = [];
        innerElems(1).Max = [];
        innerElems(1).DocUnits = '';
        innerElems(1).Description = '';

        innerElems(2) = Simulink.BusElement;
        innerElems(2).Name = 'cmdRightMotor_nd';
        innerElems(2).Dimensions = 1;
        innerElems(2).DimensionsMode = 'Fixed';
        innerElems(2).DataType = 'double';
        innerElems(2).Complexity = 'real';
        innerElems(2).Min = [];
        innerElems(2).Max = [];
        innerElems(2).DocUnits = '';
        innerElems(2).Description = '';

        ServosCommandDifferentialRoverBus = Simulink.Bus;
        ServosCommandDifferentialRoverBus.HeaderFile = '';
        ServosCommandDifferentialRoverBus.Description = '';
        ServosCommandDifferentialRoverBus.DataScope = 'Auto';
        ServosCommandDifferentialRoverBus.Alignment = -1;
        ServosCommandDifferentialRoverBus.PreserveElementDimensions = 0;
        ServosCommandDifferentialRoverBus.Elements = innerElems;
        clear innerElems;
        assignin('base', 'ServosCommandDifferentialRoverBus', ServosCommandDifferentialRoverBus);

        elems(1) = Simulink.BusElement;
        elems(1).Name = 'ServosCommandDifferentialRoverBus';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'Bus: ServosCommandDifferentialRoverBus';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        ServosCommandBus = Simulink.Bus;
        ServosCommandBus.HeaderFile = '';
        ServosCommandBus.Description = '';
        ServosCommandBus.DataScope = 'Auto';
        ServosCommandBus.Alignment = -1;
        ServosCommandBus.PreserveElementDimensions = 0;
        ServosCommandBus.Elements = elems;
        assignin('base', 'ServosCommandBus', ServosCommandBus);
    case "bicycle_rover"
        % Bicycle steer+throttle rover servo command bus definition (same shape as
        % ackermann_rover) -- see PLAN_CORRECCION_MULTIVEHICULO_SITL.md Fase 2/3.
        clear innerElems;
        innerElems(1) = Simulink.BusElement;
        innerElems(1).Name = 'cmdSteering_nd';
        innerElems(1).Dimensions = 1;
        innerElems(1).DimensionsMode = 'Fixed';
        innerElems(1).DataType = 'double';
        innerElems(1).Complexity = 'real';
        innerElems(1).Min = [];
        innerElems(1).Max = [];
        innerElems(1).DocUnits = '';
        innerElems(1).Description = '';

        innerElems(2) = Simulink.BusElement;
        innerElems(2).Name = 'cmdThrottle_nd';
        innerElems(2).Dimensions = 1;
        innerElems(2).DimensionsMode = 'Fixed';
        innerElems(2).DataType = 'double';
        innerElems(2).Complexity = 'real';
        innerElems(2).Min = [];
        innerElems(2).Max = [];
        innerElems(2).DocUnits = '';
        innerElems(2).Description = '';

        ServosCommandBicycleRoverBus = Simulink.Bus;
        ServosCommandBicycleRoverBus.HeaderFile = '';
        ServosCommandBicycleRoverBus.Description = '';
        ServosCommandBicycleRoverBus.DataScope = 'Auto';
        ServosCommandBicycleRoverBus.Alignment = -1;
        ServosCommandBicycleRoverBus.PreserveElementDimensions = 0;
        ServosCommandBicycleRoverBus.Elements = innerElems;
        clear innerElems;
        assignin('base', 'ServosCommandBicycleRoverBus', ServosCommandBicycleRoverBus);

        elems(1) = Simulink.BusElement;
        elems(1).Name = 'ServosCommandBicycleRoverBus';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'Bus: ServosCommandBicycleRoverBus';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        ServosCommandBus = Simulink.Bus;
        ServosCommandBus.HeaderFile = '';
        ServosCommandBus.Description = '';
        ServosCommandBus.DataScope = 'Auto';
        ServosCommandBus.Alignment = -1;
        ServosCommandBus.PreserveElementDimensions = 0;
        ServosCommandBus.Elements = elems;
        assignin('base', 'ServosCommandBus', ServosCommandBus);
    case {"wheel_loader", "wheel_loader_simscape"}
        % Wheel loader steer+throttle servo command bus definition (same shape as ackermann_rover)
        % -- see PLAN_CORRECCION_MULTIVEHICULO_SITL.md Fase 2/3.
        clear innerElems;
        innerElems(1) = Simulink.BusElement;
        innerElems(1).Name = 'cmdSteering_nd';
        innerElems(1).Dimensions = 1;
        innerElems(1).DimensionsMode = 'Fixed';
        innerElems(1).DataType = 'double';
        innerElems(1).Complexity = 'real';
        innerElems(1).Min = [];
        innerElems(1).Max = [];
        innerElems(1).DocUnits = '';
        innerElems(1).Description = '';

        innerElems(2) = Simulink.BusElement;
        innerElems(2).Name = 'cmdThrottle_nd';
        innerElems(2).Dimensions = 1;
        innerElems(2).DimensionsMode = 'Fixed';
        innerElems(2).DataType = 'double';
        innerElems(2).Complexity = 'real';
        innerElems(2).Min = [];
        innerElems(2).Max = [];
        innerElems(2).DocUnits = '';
        innerElems(2).Description = '';

        ServosCommandWheelLoaderBus = Simulink.Bus;
        ServosCommandWheelLoaderBus.HeaderFile = '';
        ServosCommandWheelLoaderBus.Description = '';
        ServosCommandWheelLoaderBus.DataScope = 'Auto';
        ServosCommandWheelLoaderBus.Alignment = -1;
        ServosCommandWheelLoaderBus.PreserveElementDimensions = 0;
        ServosCommandWheelLoaderBus.Elements = innerElems;
        clear innerElems;
        assignin('base', 'ServosCommandWheelLoaderBus', ServosCommandWheelLoaderBus);

        elems(1) = Simulink.BusElement;
        elems(1).Name = 'ServosCommandWheelLoaderBus';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'Bus: ServosCommandWheelLoaderBus';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        ServosCommandBus = Simulink.Bus;
        ServosCommandBus.HeaderFile = '';
        ServosCommandBus.Description = '';
        ServosCommandBus.DataScope = 'Auto';
        ServosCommandBus.Alignment = -1;
        ServosCommandBus.PreserveElementDimensions = 0;
        ServosCommandBus.Elements = elems;
        assignin('base', 'ServosCommandBus', ServosCommandBus);
    case "unicycle_rover"
        % Differential/twin-motor rover servo command bus definition (same shape as
        % differential_rover) -- see PLAN_CORRECCION_MULTIVEHICULO_SITL.md Fase 2/3.
        clear innerElems;
        innerElems(1) = Simulink.BusElement;
        innerElems(1).Name = 'cmdLeftMotor_nd';
        innerElems(1).Dimensions = 1;
        innerElems(1).DimensionsMode = 'Fixed';
        innerElems(1).DataType = 'double';
        innerElems(1).Complexity = 'real';
        innerElems(1).Min = [];
        innerElems(1).Max = [];
        innerElems(1).DocUnits = '';
        innerElems(1).Description = '';

        innerElems(2) = Simulink.BusElement;
        innerElems(2).Name = 'cmdRightMotor_nd';
        innerElems(2).Dimensions = 1;
        innerElems(2).DimensionsMode = 'Fixed';
        innerElems(2).DataType = 'double';
        innerElems(2).Complexity = 'real';
        innerElems(2).Min = [];
        innerElems(2).Max = [];
        innerElems(2).DocUnits = '';
        innerElems(2).Description = '';

        ServosCommandUnicycleRoverBus = Simulink.Bus;
        ServosCommandUnicycleRoverBus.HeaderFile = '';
        ServosCommandUnicycleRoverBus.Description = '';
        ServosCommandUnicycleRoverBus.DataScope = 'Auto';
        ServosCommandUnicycleRoverBus.Alignment = -1;
        ServosCommandUnicycleRoverBus.PreserveElementDimensions = 0;
        ServosCommandUnicycleRoverBus.Elements = innerElems;
        clear innerElems;
        assignin('base', 'ServosCommandUnicycleRoverBus', ServosCommandUnicycleRoverBus);

        elems(1) = Simulink.BusElement;
        elems(1).Name = 'ServosCommandUnicycleRoverBus';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'Bus: ServosCommandUnicycleRoverBus';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        ServosCommandBus = Simulink.Bus;
        ServosCommandBus.HeaderFile = '';
        ServosCommandBus.Description = '';
        ServosCommandBus.DataScope = 'Auto';
        ServosCommandBus.Alignment = -1;
        ServosCommandBus.PreserveElementDimensions = 0;
        ServosCommandBus.Elements = elems;
        assignin('base', 'ServosCommandBus', ServosCommandBus);
    case {"fixedwing_plane", "c172p"}
        % Generic ala-fija servo command bus -- mirrors ServosCommandF16BusDefinition's field
        % names exactly (cmdAileron_nd/cmdElevator_nd/cmdRudder_nd) since fixedwing_plane's
        % actuators.slx is a direct clone of F16's, only the ServosCommandBus type name and the
        % engine Model Reference differ -- see PLAN_VEHICULOS_AEREOS_Y_MARINOS_SITL.md.
        % c172p (PLAN_INCORPORACION_AERONAVES_JSBSIM_SITL.md) reuses this exact same bus -- same
        % 3-control-surface + throttle actuator interface as fixedwing_plane/F16.
        clear innerElems;
        innerElems(1) = Simulink.BusElement;
        innerElems(1).Name = 'cmdAileron_nd';
        innerElems(1).Dimensions = 1;
        innerElems(1).DimensionsMode = 'Fixed';
        innerElems(1).DataType = 'double';
        innerElems(1).Complexity = 'real';
        innerElems(1).Min = [];
        innerElems(1).Max = [];
        innerElems(1).DocUnits = '';
        innerElems(1).Description = '';

        innerElems(2) = Simulink.BusElement;
        innerElems(2).Name = 'cmdElevator_nd';
        innerElems(2).Dimensions = 1;
        innerElems(2).DimensionsMode = 'Fixed';
        innerElems(2).DataType = 'double';
        innerElems(2).Complexity = 'real';
        innerElems(2).Min = [];
        innerElems(2).Max = [];
        innerElems(2).DocUnits = '';
        innerElems(2).Description = '';

        innerElems(3) = Simulink.BusElement;
        innerElems(3).Name = 'cmdRudder_nd';
        innerElems(3).Dimensions = 1;
        innerElems(3).DimensionsMode = 'Fixed';
        innerElems(3).DataType = 'double';
        innerElems(3).Complexity = 'real';
        innerElems(3).Min = [];
        innerElems(3).Max = [];
        innerElems(3).DocUnits = '';
        innerElems(3).Description = '';

        ServosCommandFixedwingPlaneBus = Simulink.Bus;
        ServosCommandFixedwingPlaneBus.HeaderFile = '';
        ServosCommandFixedwingPlaneBus.Description = '';
        ServosCommandFixedwingPlaneBus.DataScope = 'Auto';
        ServosCommandFixedwingPlaneBus.Alignment = -1;
        ServosCommandFixedwingPlaneBus.PreserveElementDimensions = 0;
        ServosCommandFixedwingPlaneBus.Elements = innerElems;
        clear innerElems;
        assignin('base', 'ServosCommandFixedwingPlaneBus', ServosCommandFixedwingPlaneBus);

        elems(1) = Simulink.BusElement;
        elems(1).Name = 'ServosCommandFixedwingPlaneBus';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'Bus: ServosCommandFixedwingPlaneBus';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        ServosCommandBus = Simulink.Bus;
        ServosCommandBus.HeaderFile = '';
        ServosCommandBus.Description = '';
        ServosCommandBus.DataScope = 'Auto';
        ServosCommandBus.Alignment = -1;
        ServosCommandBus.PreserveElementDimensions = 0;
        ServosCommandBus.Elements = elems;
        clear elems;
        assignin('base', 'ServosCommandBus', ServosCommandBus);
    case "customAircraft"
        % Custom aircraft servo command bus definition
        ServosCommandCustomAircraftBusDefinition
        elems(1) = Simulink.BusElement;
        elems(1).Name = 'ServosCommandCustomAircraftBus';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'Bus: ServosCommandCustomAircraftBus';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        ServosCommandBus = Simulink.Bus;
        ServosCommandBus.HeaderFile = '';
        ServosCommandBus.Description = '';
        ServosCommandBus.DataScope = 'Auto';
        ServosCommandBus.Alignment = -1;
        ServosCommandBus.PreserveElementDimensions = 0;
        ServosCommandBus.Elements = elems;
        clear elems;
        assignin('base', 'ServosCommandBus', ServosCommandBus);

    otherwise
        error("ServosCommandBusDefinition:NoBusDefined", char("No ServosCommandBus defined yet for " + ...
            "vehicle '" + vehicleType + "'. See PLAN_CORRECCION_MULTIVEHICULO_SITL.md Fase 2 -- add a " + ...
            "case here (and a matching case in ServosBusDefinition.m) before re-enabling this vehicle."))

end






