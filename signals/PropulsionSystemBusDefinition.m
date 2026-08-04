% propulsionSystemBusDefinition: This function creates a bus that contains data for any propulsion system used by the
% specified vehicle.
% NOTE: switches on the specific vehicle key (vehicleParams.type), not baseVehicleType -- see
% PLAN_CORRECCION_MULTIVEHICULO_SITL.md Fase 1/2.
function propulsionSystemBusDefinition(vehicleType)
% Bus object: PropulsionBus

switch lower(vehicleType)
    case "f16"
        % Bus object: PropulsionBus
        clear elems;
        elems(1) = Simulink.BusElement;
        elems(1).Name = 'angVel_radps';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'double';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        elems(2) = Simulink.BusElement;
        elems(2).Name = 'EngineForcesMomentsBus';
        elems(2).Dimensions = 1;
        elems(2).DimensionsMode = 'Fixed';
        elems(2).DataType = 'Bus: ComponentForcesMomentsBus';
        elems(2).Complexity = 'real';
        elems(2).Min = [];
        elems(2).Max = [];
        elems(2).DocUnits = '';
        elems(2).Description = '';

        elems(3) = Simulink.BusElement;
        elems(3).Name = 'fuelRate_kgps';
        elems(3).Dimensions = 1;
        elems(3).DimensionsMode = 'Fixed';
        elems(3).DataType = 'double';
        elems(3).Complexity = 'real';
        elems(3).Min = [];
        elems(3).Max = [];
        elems(3).DocUnits = '';
        elems(3).Description = '';

        PropulsionBus = Simulink.Bus;
        PropulsionBus.HeaderFile = '';
        PropulsionBus.Description = '';
        PropulsionBus.DataScope = 'Auto';
        PropulsionBus.Alignment = -1;
        PropulsionBus.PreserveElementDimensions = 0;
        PropulsionBus.Elements = elems;
        assignin('base', 'PropulsionBus', PropulsionBus);
    case "hexarotor"
        elems(1) = Simulink.BusElement;
        elems(1).Name = 'rotorAngVel_radps';
        elems(1).Dimensions = 6;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'double';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        elems(2) = Simulink.BusElement;
        elems(2).Name = 'EngineForcesMomentsBus';
        elems(2).Dimensions = 1;
        elems(2).DimensionsMode = 'Fixed';
        elems(2).DataType = 'Bus: ComponentForcesMomentsBus';
        elems(2).Complexity = 'real';
        elems(2).Min = [];
        elems(2).Max = [];
        elems(2).DocUnits = '';
        elems(2).Description = '';

        PropulsionBus = Simulink.Bus;
        PropulsionBus.HeaderFile = '';
        PropulsionBus.Description = '';
        PropulsionBus.DataScope = 'Auto';
        PropulsionBus.Alignment = -1;
        PropulsionBus.PreserveElementDimensions = 0;
        PropulsionBus.Elements = elems;
        assignin('base', 'PropulsionBus', PropulsionBus);
    case {"quadrotor", "octarotor", "evtol", "evtol_simscape", "vtol_tailsitter"}
        % LADAC-family multirotor propulsion bus -- same shape convention as hexarotor's own case
        % above (rotorAngVel_radps as an N-vector, matching hexMotorModel.slx's parametric For Each
        % output), just with N=4/8/6/2 depending on rotor count.
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
        elems(1) = Simulink.BusElement;
        elems(1).Name = 'rotorAngVel_radps';
        elems(1).Dimensions = rotorCount;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'double';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        elems(2) = Simulink.BusElement;
        elems(2).Name = 'EngineForcesMomentsBus';
        elems(2).Dimensions = 1;
        elems(2).DimensionsMode = 'Fixed';
        elems(2).DataType = 'Bus: ComponentForcesMomentsBus';
        elems(2).Complexity = 'real';
        elems(2).Min = [];
        elems(2).Max = [];
        elems(2).DocUnits = '';
        elems(2).Description = '';

        PropulsionBus = Simulink.Bus;
        PropulsionBus.HeaderFile = '';
        PropulsionBus.Description = '';
        PropulsionBus.DataScope = 'Auto';
        PropulsionBus.Alignment = -1;
        PropulsionBus.PreserveElementDimensions = 0;
        PropulsionBus.Elements = elems;
        clear elems;
        assignin('base', 'PropulsionBus', PropulsionBus);
    case {"ship_surface", "ship_semisub"}
        % Single propeller-shaft propulsion bus (Nomoto-class ship, subactuated) -- same shape as
        % ackermann_rover's -- see PLAN_VEHICULOS_AEREOS_Y_MARINOS_SITL.md.
        elems(1) = Simulink.BusElement;
        elems(1).Name = 'motorAngVel_radps';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'double';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = 'propeller shaft speed, rad/s';

        elems(2) = Simulink.BusElement;
        elems(2).Name = 'EngineForcesMomentsBus';
        elems(2).Dimensions = 1;
        elems(2).DimensionsMode = 'Fixed';
        elems(2).DataType = 'Bus: ComponentForcesMomentsBus';
        elems(2).Complexity = 'real';
        elems(2).Min = [];
        elems(2).Max = [];
        elems(2).DocUnits = '';
        elems(2).Description = '';

        PropulsionBus = Simulink.Bus;
        PropulsionBus.HeaderFile = '';
        PropulsionBus.Description = '';
        PropulsionBus.DataScope = 'Auto';
        PropulsionBus.Alignment = -1;
        PropulsionBus.PreserveElementDimensions = 0;
        PropulsionBus.Elements = elems;
        assignin('base', 'PropulsionBus', PropulsionBus);
    case {"ackermann_rover", "ackermann_simscape"}
        % Single drive-motor propulsion bus for the Fase 2 pilot vehicle -- see
        % PLAN_CORRECCION_MULTIVEHICULO_SITL.md.
        elems(1) = Simulink.BusElement;
        elems(1).Name = 'motorAngVel_radps';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'double';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        elems(2) = Simulink.BusElement;
        elems(2).Name = 'EngineForcesMomentsBus';
        elems(2).Dimensions = 1;
        elems(2).DimensionsMode = 'Fixed';
        elems(2).DataType = 'Bus: ComponentForcesMomentsBus';
        elems(2).Complexity = 'real';
        elems(2).Min = [];
        elems(2).Max = [];
        elems(2).DocUnits = '';
        elems(2).Description = '';

        PropulsionBus = Simulink.Bus;
        PropulsionBus.HeaderFile = '';
        PropulsionBus.Description = '';
        PropulsionBus.DataScope = 'Auto';
        PropulsionBus.Alignment = -1;
        PropulsionBus.PreserveElementDimensions = 0;
        PropulsionBus.Elements = elems;
        assignin('base', 'PropulsionBus', PropulsionBus);
    case {"differential_rover", "tracked_vehicle", "tracked_vehicle_simscape", "usv_surface", "uuv_subsea", "uuv_npsauv", "uuv_dsrv"}
        % Left/right drive-motor propulsion bus for the differential rover (Fase 3) -- see
        % PLAN_CORRECCION_MULTIVEHICULO_SITL.md.
        %
        % NOTE (F10, PLAN_CORRECCION_MULTIVEHICULO_SITL.md): the top-level global 'PropulsionBus'
        % name is reused verbatim inside the UNTOUCHED hexarotor.slx dynamics chain
        % (hexMotorModel.slx's own internal Bus Creator1 outputs directly to 'Bus: PropulsionBus',
        % not a hex-specific sub-bus name). VehiclePlant.slx's "plant" Variant Subsystem compiles
        % all 17 plant choices' Model References together (confirmed: VariantActivationTime does
        % not change this), so hexMotorModel.slx's Bus Creator1 gets type-checked against whatever
        % shape 'PropulsionBus' currently has -- and that block hardcodes exactly 2 input ports.
        % Until F10 gets a real structural fix (per-vehicle bus type namespacing), every vehicle's
        % PropulsionBus case MUST have exactly 2 top-level elements to avoid breaking hexarotor's
        % own (never-modified) plant. Left/right are therefore packed as a 2-element VECTOR inside
        % ONE bus element (motorAngVel_radps = [left; right]) instead of two scalar elements, to
        % satisfy hex's element-COUNT check while still carrying both values.
        elems(1) = Simulink.BusElement;
        elems(1).Name = 'motorAngVel_radps';
        elems(1).Dimensions = 2;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'double';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = 'motorAngVel_radps(1)=left, motorAngVel_radps(2)=right';

        elems(2) = Simulink.BusElement;
        elems(2).Name = 'EngineForcesMomentsBus';
        elems(2).Dimensions = 1;
        elems(2).DimensionsMode = 'Fixed';
        elems(2).DataType = 'Bus: ComponentForcesMomentsBus';
        elems(2).Complexity = 'real';
        elems(2).Min = [];
        elems(2).Max = [];
        elems(2).DocUnits = '';
        elems(2).Description = '';

        PropulsionBus = Simulink.Bus;
        PropulsionBus.HeaderFile = '';
        PropulsionBus.Description = '';
        PropulsionBus.DataScope = 'Auto';
        PropulsionBus.Alignment = -1;
        PropulsionBus.PreserveElementDimensions = 0;
        PropulsionBus.Elements = elems;
        assignin('base', 'PropulsionBus', PropulsionBus);
    case "bicycle_rover"
        % Single drive-motor propulsion bus for the bicycle rover (same shape as ackermann_rover)
        % -- see PLAN_CORRECCION_MULTIVEHICULO_SITL.md.
        elems(1) = Simulink.BusElement;
        elems(1).Name = 'motorAngVel_radps';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'double';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        elems(2) = Simulink.BusElement;
        elems(2).Name = 'EngineForcesMomentsBus';
        elems(2).Dimensions = 1;
        elems(2).DimensionsMode = 'Fixed';
        elems(2).DataType = 'Bus: ComponentForcesMomentsBus';
        elems(2).Complexity = 'real';
        elems(2).Min = [];
        elems(2).Max = [];
        elems(2).DocUnits = '';
        elems(2).Description = '';

        PropulsionBus = Simulink.Bus;
        PropulsionBus.HeaderFile = '';
        PropulsionBus.Description = '';
        PropulsionBus.DataScope = 'Auto';
        PropulsionBus.Alignment = -1;
        PropulsionBus.PreserveElementDimensions = 0;
        PropulsionBus.Elements = elems;
        assignin('base', 'PropulsionBus', PropulsionBus);
    case {"wheel_loader", "wheel_loader_simscape"}
        % Single drive-motor propulsion bus for the wheel loader (same shape as ackermann_rover)
        % -- see PLAN_CORRECCION_MULTIVEHICULO_SITL.md.
        elems(1) = Simulink.BusElement;
        elems(1).Name = 'motorAngVel_radps';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'double';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        elems(2) = Simulink.BusElement;
        elems(2).Name = 'EngineForcesMomentsBus';
        elems(2).Dimensions = 1;
        elems(2).DimensionsMode = 'Fixed';
        elems(2).DataType = 'Bus: ComponentForcesMomentsBus';
        elems(2).Complexity = 'real';
        elems(2).Min = [];
        elems(2).Max = [];
        elems(2).DocUnits = '';
        elems(2).Description = '';

        PropulsionBus = Simulink.Bus;
        PropulsionBus.HeaderFile = '';
        PropulsionBus.Description = '';
        PropulsionBus.DataScope = 'Auto';
        PropulsionBus.Alignment = -1;
        PropulsionBus.PreserveElementDimensions = 0;
        PropulsionBus.Elements = elems;
        assignin('base', 'PropulsionBus', PropulsionBus);
    case "unicycle_rover"
        % Left/right drive-motor propulsion bus for the unicycle rover (same shape as
        % differential_rover: 2-element vector packing left/right) -- see
        % PLAN_CORRECCION_MULTIVEHICULO_SITL.md.
        elems(1) = Simulink.BusElement;
        elems(1).Name = 'motorAngVel_radps';
        elems(1).Dimensions = 2;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'double';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = 'motorAngVel_radps(1)=left, motorAngVel_radps(2)=right';

        elems(2) = Simulink.BusElement;
        elems(2).Name = 'EngineForcesMomentsBus';
        elems(2).Dimensions = 1;
        elems(2).DimensionsMode = 'Fixed';
        elems(2).DataType = 'Bus: ComponentForcesMomentsBus';
        elems(2).Complexity = 'real';
        elems(2).Min = [];
        elems(2).Max = [];
        elems(2).DocUnits = '';
        elems(2).Description = '';

        PropulsionBus = Simulink.Bus;
        PropulsionBus.HeaderFile = '';
        PropulsionBus.Description = '';
        PropulsionBus.DataScope = 'Auto';
        PropulsionBus.Alignment = -1;
        PropulsionBus.PreserveElementDimensions = 0;
        PropulsionBus.Elements = elems;
        assignin('base', 'PropulsionBus', PropulsionBus);
    case {"fixedwing_plane", "c172p", "c172pjsbsim", "c130jsbsim", "f22jsbsim"}
        % Single tractor-propeller propulsion bus, same shape as F16's own PropulsionBus case
        % above (angVel_radps scalar + EngineForcesMomentsBus + fuelRate_kgps=0, electric motor)
        % -- see PLAN_VEHICULOS_AEREOS_Y_MARINOS_SITL.md. c172p
        % (PLAN_INCORPORACION_AERONAVES_JSBSIM_SITL.md) reuses this exact same bus shape --
        % fuelRate_kgps is nonzero for c172p's real piston engine (see setUpActuators.m's "c172p"
        % case / build_enginePistonPropC172p.m), unlike fixedwing_plane's electric motor.
        clear elems;
        elems(1) = Simulink.BusElement;
        elems(1).Name = 'angVel_radps';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'double';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        elems(2) = Simulink.BusElement;
        elems(2).Name = 'EngineForcesMomentsBus';
        elems(2).Dimensions = 1;
        elems(2).DimensionsMode = 'Fixed';
        elems(2).DataType = 'Bus: ComponentForcesMomentsBus';
        elems(2).Complexity = 'real';
        elems(2).Min = [];
        elems(2).Max = [];
        elems(2).DocUnits = '';
        elems(2).Description = '';

        elems(3) = Simulink.BusElement;
        elems(3).Name = 'fuelRate_kgps';
        elems(3).Dimensions = 1;
        elems(3).DimensionsMode = 'Fixed';
        elems(3).DataType = 'double';
        elems(3).Complexity = 'real';
        elems(3).Min = [];
        elems(3).Max = [];
        elems(3).DocUnits = '';
        elems(3).Description = '';

        PropulsionBus = Simulink.Bus;
        PropulsionBus.HeaderFile = '';
        PropulsionBus.Description = '';
        PropulsionBus.DataScope = 'Auto';
        PropulsionBus.Alignment = -1;
        PropulsionBus.PreserveElementDimensions = 0;
        PropulsionBus.Elements = elems;
        assignin('base', 'PropulsionBus', PropulsionBus);
    case "customAircraft"
        % Custom aircraft servo command bus definition
        servosCommandCustomAircraftBusDefinition
        elems(1) = Simulink.BusElement;
        elems(1).Name = 'PropulsionSystemCustomAircraftBus';
        elems(1).Dimensions = 1;
        elems(1).DimensionsMode = 'Fixed';
        elems(1).DataType = 'Bus: PropulsionSystemCustomAircraftBus';
        elems(1).Complexity = 'real';
        elems(1).Min = [];
        elems(1).Max = [];
        elems(1).DocUnits = '';
        elems(1).Description = '';

        PropulsionBus = Simulink.Bus;
        PropulsionBus.HeaderFile = '';
        PropulsionBus.Description = '';
        PropulsionBus.DataScope = 'Auto';
        PropulsionBus.Alignment = -1;
        PropulsionBus.PreserveElementDimensions = 0;
        PropulsionBus.Elements = elems;
        clear elems;
        assignin('base', 'ServosCommandBus', PropulsionBus);

    otherwise
        error("PropulsionSystemBusDefinition:NoBusDefined", char("No PropulsionBus defined yet for " + ...
            "vehicle '" + vehicleType + "'. See PLAN_CORRECCION_MULTIVEHICULO_SITL.md Fase 2."))

end






