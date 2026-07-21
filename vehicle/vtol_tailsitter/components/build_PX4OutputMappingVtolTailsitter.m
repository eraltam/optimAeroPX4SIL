%% build_PX4OutputMappingVtolTailsitter.m
% Programmatically creates
% vehicle/vtol_tailsitter/components/PX4OutputMappingVtolTailsitter.slx -- same structure as
% vehicle/hexarotor/components/PX4OutputMappingHex.slx (Selector-per-channel + BusAssignment onto
% a generic 'Bus: ServosCommandBus' Constant), but with only 2 Selectors (twin-rotor tailsitter,
% not 6), assigning into MotorCommandHexBus.cmdRotor1_nd/cmdRotor2_nd (the same bus TYPE NAME the
% quadrotor/octarotor/evtol family already reuses with N=4/8/6 elements -- here N=2, see
% signals/ServosCommandBusDefinition.m's "vtol_tailsitter" case).
%
% Channel convention: PWM_MAIN_FUNC1/2 = rotor1/rotor2, matching the twin-motor mixer convention
% already used by differential_rover/tracked_vehicle/usv_surface (FUNC1=101, FUNC2=102) -- see
% PLAN_VEHICULOS_AEREOS_Y_MARINOS_SITL.md.
%
% Prerequisites: BusDefinition('vtol_tailsitter') must have run at least once in this MATLAB
% session so 'ServosCommandBus' and 'MotorCommandHexBus' exist in the base workspace.

clear; clc;

rootDir = fileparts(mfilename('fullpath'));
modelName = 'PX4OutputMappingVtolTailsitter';
savePath  = fullfile(rootDir, [modelName '.slx']);

if bdIsLoaded(modelName)
    close_system(modelName, 0);
end

new_system(modelName);
set_param(modelName, 'SolverType', 'Fixed-step');

%% ====================================================================
%% INPORT
%% ====================================================================
add_block('simulink/Sources/In1', [modelName '/PX4 Output Commands'], ...
    'Position', [40, 100, 80, 120], 'Port', '1', 'PortDimensions', '16');

%% ====================================================================
%% SELECTORS (channel 1 -> rotor1, channel 2 -> rotor2)
%% ====================================================================
add_block('simulink/Signal Routing/Selector', [modelName '/Selector1'], ...
    'Position', [200, 60, 260, 80], 'Indices', '1', 'InputPortWidth', '16');
add_block('simulink/Signal Routing/Selector', [modelName '/Selector2'], ...
    'Position', [200, 140, 260, 160], 'Indices', '2', 'InputPortWidth', '16');

%% ====================================================================
%% BASE CONSTANT + BUS ASSIGNMENT
%% ====================================================================
add_block('simulink/Sources/Constant', [modelName '/ServosCommandBus'], ...
    'Position', [200, 220, 260, 240], 'Value', '0', 'OutDataTypeStr', 'Bus: ServosCommandBus');

add_block('simulink/Signal Routing/Bus Assignment', [modelName '/Bus Assignment4'], ...
    'Position', [340, 60, 400, 240], ...
    'AssignedSignals', 'MotorCommandHexBus.cmdRotor1_nd,MotorCommandHexBus.cmdRotor2_nd');

%% ====================================================================
%% OUTPORT
%% ====================================================================
add_block('simulink/Sinks/Out1', [modelName '/ServosCommandBusOut'], ...
    'Position', [440, 140, 480, 160], 'Port', '1', 'OutDataTypeStr', 'Bus: ServosCommandBus');

%% ====================================================================
%% CONNECT
%% ====================================================================
add_line(modelName, 'PX4 Output Commands/1', 'Selector1/1', 'autorouting', 'on');
add_line(modelName, 'PX4 Output Commands/1', 'Selector2/1', 'autorouting', 'on');
add_line(modelName, 'ServosCommandBus/1', 'Bus Assignment4/1', 'autorouting', 'on');
add_line(modelName, 'Selector1/1', 'Bus Assignment4/2', 'autorouting', 'on');
add_line(modelName, 'Selector2/1', 'Bus Assignment4/3', 'autorouting', 'on');
add_line(modelName, 'Bus Assignment4/1', 'ServosCommandBusOut/1', 'autorouting', 'on');

%% ====================================================================
%% CONFIG SET -- share the same external ConfigSetRef every other component uses (see
%% vehicle/fixedwing_plane/components/build_*.m for the matching pattern/rationale).
%% ====================================================================
if evalin('base', "exist('standardSILConfiguraitonParams','var')") == 1
    configSetRef = Simulink.ConfigSetRef;
    configSetRef.SourceName = 'standardSILConfiguraitonParams';
    attachConfigSet(modelName, configSetRef, true);
    setActiveConfigSet(modelName, configSetRef.Name);
else
    warning(['standardSILConfiguraitonParams not found in base workspace -- ' ...
        'leaving this model on its own local Fixed-step config.']);
end

%% ---- Save ----------------------------------------------------------
Simulink.BlockDiagram.arrangeSystem(modelName);
save_system(modelName, savePath);
fprintf('\nModel saved: %s\n', savePath);
