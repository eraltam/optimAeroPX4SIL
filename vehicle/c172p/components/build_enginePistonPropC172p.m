%% build_enginePistonPropC172p.m
% Programmatically creates vehicle/c172p/components/enginePistonPropC172p.slx -- a drop-in
% replacement for vehicle/F16/components/engineModelF16.slx (single-engine turbojet) with the
% EXACT SAME interface contract, so it can be swapped into a clone of
% vehicle/F16/components/actuators.slx's engine Model Reference block without touching anything
% else in that file (same pattern as
% vehicle/fixedwing_plane/components/build_enginePropFixedwing.m):
%
%   Inputs:  EngineCommandBus, controllerArmed, AirDataBus, EnvironmentBus, BodyStateBus,
%            failureBus
%   Outputs: PropulsionBus (Bus: PropulsionBus -- for "c172p" this is {angVel_radps,
%            EngineForcesMomentsBus, fuelRate_kgps}, same shape as F16's/fixedwing_plane's own
%            PropulsionBus case in signals/PropulsionSystemBusDefinition.m)
%
% Unlike enginePropFixedwing.slx (a constant-kct_nd/kcp_nd electric-motor fit), this model uses
% the REAL Lycoming IO-320 piston-engine limits (jsbsim/engine/eng_io320.xml: maxhp=160,
% maxrpm=2700, idlerpm=550, bsfc=0.32 lb/hp/hr) and the REAL fixed-pitch propeller thrust/power
% coefficient tables (jsbsim/engine/prop_75in2f.xml: C_THRUST(J)/C_POWER(J), J = advance ratio =
% V/(n*D)) evaluated against the vehicle's actual airspeed and air density each step -- see
% vehicle/common/setUpActuators.m's "c172p" case and PLAN_INCORPORACION_AERONAVES_JSBSIM_SITL.md
% §2.3 for sourcing. Throttle command maps linearly to a target engine RPM between idle and max
% (JSBSim's own internal piston-power-vs-manifold-pressure/RPM model is not replicated -- this is
% an explicit engineering simplification, same practice as this project's other actuator-dynamics
% approximations), then a first-order lag (same style as enginePropFixedwing.slx's
% motorAngVel_radps_state) drives the propeller toward that RPM. Thrust acts along +X body axis
% (tractor prop on the nose); reaction torque acts about body X, opposing the propeller's spin.
%
% fuelRate_kgps IS nonzero here (bsfc-based estimate) -- unlike fixedwing_plane's electric motor.
% NOTE (documented fidelity gap): this SIL has no mechanism anywhere to actually decrement vehicle
% mass from a nonzero fuelRate_kgps (dryMass_kg is set once in setUpVehicle.m and never updated by
% any vehicle in this registry) -- fuelRate_kgps is informational/telemetry only at this fidelity
% level, matching how EngineForcesMomentsBus/PropulsionBus.fuelRate_kgps is already only ever
% consumed as a bus field, never fed back into MassPropertiesBus by cgMassInertia.slx (confirmed
% by inspecting F16's own case, which also never gets real mass reduction despite F16's turbojet
% reporting a real fuelRate_kgps).
%
% Prerequisites: BusDefinition('c172p') must have been run at least once in this MATLAB session so
% 'ComponentForcesMomentsBus' and 'PropulsionBus' exist in the base workspace before this model is
% compiled/simulated (not required to just build+save the .slx file itself).

clear; clc;

rootDir = fileparts(mfilename('fullpath'));
modelName = 'enginePistonPropC172p';
savePath  = fullfile(rootDir, [modelName '.slx']);

if bdIsLoaded(modelName)
    close_system(modelName, 0);
end

new_system(modelName);
set_param(modelName, 'SolverType', 'Fixed-step');

%% ====================================================================
%% INPORTS (order matches engineModelF16.slx / enginePropFixedwing.slx exactly)
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
%% BUS SELECTORS -- unlike enginePropFixedwing.slx (electric motor, ignores air data), this piston
%% engine genuinely needs airspeed (for advance ratio J) and air density (for thrust/power).
%% ====================================================================
add_block('simulink/Signal Routing/Bus Selector', [modelName '/Select_EngineCmd'], ...
    'Position', [140, 30, 220, 70], 'OutputSignals', 'cmdThrottle_unit');
add_block('simulink/Signal Routing/Bus Selector', [modelName '/Select_Failure'], ...
    'Position', [140, 330, 220, 370], 'OutputSignals', 'engineFailure_isTrue');
add_block('simulink/Signal Routing/Bus Selector', [modelName '/Select_AirData'], ...
    'Position', [140, 160, 220, 200], 'OutputSignals', 'airspeedInBody_mps');
