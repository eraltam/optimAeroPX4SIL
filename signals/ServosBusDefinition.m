% servosBusDefinition: This function creates a bus that contains the actuator positions/dynamics after the corresponding
% servo commands have been processed.
% Note: For new vehicle designs, servo busses can be added here.
%
% NOTE: switches on the specific vehicle key (vehicleParams.type from the caller), not
% baseVehicleType -- see PLAN_CORRECCION_MULTIVEHICULO_SITL.md Fase 1/2.
function servosBusDefinition(vehicleType)

switch lower(vehicleType)
    case "f16"
        % Call bus definition for F16
        ServosBusDefinitionF16
        elems(1) = Simulink.BusElement;
        elems(1).Name = 'ServosF16Bus';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType   = 'Bus: ServosF16Bus';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        ServosBus = Simulink.Bus;
        ServosBus.HeaderFile = '';
        ServosBus.Description = '';
        ServosBus.DataScope = 'Auto';
        ServosBus.Alignment = -1;
        ServosBus.PreserveElementDimensions = 0;
        ServosBus.Elements = elems;
        clear elems;
        assignin('base', 'ServosBus', ServosBus);
    case "hexarotor"
        % Call bus definition for Hex
        ServosBusDefinitionHex
        elems(1) = Simulink.BusElement;
        elems(1).Name = 'ServosHexBus';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType   = 'Bus: ServosHexBus';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        ServosBus = Simulink.Bus;
        ServosBus.HeaderFile = '';
        ServosBus.Description = '';
        ServosBus.DataScope = 'Auto';
        ServosBus.Alignment = -1;
        ServosBus.PreserveElementDimensions = 0;
        ServosBus.Elements = elems;
        clear elems;
        assignin('base', 'ServosBus', ServosBus);
    case {"quadrotor", "octarotor", "evtol", "evtol_simscape"}
        % LADAC-family multirotor servo (rotor speed readback) bus -- same reuse rationale as
        % ServosCommandBusDefinition.m's matching case: reuses the global bus type name
        % 'ServosHexBus' so it stays compatible with any shared block that keys off that literal
        % name, redefined per active vehicle like every other global bus type in this file.
        switch lower(vehicleType)
            case "quadrotor"
                rotorCount = 4;
            case "octarotor"
                rotorCount = 8;
            case {"evtol", "evtol_simscape"}
                rotorCount = 6;
        end
        clear rotorElems;
        for ii = 1:rotorCount
            rotorElems(ii) = Simulink.BusElement;
            rotorElems(ii).Name = sprintf('rotor%d_rpm', ii);
            rotorElems(ii).Dimensions = 1;
            rotorElems(ii).DimensionsMode = 'Fixed';
            rotorElems(ii).DataType = 'double';
            rotorElems(ii).Complexity = 'real';
            rotorElems(ii).Min = [];
            rotorElems(ii).Max = [];
            rotorElems(ii).DocUnits = '';
            rotorElems(ii).Description = '';
        end
        ServosHexBus = Simulink.Bus;
        ServosHexBus.HeaderFile = '';
        ServosHexBus.Description = '';
        ServosHexBus.DataScope = 'Auto';
        ServosHexBus.Alignment = -1;
        ServosHexBus.PreserveElementDimensions = 0;
        ServosHexBus.Elements = rotorElems;
        clear rotorElems;
        assignin('base', 'ServosHexBus', ServosHexBus);

        elems(1) = Simulink.BusElement;
        elems(1).Name = 'ServosHexBus';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'Bus: ServosHexBus';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        ServosBus = Simulink.Bus;
        ServosBus.HeaderFile = '';
        ServosBus.Description = '';
        ServosBus.DataScope = 'Auto';
        ServosBus.Alignment = -1;
        ServosBus.PreserveElementDimensions = 0;
        ServosBus.Elements = elems;
        clear elems;
        assignin('base', 'ServosBus', ServosBus);
    case {"ackermann_rover", "ackermann_simscape"}
        % Call bus definition for the Ackermann steer+throttle rover -- see
        % PLAN_CORRECCION_MULTIVEHICULO_SITL.md Fase 2 (pilot vehicle for the new
        % steer+throttle actuator mapping class). Inlined directly, not called out to
        % signals/ackermann_rover/ServosBusDefinitionAckermannRover.m -- see the matching note in
        % ServosCommandBusDefinition.m's ackermann_rover case for why.
        clear innerElems;
        innerElems(1) = Simulink.BusElement;
        innerElems(1).Name = 'steeringAngle_rad';
        innerElems(1).Dimensions = 1;
        innerElems(1).DimensionsMode = 'Fixed';
        innerElems(1).DataType = 'double';
        innerElems(1).Complexity = 'real';
        innerElems(1).Min = [];
        innerElems(1).Max = [];
        innerElems(1).DocUnits = '';
        innerElems(1).Description = '';

        innerElems(2) = Simulink.BusElement;
        innerElems(2).Name = 'wheelSpeed_radps';
        innerElems(2).Dimensions = 1;
        innerElems(2).DimensionsMode = 'Fixed';
        innerElems(2).DataType = 'double';
        innerElems(2).Complexity = 'real';
        innerElems(2).Min = [];
        innerElems(2).Max = [];
        innerElems(2).DocUnits = '';
        innerElems(2).Description = '';

        ServosAckermannRoverBus = Simulink.Bus;
        ServosAckermannRoverBus.HeaderFile = '';
        ServosAckermannRoverBus.Description = '';
        ServosAckermannRoverBus.DataScope = 'Auto';
        ServosAckermannRoverBus.Alignment = -1;
        ServosAckermannRoverBus.PreserveElementDimensions = 0;
        ServosAckermannRoverBus.Elements = innerElems;
        clear innerElems;
        assignin('base', 'ServosAckermannRoverBus', ServosAckermannRoverBus);

        elems(1) = Simulink.BusElement;
        elems(1).Name = 'ServosAckermannRoverBus';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType   = 'Bus: ServosAckermannRoverBus';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        ServosBus = Simulink.Bus;
        ServosBus.HeaderFile = '';
        ServosBus.Description = '';
        ServosBus.DataScope = 'Auto';
        ServosBus.Alignment = -1;
        ServosBus.PreserveElementDimensions = 0;
        ServosBus.Elements = elems;
        clear elems;
        assignin('base', 'ServosBus', ServosBus);
    case {"differential_rover", "tracked_vehicle", "tracked_vehicle_simscape", "usv_surface", "uuv_subsea"}
        % Achieved (post actuator-dynamics) state for the differential/twin-motor rover -- see
        % PLAN_CORRECCION_MULTIVEHICULO_SITL.md Fase 2/3.
        clear innerElems;
        innerElems(1) = Simulink.BusElement;
        innerElems(1).Name = 'leftWheelSpeed_radps';
        innerElems(1).Dimensions = 1;
        innerElems(1).DimensionsMode = 'Fixed';
        innerElems(1).DataType = 'double';
        innerElems(1).Complexity = 'real';
        innerElems(1).Min = [];
        innerElems(1).Max = [];
        innerElems(1).DocUnits = '';
        innerElems(1).Description = '';

        innerElems(2) = Simulink.BusElement;
        innerElems(2).Name = 'rightWheelSpeed_radps';
        innerElems(2).Dimensions = 1;
        innerElems(2).DimensionsMode = 'Fixed';
        innerElems(2).DataType = 'double';
        innerElems(2).Complexity = 'real';
        innerElems(2).Min = [];
        innerElems(2).Max = [];
        innerElems(2).DocUnits = '';
        innerElems(2).Description = '';

        ServosDifferentialRoverBus = Simulink.Bus;
        ServosDifferentialRoverBus.HeaderFile = '';
        ServosDifferentialRoverBus.Description = '';
        ServosDifferentialRoverBus.DataScope = 'Auto';
        ServosDifferentialRoverBus.Alignment = -1;
        ServosDifferentialRoverBus.PreserveElementDimensions = 0;
        ServosDifferentialRoverBus.Elements = innerElems;
        clear innerElems;
        assignin('base', 'ServosDifferentialRoverBus', ServosDifferentialRoverBus);

        elems(1) = Simulink.BusElement;
        elems(1).Name = 'ServosDifferentialRoverBus';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType   = 'Bus: ServosDifferentialRoverBus';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        ServosBus = Simulink.Bus;
        ServosBus.HeaderFile = '';
        ServosBus.Description = '';
        ServosBus.DataScope = 'Auto';
        ServosBus.Alignment = -1;
        ServosBus.PreserveElementDimensions = 0;
        ServosBus.Elements = elems;
        clear elems;
        assignin('base', 'ServosBus', ServosBus);
    case "bicycle_rover"
        % Achieved (post actuator-dynamics) state for the bicycle steer+throttle rover (same shape
        % as ackermann_rover) -- see PLAN_CORRECCION_MULTIVEHICULO_SITL.md Fase 2/3.
        clear innerElems;
        innerElems(1) = Simulink.BusElement;
        innerElems(1).Name = 'steeringAngle_rad';
        innerElems(1).Dimensions = 1;
        innerElems(1).DimensionsMode = 'Fixed';
        innerElems(1).DataType = 'double';
        innerElems(1).Complexity = 'real';
        innerElems(1).Min = [];
        innerElems(1).Max = [];
        innerElems(1).DocUnits = '';
        innerElems(1).Description = '';

        innerElems(2) = Simulink.BusElement;
        innerElems(2).Name = 'wheelSpeed_radps';
        innerElems(2).Dimensions = 1;
        innerElems(2).DimensionsMode = 'Fixed';
        innerElems(2).DataType = 'double';
        innerElems(2).Complexity = 'real';
        innerElems(2).Min = [];
        innerElems(2).Max = [];
        innerElems(2).DocUnits = '';
        innerElems(2).Description = '';

        ServosBicycleRoverBus = Simulink.Bus;
        ServosBicycleRoverBus.HeaderFile = '';
        ServosBicycleRoverBus.Description = '';
        ServosBicycleRoverBus.DataScope = 'Auto';
        ServosBicycleRoverBus.Alignment = -1;
        ServosBicycleRoverBus.PreserveElementDimensions = 0;
        ServosBicycleRoverBus.Elements = innerElems;
        clear innerElems;
        assignin('base', 'ServosBicycleRoverBus', ServosBicycleRoverBus);

        elems(1) = Simulink.BusElement;
        elems(1).Name = 'ServosBicycleRoverBus';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType   = 'Bus: ServosBicycleRoverBus';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        ServosBus = Simulink.Bus;
        ServosBus.HeaderFile = '';
        ServosBus.Description = '';
        ServosBus.DataScope = 'Auto';
        ServosBus.Alignment = -1;
        ServosBus.PreserveElementDimensions = 0;
        ServosBus.Elements = elems;
        clear elems;
        assignin('base', 'ServosBus', ServosBus);
    case {"wheel_loader", "wheel_loader_simscape"}
        % Achieved (post actuator-dynamics) state for the wheel loader (same shape as
        % ackermann_rover) -- see PLAN_CORRECCION_MULTIVEHICULO_SITL.md Fase 2/3.
        clear innerElems;
        innerElems(1) = Simulink.BusElement;
        innerElems(1).Name = 'steeringAngle_rad';
        innerElems(1).Dimensions = 1;
        innerElems(1).DimensionsMode = 'Fixed';
        innerElems(1).DataType = 'double';
        innerElems(1).Complexity = 'real';
        innerElems(1).Min = [];
        innerElems(1).Max = [];
        innerElems(1).DocUnits = '';
        innerElems(1).Description = '';

        innerElems(2) = Simulink.BusElement;
        innerElems(2).Name = 'wheelSpeed_radps';
        innerElems(2).Dimensions = 1;
        innerElems(2).DimensionsMode = 'Fixed';
        innerElems(2).DataType = 'double';
        innerElems(2).Complexity = 'real';
        innerElems(2).Min = [];
        innerElems(2).Max = [];
        innerElems(2).DocUnits = '';
        innerElems(2).Description = '';

        ServosWheelLoaderBus = Simulink.Bus;
        ServosWheelLoaderBus.HeaderFile = '';
        ServosWheelLoaderBus.Description = '';
        ServosWheelLoaderBus.DataScope = 'Auto';
        ServosWheelLoaderBus.Alignment = -1;
        ServosWheelLoaderBus.PreserveElementDimensions = 0;
        ServosWheelLoaderBus.Elements = innerElems;
        clear innerElems;
        assignin('base', 'ServosWheelLoaderBus', ServosWheelLoaderBus);

        elems(1) = Simulink.BusElement;
        elems(1).Name = 'ServosWheelLoaderBus';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType   = 'Bus: ServosWheelLoaderBus';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        ServosBus = Simulink.Bus;
        ServosBus.HeaderFile = '';
        ServosBus.Description = '';
        ServosBus.DataScope = 'Auto';
        ServosBus.Alignment = -1;
        ServosBus.PreserveElementDimensions = 0;
        ServosBus.Elements = elems;
        clear elems;
        assignin('base', 'ServosBus', ServosBus);
    case "unicycle_rover"
        % Achieved (post actuator-dynamics) state for the differential/twin-motor rover (same
        % shape as differential_rover) -- see PLAN_CORRECCION_MULTIVEHICULO_SITL.md Fase 2/3.
        clear innerElems;
        innerElems(1) = Simulink.BusElement;
        innerElems(1).Name = 'leftWheelSpeed_radps';
        innerElems(1).Dimensions = 1;
        innerElems(1).DimensionsMode = 'Fixed';
        innerElems(1).DataType = 'double';
        innerElems(1).Complexity = 'real';
        innerElems(1).Min = [];
        innerElems(1).Max = [];
        innerElems(1).DocUnits = '';
        innerElems(1).Description = '';

        innerElems(2) = Simulink.BusElement;
        innerElems(2).Name = 'rightWheelSpeed_radps';
        innerElems(2).Dimensions = 1;
        innerElems(2).DimensionsMode = 'Fixed';
        innerElems(2).DataType = 'double';
        innerElems(2).Complexity = 'real';
        innerElems(2).Min = [];
        innerElems(2).Max = [];
        innerElems(2).DocUnits = '';
        innerElems(2).Description = '';

        ServosUnicycleRoverBus = Simulink.Bus;
        ServosUnicycleRoverBus.HeaderFile = '';
        ServosUnicycleRoverBus.Description = '';
        ServosUnicycleRoverBus.DataScope = 'Auto';
        ServosUnicycleRoverBus.Alignment = -1;
        ServosUnicycleRoverBus.PreserveElementDimensions = 0;
        ServosUnicycleRoverBus.Elements = innerElems;
        clear innerElems;
        assignin('base', 'ServosUnicycleRoverBus', ServosUnicycleRoverBus);

        elems(1) = Simulink.BusElement;
        elems(1).Name = 'ServosUnicycleRoverBus';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType   = 'Bus: ServosUnicycleRoverBus';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        ServosBus = Simulink.Bus;
        ServosBus.HeaderFile = '';
        ServosBus.Description = '';
        ServosBus.DataScope = 'Auto';
        ServosBus.Alignment = -1;
        ServosBus.PreserveElementDimensions = 0;
        ServosBus.Elements = elems;
        clear elems;
        assignin('base', 'ServosBus', ServosBus);
    case "fixedwing_plane"
        % Achieved (post actuator-dynamics) control-surface state for the generic ala-fija
        % vehicle -- see PLAN_VEHICULOS_AEREOS_Y_MARINOS_SITL.md. Mirrors ServosBusDefinitionF16's
        % field names exactly (posAileron_rad/posRudder_rad/posElevator_rad) since
        % fixedwing_plane's actuators.slx is a direct clone of F16's actuators.slx (same
        % servo/rate-limiter/failure-injection topology, only the engine Model Reference and the
        % ServosCommandBus type differ).
        clear innerElems;
        innerElems(1) = Simulink.BusElement;
        innerElems(1).Name = 'posAileron_rad';
        innerElems(1).Dimensions = 1;
        innerElems(1).DimensionsMode = 'Fixed';
        innerElems(1).DataType = 'double';
        innerElems(1).Complexity = 'real';
        innerElems(1).Min = [];
        innerElems(1).Max = [];
        innerElems(1).DocUnits = '';
        innerElems(1).Description = '';

        innerElems(2) = Simulink.BusElement;
        innerElems(2).Name = 'posRudder_rad';
        innerElems(2).Dimensions = 1;
        innerElems(2).DimensionsMode = 'Fixed';
        innerElems(2).DataType = 'double';
        innerElems(2).Complexity = 'real';
        innerElems(2).Min = [];
        innerElems(2).Max = [];
        innerElems(2).DocUnits = '';
        innerElems(2).Description = '';

        innerElems(3) = Simulink.BusElement;
        innerElems(3).Name = 'posElevator_rad';
        innerElems(3).Dimensions = 1;
        innerElems(3).DimensionsMode = 'Fixed';
        innerElems(3).DataType = 'double';
        innerElems(3).Complexity = 'real';
        innerElems(3).Min = [];
        innerElems(3).Max = [];
        innerElems(3).DocUnits = '';
        innerElems(3).Description = '';

        ServosFixedwingPlaneBus = Simulink.Bus;
        ServosFixedwingPlaneBus.HeaderFile = '';
        ServosFixedwingPlaneBus.Description = '';
        ServosFixedwingPlaneBus.DataScope = 'Auto';
        ServosFixedwingPlaneBus.Alignment = -1;
        ServosFixedwingPlaneBus.PreserveElementDimensions = 0;
        ServosFixedwingPlaneBus.Elements = innerElems;
        clear innerElems;
        assignin('base', 'ServosFixedwingPlaneBus', ServosFixedwingPlaneBus);

        elems(1) = Simulink.BusElement;
        elems(1).Name = 'ServosFixedwingPlaneBus';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType   = 'Bus: ServosFixedwingPlaneBus';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        ServosBus = Simulink.Bus;
        ServosBus.HeaderFile = '';
        ServosBus.Description = '';
        ServosBus.DataScope = 'Auto';
        ServosBus.Alignment = -1;
        ServosBus.PreserveElementDimensions = 0;
        ServosBus.Elements = elems;
        clear elems;
        assignin('base', 'ServosBus', ServosBus);
    case "customAircraft"
        % Call bus definition for Custom Aircraft
        ServosBusDefinitionCustomAircraft
        elems(1) = Simulink.BusElement;
        elems(1).Name = 'ServosCustomAircraftBus';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType   = 'Bus: ServosCustomAircraftBus';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        ServosBus = Simulink.Bus;
        ServosBus.HeaderFile = '';
        ServosBus.Description = '';
        ServosBus.DataScope = 'Auto';
        ServosBus.Alignment = -1;
        ServosBus.PreserveElementDimensions = 0;
        ServosBus.Elements = elems;
        clear elems;
        assignin('base', 'ServosBus', ServosBus);
    otherwise
        error("ServosBusDefinition:NoBusDefined", char("No ServosBus defined yet for vehicle '" + ...
            vehicleType + "'. See PLAN_CORRECCION_MULTIVEHICULO_SITL.md Fase 2 -- add a case here " + ...
            "(and a matching case in ServosCommandBusDefinition.m) before re-enabling this vehicle."))

end







