%% build_PX4HITLConnector.m
% Programmatically creates PX4HITLConnector.slx -- a drop-in alternative to VehicleSilSimulation's
% "PX4 Interface" subsystem (pixhawk_sil_connector / TCP:4560, SITL-only) that talks to a REAL PX4
% board over serial MAVLink instead, using the UAV Toolbox Support Package for PX4 Autopilots'
% px4MAVLinkBridgelib.
%
% Interface contract (matches the existing "PX4 Interface" block exactly, so it can be a selectable
% alternative -- see CLAUDE_HITL.md):
%   Inputs:  SensorsBus (Bus: SensorsBus), VehicleBus (Bus: VehicleBus), EnvironmentBus (Bus: EnvironmentBus)
%   Outputs: ActuatorCommandBus (Bus: ActuatorCommandBus)
%
% Internals, built by reusing PROVEN blocks from uav_HITL_sample's HITL_Plant_top.slx (MathWorks'
% own PX4 HITL reference example) rather than inventing MAVLink message construction from scratch:
%   - HIL Sensor / HIL_GPS / Send Heartbeat subsystems (copied from the reference model, unmodified
%     internals) build the HIL_SENSOR / HIL_GPS / HEARTBEAT MAVLink byte streams from this repo's
%     SensorsBus fields.
%   - MAVLink Bridge Sink (px4MAVLinkBridgelib) sends the combined stream to PX4 over serial.
%   - MAVLink Bridge Source (px4MAVLinkBridgelib) + a MAVLink Deserializer configured for
%     HIL_ACTUATOR_CONTROLS (same message pixhawk_sil_connector.cpp already decodes for SITL,
%     confirmed by reading its source) recovers the 16-element actuator controls array.
%   - That array feeds the SAME PX4OutputMappingHex.slx model reference the existing SITL "PX4
%     Interface" already uses (Command Output Variant > Hex Output Mapping) -- so the actual
%     motor/channel mapping is the repo's own already-validated logic, not something invented here.
%
% Units verified by reading the reference subsystems' internal Gain blocks (see CLAUDE_HITL.md):
%   Acc [m/s^2], Gyro [rad/s], Mag [Gauss -> the subsystem itself wants uT, so x100 here],
%   Pressure/DiffPressure [Pa, the subsystem itself converts to hPa], LLA [deg,deg,m],
%   Velocity (NED) [m/s], GndSpeed [m/s], Course [deg] -- all match this repo's sensor bus units
%   directly except Mag (Gauss -> uT).
%
% NOT included: the reference project's custom "Rotor parameter" MAVLink message (VTOL-specific,
% not a standard MAVLink message -- PX4 wouldn't recognize it anyway since unknown message IDs are
% simply ignored by MAVLink-compliant receivers, but there's no reason to fabricate a fake message
% just to match the reference project's exact byte stream).
%
% Prerequisites: run from optimAeroPX4SIL root after sourcing setUpHITLConnection.m (adds
% px4MAVLinkBridgelib to path) and BusDefinition (defines SensorsBus/VehicleBus/EnvironmentBus/
% ActuatorCommandBus etc. in the base workspace), and after addpath(genpath('vehicle/hexarotor'))
% (so PX4OutputMappingHex.slx resolves -- this is also done by initVehicleSIL.m for hexarotor runs).
% Also requires
% uav_HITL_sample/TuneControlDesignForUAVInHoverExample/VTOLApp/models/HITL_Plant_top.slx to exist
% (only at build time -- the built PX4HITLConnector.slx has no runtime dependency on it).
%
% See optimAeroPX4SIL/CLAUDE_HITL.md for the full design rationale and what remains for hardware
% validation (this script produces a model that compiles; it has NOT been run against real PX4
% hardware).

clear; clc;

refModelPath = 'C:\Users\Usuario\AnelloSummer\uav_HITL_sample\TuneControlDesignForUAVInHoverExample\VTOLApp\models\HITL_Plant_top.slx';
if ~bdIsLoaded('HITL_Plant_top')
    load_system(refModelPath);
end

modelName = 'PX4HITLConnector';
rootDir   = fileparts(mfilename('fullpath'));
savePath  = fullfile(rootDir, [modelName '.slx']);