add_block('simulink/Signal Routing/Bus Selector', [modelName '/Select_Environment'], ...
    'Position', [140, 220, 220, 260], 'OutputSignals', 'AirEnvironment.airDensity_kgpm3');

%% ====================================================================
%% MATLAB FUNCTION: throttle+airspeed+density+failure -> thrust_N, torque_Nm, motorAngVel_radps,
%% fuelRate_kgps
%% ====================================================================
fcn_blk = [modelName '/pistonPropThrustModel'];
add_block('simulink/User-Defined Functions/MATLAB Function', fcn_blk, ...
    'Position', [280, 30, 560, 260]);

%% ====================================================================
%% FORCE/MOMENT VECTOR ASSEMBLY (thrust along +X body, reaction torque about X)
%% ====================================================================
add_block('simulink/Sources/Constant', [modelName '/ZeroForceYZ'], ...
    'Position', [600, 60, 640, 80], 'Value', '[0;0]');
add_block('simulink/Signal Routing/Mux', [modelName '/ForceVecMux'], ...
    'Position', [680, 40, 700, 100], 'Inputs', '2');

add_block('simulink/Sources/Constant', [modelName '/ZeroMomentYZ'], ...
    'Position', [600, 140, 640, 160], 'Value', '[0;0]');
add_block('simulink/Signal Routing/Mux', [modelName '/MomentVecMux'], ...
    'Position', [680, 120, 700, 180], 'Inputs', '2');

add_block('simulink/Signal Routing/Bus Creator', [modelName '/EngineForcesMomentsBus_Creator'], ...
    'Position', [740, 60, 800, 160], 'Inputs', '2', ...
    'UseBusObject', 'on', 'BusObject', 'ComponentForcesMomentsBus');

%% ====================================================================
%% PropulsionBus ASSEMBLY
%% ====================================================================
add_block('simulink/Signal Routing/Bus Creator', [modelName '/PropulsionBus_Creator'], ...
    'Position', [860, 90, 920, 260], 'Inputs', '3', ...
    'UseBusObject', 'on', 'BusObject', 'PropulsionBus');

%% ====================================================================
%% OUTPORT
%% ====================================================================
add_block('simulink/Sinks/Out1', [modelName '/PropulsionBus'], ...
    'Position', [980, 165, 1020, 185], 'Port', '1', ...
    'OutDataTypeStr', 'Bus: PropulsionBus');

%% ====================================================================
%% MATLAB FUNCTION SCRIPT
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

    lines = { ...
'function [thrust_N, torque_Nm, motorAngVel_radps, fuelRate_kgps] = pistonPropThrustModel(cmdThrottle_unit, engineFailure_isTrue, airspeedInBody_mps, airDensity_kgpm3)'
'%#codegen'
'persistent omega_radps_state;'
'if isempty(omega_radps_state)'
'    omega_radps_state = 550 * 2 * pi / 60;'  % start at idle RPM, not zero -- a piston engine
                                                % does not stop between samples like the electric
                                                % motor case
'end'
''
'% Real Lycoming IO-320 limits (jsbsim/engine/eng_io320.xml) and Fixed-Pitch 75in prop tables'
'% (jsbsim/engine/prop_75in2f.xml) -- see vehicle/common/setUpActuators.m ''c172p'' case for'
'% sourcing. Constants inlined (not read from vehicleParams) per the same MATLAB-Function-block'
'% constraint documented in optimAeroPX4SIL/CLAUDE.md section 3. KEEP IN SYNC BY HAND with'
'% setUpActuators.m if changed.'
'idleRPM = 550;'
'maxRPM = 2700;'
'maxPower_W = 160 * 745.7;'
'bsfc_lb_per_hp_hr = 0.32;'
'tauEngine_s = 1.0;'
'Ts_s = 0.01;'
'D_m = 75 * 0.0254;'
'J_bkpts_CT = [0.0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2 1.3 1.4 1.5 1.6 1.7 1.8 1.9 2.0 2.1 2.2 2.3 5.0];'
'CT_data    = [0.073 0.073 0.072 0.071 0.069 0.066 0.062 0.055 0.045 0.034 0.024 0.013 -0.006 -0.013 -0.024 -0.034 -0.045 -0.055 -0.062 -0.066 -0.069 -0.071 -0.072 -0.073 -0.073];'
'J_bkpts_CP = [0.0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2 1.3 1.4 1.5 1.6 1.7 1.8 1.9 2.0 2.1 2.2 2.3 2.4 5.0];'
'CP_data    = [0.0660 0.0700 0.0700 0.0660 0.0600 0.0530 0.0501 0.0469 0.0426 0.0360 0.0282 0.0191 0.0155 0.0191 0.0282 0.0360 0.0426 0.0469 0.0501 0.0516 0.0525 0.0525 0.0522 0.0511 0.0504 0.0493];'
''
'if engineFailure_isTrue'
'    throttleCmd_nd = 0;'
'else'
'    throttleCmd_nd = max(min(cmdThrottle_unit, 1), 0);'
'end'
'targetRPM = idleRPM + (maxRPM - idleRPM) * throttleCmd_nd;'
'targetOmega_radps = targetRPM * 2 * pi / 60;'
''
'alpha = Ts_s / (tauEngine_s + Ts_s);'
'omega_radps_state = omega_radps_state + alpha * (targetOmega_radps - omega_radps_state);'
'motorAngVel_radps = omega_radps_state;'
''
'n_rps = motorAngVel_radps / (2 * pi);'
'V_mps = norm(airspeedInBody_mps);'
'if n_rps > 1e-3'
'    J_nd = V_mps / (n_rps * D_m);'
'else'
'    J_nd = 0;'
'end'
'J_nd = max(min(J_nd, 3.0), 0);'  % clamp to avoid unbounded extrapolation far outside the
                                    % tested table range
''
'Ct = interp1(J_bkpts_CT, CT_data, J_nd, ''linear'', ''extrap'');'
'Cp = interp1(J_bkpts_CP, CP_data, J_nd, ''linear'', ''extrap'');'
''
'thrust_N = Ct * airDensity_kgpm3 * n_rps^2 * D_m^4;'
'power_W = Cp * airDensity_kgpm3 * n_rps^3 * D_m^5;'
'power_W = min(power_W, maxPower_W);'
'if motorAngVel_radps > 1e-3'
'    torque_Nm = -power_W / motorAngVel_radps;'
'else'
'    torque_Nm = 0;'
'end'
''
'% bsfc-based fuel flow estimate (telemetry only -- see file header, this SIL never feeds'
'% fuelRate_kgps back into vehicle mass for ANY vehicle, not just this one).'
'power_hp = power_W / 745.7;'
'fuelRate_kgps = bsfc_lb_per_hp_hr * power_hp / 3600 * 0.453592;'
'end' ...
    };
    chart.Script = strjoin(lines, newline);
