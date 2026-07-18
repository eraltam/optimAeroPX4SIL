%% build_enginePropFixedwing.m
% Programmatically creates
% vehicle/fixedwing_plane/components/enginePropFixedwing.slx -- a drop-in
% replacement for vehicle/F16/components/engineModelF16.slx (single-engine
% turbojet, Stevens & Lewis THRUST/TGEAR/PDOT tables) with the EXACT SAME
% interface contract, so it can be swapped into a clone of
% vehicle/F16/components/actuators.slx's engine Model Reference block without
% touching anything else in that file:
%
%   Inputs:  EngineCommandBus, controllerArmed, AirDataBus, EnvironmentBus,
%            BodyStateBus, failureBus
%   Outputs: PropulsionBus (Bus: PropulsionBus -- for "fixedwing_plane" this is
%            {angVel_radps, EngineForcesMomentsBus, fuelRate_kgps}, same shape
%            as F16's own PropulsionBus case in
%            signals/PropulsionSystemBusDefinition.m)
%
% Models a single electric motor + tractor propeller instead of a turbojet:
% throttle command -> first-order motor/ESC lag -> propeller angular velocity
% -> thrust/torque via the same kct_nd/kcp_nd empirical-coefficient style
% already used by this project's multirotor rotor models (T=kct*omega^2,
% Q=kcp*omega^2). Thrust acts purely along +X body axis (tractor prop on the
% nose); reaction torque acts about the body X axis, opposing the propeller's
% own spin direction. fuelRate_kgps=0 (electric, no fuel burn -- MassProperties
% stays constant, same simplification as every other electric-motor vehicle in
% this repo). See vehicle/common/setUpActuators.m's "fixedwing_plane" case for
% vehicleParams.prop.* parameter sourcing/rationale.
%
% Prerequisites: BusDefinition('fixedwing_plane') must have been run at least
% once in this MATLAB session so 'ComponentForcesMomentsBus' and
% 'PropulsionBus' exist in the base workspace before this model is compiled/
% simulated (not required to just build+save the .slx file itself).

clear; clc;

rootDir = fileparts(mfilename('fullpath'));
modelName = 'enginePropFixedwing';
savePath  = fullfile(rootDir, [modelName '.slx']);

if bdIsLoaded(modelName)
    close_system(modelName, 0);
end

new_system(modelName);
set_param(modelName, 'SolverType', 'Fixed-step');

%% ====================================================================
%% INPORTS (order matches engineModelF16.slx exactly)
%% ====================================================================
add_block('simulink/Sources/In1', [modelName '/EngineCommandBus'], ...
    'Position', [40, 40, 80, 60], 'Port', '1', ...
    'OutDataTypeStr', 'Bus: EngineCommandBus');
add_block('simulink/Sources/In1', [modelName '/controllerArmed'], ...
    'Position', [40, 100, 80, 120], 'Port', '2');
add_block('simulink/Sources/In1', [modelName '/AirDataBus'], ...
    'Position', [40, 160, 80, 180], 'Port', '3', ...
    'OutDataTypeStr', 'Bus: AirDataBus');
add_block('simulink/Sources/In1', [modelName '/EnvironmentBus'], ...
    'Position', [40, 220, 80, 240], 'Port', '4', ...
    'OutDataTypeStr', 'Bus: EnvironmentBus');
add_block('simulink/Sources/In1', [modelName '/BodyStateBus'], ...
    'Position', [40, 280, 80, 300], 'Port', '5', ...
    'OutDataTypeStr', 'Bus: BodyStateBus');
add_block('simulink/Sources/In1', [modelName '/failureBus'], ...
    'Position', [40, 340, 80, 360], 'Port', '6', ...
    'OutDataTypeStr', 'Bus: FailureBus');

%% ====================================================================
%% BUS SELECTORS
%% ====================================================================
add_block('simulink/Signal Routing/Bus Selector', [modelName '/Select_EngineCmd'], ...
    'Position', [140, 30, 220, 70], 'OutputSignals', 'cmdThrottle_unit');