if bdIsLoaded(modelName)
    close_system(modelName, 0);
end

new_system(modelName);
open_system(modelName);

%% ====================================================================
%% TOP-LEVEL PORTS (match PX4 Interface's contract exactly)
%% ====================================================================
add_block('simulink/Sources/In1', [modelName '/SensorsBus'], ...
    'Position', [40, 60, 80, 74], 'Port', '1', 'OutDataTypeStr', 'Bus: SensorsBus');
add_block('simulink/Sources/In1', [modelName '/VehicleBus'], ...
    'Position', [40, 140, 80, 154], 'Port', '2', 'OutDataTypeStr', 'Bus: VehicleBus');
add_block('simulink/Sources/In1', [modelName '/EnvironmentBus'], ...
    'Position', [40, 220, 80, 234], 'Port', '3', 'OutDataTypeStr', 'Bus: EnvironmentBus');
add_block('simulink/Sinks/Out1', [modelName '/ActuatorCommandBus'], ...
    'Position', [1500, 400, 1540, 414], 'Port', '1', 'OutDataTypeStr', 'Bus: ActuatorCommandBus');

% VehicleBus/EnvironmentBus are not used (the existing SITL PX4 Interface doesn't use them either --
% it only consumes SensorsBus, see CLAUDE_HITL.md). Terminate to match that established pattern.
add_block('simulink/Sinks/Terminator', [modelName '/Term_VehicleBus'], 'Position', [120, 144, 140, 164]);
add_block('simulink/Sinks/Terminator', [modelName '/Term_EnvironmentBus'], 'Position', [120, 224, 140, 244]);
add_line(modelName, 'VehicleBus/1', 'Term_VehicleBus/1', 'autorouting', 'on');
add_line(modelName, 'EnvironmentBus/1', 'Term_EnvironmentBus/1', 'autorouting', 'on');

%% ====================================================================
%% SENSOR EXTRACTION: one BusSelector pulling everything needed from SensorsBus
%% ====================================================================
add_block('simulink/Signal Routing/Bus Selector', [modelName '/Select_Sensors'], ...
    'Position', [120, 40, 220, 380]);
sensorSignals = strjoin({ ...
    'GPSSensorBus.lat_deg', 'GPSSensorBus.lon_deg', 'GPSSensorBus.alt_m', ...
    'GPSSensorBus.vel_mps', 'GPSSensorBus.vel_n_mps', 'GPSSensorBus.vel_e_mps', 'GPSSensorBus.vel_d_mps', ...
    'GPSSensorBus.cog_deg', ...
    'INSSensorBus.GyroSensorBus.x_radps', 'INSSensorBus.GyroSensorBus.y_radps', 'INSSensorBus.GyroSensorBus.z_radps', ...
    'INSSensorBus.AccelSensorBus.x_mps2', 'INSSensorBus.AccelSensorBus.y_mps2', 'INSSensorBus.AccelSensorBus.z_mps2', ...
    'INSSensorBus.MagSensorBus.x_Gauss', 'INSSensorBus.MagSensorBus.y_Gauss', 'INSSensorBus.MagSensorBus.z_Gauss', ...
    'ADCSensorBus.BaroSensorBus.pressure_pa', 'ADCSensorBus.DiffPressureSensorBus.differential_pressure_pa' ...
    }, ',');
set_param([modelName '/Select_Sensors'], 'OutputSignals', sensorSignals);
add_line(modelName, 'SensorsBus/1', 'Select_Sensors/1', 'autorouting', 'on');

% Port order matches sensorSignals above:
% 1 lat_deg, 2 lon_deg, 3 alt_m, 4 vel_mps, 5 vel_n_mps, 6 vel_e_mps, 7 vel_d_mps, 8 cog_deg,
% 9 gyro_x, 10 gyro_y, 11 gyro_z, 12 acc_x, 13 acc_y, 14 acc_z, 15 mag_x, 16 mag_y, 17 mag_z,
% 18 pressure_pa, 19 diff_pressure_pa

%% ====================================================================
%% MUX INTO Send-Plant-Feedback's EXPECTED PORT SHAPES
%% ====================================================================
add_block('simulink/Signal Routing/Mux', [modelName '/Mux_Acc'],      'Position', [260, 40, 280, 90], 'Inputs', '3');
add_block('simulink/Signal Routing/Mux', [modelName '/Mux_Gyro'],     'Position', [260, 100, 280, 150], 'Inputs', '3');
add_block('simulink/Signal Routing/Mux', [modelName '/Mux_Mag_raw'],  'Position', [260, 160, 280, 210], 'Inputs', '3');
add_block('simulink/Math Operations/Gain', [modelName '/Gauss_to_uT'], 'Position', [300, 165, 340, 205], 'Gain', '100');
add_block('simulink/Signal Routing/Mux', [modelName '/Mux_LLA'],      'Position', [260, 220, 280, 270], 'Inputs', '3');
add_block('simulink/Signal Routing/Mux', [modelName '/Mux_Velocity'], 'Position', [260, 280, 280, 330], 'Inputs', '3');

add_line(modelName, 'Select_Sensors/12', 'Mux_Acc/1', 'autorouting', 'on');
add_line(modelName, 'Select_Sensors/13', 'Mux_Acc/2', 'autorouting', 'on');
add_line(modelName, 'Select_Sensors/14', 'Mux_Acc/3', 'autorouting', 'on');

add_line(modelName, 'Select_Sensors/9',  'Mux_Gyro/1', 'autorouting', 'on');
add_line(modelName, 'Select_Sensors/10', 'Mux_Gyro/2', 'autorouting', 'on');
add_line(modelName, 'Select_Sensors/11', 'Mux_Gyro/3', 'autorouting', 'on');

add_line(modelName, 'Select_Sensors/15', 'Mux_Mag_raw/1', 'autorouting', 'on');
add_line(modelName, 'Select_Sensors/16', 'Mux_Mag_raw/2', 'autorouting', 'on');
add_line(modelName, 'Select_Sensors/17', 'Mux_Mag_raw/3', 'autorouting', 'on');
add_line(modelName, 'Mux_Mag_raw/1', 'Gauss_to_uT/1', 'autorouting', 'on');

add_line(modelName, 'Select_Sensors/1', 'Mux_LLA/1', 'autorouting', 'on');
add_line(modelName, 'Select_Sensors/2', 'Mux_LLA/2', 'autorouting', 'on');
add_line(modelName, 'Select_Sensors/3', 'Mux_LLA/3', 'autorouting', 'on');

add_line(modelName, 'Select_Sensors/5', 'Mux_Velocity/1', 'autorouting', 'on');
add_line(modelName, 'Select_Sensors/6', 'Mux_Velocity/2', 'autorouting', 'on');
add_line(modelName, 'Select_Sensors/7', 'Mux_Velocity/3', 'autorouting', 'on');

%% ====================================================================
%% COPY proven MAVLink-construction subsystems from the reference model
%% ====================================================================
refFeedback = 'HITL_Plant_top/VTOL Plant/Send Plant Feedback';
add_block([refFeedback '/Compute Timestamp'], [modelName '/Compute Timestamp'], 'Position', [380, 40, 480, 90]);
add_block([refFeedback '/HIL Sensor'],        [modelName '/HIL Sensor'],        'Position', [380, 110, 520, 260]);
add_block([refFeedback '/HIL_GPS'],           [modelName '/HIL_GPS'],           'Position', [380, 280, 520, 400]);
add_block([refFeedback '/Send Heartbeat'],    [modelName '/Send Heartbeat'],    'Position', [380, 420, 520, 470]);

% Replace the copied blocks' named-workspace-variable Gain values with literal numbers so this
% model has no dependency on uav_HITL_sample's data files (uT_to_gauss=0.01, m_to_mm=1000,
% ms_to_cms=100 -- standard unit-conversion constants, not tunable parameters).
set_param([modelName '/HIL Sensor/HIL_SENSOR Creation subsystem/Gain3'], 'Gain', '0.01'); % uT_to_gauss
set_param([modelName '/HIL_GPS/GPS Serialization/HIL_GPS Subsystem/Altitude in mm'], 'Gain', '1000');  % m_to_mm
set_param([modelName '/HIL_GPS/GPS Serialization/HIL_GPS Subsystem/Gain1'], 'Gain', '100');   % ms_to_cms (Velocity)
set_param([modelName '/HIL_GPS/GPS Serialization/HIL_GPS Subsystem/Gain2'], 'Gain', '100');   % ms_to_cms (GndSpeed)

