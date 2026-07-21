%% build_failureInputReadVtolTailsitter.m
% Programmatically creates
% vehicle/vtol_tailsitter/components/failureInputReadVtolTailsitter.slx -- same interface contract
% and topology as vehicle/hexarotor/components/failureInputReadHex.slx (joystick button -> sticky
% latch -> AND with per-rotor failureType comparison -> FailureBus), but for 2 rotors instead of 6.
% Reuses EnumHexFailureType (motor1/motor2 members) rather than defining a new
% EnumVtolTailsitterFailureType, since this is a symbolic constant, not vehicle-specific data --
% see PLAN_VEHICULOS_AEREOS_Y_MARINOS_SITL.md.
%
%   Inputs:  analog, buttons, POV, failureType
%   Outputs: FailureBus (Bus: FailureBus -- {rotorFailure1_nd, rotorFailure2_nd} for
%            vtol_tailsitter, see signals/FailureBusDefinition.m)
%
% Prerequisites: BusDefinition('vtol_tailsitter') must have run at least once in this MATLAB
% session so 'FailureBus' exists in the base workspace with the 2-element shape.

clear; clc;

rootDir = fileparts(mfilename('fullpath'));
modelName = 'failureInputReadVtolTailsitter';
savePath  = fullfile(rootDir, [modelName '.slx']);

if bdIsLoaded(modelName)
    close_system(modelName, 0);
end

new_system(modelName);
set_param(modelName, 'SolverType', 'Fixed-step');

%% ====================================================================
%% INPORTS
%% ====================================================================
add_block('simulink/Sources/In1', [modelName '/analog'], 'Position', [40, 40, 80, 60], 'Port', '1', ...
    'PortDimensions', '6');
add_block('simulink/Sources/In1', [modelName '/buttons'], 'Position', [40, 100, 80, 120], 'Port', '2', ...
    'OutDataTypeStr', 'uint32');
add_block('simulink/Sources/In1', [modelName '/POV'], 'Position', [40, 160, 80, 180], 'Port', '3');
add_block('simulink/Sources/In1', [modelName '/failureType'], 'Position', [40, 220, 80, 240], 'Port', '4', ...
    'OutDataTypeStr', 'Enum: EnumHexFailureType');

%% ====================================================================
%% UNUSED INPUTS
%% ====================================================================
add_block('simulink/Sinks/Terminator', [modelName '/Terminator'], 'Position', [120, 44, 140, 56]);
add_block('simulink/Sinks/Terminator', [modelName '/Terminator1'], 'Position', [120, 164, 140, 176]);

%% ====================================================================
%% JOYSTICK BUTTON -> STICKY LATCH ("failureTrue")
%% ====================================================================
add_block('simulink/Logic and Bit Operations/Compare To Constant', [modelName '/CompareToConstant1'], ...
    'Position', [140, 100, 220, 120], 'relop', '==', 'const', 'opts.failureInjectionButton');
add_block('simulink/Signal Routing/Switch', [modelName '/LatchSwitch'], ...
    'Position', [260, 90, 300, 130], 'Criteria', 'u2 > Threshold', 'Threshold', '0');
add_block('simulink/Discrete/Memory', [modelName '/LatchMemory'], 'Position', [320, 150, 350, 170]);

%% ====================================================================
%% PER-ROTOR FAILURE COMPARISONS
%% ====================================================================
add_block('simulink/Logic and Bit Operations/Compare To Constant', [modelName '/CompareToConstant4'], ...
    'Position', [140, 260, 220, 280], 'relop', '==', 'const', 'EnumHexFailureType.motor1');
add_block('simulink/Logic and Bit Operations/Compare To Constant', [modelName '/CompareToConstant2'], ...
    'Position', [140, 320, 220, 340], 'relop', '==', 'const', 'EnumHexFailureType.motor2');

add_block('simulink/Logic and Bit Operations/Logical Operator', [modelName '/AND1'], ...
    'Position', [400, 260, 440, 280], 'Operator', 'AND', 'Inputs', '2');
add_block('simulink/Logic and Bit Operations/Logical Operator', [modelName '/AND2'], ...
    'Position', [400, 320, 440, 340], 'Operator', 'AND', 'Inputs', '2');

%% ====================================================================
%% BUS CREATOR + OUTPORT
%% ====================================================================
add_block('simulink/Signal Routing/Bus Creator', [modelName '/BusCreator1'], ...
    'Position', [480, 270, 540, 330], 'Inputs', '2', 'UseBusObject', 'on', 'BusObject', 'FailureBus');
add_block('simulink/Sinks/Out1', [modelName '/FailureBus'], 'Position', [580, 290, 620, 310], 'Port', '1', ...
    'OutDataTypeStr', 'Bus: FailureBus');

%% ====================================================================
%% CONNECT
%% ====================================================================
add_line(modelName, 'analog/1', 'Terminator/1', 'autorouting', 'on');
add_line(modelName, 'POV/1', 'Terminator1/1', 'autorouting', 'on');

add_line(modelName, 'buttons/1', 'CompareToConstant1/1', 'autorouting', 'on');
add_line(modelName, 'CompareToConstant1/1', 'LatchSwitch/1', 'autorouting', 'on');
add_line(modelName, 'CompareToConstant1/1', 'LatchSwitch/2', 'autorouting', 'on');
add_line(modelName, 'LatchMemory/1', 'LatchSwitch/3', 'autorouting', 'on');
add_line(modelName, 'LatchSwitch/1', 'LatchMemory/1', 'autorouting', 'on');
add_line(modelName, 'LatchSwitch/1', 'AND1/1', 'autorouting', 'on');
add_line(modelName, 'LatchSwitch/1', 'AND2/1', 'autorouting', 'on');

add_line(modelName, 'failureType/1', 'CompareToConstant4/1', 'autorouting', 'on');
add_line(modelName, 'failureType/1', 'CompareToConstant2/1', 'autorouting', 'on');
add_line(modelName, 'CompareToConstant4/1', 'AND1/2', 'autorouting', 'on');
add_line(modelName, 'CompareToConstant2/1', 'AND2/2', 'autorouting', 'on');

add_line(modelName, 'AND1/1', 'BusCreator1/1', 'autorouting', 'on');
add_line(modelName, 'AND2/1', 'BusCreator1/2', 'autorouting', 'on');
add_line(modelName, 'BusCreator1/1', 'FailureBus/1', 'autorouting', 'on');

%% ====================================================================
%% CONFIG SET
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