add_block('simulink/Signal Routing/Bus Selector', [modelName '/Select_Failure'], ...
    'Position', [140, 330, 220, 370], 'OutputSignals', 'engineFailure_isTrue');

%% ====================================================================
%% MATLAB FUNCTION: throttle+failure -> thrust_N, torque_Nm, motorAngVel_radps
%% ====================================================================
fcn_blk = [modelName '/propThrustModel'];
add_block('simulink/User-Defined Functions/MATLAB Function', fcn_blk, ...
    'Position', [280, 30, 520, 200]);

%% ====================================================================
%% FORCE/MOMENT VECTOR ASSEMBLY (thrust along +X body, reaction torque about X)
%% ====================================================================
add_block('simulink/Sources/Constant', [modelName '/ZeroForceYZ'], ...
    'Position', [560, 60, 600, 80], 'Value', '[0;0]');
add_block('simulink/Signal Routing/Mux', [modelName '/ForceVecMux'], ...
    'Position', [640, 40, 660, 100], 'Inputs', '2');

add_block('simulink/Sources/Constant', [modelName '/ZeroMomentYZ'], ...
    'Position', [560, 140, 600, 160], 'Value', '[0;0]');
add_block('simulink/Signal Routing/Mux', [modelName '/MomentVecMux'], ...
    'Position', [640, 120, 660, 180], 'Inputs', '2');

add_block('simulink/Signal Routing/Bus Creator', [modelName '/EngineForcesMomentsBus_Creator'], ...
    'Position', [700, 60, 760, 160], 'Inputs', '2', ...
    'UseBusObject', 'on', 'BusObject', 'ComponentForcesMomentsBus');

%% ====================================================================
%% PropulsionBus ASSEMBLY
%% ====================================================================
add_block('simulink/Sources/Constant', [modelName '/fuelRate_zero'], ...
    'Position', [560, 260, 600, 280], 'Value', '0');
add_block('simulink/Signal Routing/Bus Creator', [modelName '/PropulsionBus_Creator'], ...
    'Position', [820, 90, 880, 250], 'Inputs', '3', ...
    'UseBusObject', 'on', 'BusObject', 'PropulsionBus');

%% ====================================================================
%% OUTPORT
%% ====================================================================
add_block('simulink/Sinks/Out1', [modelName '/PropulsionBus'], ...
    'Position', [940, 160, 980, 180], 'Port', '1', ...
    'OutDataTypeStr', 'Bus: PropulsionBus');

%% ====================================================================
%% MATLAB FUNCTION SCRIPT (must be set BEFORE add_line on its ports -- a
%% freshly added MATLAB Function block only exposes 1 in/1 out port until
%% the chart script defines the real signature)
%% ====================================================================
open_system(modelName);
pause(0.5);
rt  = sfroot();
mdl = rt.find('-isa', 'Simulink.BlockDiagram', 'Name', modelName);
charts = mdl.find('-isa', 'Stateflow.EMChart');
if isempty(charts)
    warning('Stateflow chart not found. Open the model manually and paste the function code.');