% "Compute Timestamp"/"HIL_GPS"/"Send Heartbeat" each use a DataStoreRead/Write pair internally for
% their own rate-throttling and running-timestamp state, but the matching DataStoreMemory
% declaration blocks live as SIBLINGS one level up in the reference model (under "Send Plant
% Feedback"), not inside the subsystems just copied above -- so they must be copied too, or the
% data store names ("Timestamp", "Heartbeat", "GPS") won't resolve.
add_block([refFeedback '/Data Store Memory'],  [modelName '/Data Store Memory (Timestamp)'],  'Position', [40, 40, 100, 60]);
add_block([refFeedback '/Data Store Memory1'], [modelName '/Data Store Memory (Heartbeat)'],  'Position', [40, 70, 100, 90]);
add_block([refFeedback '/Data Store Memory2'], [modelName '/Data Store Memory (GPS)'],        'Position', [40, 100, 100, 120]);

% Wire sensor feedback inputs
add_line(modelName, 'Mux_Acc/1',      'HIL Sensor/1', 'autorouting', 'on');
add_line(modelName, 'Mux_Gyro/1',     'HIL Sensor/2', 'autorouting', 'on');
add_line(modelName, 'Gauss_to_uT/1',  'HIL Sensor/3', 'autorouting', 'on');
add_line(modelName, 'Select_Sensors/18', 'HIL Sensor/4', 'autorouting', 'on');
add_line(modelName, 'Select_Sensors/19', 'HIL Sensor/5', 'autorouting', 'on');
add_line(modelName, 'Compute Timestamp/1', 'HIL Sensor/6', 'autorouting', 'on');

add_line(modelName, 'Mux_LLA/1',      'HIL_GPS/1', 'autorouting', 'on');
add_line(modelName, 'Mux_Velocity/1', 'HIL_GPS/2', 'autorouting', 'on');
add_line(modelName, 'Select_Sensors/4', 'HIL_GPS/3', 'autorouting', 'on');
add_line(modelName, 'Select_Sensors/8', 'HIL_GPS/4', 'autorouting', 'on');
add_line(modelName, 'Compute Timestamp/1', 'HIL_GPS/5', 'autorouting', 'on');

add_line(modelName, 'Compute Timestamp/1', 'Send Heartbeat/1', 'autorouting', 'on');

%% ====================================================================
%% COMBINE + SEND (no RotorParameters -- see header note)
%% ====================================================================
add_block('simulink/Signal Routing/Mux', [modelName '/Combine_MAVLink_Streams'], ...
    'Position', [580, 100, 620, 460], 'Inputs', '3');
add_line(modelName, 'HIL Sensor/1', 'Combine_MAVLink_Streams/1', 'autorouting', 'on');
add_line(modelName, 'HIL_GPS/1', 'Combine_MAVLink_Streams/2', 'autorouting', 'on');
add_line(modelName, 'Send Heartbeat/1', 'Combine_MAVLink_Streams/3', 'autorouting', 'on');

add_block('HITL_Plant_top/MAVLink Bridge Sink', [modelName '/MAVLink Bridge Sink'], ...
    'Position', [680, 250, 760, 310]);
add_line(modelName, 'Combine_MAVLink_Streams/1', 'MAVLink Bridge Sink/1', 'autorouting', 'on');

% NOTE: the block's real mask parameters are PixhawkSerialPorttemp/PixhawkSerialPortManually/
% FieldTable, not the raw px4.internal.block.MAVLinkBridgeBase property names -- confirmed by
% reading the instantiated block's MaskNames (the abstract library block's MaskNames came back
% empty earlier in this session, which is what led to guessing wrong the first time).
% PixhawkSerialPortManually is set to the placeholder string 'hitlParams.serialPort' below, but
% this is NOT evaluated as an expression -- MaskVariables shows this parameter as "&2" ("take
% literally"), not "@2" ("evaluate"). Confirmed by actually running the model: it tried to open a
% port literally named '\\.\hitlParams.serialPort'. The placeholder is harmless once this model is
% inlined into VehicleSilSimulation.slx, because initVehicleSIL.m overwrites it with the real COM
% port on every run via a direct set_param call (see initVehicleSIL.m, after load_system). But if
% you ever run PX4HITLConnector.slx standalone (not inlined), you must set_param this yourself
% first. FieldTable (the QGC UDP relay) is a fixed default here -- change it by editing this
% block's mask directly, or rerun this build script with a different default, if a non-standard
% QGC port is ever needed.
set_param([modelName '/MAVLink Bridge Sink'], ...
    'PixhawkSerialPorttemp', 'Specify manually', ...
    'PixhawkSerialPortManually', 'hitlParams.serialPort', ...
    'FieldTable', "{'UDP', '127.0.0.1', '14550'}");

%% ====================================================================
%% ACTUATOR READBACK: MAVLink Bridge Source -> raw bytes -> HIL_ACTUATOR_CONTROLS.controls[16]
%% ====================================================================
add_block('HITL_Plant_top/MAVLink Bridge Source', [modelName '/MAVLink Bridge Source'], ...
    'Position', [680, 500, 760, 560]);
% Must reference the SAME PixhawkSerialPortManually value as the Sink block above so the two share
% one serial connection object instead of each opening the port separately (see
% px4.internal.block.MAVLinkBridgeBase.setupImpl -- connections are keyed by this exact string).
set_param([modelName '/MAVLink Bridge Source'], ...
    'PixhawkSerialPorttemp', 'Specify manually', ...
    'PixhawkSerialPortManually', 'hitlParams.serialPort', ...
    'FieldTable', "{'UDP', '127.0.0.1', '14550'}", ...
    'dataLength', '1024');   % literal instead of the copied default 'MAVLink_Input_Read_Size'
                              % expression (undefined here) -- matches MAVLinkSource.m's own
                              % property default (dataLength = 1024)

% Reuse "Read actuator output"'s byte-slicing logic verbatim (it's generic -- just trims the raw
% stream to Length bytes), but build the deserialize+select step fresh (see header note) instead of
% copying "Split actuator command" wholesale, since that subsystem also contains the VTOL-specific
% motor/tilt/control-surface split this repo doesn't need -- only the raw controls[16] array does.
add_block('HITL_Plant_top/Read actuator output', [modelName '/Read actuator output'], ...
    'Position', [800, 500, 920, 560]);
