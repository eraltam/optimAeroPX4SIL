% failureBusDefinition:
% NOTE: switches on the specific vehicle key (vehicleParams.type), not baseVehicleType -- see
% PLAN_CORRECCION_MULTIVEHICULO_SITL.md Fase 1/2.
function FailureBusDefinition(vehicleType)
% Bus object: failureBus

switch lower(vehicleType)
    case "f16"
        % F16 servo command bus definition
        FailureF16BusDefinition

    case "hexarotor"
        FailureHexBusDefinition

    case {"quadrotor", "octarotor", "evtol", "evtol_simscape", "vtol_tailsitter"}
        % LADAC-family multirotor per-rotor failure bus -- see ServosCommandBusDefinition.m's
        % matching case for the reuse rationale (redefines the global 'FailureBus' type, same
        % pattern FailureHexBusDefinition already uses for hexarotor, just with N=4/8/6/2 elements).
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
            rotorElems(ii).Name = sprintf('rotorFailure%d_nd', ii);
            rotorElems(ii).Dimensions = 1;
            rotorElems(ii).DimensionsMode = 'Fixed';
            rotorElems(ii).DataType = 'boolean';
            rotorElems(ii).Complexity = 'real';
            rotorElems(ii).Min = [];
            rotorElems(ii).Max = [];
            rotorElems(ii).DocUnits = '';
            rotorElems(ii).Description = '';
        end
        FailureBus = Simulink.Bus;
        FailureBus.HeaderFile = '';
        FailureBus.Description = '';
        FailureBus.DataScope = 'Auto';
        FailureBus.Alignment = -1;
        FailureBus.PreserveElementDimensions = 0;
        FailureBus.Elements = rotorElems;
        clear rotorElems;
        assignin('base', 'FailureBus', FailureBus);

    case {"ship_surface", "ship_semisub"}
        % Same shape as ackermann_rover's failure bus (rudder/throttle instead of steering/
        % throttle) -- see PLAN_VEHICULOS_AEREOS_Y_MARINOS_SITL.md.
        elems(1) = Simulink.BusElement;
        elems(1).Name = 'steeringFailure_nd';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'boolean';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = 'rudder failure';

        elems(2) = Simulink.BusElement;
        elems(2).Name = 'throttleFailure_nd';
        elems(2).Dimensions = 1;
        elems(2).DimensionsMode = 'Fixed';
        elems(2).DataType = 'boolean';
        elems(2).Complexity = 'real';
        elems(2).Min = [];
        elems(2).Max = [];
        elems(2).DocUnits = '';
        elems(2).Description = '';

        FailureBus = Simulink.Bus;
        FailureBus.HeaderFile = '';
        FailureBus.Description = '';
        FailureBus.DataScope = 'Auto';
        FailureBus.Alignment = -1;
        FailureBus.PreserveElementDimensions = 0;
        FailureBus.Elements = elems;
        clear elems;
        assignin('base', 'FailureBus', FailureBus);

    case {"ackermann_rover", "ackermann_simscape"}
        % Minimal steering/throttle failure bus for the Fase 2 pilot vehicle -- see
        % PLAN_CORRECCION_MULTIVEHICULO_SITL.md.
        elems(1) = Simulink.BusElement;
        elems(1).Name = 'steeringFailure_nd';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'boolean';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        elems(2) = Simulink.BusElement;
        elems(2).Name = 'throttleFailure_nd';
        elems(2).Dimensions = 1;
        elems(2).DimensionsMode = 'Fixed';
        elems(2).DataType = 'boolean';
        elems(2).Complexity = 'real';
        elems(2).Min = [];
        elems(2).Max = [];
        elems(2).DocUnits = '';
        elems(2).Description = '';

        FailureBus = Simulink.Bus;
        FailureBus.HeaderFile = '';
        FailureBus.Description = '';
        FailureBus.DataScope = 'Auto';
        FailureBus.Alignment = -1;
        FailureBus.PreserveElementDimensions = 0;
        FailureBus.Elements = elems;
        clear elems;
        assignin('base', 'FailureBus', FailureBus);

    case {"differential_rover", "tracked_vehicle", "tracked_vehicle_simscape", "usv_surface", "uuv_subsea", "uuv_npsauv", "uuv_dsrv"}
        % Minimal left/right motor failure bus for the differential rover (Fase 3) -- see
        % PLAN_CORRECCION_MULTIVEHICULO_SITL.md.
        elems(1) = Simulink.BusElement;
        elems(1).Name = 'leftMotorFailure_nd';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'boolean';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        elems(2) = Simulink.BusElement;
        elems(2).Name = 'rightMotorFailure_nd';
        elems(2).Dimensions = 1;
        elems(2).DimensionsMode = 'Fixed';
        elems(2).DataType = 'boolean';
        elems(2).Complexity = 'real';
        elems(2).Min = [];
        elems(2).Max = [];
        elems(2).DocUnits = '';
        elems(2).Description = '';

        FailureBus = Simulink.Bus;
        FailureBus.HeaderFile = '';
        FailureBus.Description = '';
        FailureBus.DataScope = 'Auto';
        FailureBus.Alignment = -1;
        FailureBus.PreserveElementDimensions = 0;
        FailureBus.Elements = elems;
        clear elems;
        assignin('base', 'FailureBus', FailureBus);

    case "bicycle_rover"
        % Minimal steering/throttle failure bus for the bicycle rover (Fase 3) -- see
        % PLAN_CORRECCION_MULTIVEHICULO_SITL.md.
        elems(1) = Simulink.BusElement;
        elems(1).Name = 'steeringFailure_nd';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'boolean';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        elems(2) = Simulink.BusElement;
        elems(2).Name = 'throttleFailure_nd';
        elems(2).Dimensions = 1;
        elems(2).DimensionsMode = 'Fixed';
        elems(2).DataType = 'boolean';
        elems(2).Complexity = 'real';
        elems(2).Min = [];
        elems(2).Max = [];
        elems(2).DocUnits = '';
        elems(2).Description = '';

        FailureBus = Simulink.Bus;
        FailureBus.HeaderFile = '';
        FailureBus.Description = '';
        FailureBus.DataScope = 'Auto';
        FailureBus.Alignment = -1;
        FailureBus.PreserveElementDimensions = 0;
        FailureBus.Elements = elems;
        clear elems;
        assignin('base', 'FailureBus', FailureBus);

    case {"wheel_loader", "wheel_loader_simscape"}
        % Minimal steering/throttle failure bus for the wheel loader (Fase 3) -- see
        % PLAN_CORRECCION_MULTIVEHICULO_SITL.md.
        elems(1) = Simulink.BusElement;
        elems(1).Name = 'steeringFailure_nd';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'boolean';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        elems(2) = Simulink.BusElement;
        elems(2).Name = 'throttleFailure_nd';
        elems(2).Dimensions = 1;
        elems(2).DimensionsMode = 'Fixed';
        elems(2).DataType = 'boolean';
        elems(2).Complexity = 'real';
        elems(2).Min = [];
        elems(2).Max = [];
        elems(2).DocUnits = '';
        elems(2).Description = '';

        FailureBus = Simulink.Bus;
        FailureBus.HeaderFile = '';
        FailureBus.Description = '';
        FailureBus.DataScope = 'Auto';
        FailureBus.Alignment = -1;
        FailureBus.PreserveElementDimensions = 0;
        FailureBus.Elements = elems;
        clear elems;
        assignin('base', 'FailureBus', FailureBus);

    case "unicycle_rover"
        % Minimal left/right motor failure bus for the unicycle rover (Fase 3) -- see
        % PLAN_CORRECCION_MULTIVEHICULO_SITL.md.
        elems(1) = Simulink.BusElement;
        elems(1).Name = 'leftMotorFailure_nd';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'boolean';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        elems(2) = Simulink.BusElement;
        elems(2).Name = 'rightMotorFailure_nd';
        elems(2).Dimensions = 1;
        elems(2).DimensionsMode = 'Fixed';
        elems(2).DataType = 'boolean';
        elems(2).Complexity = 'real';
        elems(2).Min = [];
        elems(2).Max = [];
        elems(2).DocUnits = '';
        elems(2).Description = '';

        FailureBus = Simulink.Bus;
        FailureBus.HeaderFile = '';
        FailureBus.Description = '';
        FailureBus.DataScope = 'Auto';
        FailureBus.Alignment = -1;
        FailureBus.PreserveElementDimensions = 0;
        FailureBus.Elements = elems;
        clear elems;
        assignin('base', 'FailureBus', FailureBus);

    case {"fixedwing_plane", "c172p"}
        % Mirrors FailureF16BusDefinition's field names exactly (same failureInjection subsystem
        % topology, cloned from F16's actuators.slx) -- see
        % PLAN_VEHICULOS_AEREOS_Y_MARINOS_SITL.md. c172p
        % (PLAN_INCORPORACION_AERONAVES_JSBSIM_SITL.md) reuses this exact same bus.
        elems(1) = Simulink.BusElement;
        elems(1).Name = 'rudderFreeze_isTrue';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'boolean';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        elems(2) = Simulink.BusElement;
        elems(2).Name = 'elevatorFreeze_isTrue';
        elems(2).Dimensions = 1;
        elems(2).DimensionsMode = 'Fixed';
        elems(2).DataType = 'boolean';
        elems(2).Complexity = 'real';
        elems(2).Min = [];
        elems(2).Max = [];
        elems(2).DocUnits = '';
        elems(2).Description = '';

        elems(3) = Simulink.BusElement;
        elems(3).Name = 'aileronsFreeze_isTrue';
        elems(3).Dimensions = 1;
        elems(3).DimensionsMode = 'Fixed';
        elems(3).DataType = 'boolean';
        elems(3).Complexity = 'real';
        elems(3).Min = [];
        elems(3).Max = [];
        elems(3).DocUnits = '';
        elems(3).Description = '';

        elems(4) = Simulink.BusElement;
        elems(4).Name = 'engineFailure_isTrue';
        elems(4).Dimensions = 1;
        elems(4).DimensionsMode = 'Fixed';
        elems(4).DataType = 'boolean';
        elems(4).Complexity = 'real';
        elems(4).Min = [];
        elems(4).Max = [];
        elems(4).DocUnits = '';
        elems(4).Description = '';

        FailureBus = Simulink.Bus;
        FailureBus.HeaderFile = '';
        FailureBus.Description = '';
        FailureBus.DataScope = 'Auto';
        FailureBus.Alignment = -1;
        FailureBus.PreserveElementDimensions = 0;
        FailureBus.Elements = elems;
        clear elems;
        assignin('base', 'FailureBus', FailureBus);

    case "customAircraft"
        % Custom aircraft failureBus bus definition
        disp("Place Failure Bus for custom aircraft here")

    otherwise
        error("FailureBusDefinition:NoBusDefined", char("No FailureBus defined yet for vehicle '" + ...
            vehicleType + "'. See PLAN_CORRECCION_MULTIVEHICULO_SITL.md Fase 2."))

end






