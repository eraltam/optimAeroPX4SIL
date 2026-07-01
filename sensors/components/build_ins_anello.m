%% build_ins_anello.m
% Programmatically creates sensors/components/ins_anello.slx — a drop-in
% alternative to sensors/components/ins.slx that uses the ANELLO X3 IMU
% datasheet-accurate sensor model instead of the generic ICM-42688-P-like
% Aerospace Blockset IMU.
%
% Interface contract (must match ins.slx exactly so it can be used as a
% Model Reference choice for the same Goto/From-routed signals in sensors.slx):
%   Inputs:  EnvironmentBus (Bus: EnvironmentBus), BodyStatesBus (Bus: BodyStateBus)
%   Outputs: INSSensorBus (Bus: INSSensorBus)
%
% Prerequisites (run from optimAeroPX4SIL root, or run initVehicleSIL first):
%   cd(optimAeroPX4SIL root)
%   addpath(genpath('sensors')); addpath(genpath('signals')); addpath(genpath('data'));
%   constants
%   BusDefinition('hexarotor')   % or whichever vehicle type — defines the bus objects
%
% See optimAeroPX4SIL/CLAUDE.md for the full design rationale (rotation
% convention, sample-rate handling, open decisions).

clear; clc;

rootDir = fileparts(mfilename('fullpath'));   % .../optimAeroPX4SIL/sensors/components
addpath(fullfile(rootDir, 'anelloX3'));

modelName = 'ins_anello';
savePath  = fullfile(rootDir, [modelName '.slx']);

if bdIsLoaded(modelName)
    close_system(modelName, 0);
end

new_system(modelName);
set_param(modelName, 'SolverType', 'Fixed-step');

%% ====================================================================
%% INPORTS
%% ====================================================================
add_block('simulink/Sources/In1', [modelName '/EnvironmentBus'], ...
    'Position', [40, 120, 80, 140], 'Port', '1', ...
    'OutDataTypeStr', 'Bus: EnvironmentBus');

add_block('simulink/Sources/In1', [modelName '/BodyStatesBus'], ...
    'Position', [40, 320, 80, 340], 'Port', '2', ...
    'OutDataTypeStr', 'Bus: BodyStateBus');

%% ====================================================================
%% BUS SELECTORS
%% ====================================================================
add_block('simulink/Signal Routing/Bus Selector', [modelName '/Select_Env'], ...
    'Position', [140, 90, 220, 170], ...
    'OutputSignals', 'EarthEnvironment.worldMagneticFieldInNED_nT,EarthEnvironment.gravityScalar_mps2,AirEnvironment.airTemperature_K');

add_block('simulink/Signal Routing/Bus Selector', [modelName '/Select_Body'], ...
    'Position', [140, 290, 220, 390], ...
    'OutputSignals', 'DCM_be,aircraftAccelbe_mps2,aircraftAngVelInBody_radps,aircraftAngAccelInBody_radps2');

%% ====================================================================
%% PRE-PROCESSING: units + gravity vector
%% ====================================================================
% nT -> Gauss
add_block('simulink/Math Operations/Gain', [modelName '/nT_to_Gauss'], ...
    'Position', [280, 90, 340, 120], 'Gain', '1e-5');

% K -> degC
add_block('simulink/Math Operations/Bias', [modelName '/K_to_degC'], ...
    'Position', [280, 200, 340, 230], 'Bias', '-273.15');

% Gravity vector in nav frame: [0; 0; gravityScalar_mps2]
add_block('simulink/Sources/Constant', [modelName '/Zero1'], ...
    'Position', [280, 250, 320, 270], 'Value', '0');
add_block('simulink/Sources/Constant', [modelName '/Zero2'], ...
    'Position', [280, 280, 320, 300], 'Value', '0');
add_block('simulink/Signal Routing/Mux', [modelName '/g_nav_Mux'], ...
    'Position', [360, 240, 380, 300], 'Inputs', '3');

% g_body = DCM_be * g_nav
add_block('simulink/Math Operations/Product', [modelName '/g_body_MatMul'], ...
    'Position', [420, 250, 460, 290], 'Multiplication', 'Matrix(*)');

%% ====================================================================
%% reset / device_id constants
%% ====================================================================
add_block('simulink/Sources/Constant', [modelName '/reset_const'], ...
    'Position', [280, 420, 320, 440], 'Value', '0');

add_block('simulink/Sources/Constant', [modelName '/accel_device_id'], ...
    'Position', [780, 60, 840, 80], 'Value', '1310988');
add_block('simulink/Sources/Constant', [modelName '/gyro_device_id'], ...
    'Position', [780, 200, 840, 220], 'Value', '1310988');
add_block('simulink/Sources/Constant', [modelName '/mag_device_id'], ...
    'Position', [780, 340, 840, 360], 'Value', '197388');