add_line(modelName, 'MAVLink Bridge Source/1', 'Read actuator output/1', 'autorouting', 'on');
add_line(modelName, 'MAVLink Bridge Source/2', 'Read actuator output/2', 'autorouting', 'on');

% Copy an ALREADY-configured-for-HIL_ACTUATOR_CONTROLS instance rather than adding a fresh
% uavmavlinklib/MAVLink Deserializer and reconfiguring it: the messageType mask parameter's
% callback (uav.sluav.internal.mavlink.block.DeserializerBlockMask) errors with "Not enough args"
% when driven via set_param outside an actual mask-dialog GUI context, so reconfiguring headlessly
% isn't reliable -- copying a working instance sidesteps that entirely.
add_block('HITL_Plant_top/Split actuator command/MAVLink Deserializer', ...
    [modelName '/Actuator_MAVLink_Deserializer'], 'Position', [960, 500, 1080, 560]);
add_line(modelName, 'Read actuator output/1', 'Actuator_MAVLink_Deserializer/1', 'autorouting', 'on');

add_block('simulink/Sinks/Terminator', [modelName '/Term_Deserializer_Status'], 'Position', [1100, 500, 1120, 520]);
add_line(modelName, 'Actuator_MAVLink_Deserializer/1', 'Term_Deserializer_Status/1', 'autorouting', 'on');