end

%% ====================================================================
%% CONNECT
%% ====================================================================
fcnName = get_param(fcn_blk, 'Name');

add_line(modelName, 'EngineCommandBus/1', 'Select_EngineCmd/1', 'autorouting', 'on');
add_line(modelName, 'failureBus/1', 'Select_Failure/1', 'autorouting', 'on');
add_line(modelName, 'AirDataBus/1', 'Select_AirData/1', 'autorouting', 'on');
add_line(modelName, 'EnvironmentBus/1', 'Select_Environment/1', 'autorouting', 'on');

add_line(modelName, 'Select_EngineCmd/1', [fcnName '/1'], 'autorouting', 'on');
add_line(modelName, 'Select_Failure/1', [fcnName '/2'], 'autorouting', 'on');
add_line(modelName, 'Select_AirData/1', [fcnName '/3'], 'autorouting', 'on');
add_line(modelName, 'Select_Environment/1', [fcnName '/4'], 'autorouting', 'on');

% pistonPropThrustModel outputs: thrust_N, torque_Nm, motorAngVel_radps, fuelRate_kgps
add_line(modelName, [fcnName '/1'], 'ForceVecMux/1', 'autorouting', 'on');
add_line(modelName, 'ZeroForceYZ/1', 'ForceVecMux/2', 'autorouting', 'on');
add_line(modelName, [fcnName '/2'], 'MomentVecMux/1', 'autorouting', 'on');
add_line(modelName, 'ZeroMomentYZ/1', 'MomentVecMux/2', 'autorouting', 'on');

add_line(modelName, 'ForceVecMux/1', 'EngineForcesMomentsBus_Creator/1', 'autorouting', 'on');
add_line(modelName, 'MomentVecMux/1', 'EngineForcesMomentsBus_Creator/2', 'autorouting', 'on');

add_line(modelName, [fcnName '/3'], 'PropulsionBus_Creator/1', 'autorouting', 'on');
add_line(modelName, 'EngineForcesMomentsBus_Creator/1', 'PropulsionBus_Creator/2', 'autorouting', 'on');
add_line(modelName, [fcnName '/4'], 'PropulsionBus_Creator/3', 'autorouting', 'on');

add_line(modelName, 'PropulsionBus_Creator/1', 'PropulsionBus/1', 'autorouting', 'on');

%% ====================================================================
%% CONFIG SET -- see the matching comment in build_enginePropFixedwing.m.
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
