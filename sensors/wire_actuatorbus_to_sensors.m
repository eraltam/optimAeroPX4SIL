%% wire_actuatorbus_to_sensors.m
% Routes ActuatorBus (already produced by the plant, contains PropulsionBus.rotorAngVel_radps --
% see signals/PropulsionSystemBusDefinition.m) into sensors.slx and down into the "ins" subsystem's
% AnelloX3 branch only (Generic/Ideal/AnelloV2 are left untouched -- they don't need this signal
% and must keep behaving exactly as before). See
% PLAN_HEXAROTOR_VEHICLE_PLANT_AUDIT_AND_VIBRATION_MODEL.md Section 5.
%
% Idempotent: safe to re-run.

modelName = 'sensors';
rootDir = fileparts(mfilename('fullpath'));

if ~bdIsLoaded(modelName)
    load_system(fullfile(rootDir, [modelName '.slx']));
end

% --- Idempotency: remove prior additions if already run ---
if ~isempty(find_system(modelName, 'SearchDepth', 1, 'Name', 'ActuatorBus'))
    delete_block([modelName '/ActuatorBus']);
end
if ~isempty(find_system(modelName, 'SearchDepth', 1, 'Name', 'Select_RotorAngVel'))
    delete_block([modelName '/Select_RotorAngVel']);
end

%% ====================================================================
%% New root Inport: ActuatorBus (already exists as an output of VehiclePlant.slx's "plant" --
%% just not previously routed into sensors.slx)
%% ====================================================================
add_block('simulink/Sources/In1', [modelName '/ActuatorBus'], ...
    'Position', [40, 470, 80, 490], 'Port', '3', 'OutDataTypeStr', 'Bus: ActuatorBus');

%% ====================================================================
%% Extract PropulsionBus.rotorAngVel_radps
%% ====================================================================
add_block('simulink/Signal Routing/Bus Selector', [modelName '/Select_RotorAngVel'], ...
    'Position', [140, 460, 220, 500], 'OutputSignals', 'PropulsionBus.rotorAngVel_radps');
add_line(modelName, 'ActuatorBus/1', 'Select_RotorAngVel/1', 'autorouting', 'on');

%% ====================================================================
%% Route into the "ins" subsystem: add a new port to that subsystem, wire it only to AnelloX3
%% ====================================================================
insSub = [modelName '/ins'];

% Adding an Inport block inside the subsystem auto-creates the matching external port.
if isempty(find_system(insSub, 'SearchDepth', 1, 'Name', 'RotorAngVel_radps'))
    add_block('simulink/Sources/In1', [insSub '/RotorAngVel_radps'], ...
        'Position', [40, 470, 80, 490], 'Port', '3');
end
add_line(modelName, 'Select_RotorAngVel/1', 'ins/3', 'autorouting', 'on');

% Inside "ins": wire the new inport only to AnelloX3 (ins_anello.slx), which now has a 3rd input
% port (RotorAngVel_radps) added by add_vibration_to_ins_anello.m. Generic/Ideal/AnelloV2 keep
% their existing 2-input interface untouched.
add_line(insSub, 'RotorAngVel_radps/1', 'AnelloX3/3', 'autorouting', 'on');

%% ---- Save ----------------------------------------------------------
Simulink.BlockDiagram.arrangeSystem(modelName);
save_system(modelName, fullfile(rootDir, [modelName '.slx']));
fprintf('\nActuatorBus/rotorAngVel_radps routed into: %s\n', fullfile(rootDir, [modelName '.slx']));