add_block('simulink/Signal Routing/Bus Selector', [modelName '/Select_Controls'], ...
    'Position', [1100, 540, 1200, 570]);
set_param([modelName '/Select_Controls'], 'OutputSignals', 'Payload.controls');
add_line(modelName, 'Actuator_MAVLink_Deserializer/2', 'Select_Controls/1', 'autorouting', 'on');

add_block('simulink/Signal Attributes/Data Type Conversion', [modelName '/Controls_to_double'], ...
    'Position', [1220, 540, 1300, 570], 'OutDataTypeStr', 'double');
add_line(modelName, 'Select_Controls/1', 'Controls_to_double/1', 'autorouting', 'on');

%% ====================================================================
%% Reuse this repo's OWN, already-validated actuator output mapping
%% (same model the existing SITL "PX4 Interface" uses -- see CLAUDE_HITL.md section 10)
%% ====================================================================
add_block('simulink/Ports & Subsystems/Model', [modelName '/Hex Output Mapping'], ...
    'Position', [1320, 540, 1450, 600], 'ModelName', 'PX4OutputMappingHex.slx');
add_line(modelName, 'Controls_to_double/1', 'Hex Output Mapping/1', 'autorouting', 'on');

%% ====================================================================
%% ASSEMBLE ActuatorCommandBus
%% ====================================================================
% EngineCommandBus: zero, same default the existing SITL hexarotor path already uses (PX4 Interface
% block "EngineCommandBus" Constant=0) -- not something this repo currently derives from PX4 either
% way for the hexarotor vehicle.
add_block('simulink/Sources/Constant', [modelName '/EngineCommandBus_zero'], ...
    'Position', [1320, 620, 1360, 640], 'Value', '0', 'OutDataTypeStr', 'Bus: EngineCommandBus');

