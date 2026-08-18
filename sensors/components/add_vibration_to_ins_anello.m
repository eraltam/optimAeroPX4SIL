%% add_vibration_to_ins_anello.m
% Adds the Level A motor/rotor vibration model (ANELLO_X3_Vibration_LevelA_fcn.m) into the
% ALREADY-BUILT ins_anello.slx, injected between the existing Select_Body BusSelector and the
% existing ANELLO_X3_IMU_Vehicle_fcn_SIL_wrapper MATLAB Function block -- see
% PLAN_HEXAROTOR_VEHICLE_PLANT_AUDIT_AND_VIBRATION_MODEL.md Section 5 for the architecture
% rationale (why here, not upstream in BodyStatesBus or inside the rigid-body integrator).
%
% New root Inport: RotorAngVel_radps (6-element, one per motor -- fed from a new ActuatorBus route
% added in sensors.slx / VehiclePlant.slx, see wire_actuatorbus_to_sensors.m). The enable flag is a
% Constant reading a base-workspace variable (vehicle.vibration.enable, default 0/off for
% regression safety -- see setUpSensors.m), NOT a new port, since it's a static per-run config
% value, not a per-timestep signal, same convention as INS_VARIANT/CONTROLLER_RUNTIME elsewhere in
% this project.
%
% Idempotent: safe to re-run (deletes and re-adds the blocks/lines it owns if already present).
%
% Prerequisites: same as build_ins_anello.m (run from optimAeroPX4SIL root or after initVehicleSIL).

modelName = 'ins_anello';
rootDir = fileparts(mfilename('fullpath'));
addpath(fullfile(rootDir, 'anelloX3'));

if ~bdIsLoaded(modelName)
    load_system(fullfile(rootDir, [modelName '.slx']));
end

fcnBlk = [modelName '/ANELLO_X3_IMU_Vehicle_fcn_SIL_wrapper'];

% --- Idempotency: remove prior vibration blocks/lines if this script was already run ---
newBlockNames = {'RotorAngVel_radps', 'VibrationEnable', 'ANELLO_X3_Vibration_LevelA_wrapper', ...
                 'SumAccelVib', 'SumGyroVib'};
for i = 1:numel(newBlockNames)
    bp = [modelName '/' newBlockNames{i}];
    if ~isempty(find_system(modelName, 'SearchDepth', 1, 'Name', newBlockNames{i}))
        delete_block(bp);
    end
end

% Restore the original direct Select_Body -> wrapper lines if a prior run redirected them through
% the Sum blocks (delete_block above already removed the Sum blocks' lines; just ensure ports 1/2
% on the wrapper are unconnected before re-wiring, harmless if already unconnected).

%% ====================================================================
%% NEW INPORT: per-motor rotor angular velocity
%% ====================================================================
add_block('simulink/Sources/In1', [modelName '/RotorAngVel_radps'], ...
    'Position', [40, 470, 80, 490], 'Port', '3');

%% ====================================================================
%% ENABLE FLAG (base-workspace switch, default off -- see setUpSensors.m)
%% ====================================================================
add_block('simulink/Sources/Constant', [modelName '/VibrationEnable'], ...
    'Position', [280, 470, 340, 490], 'Value', 'vehicle.vibration.enable');

%% ====================================================================
%% VIBRATION MATLAB FUNCTION BLOCK
%% ====================================================================
vibBlk = [modelName '/ANELLO_X3_Vibration_LevelA_wrapper'];
add_block('simulink/User-Defined Functions/MATLAB Function', vibBlk, ...
    'Position', [420, 440, 620, 520]);

open_system(modelName);
pause(0.5);
rt = sfroot();
mdl = rt.find('-isa', 'Simulink.BlockDiagram', 'Name', modelName);
charts = mdl.find('-isa', 'Stateflow.EMChart');
vibChart = [];
for i = 1:numel(charts)
    if strcmp(charts(i).Name, 'ANELLO_X3_Vibration_LevelA_wrapper')
        vibChart = charts(i);
        break
    end
end
if isempty(vibChart)
    warning('Vibration MATLAB Function chart not found. Open the model manually and paste the wrapper code.');
else
    vibChart.Script = [ ...
'function [accel_vib_mps2, gyro_vib_radps] = vibration_wrapper(rotorAngVel_radps, enableVibration)' newline ...
'%#codegen' newline ...
'[accel_vib_mps2, gyro_vib_radps] = ANELLO_X3_Vibration_LevelA_fcn(rotorAngVel_radps, enableVibration);' newline ...
'end'];
end

%% ====================================================================
%% SUM BLOCKS: inject vibration into Ab (accel) and omega (gyro) paths
%% ====================================================================
add_block('simulink/Math Operations/Add', [modelName '/SumAccelVib'], ...
    'Position', [340, 300, 380, 340], 'Inputs', '++');
add_block('simulink/Math Operations/Add', [modelName '/SumGyroVib'], ...
    'Position', [340, 350, 380, 390], 'Inputs', '++');

%% ====================================================================
%% REWIRE: remove the old direct Select_Body -> wrapper connections for Ab/omega,
%% route them through the new Sum blocks instead
%% ====================================================================
fcnName = get_param(fcnBlk, 'Name');
oldLines = find_system(modelName, 'FindAll', 'on', 'type', 'line', 'SrcBlockHandle', ...
    get_param([modelName '/Select_Body'], 'Handle'));
for i = 1:numel(oldLines)
    dst = get(oldLines(i), 'DstPortHandle');
    if ~isempty(dst)
        dstBlock = get_param(get(dst(1), 'Parent'), 'Handle');
        if dstBlock == get_param(fcnBlk, 'Handle')
            dstPortNum = get(dst(1), 'PortNumber');
            if dstPortNum == 1 || dstPortNum == 2
                delete_line(oldLines(i));
            end
        end
    end
end

% Select_Body/2 (Ab) -> SumAccelVib/1 ; vib accel -> SumAccelVib/2 ; SumAccelVib -> wrapper u1
add_line(modelName, 'Select_Body/2', 'SumAccelVib/1', 'autorouting', 'on');
add_line(modelName, [get_param(vibBlk,'Name') '/1'], 'SumAccelVib/2', 'autorouting', 'on');
add_line(modelName, 'SumAccelVib/1', [fcnName '/1'], 'autorouting', 'on');

% Select_Body/3 (omega) -> SumGyroVib/1 ; vib gyro -> SumGyroVib/2 ; SumGyroVib -> wrapper u2
add_line(modelName, 'Select_Body/3', 'SumGyroVib/1', 'autorouting', 'on');
add_line(modelName, [get_param(vibBlk,'Name') '/2'], 'SumGyroVib/2', 'autorouting', 'on');
add_line(modelName, 'SumGyroVib/1', [fcnName '/2'], 'autorouting', 'on');

% RotorAngVel_radps -> vib wrapper u1 ; VibrationEnable -> vib wrapper u2
add_line(modelName, 'RotorAngVel_radps/1', [get_param(vibBlk,'Name') '/1'], 'autorouting', 'on');
add_line(modelName, 'VibrationEnable/1', [get_param(vibBlk,'Name') '/2'], 'autorouting', 'on');

%% ---- Save ----------------------------------------------------------
Simulink.BlockDiagram.arrangeSystem(modelName);
save_system(modelName, fullfile(rootDir, [modelName '.slx']));
fprintf('\nVibration model added to: %s\n', fullfile(rootDir, [modelName '.slx']));