else
    chart = charts(1);
    chart.Script = [ ...
'function [thrust_N, torque_Nm, motorAngVel_radps] = ...' newline ...
'    propThrustModel(cmdThrottle_unit, engineFailure_isTrue)' newline ...
'%#codegen' newline ...
'persistent motorAngVel_radps_state;' newline ...
'if isempty(motorAngVel_radps_state)' newline ...
'    motorAngVel_radps_state = 0;' newline ...
'end' newline ...
'' newline ...
'% Single tractor propeller, electric motor -- see' newline ...
'% vehicle/common/setUpActuators.m ''fixedwing_plane'' case for parameter' newline ...
'% sourcing. Constants inlined (not read from vehicleParams) per the same' newline ...
'% MATLAB-Function-block struct-compile constraint documented in' newline ...
'% optimAeroPX4SIL/CLAUDE.md section 3.' newline ...
'maxRPM = 9000;' newline ...
'maxAngVel_radps = maxRPM * 2 * pi / 60;' newline ...
'tauMotor_s = 0.15;' newline ...
'Ts_s = 0.01;' newline ...
'kct_nd = 3.6e-5;' newline ...
'kcp_nd = 2.24e-6;' newline ...
'' newline ...
'if engineFailure_isTrue' newline ...
'    throttleCmd_nd = 0;' newline ...
'else' newline ...
'    throttleCmd_nd = max(min(cmdThrottle_unit, 1), 0);' newline ...
'end' newline ...
'targetAngVel_radps = throttleCmd_nd * maxAngVel_radps;' newline ...
'' newline ...
'alpha = Ts_s / (tauMotor_s + Ts_s);' newline ...
'motorAngVel_radps_state = motorAngVel_radps_state + alpha * ...' newline ...
'    (targetAngVel_radps - motorAngVel_radps_state);' newline ...
'' newline ...
'motorAngVel_radps = motorAngVel_radps_state;' newline ...
'thrust_N = kct_nd * motorAngVel_radps^2;' newline ...
'torque_Nm = -kcp_nd * motorAngVel_radps^2;' newline ...
'end'];
end

%% ====================================================================
%% CONNECT
%% ====================================================================
fcnName = get_param(fcn_blk, 'Name');

add_line(modelName, 'EngineCommandBus/1', 'Select_EngineCmd/1', 'autorouting', 'on');
add_line(modelName, 'failureBus/1', 'Select_Failure/1', 'autorouting', 'on');

add_line(modelName, 'Select_EngineCmd/1', [fcnName '/1'], 'autorouting', 'on');
add_line(modelName, 'Select_Failure/1', [fcnName '/2'], 'autorouting', 'on');

% propThrustModel outputs: thrust_N, torque_Nm, motorAngVel_radps
add_line(modelName, [fcnName '/1'], 'ForceVecMux/1', 'autorouting', 'on');
add_line(modelName, 'ZeroForceYZ/1', 'ForceVecMux/2', 'autorouting', 'on');
add_line(modelName, [fcnName '/2'], 'MomentVecMux/1', 'autorouting', 'on');
add_line(modelName, 'ZeroMomentYZ/1', 'MomentVecMux/2', 'autorouting', 'on');

add_line(modelName, 'ForceVecMux/1', 'EngineForcesMomentsBus_Creator/1', 'autorouting', 'on');
add_line(modelName, 'MomentVecMux/1', 'EngineForcesMomentsBus_Creator/2', 'autorouting', 'on');

add_line(modelName, [fcnName '/3'], 'PropulsionBus_Creator/1', 'autorouting', 'on');
add_line(modelName, 'EngineForcesMomentsBus_Creator/1', 'PropulsionBus_Creator/2', 'autorouting', 'on');
add_line(modelName, 'fuelRate_zero/1', 'PropulsionBus_Creator/3', 'autorouting', 'on');

add_line(modelName, 'PropulsionBus_Creator/1', 'PropulsionBus/1', 'autorouting', 'on');

%% ====================================================================
%% CONFIG SET -- see the matching comment in
%% build_FixedwingPlaneAeroCoefficientsModel.m for why this is needed (Model
%% Reference blocks with an inherited/ambiguous sample time fail to compile
%% against a parent using the shared ConfigSetRef unless this submodel also
%% uses it).
%% ====================================================================
if evalin('base', "exist('standardSILConfiguraitonParams','var')") == 1
    configSetRef = Simulink.ConfigSetRef;
    configSetRef.SourceName = 'standardSILConfiguraitonParams';
    attachConfigSet(modelName, configSetRef, true);
    setActiveConfigSet(modelName, configSetRef.Name);
else
    warning(['standardSILConfiguraitonParams not found in base workspace -- ' ...
        'leaving this model on its own local Fixed-step config. Re-run ' ...
        'attachConfigSet/setActiveConfigSet manually once it is loaded.']);
end

%% ---- Save ----------------------------------------------------------
Simulink.BlockDiagram.arrangeSystem(modelName);
save_system(modelName, savePath);
fprintf('\nModel saved: %s\n', savePath);