%% ====================================================================
%% MATLAB FUNCTION BLOCK — ANELLO_X3_IMU_Vehicle_fcn_SIL wrapper
%% ====================================================================
fcn_blk = [modelName '/ANELLO_X3_IMU_Vehicle_fcn_SIL_wrapper'];
add_block('simulink/User-Defined Functions/MATLAB Function', fcn_blk, ...
    'Position', [500, 80, 700, 420]);

open_system(modelName);
pause(0.5);
rt  = sfroot();
mdl = rt.find('-isa', 'Simulink.BlockDiagram', 'Name', modelName);
charts = mdl.find('-isa', 'Stateflow.EMChart');
if isempty(charts)
    warning('Stateflow chart not found. Open the model manually and paste the wrapper code.');
else
    chart = charts(1);
    chart.Script = [ ...
'function [Ameas, omegaMeas, magMeas, accel_bias, gyro_bias, mag_bias] = ...' newline ...
'         ins_anello_wrapper(Ab, omega, omegaDot, g_body, mag_nav_G, DCM_be, T, reset)' newline ...
'%#codegen' newline ...
'R_bn_vec = DCM_be(:);' newline ...
'[Ameas, omegaMeas, magMeas, accel_bias, gyro_bias, mag_bias] = ...' newline ...
'    ANELLO_X3_IMU_Vehicle_fcn_SIL(Ab, omega, omegaDot, g_body, mag_nav_G, R_bn_vec, T, reset);' newline ...
'end'];
end

%% ====================================================================
%% TERMINATORS for unused debug outputs
%% ====================================================================
add_block('simulink/Sinks/Terminator', [modelName '/Term_accel_bias'], 'Position', [740, 90, 760, 110]);
add_block('simulink/Sinks/Terminator', [modelName '/Term_gyro_bias'],  'Position', [740, 230, 760, 250]);
add_block('simulink/Sinks/Terminator', [modelName '/Term_mag_bias'],   'Position', [740, 370, 760, 390]);

%% ====================================================================
%% DEMUX
%% ====================================================================
add_block('simulink/Signal Routing/Demux', [modelName '/Demux_Accel'], ...
    'Position', [780, 100, 800, 160], 'Outputs', '3');
add_block('simulink/Signal Routing/Demux', [modelName '/Demux_Gyro'], ...
    'Position', [780, 240, 800, 300], 'Outputs', '3');
add_block('simulink/Signal Routing/Demux', [modelName '/Demux_Mag'], ...
    'Position', [780, 380, 800, 440], 'Outputs', '3');

%% ====================================================================
%% BUS CREATORS
%% ====================================================================
add_block('simulink/Signal Routing/Bus Creator', [modelName '/AccelSensorBus_Creator'], ...
    'Position', [900, 50, 960, 170], 'Inputs', '5', ...
    'UseBusObject', 'on', 'BusObject', 'AccelSensorBus');

add_block('simulink/Signal Routing/Bus Creator', [modelName '/GyroSensorBus_Creator'], ...
    'Position', [900, 190, 960, 310], 'Inputs', '5', ...
    'UseBusObject', 'on', 'BusObject', 'GyroSensorBus');

add_block('simulink/Signal Routing/Bus Creator', [modelName '/MagSensorBus_Creator'], ...
    'Position', [900, 330, 960, 450], 'Inputs', '5', ...
    'UseBusObject', 'on', 'BusObject', 'MagSensorBus');

add_block('simulink/Signal Routing/Bus Creator', [modelName '/INSSensorBus_Creator'], ...
    'Position', [1020, 150, 1080, 350], 'Inputs', '3', ...
    'UseBusObject', 'on', 'BusObject', 'INSSensorBus');

%% ====================================================================
%% OUTPORT
%% ====================================================================
add_block('simulink/Sinks/Out1', [modelName '/INSSensorBus'], ...
    'Position', [1140, 240, 1180, 260], 'Port', '1', ...
    'OutDataTypeStr', 'Bus: INSSensorBus');

%% ====================================================================
%% CONNECT
%% ====================================================================
% Inports -> selectors
add_line(modelName, 'EnvironmentBus/1', 'Select_Env/1', 'autorouting', 'on');
add_line(modelName, 'BodyStatesBus/1',  'Select_Body/1', 'autorouting', 'on');

% Env selector outputs: y1=mag(nT) y2=gravityScalar y3=airTemp(K)
add_line(modelName, 'Select_Env/1', 'nT_to_Gauss/1', 'autorouting', 'on');
add_line(modelName, 'Select_Env/2', 'g_nav_Mux/3',    'autorouting', 'on');
add_line(modelName, 'Select_Env/3', 'K_to_degC/1',    'autorouting', 'on');

add_line(modelName, 'Zero1/1', 'g_nav_Mux/1', 'autorouting', 'on');
add_line(modelName, 'Zero2/1', 'g_nav_Mux/2', 'autorouting', 'on');

