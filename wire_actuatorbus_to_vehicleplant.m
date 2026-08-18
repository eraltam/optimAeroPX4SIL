%% wire_actuatorbus_to_vehicleplant.m
% VehiclePlant.slx's "plant" subsystem already produces ActuatorBus (routed today only to
% VehiclePlant's own root Outport #4). This adds the same signal as a 3rd input to the "sensors"
% Model block, matching the new 3rd input port added by sensors/wire_actuatorbus_to_sensors.m. See
% PLAN_HEXAROTOR_VEHICLE_PLANT_AUDIT_AND_VIBRATION_MODEL.md Section 5.
%
% Idempotent: safe to re-run.

modelName = 'VehiclePlant';
rootDir = fileparts(mfilename('fullpath'));

if bdIsLoaded(modelName)
    bdclose(modelName);  % force a fresh reload so the "sensors" Model block picks up its new port
end
load_system(fullfile(rootDir, [modelName '.slx']));

% Remove a prior connection into sensors/3 if this was already run (add_line errors if the
% destination port is already connected to something else; safe to attempt a clean reconnect).
existingLines = find_system(modelName, 'FindAll', 'on', 'type', 'line', ...
    'DstBlockHandle', get_param([modelName '/sensors'], 'Handle'));
for i = 1:numel(existingLines)
    dst = get(existingLines(i), 'DstPortHandle');
    if get(dst, 'PortNumber') == 3
        delete_line(existingLines(i));
    end
end

add_line(modelName, 'plant/2', 'sensors/3', 'autorouting', 'on');

Simulink.BlockDiagram.arrangeSystem(modelName);
save_system(modelName, fullfile(rootDir, [modelName '.slx']));
fprintf('\nActuatorBus routed plant -> sensors in: %s\n', fullfile(rootDir, [modelName '.slx']));