% controllerArmed: intentionally hardcoded false for now. The existing SITL "PX4 Interface" derives
% an analogous "got valid data yet" gate from pixhawk_sil_connector's own internal mode/data-ready
% signal (a documented field of that custom S-function). The MAVLink Deserializer's "status" output
% here has no equivalent documentation available in this session, and uav_HITL_sample's own
% reference implementation just terminates it unused -- guessing at its semantics risks getting the
% polarity backwards (status==X meaning "no new data" vs "new data"), which for an *armed* flag
% means guessing wrong fails UNSAFE (stuck armed) instead of safe. Hardcoding false until someone
% with the MAVLink HIL_ACTUATOR_CONTROLS spec / hardware in hand verifies the right condition is a
% deliberate, documented gap -- see CLAUDE_HITL.md section "what remains".
add_block('simulink/Sources/Constant', [modelName '/ControllerArmed_FALSE'], ...
    'Position', [1100, 600, 1140, 620], 'Value', '0', 'OutDataTypeStr', 'double');

add_block('simulink/Signal Routing/Bus Assignment', [modelName '/Assign_ActuatorCommandBus'], ...
    'Position', [1460, 400, 1500, 440]);
set_param([modelName '/Assign_ActuatorCommandBus'], 'AssignedSignals', 'controllerArmed,ServosCommandBus,EngineCommandBus');
add_block('simulink/Sources/Constant', [modelName '/ActuatorCommandBus_template'], ...
    'Position', [1400, 380, 1450, 400], 'Value', '0', 'OutDataTypeStr', 'Bus: ActuatorCommandBus');

add_line(modelName, 'ActuatorCommandBus_template/1', 'Assign_ActuatorCommandBus/1', 'autorouting', 'on');
add_line(modelName, 'ControllerArmed_FALSE/1', 'Assign_ActuatorCommandBus/2', 'autorouting', 'on');
add_line(modelName, 'Hex Output Mapping/1', 'Assign_ActuatorCommandBus/3', 'autorouting', 'on');
add_line(modelName, 'EngineCommandBus_zero/1', 'Assign_ActuatorCommandBus/4', 'autorouting', 'on');
add_line(modelName, 'Assign_ActuatorCommandBus/1', 'ActuatorCommandBus/1', 'autorouting', 'on');

%% ---- Save ----------------------------------------------------------
Simulink.BlockDiagram.arrangeSystem(modelName);
save_system(modelName, savePath);
fprintf('\nModel saved: %s\n', savePath);


