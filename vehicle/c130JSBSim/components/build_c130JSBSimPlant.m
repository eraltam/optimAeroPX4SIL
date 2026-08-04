%% build_c130JSBSimPlant.m
% Populates c130JSBSim.slx's "physics" middle section, same pattern as
% vehicle/c172pJSBSim/components/build_c172pJSBSimPlant.m -- see PLAN_JSBSIM_SFUNCTION_F22_C130.md
% Phase C. The actuators Model Reference has already been repointed from actuatorsC172p.slx to
% this vehicle's own local actuatorsC130JSBSim.slx (a byte-for-byte clone, see setUpActuators.m's
% "c130jsbsim" case for why its inert enginePistonPropC172p.slx Model Reference stays in place),
% and the 18 c172p-specific plant-wiring blocks have already been deleted, leaving the same
% 16-block keep-list documented in build_c172pJSBSimPlant.m's own header comment: Inport
% (ActuatorCommandBus, EnvironmentBus, FailureBus), Outport (VehicleBus, ActuatorBus), Ground ->
% ActuatorBus stub, actuators (ModelReference) + its ActuatorCommandBus/EnvironmentBus/FailureBus
% feed chains (Goto10/From28, Goto11/From13+delay, Goto2/From10), and Goto7 (harmless leftover).
%
% Differences from build_c172pJSBSimPlant.m, all driven by C130's own io-config
% (c130JSBSim_io.xml):
%   - packC130JSBSimInputs.m outputs a 10-element vector (aileron, elevator, rudder, throttle x4,
%     gear-cmd-norm=1, left/right brakes) instead of 4 -- still only ONE MATLAB Function block, still only one signal
%     into JSBSim_SFunction's input port.
%   - JSBSim_SFunction's block parameters point at aircraft='C130' (matching
%     aircraft/C130/C130.xml), reset_file='c130JSBSimInit', and this repo's own
%     c130JSBSim_io.xml.
%   - unpackC130JSBSimOutputs.m has only 5 inputs (jsbsimState, jsbsimAero, gravityScalar_mps2,
%     referenceLatitude_deg, referenceLongitude_deg) -- confirmed by reading the ACTUAL live
%     c172pJSBSim.slx via model_read before writing this script: its own unpackJSBSimOutputs block
%     only has 5 connected inputs, not the 6 build_c172pJSBSimPlant.m's own comments describe (that
%     script's refAlt_m Constant + 6th input wire is stale/aspirational, never actually built this
%     way in the deployed model). Do not add a refAlt_m Constant here to "match" that stale
%     comment.
%
% PREREQUISITE (run once per MATLAB session before this script):
%   cd(optimAeroPX4SIL root); addpath(genpath('signals')); addpath('vehicle');
%   BusDefinition('c130JSBSim'); stepSize_s = 0.004;
% (c130JSBSim shares c172p's/c172pJSBSim's Servos/ServosCommand/Failure bus definitions -- see
% Phase D.)
%
% PREREQUISITE: c130JSBSim.slx must already exist (cloned from vehicle/c172pJSBSim/c172pJSBSim.slx)
% with the actuators Model Reference repointed to actuatorsC130JSBSim.slx and the same 18 blocks
% build_c172pJSBSimPlant.m's own header describes as "non-reused" already deleted -- this
% precondition was executed once interactively via model_edit (see this session's own notes) and is
% not re-encoded here as a script step.

mdl = 'c130JSBSim';
if ~bdIsLoaded(mdl)
    error('build_c130JSBSimPlant:NotLoaded', ...
        '%s must already be loaded (cloned from c172pJSBSim.slx with unused blocks deleted and actuators repointed) before running this script.', mdl);
end

thisDir = fileparts(mfilename('fullpath'));   % vehicle/c130JSBSim/components
silRoot = fileparts(fileparts(fileparts(thisDir)));  % optimAeroPX4SIL root

%% ====================================================================
%% 1. Bus selectors tapping the kept signals
%% ====================================================================
add_block('simulink/Signal Routing/Bus Selector', [mdl '/Select_Gravity'], ...
    'Position', [140, 500, 220, 520], 'OutputSignals', 'EarthEnvironment.gravityScalar_mps2');
add_line(mdl, 'EnvironmentBus/1', 'Select_Gravity/1', 'autorouting', 'on');

add_block('simulink/Signal Routing/Bus Selector', [mdl '/Select_Throttle'], ...
    'Position', [140, 540, 220, 560], 'OutputSignals', 'EngineCommandBus.cmdThrottle_unit');
add_line(mdl, 'Variable Integer Delay/1', 'Select_Throttle/1', 'autorouting', 'on');

add_block('simulink/Signal Routing/Bus Selector', [mdl '/Select_ServoPos'], ...
    'Position', [140, 580, 220, 620], ...
    'OutputSignals', 'ServosBus.posAileron_rad,ServosBus.posElevator_rad,ServosBus.posRudder_rad');
add_line(mdl, 'actuators/1', 'Select_ServoPos/1', 'autorouting', 'on');

%% ====================================================================
%% 2. packC130JSBSimInputs (MATLAB Function)
%% ====================================================================
packBlk = [mdl '/packC130JSBSimInputs'];
add_block('simulink/User-Defined Functions/MATLAB Function', packBlk, 'Position', [280, 560, 420, 640]);

open_system(mdl);
pause(0.5);
rt = sfroot();
mdlObj = rt.find('-isa', 'Simulink.BlockDiagram', 'Name', mdl);
charts = mdlObj.find('-isa', 'Stateflow.EMChart');
packChart = [];
for i = 1:numel(charts)
    if isempty(packChart) && contains(charts(i).Path, 'packC130JSBSimInputs')
        packChart = charts(i);
    end
end
if isempty(packChart)
    error('build_c130JSBSimPlant:ChartNotFound', 'Could not find packC130JSBSimInputs chart.');
end
packChart.Script = fileread(fullfile(thisDir, 'packC130JSBSimInputs.m'));

add_line(mdl, 'Select_ServoPos/1', 'packC130JSBSimInputs/1', 'autorouting', 'on');
add_line(mdl, 'Select_ServoPos/2', 'packC130JSBSimInputs/2', 'autorouting', 'on');
add_line(mdl, 'Select_ServoPos/3', 'packC130JSBSimInputs/3', 'autorouting', 'on');
add_line(mdl, 'Select_Throttle/1', 'packC130JSBSimInputs/4', 'autorouting', 'on');

%% ====================================================================
%% 3. JSBSim_SFunction block
%% ====================================================================
sfunBlk = [mdl '/JSBSim_SFunction'];
ioConfigPath = strrep(fullfile(silRoot, 'c130JSBSim_io.xml'), '\', '/');
% io_config uses an ABSOLUTE path deliberately -- same CWD-at-compile-time reasoning as
% c172pJSBSim's own JSBSim_SFunction block (a relative 'c130JSBSim_io' resolves against MATLAB's
% CWD at compile time, which differs between a standalone update_diagram and compiling nested
% under the full VehicleSilSimulation.slx). aircraft name / reset file stay relative -- those
% resolve via JSBSim's own internal SetAircraftPath("aircraft")/JSBSIM_ROOT_OVERRIDE mechanism.
add_block('simulink/User-Defined Functions/S-Function', sfunBlk, 'Position', [480, 560, 600, 620], ...
    'FunctionName', 'JSBSim_SFunction', ...
    'Parameters', sprintf('''C130'', stepSize_s, 0, '''', ''c130JSBSimInit'', ''%s''', ioConfigPath));
add_line(mdl, 'packC130JSBSimInputs/1', 'JSBSim_SFunction/1', 'autorouting', 'on');

%% ====================================================================
%% 4. unpackC130JSBSimOutputs (MATLAB Function)
%% ====================================================================
unpackBlk = [mdl '/unpackC130JSBSimOutputs'];
add_block('simulink/User-Defined Functions/MATLAB Function', unpackBlk, 'Position', [660, 500, 820, 700]);

pause(0.5);
charts = mdlObj.find('-isa', 'Stateflow.EMChart');
unpackChart = [];
for i = 1:numel(charts)
    if contains(charts(i).Path, 'unpackC130JSBSimOutputs')
        unpackChart = charts(i);
    end
end
if isempty(unpackChart)
    error('build_c130JSBSimPlant:ChartNotFound', 'Could not find unpackC130JSBSimOutputs chart.');
end
unpackChart.Script = fileread(fullfile(thisDir, 'unpackC130JSBSimOutputs.m'));

% unpackC130JSBSimOutputs inputs: jsbsimState, jsbsimAero, gravityScalar_mps2,
% referenceLatitude_deg, referenceLongitude_deg -- only 5, see this file's own header note above.
add_line(mdl, 'JSBSim_SFunction/1', 'unpackC130JSBSimOutputs/1', 'autorouting', 'on');
add_line(mdl, 'JSBSim_SFunction/2', 'unpackC130JSBSimOutputs/2', 'autorouting', 'on');
add_line(mdl, 'Select_Gravity/1', 'unpackC130JSBSimOutputs/3', 'autorouting', 'on');

add_block('simulink/Sources/Constant', [mdl '/refLat_deg'], 'Position', [560, 700, 620, 720], ...
    'Value', 'referenceLatitude_deg');
add_block('simulink/Sources/Constant', [mdl '/refLon_deg'], 'Position', [560, 730, 620, 750], ...
    'Value', 'referenceLongitude_deg');
add_line(mdl, 'refLat_deg/1', 'unpackC130JSBSimOutputs/4', 'autorouting', 'on');
add_line(mdl, 'refLon_deg/1', 'unpackC130JSBSimOutputs/5', 'autorouting', 'on');

%% ====================================================================
%% 5. Feed AirDataBus/BodyStateBus back into actuators (u2/u4), one-step-delayed to break the
%%    algebraic loop (same Memory-block pattern build_c172pJSBSimPlant.m already used).
%% ====================================================================
% unpackC130JSBSimOutputs outputs (order): aircraftVelInNED_mps(1), aircraftPosInNED_m(2),
% aircraftEulerAngles_rad(3), aircraftVelInBody_mps(4), aircraftAngVelInBody_radps(5),
% aircraftAngAccelInBody_radps2(6), aircraftAccelInBody_mps2(7), DCM_be(8), aircraftAccelbe_mps2(9),
% airspeedInBody_mps(10), alpha_rad(11), beta_rad(12), isVehicleAirborne(13)

% -- BodyStateBus (9 elements, order per signals/BusDefinition.m) --
add_block('simulink/Signal Routing/Bus Creator', [mdl '/BodyStateBus_Creator'], ...
    'Position', [900, 400, 960, 600], 'Inputs', '9', 'UseBusObject', 'on', 'BusObject', 'BodyStateBus');
for i = 1:9
    add_line(mdl, sprintf('unpackC130JSBSimOutputs/%d', i), sprintf('BodyStateBus_Creator/%d', i), 'autorouting', 'on');
end

add_block('simulink/Discrete/Memory', [mdl '/Memory_BodyStateBus'], 'Position', [1000, 480, 1040, 520]);
add_line(mdl, 'BodyStateBus_Creator/1', 'Memory_BodyStateBus/1', 'autorouting', 'on');
add_line(mdl, 'Memory_BodyStateBus/1', 'actuators/4', 'autorouting', 'on');

% -- AirDataBus (3 elements: airspeedInBody_mps, alpha_rad, beta_rad) --
add_block('simulink/Signal Routing/Bus Creator', [mdl '/AirDataBus_Creator'], ...
    'Position', [900, 620, 960, 700], 'Inputs', '3', 'UseBusObject', 'on', 'BusObject', 'AirDataBus');
add_line(mdl, 'unpackC130JSBSimOutputs/10', 'AirDataBus_Creator/1', 'autorouting', 'on');
add_line(mdl, 'unpackC130JSBSimOutputs/11', 'AirDataBus_Creator/2', 'autorouting', 'on');
add_line(mdl, 'unpackC130JSBSimOutputs/12', 'AirDataBus_Creator/3', 'autorouting', 'on');

add_block('simulink/Discrete/Memory', [mdl '/Memory_AirDataBus'], 'Position', [1000, 640, 1040, 680]);
add_line(mdl, 'AirDataBus_Creator/1', 'Memory_AirDataBus/1', 'autorouting', 'on');
add_line(mdl, 'Memory_AirDataBus/1', 'actuators/2', 'autorouting', 'on');

%% ====================================================================
%% 6. VehicleConfigurationBus, MassPropertiesBus, AircraftForcesMomentsBus (vestigial once JSBSim
%%    owns the dynamics; VehicleConfigurationBus.isVehicleAirborne is real data from JSBSim's own
%%    h-agl-ft).
%% ====================================================================
add_block('simulink/Signal Routing/Bus Creator', [mdl '/VehicleConfigurationBus_Creator'], ...
    'Position', [900, 720, 960, 760], 'Inputs', '1', 'UseBusObject', 'on', 'BusObject', 'VehicleConfigurationBus');
add_line(mdl, 'unpackC130JSBSimOutputs/13', 'VehicleConfigurationBus_Creator/1', 'autorouting', 'on');

add_block('simulink/Sources/Constant', [mdl '/MassPropertiesBus_Const'], ...
    'Position', [900, 780, 980, 820], 'OutDataTypeStr', 'Bus: MassPropertiesBus', ...
    'Value', ['setfield(setfield(setfield(setfield(' ...
              'Simulink.Bus.createMATLABStruct(''MassPropertiesBus''),' ...
              '''aircraftMass_kg'',vehicleParams.dryMass_kg),' ...
              '''aircraftCg_m'',vehicleParams.aircraftCg_m),' ...
              '''aircraftInertiaInBody_kgm2'',vehicleParams.aircraftInertialBody_kgm2),' ...
              '''fuelLevelFraction'',1)']);

add_block('simulink/Sources/Constant', [mdl '/AircraftForcesMomentsBus_Const'], ...
    'Position', [900, 840, 980, 880], 'OutDataTypeStr', 'Bus: AircraftForcesMomentsBus', ...
    'Value', 'Simulink.Bus.createMATLABStruct(''AircraftForcesMomentsBus'')');

%% ====================================================================
%% 7. Final VehicleBus BusCreator -> root Outport (already exists: VehicleBus)
%% ====================================================================
add_block('simulink/Signal Routing/Bus Creator', [mdl '/VehicleBus_Creator'], ...
    'Position', [1080, 550, 1140, 700], 'Inputs', '5', 'UseBusObject', 'on', 'BusObject', 'VehicleBus');
add_line(mdl, 'VehicleConfigurationBus_Creator/1', 'VehicleBus_Creator/1', 'autorouting', 'on');
add_line(mdl, 'BodyStateBus_Creator/1', 'VehicleBus_Creator/2', 'autorouting', 'on');
add_line(mdl, 'MassPropertiesBus_Const/1', 'VehicleBus_Creator/3', 'autorouting', 'on');
add_line(mdl, 'AircraftForcesMomentsBus_Const/1', 'VehicleBus_Creator/4', 'autorouting', 'on');
add_line(mdl, 'AirDataBus_Creator/1', 'VehicleBus_Creator/5', 'autorouting', 'on');

add_line(mdl, 'VehicleBus_Creator/1', 'VehicleBus/1', 'autorouting', 'on');

%% ---- Save ------------------------------------------------------------
Simulink.BlockDiagram.arrangeSystem(mdl);
save_system(mdl);
fprintf('\n%s built and saved.\n', mdl);