% Body selector outputs: y1=DCM_be y2=Ab y3=omega y4=omegaDot
add_line(modelName, 'Select_Body/1', 'g_body_MatMul/1', 'autorouting', 'on');
add_line(modelName, 'g_nav_Mux/1',   'g_body_MatMul/2', 'autorouting', 'on');

% MATLAB Function inputs: Ab, omega, omegaDot, g_body, mag_nav_G, DCM_be, T, reset
add_line(modelName, 'Select_Body/2', [get_param(fcn_blk,'Name') '/1'], 'autorouting', 'on');
add_line(modelName, 'Select_Body/3', [get_param(fcn_blk,'Name') '/2'], 'autorouting', 'on');
add_line(modelName, 'Select_Body/4', [get_param(fcn_blk,'Name') '/3'], 'autorouting', 'on');
add_line(modelName, 'g_body_MatMul/1', [get_param(fcn_blk,'Name') '/4'], 'autorouting', 'on');
add_line(modelName, 'nT_to_Gauss/1', [get_param(fcn_blk,'Name') '/5'], 'autorouting', 'on');
add_line(modelName, 'Select_Body/1', [get_param(fcn_blk,'Name') '/6'], 'autorouting', 'on');
add_line(modelName, 'K_to_degC/1',   [get_param(fcn_blk,'Name') '/7'], 'autorouting', 'on');
add_line(modelName, 'reset_const/1', [get_param(fcn_blk,'Name') '/8'], 'autorouting', 'on');

% MATLAB Function outputs: Ameas, omegaMeas, magMeas, accel_bias, gyro_bias, mag_bias
fcnName = get_param(fcn_blk, 'Name');
add_line(modelName, [fcnName '/1'], 'Demux_Accel/1', 'autorouting', 'on');
add_line(modelName, [fcnName '/2'], 'Demux_Gyro/1',  'autorouting', 'on');
add_line(modelName, [fcnName '/3'], 'Demux_Mag/1',   'autorouting', 'on');
add_line(modelName, [fcnName '/4'], 'Term_accel_bias/1', 'autorouting', 'on');
add_line(modelName, [fcnName '/5'], 'Term_gyro_bias/1',  'autorouting', 'on');
add_line(modelName, [fcnName '/6'], 'Term_mag_bias/1',   'autorouting', 'on');

% Accel bus: device_id, x, y, z, temperature
add_line(modelName, 'accel_device_id/1', 'AccelSensorBus_Creator/1', 'autorouting', 'on');
add_line(modelName, 'Demux_Accel/1',     'AccelSensorBus_Creator/2', 'autorouting', 'on');
add_line(modelName, 'Demux_Accel/2',     'AccelSensorBus_Creator/3', 'autorouting', 'on');
add_line(modelName, 'Demux_Accel/3',     'AccelSensorBus_Creator/4', 'autorouting', 'on');
add_line(modelName, 'K_to_degC/1',       'AccelSensorBus_Creator/5', 'autorouting', 'on');

% Gyro bus
add_line(modelName, 'gyro_device_id/1', 'GyroSensorBus_Creator/1', 'autorouting', 'on');
add_line(modelName, 'Demux_Gyro/1',     'GyroSensorBus_Creator/2', 'autorouting', 'on');
add_line(modelName, 'Demux_Gyro/2',     'GyroSensorBus_Creator/3', 'autorouting', 'on');
add_line(modelName, 'Demux_Gyro/3',     'GyroSensorBus_Creator/4', 'autorouting', 'on');
add_line(modelName, 'K_to_degC/1',      'GyroSensorBus_Creator/5', 'autorouting', 'on');

% Mag bus
add_line(modelName, 'mag_device_id/1', 'MagSensorBus_Creator/1', 'autorouting', 'on');
add_line(modelName, 'Demux_Mag/1',     'MagSensorBus_Creator/2', 'autorouting', 'on');
add_line(modelName, 'Demux_Mag/2',     'MagSensorBus_Creator/3', 'autorouting', 'on');
add_line(modelName, 'Demux_Mag/3',     'MagSensorBus_Creator/4', 'autorouting', 'on');
add_line(modelName, 'K_to_degC/1',     'MagSensorBus_Creator/5', 'autorouting', 'on');

% INS bus: Gyro(1), Mag(2), Accel(3) — order per signals/BusDefinition.m
add_line(modelName, 'GyroSensorBus_Creator/1',  'INSSensorBus_Creator/1', 'autorouting', 'on');
add_line(modelName, 'MagSensorBus_Creator/1',   'INSSensorBus_Creator/2', 'autorouting', 'on');
add_line(modelName, 'AccelSensorBus_Creator/1', 'INSSensorBus_Creator/3', 'autorouting', 'on');

add_line(modelName, 'INSSensorBus_Creator/1', 'INSSensorBus/1', 'autorouting', 'on');

%% ---- Save ----------------------------------------------------------
Simulink.BlockDiagram.arrangeSystem(modelName);
save_system(modelName, savePath);
fprintf('\nModel saved: %s\n', savePath);
