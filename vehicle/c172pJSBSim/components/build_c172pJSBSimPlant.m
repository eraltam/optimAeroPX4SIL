%% build_c172pJSBSimPlant.m
% Populates c172pJSBSim.slx's "physics" middle section: the actuators Model Reference is reused
% unchanged (already present, cloned from c172p.slx along with the root Inport/Outport contract);
% this script deletes nothing further and only ADDS the JSBSim S-Function + its 2 adapter MATLAB
% Function blocks + the bus assembly feeding the root VehicleBus outport. See
% PLAN_JSBSIM_SFUNCTION_HYBRID_C172P.md Phase C.
%
% PREREQUISITE (run once per MATLAB session before this script):
%   cd(optimAeroPX4SIL root); addpath(genpath('signals')); addpath('vehicle');
%   BusDefinition('c172p'); stepSize_s = 0.004;
% (c172pJSBSim shares c172p's Servos/ServosCommand/Failure bus definitions -- see Phase D.)
%
% PREREQUISITE: c172pJSBSim.slx must already exist with its 60 non-reused blocks deleted, leaving
% only: Inport(ActuatorCommandBus,EnvironmentBus,FailureBus), Outport(VehicleBus,ActuatorBus),
% Ground->ActuatorBus (unchanged from the c172p.slx baseline -- that root output was already just
% a Ground stub there, not wired to real data; not this script's problem to fix), actuators
% (ModelReference, actuatorsC172p.slx) + its ActuatorCommandBus/EnvironmentBus/FailureBus feed
% chains (Goto10/From28, Goto11/From13+delay, Goto2/From10), and Goto7 (harmless leftover reading
% actuators' own output, unused elsewhere). This precondition was executed once interactively and
% is not re-encoded here as a script step -- if rebuilding from a fresh clone of c172p.slx, re-run
% that deletion (see the plan's Phase C session notes for the exact keep-list).

mdl = 'c172pJSBSim';
if ~bdIsLoaded(mdl)
    error('build_c172pJSBSimPlant:NotLoaded', ...
        '%s must already be loaded (cloned from c172p.slx with unused blocks deleted) before running this script.', mdl);
end

thisDir = fileparts(mfilename('fullpath'));   % vehicle/c172pJSBSim/components

%% ====================================================================
%% 1. Bus selectors tapping the kept signals
%% ====================================================================
% Gravity, straight from the EnvironmentBus Inport (feeds unpackJSBSimOutputs).
add_block('simulink/Signal Routing/Bus Selector', [mdl '/Select_Gravity'], ...
    'Position', [140, 500, 220, 520], 'OutputSignals', 'EarthEnvironment.gravityScalar_mps2');
add_line(mdl, 'EnvironmentBus/1', 'Select_Gravity/1', 'autorouting', 'on');

% Throttle command, straight off the already-delayed ActuatorCommandBus (same signal that feeds
% actuators.u1) -- throttle isn't rate-limited by actuatorsC172p, so no need to go through it.
add_block('simulink/Signal Routing/Bus Selector', [mdl '/Select_Throttle'], ...
    'Position', [140, 540, 220, 560], 'OutputSignals', 'EngineCommandBus.cmdThrottle_unit');
add_line(mdl, 'Variable Integer Delay/1', 'Select_Throttle/1', 'autorouting', 'on');

% Achieved (rate-limited) surface positions, off actuators' own ActuatorBus output.
add_block('simulink/Signal Routing/Bus Selector', [mdl '/Select_ServoPos'], ...
    'Position', [140, 580, 220, 620], ...
    'OutputSignals', 'ServosBus.posAileron_rad,ServosBus.posElevator_rad,ServosBus.posRudder_rad');
add_line(mdl, 'actuators/1', 'Select_ServoPos/1', 'autorouting', 'on');

%% ====================================================================
%% 2. packJSBSimInputs (MATLAB Function)
%% ====================================================================
packBlk = [mdl '/packJSBSimInputs'];
add_block('simulink/User-Defined Functions/MATLAB Function', packBlk, 'Position', [280, 560, 420, 640]);

open_system(mdl);
pause(0.5);
rt = sfroot();
mdlObj = rt.find('-isa', 'Simulink.BlockDiagram', 'Name', mdl);
charts = mdlObj.find('-isa', 'Stateflow.EMChart');
% Freshly added block -> its chart is the one not yet scripted (Script is the template default).
packChart = [];
for i = 1:numel(charts)
    if isempty(packChart) && contains(charts(i).Path, 'packJSBSimInputs')
        packChart = charts(i);
    end
end
if isempty(packChart)
    error('build_c172pJSBSimPlant:ChartNotFound', 'Could not find packJSBSimInputs chart.');
end
packChart.Script = fileread(fullfile(thisDir, 'packJSBSimInputs.m'));

add_line(mdl, 'Select_ServoPos/1', 'packJSBSimInputs/1', 'autorouting', 'on');
add_line(mdl, 'Select_ServoPos/2', 'packJSBSimInputs/2', 'autorouting', 'on');
add_line(mdl, 'Select_ServoPos/3', 'packJSBSimInputs/3', 'autorouting', 'on');
add_line(mdl, 'Select_Throttle/1', 'packJSBSimInputs/4', 'autorouting', 'on');

%% ====================================================================
%% 3. JSBSim_SFunction block
%% ====================================================================
sfunBlk = [mdl '/JSBSim_SFunction'];
% io_config uses an ABSOLUTE path deliberately -- a relative 'c172pJSBSim_io' resolves against
% MATLAB's CWD at compile time, which differs between a standalone update_diagram of this model
% and compiling it as a nested Model Reference under the full VehicleSilSimulation.slx (confirmed
% by direct testing: relative path works standalone, fails nested). aircraft name / reset file
% stay relative -- those resolve via JSBSim's own internal SetAircraftPath("aircraft") mechanism,
% not a bare SGPath(string) call, and were not observed to have this problem.
add_block('simulink/User-Defined Functions/S-Function', sfunBlk, 'Position', [480, 560, 600, 620], ...
    'FunctionName', 'JSBSim_SFunction', ...
    'Parameters', '''c172p'', stepSize_s, 0, '''', ''c172pJSBSimInit'', ''C:/SITL/AnelloSummer/optimAeroPX4SIL/c172pJSBSim_io.xml''');
add_line(mdl, 'packJSBSimInputs/1', 'JSBSim_SFunction/1', 'autorouting', 'on');

%% ====================================================================
%% 4. unpackJSBSimOutputs (MATLAB Function)
%% ====================================================================
unpackBlk = [mdl '/unpackJSBSimOutputs'];
add_block('simulink/User-Defined Functions/MATLAB Function', unpackBlk, 'Position', [660, 500, 820, 700]);

pause(0.5);
charts = mdlObj.find('-isa', 'Stateflow.EMChart');
unpackChart = [];
for i = 1:numel(charts)
    if contains(charts(i).Path, 'unpackJSBSimOutputs')
        unpackChart = charts(i);
    end
end
if isempty(unpackChart)
    error('build_c172pJSBSimPlant:ChartNotFound', 'Could not find unpackJSBSimOutputs chart.');
end
unpackChart.Script = fileread(fullfile(thisDir, 'unpackJSBSimOutputs.m'));

% unpackJSBSimOutputs inputs: jsbsimState, jsbsimAero, gravityScalar_mps2, referenceLatitude_deg,
% referenceLongitude_deg, referenceAltitude_m
add_line(mdl, 'JSBSim_SFunction/1', 'unpackJSBSimOutputs/1', 'autorouting', 'on');
add_line(mdl, 'JSBSim_SFunction/2', 'unpackJSBSimOutputs/2', 'autorouting', 'on');
add_line(mdl, 'Select_Gravity/1', 'unpackJSBSimOutputs/3', 'autorouting', 'on');

add_block('simulink/Sources/Constant', [mdl '/refLat_deg'], 'Position', [560, 700, 620, 720], ...
    'Value', 'referenceLatitude_deg');
add_block('simulink/Sources/Constant', [mdl '/refLon_deg'], 'Position', [560, 730, 620, 750], ...
    'Value', 'referenceLongitude_deg');
add_block('simulink/Sources/Constant', [mdl '/refAlt_m'], 'Position', [560, 760, 620, 780], ...
    'Value', 'referenceAltitude_m');
add_line(mdl, 'refLat_deg/1', 'unpackJSBSimOutputs/4', 'autorouting', 'on');
add_line(mdl, 'refLon_deg/1', 'unpackJSBSimOutputs/5', 'autorouting', 'on');
add_line(mdl, 'refAlt_m/1', 'unpackJSBSimOutputs/6', 'autorouting', 'on');

%% ====================================================================
%% 5. Feed AirDataBus/BodyStateBus back into actuators (u2/u4), one-step-delayed to break the
%%    algebraic loop (same Memory-block pattern the deleted graph already used elsewhere).
%% ====================================================================
% unpackJSBSimOutputs outputs (order): aircraftVelInNED_mps(1), aircraftPosInNED_m(2),
% aircraftEulerAngles_rad(3), aircraftVelInBody_mps(4), aircraftAngVelInBody_radps(5),
% aircraftAngAccelInBody_radps2(6), aircraftAccelInBody_mps2(7), DCM_be(8), aircraftAccelbe_mps2(9),
% airspeedInBody_mps(10), alpha_rad(11), beta_rad(12), isVehicleAirborne(13)

% -- BodyStateBus (9 elements, order per signals/BusDefinition.m) --
add_block('simulink/Signal Routing/Bus Creator', [mdl '/BodyStateBus_Creator'], ...
    'Position', [900, 400, 960, 600], 'Inputs', '9', 'UseBusObject', 'on', 'BusObject', 'BodyStateBus');
for i = 1:9
    add_line(mdl, sprintf('unpackJSBSimOutputs/%d', i), sprintf('BodyStateBus_Creator/%d', i), 'autorouting', 'on');
end

add_block('simulink/Discrete/Memory', [mdl '/Memory_BodyStateBus'], 'Position', [1000, 480, 1040, 520]);
add_line(mdl, 'BodyStateBus_Creator/1', 'Memory_BodyStateBus/1', 'autorouting', 'on');
add_line(mdl, 'Memory_BodyStateBus/1', 'actuators/4', 'autorouting', 'on');

% -- AirDataBus (3 elements: airspeedInBody_mps, alpha_rad, beta_rad) --
add_block('simulink/Signal Routing/Bus Creator', [mdl '/AirDataBus_Creator'], ...
    'Position', [900, 620, 960, 700], 'Inputs', '3', 'UseBusObject', 'on', 'BusObject', 'AirDataBus');
add_line(mdl, 'unpackJSBSimOutputs/10', 'AirDataBus_Creator/1', 'autorouting', 'on');
add_line(mdl, 'unpackJSBSimOutputs/11', 'AirDataBus_Creator/2', 'autorouting', 'on');
add_line(mdl, 'unpackJSBSimOutputs/12', 'AirDataBus_Creator/3', 'autorouting', 'on');

add_block('simulink/Discrete/Memory', [mdl '/Memory_AirDataBus'], 'Position', [1000, 640, 1040, 680]);
add_line(mdl, 'AirDataBus_Creator/1', 'Memory_AirDataBus/1', 'autorouting', 'on');
add_line(mdl, 'Memory_AirDataBus/1', 'actuators/2', 'autorouting', 'on');

%% ====================================================================
%% 6. VehicleConfigurationBus, MassPropertiesBus, AircraftForcesMomentsBus (see plan Phase C --
%%    MassPropertiesBus/AircraftForcesMomentsBus are vestigial once JSBSim owns the dynamics;
%%    VehicleConfigurationBus.isVehicleAirborne is real data from JSBSim's own h-agl-ft).
%% ====================================================================
add_block('simulink/Signal Routing/Bus Creator', [mdl '/VehicleConfigurationBus_Creator'], ...
    'Position', [900, 720, 960, 760], 'Inputs', '1', 'UseBusObject', 'on', 'BusObject', 'VehicleConfigurationBus');
add_line(mdl, 'unpackJSBSimOutputs/13', 'VehicleConfigurationBus_Creator/1', 'autorouting', 'on');

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
