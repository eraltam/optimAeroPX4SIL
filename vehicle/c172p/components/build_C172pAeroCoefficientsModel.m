%% build_C172pAeroCoefficientsModel.m
% Programmatically creates vehicle/c172p/components/C172pAeroCoefficientsModel.slx -- a drop-in
% replacement for vehicle/F16/components/F16AeroCoefficientsModel.slx with the EXACT SAME
% interface contract (so it can be swapped into a clone of vehicle/F16/components/aero.slx's
% "aeroModel" Model Reference block, same pattern as
% vehicle/fixedwing_plane/components/build_FixedwingPlaneAeroCoefficientsModel.m):
%
%   Inputs:  alpha_deg, beta_deg, posElevator_deg, posAilerons_deg, posRudder_deg,
%            airspeedInBody_ftps (3x1), aircraftAngVelInBody_radps (3x1)
%   Outputs: CF (3x1: CX,CY,CZ body-axis force coefficients)
%            CM (3x1: Cl,Cm,Cn body-axis moment coefficients)
%
% Unlike FixedwingPlaneAeroCoefficientsModel.slx (a simple linear stability-derivative model with
% engineering-estimated coefficients), this model ports the REAL coefficient buildup from
% jsbsim/aircraft/c172p/c172p.xml (a published, community-validated JSBSim dataset) -- see
% vehicle/c172p/data/aeroDataC172p.m for the full derivation/sourcing of every constant below and
% PLAN_INCORPORACION_AERONAVES_JSBSIM_SITL.md §2 for the documented fidelity gaps (no ground
% effect, no flap channel, no alpha-rate terms, no stall hysteresis -- pre-stall curve only).
%
% Prerequisites: none beyond a plain MATLAB session (no bus objects needed -- this model has no
% Bus inputs/outputs, only plain doubles, same as FixedwingPlaneAeroCoefficientsModel.slx).
%
% Same two gotchas documented in build_FixedwingPlaneAeroCoefficientsModel.m apply here (repeated
% for anyone who only reads this file):
%   1. A MATLAB Function block's chart.Script MUST have its function signature on a SINGLE line.
%   2. Vector-valued Inports (airspeedInBody_ftps, aircraftAngVelInBody_radps, both 3-vectors)
%      need 'PortDimensions' set explicitly to compile standalone.

clear; clc;

rootDir = fileparts(mfilename('fullpath'));
modelName = 'C172pAeroCoefficientsModel';
savePath  = fullfile(rootDir, [modelName '.slx']);

if bdIsLoaded(modelName)
    close_system(modelName, 0);
end

new_system(modelName);
set_param(modelName, 'SolverType', 'Fixed-step');

%% ====================================================================
%% INPORTS (order matches F16AeroCoefficientsModel.slx / FixedwingPlaneAeroCoefficientsModel.slx)
%% ====================================================================
add_block('simulink/Sources/In1', [modelName '/alpha_deg'], ...
    'Position', [40, 40, 80, 60], 'Port', '1');
add_block('simulink/Sources/In1', [modelName '/beta_deg'], ...
    'Position', [40, 100, 80, 120], 'Port', '2');
add_block('simulink/Sources/In1', [modelName '/posElevator_deg'], ...
    'Position', [40, 160, 80, 180], 'Port', '3');
add_block('simulink/Sources/In1', [modelName '/posAilerons_deg'], ...
    'Position', [40, 220, 80, 240], 'Port', '4');
add_block('simulink/Sources/In1', [modelName '/posRudder_deg'], ...
    'Position', [40, 280, 80, 300], 'Port', '5');
add_block('simulink/Sources/In1', [modelName '/airspeedInBody_ftps'], ...
    'Position', [40, 340, 80, 360], 'Port', '6');
set_param([modelName '/airspeedInBody_ftps'], 'PortDimensions', '3');
add_block('simulink/Sources/In1', [modelName '/aircraftAngVelInBody_radps'], ...
    'Position', [40, 400, 80, 420], 'Port', '7');
set_param([modelName '/aircraftAngVelInBody_radps'], 'PortDimensions', '3');

%% ====================================================================
%% MATLAB FUNCTION BLOCK
%% ====================================================================
fcn_blk = [modelName '/jsbsimAeroCoefficients'];
add_block('simulink/User-Defined Functions/MATLAB Function', fcn_blk, ...
    'Position', [200, 40, 560, 420]);

%% ====================================================================
%% OUTPORTS
%% ====================================================================
add_block('simulink/Sinks/Out1', [modelName '/CF'], ...
    'Position', [620, 100, 660, 120], 'Port', '1');
add_block('simulink/Sinks/Out1', [modelName '/CM'], ...
    'Position', [620, 300, 660, 320], 'Port', '2');

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
'function [CF, CM] = jsbsimAeroCoefficients(alpha_deg, beta_deg, posElevator_deg, posAilerons_deg, posRudder_deg, airspeedInBody_ftps, aircraftAngVelInBody_radps)'
'%#codegen'
'% Cessna 172P aero coefficient buildup, ported from jsbsim/aircraft/c172p/c172p.xml.'
'% See vehicle/c172p/data/aeroDataC172p.m for full sourcing/derivation of every constant and'
'% PLAN_INCORPORACION_AERONAVES_JSBSIM_SITL.md for documented fidelity gaps (no ground effect,'
'% no flap channel, no alpha-rate terms, no stall hysteresis -- pre-stall curve only).'
'% All constants inlined as local values, not read from a struct-returning params function --'
'% see optimAeroPX4SIL/CLAUDE.md section 3 on why MATLAB Function blocks in this MATLAB release'
'% cannot reliably compile a call to a large-nested-struct-returning function.'
''
'% c172p.xml <metrics>, converted to SI'
'bRef_m = 10.9118;'
'cRef_m = 1.4935;'
''
'% DRAG axis'
'CD0 = 0.027;'
'CDde_coeff = 0.06;'
'CDbeta_coeff = 0.17;'
'CDwbh_alpha_bkpts_rad = [-0.0873 -0.0698 -0.0524 -0.0349 -0.0175 0.0000 0.0175 0.0349 0.0524 0.0698 0.0873 0.1047 0.1222 0.1396 0.1571 0.1745 0.1920 0.2094 0.2269 0.2443 0.2618 0.2793 0.2967 0.3142 0.3316 0.3491];'
'CDwbh_data = [0.0041 0.0013 0.0001 0.0003 0.0020 0.0052 0.0099 0.0162 0.0240 0.0334 0.0442 0.0566 0.0706 0.0860 0.0962 0.1069 0.1180 0.1298 0.1424 0.1565 0.1727 0.1782 0.1716 0.1618 0.1475 0.1097];'
''
'% SIDE axis'
'CYbeta_coeff = -0.1370 / 0.3490;'
'CYdr_coeff = 0.187;'
'CYp_alpha_bkpts_rad = [0.0000 0.0940];'
'CYp_data = [-0.0750 -0.1450];'
'CYr_alpha_bkpts_rad = [0.0000 0.0940];'
'CYr_data = [0.2140 0.2670];'
''
'% LIFT axis (CLwbh_data already includes the CL0 intercept -- no separate additive term)'
'CLwbh_alpha_bkpts_rad = [-0.0900 0.0000 0.0900 0.1000 0.1200 0.1400 0.1600 0.1700 0.1900 0.2100 0.2400 0.2600 0.2800 0.3000 0.3200 0.3400 0.3600];'
'CLwbh_data = [-0.2200 0.2500 0.7300 0.8300 0.9200 1.0200 1.0800 1.1300 1.1900 1.2500 1.3500 1.4400 1.4700 1.4300 1.3800 1.3000 1.1500];'
'CLde_coeff = 0.43;'
'CLq_coeff = 3.9;'
''
'% ROLL axis'
'ClBeta_coeff = -0.0322 / 0.3490;'
'Clp_coeff = -0.484;'
'Clr_alpha_bkpts_rad = [0.0000 0.0940];'
'Clr_data = [0.0798 0.1869];'
'ClDa_coeff = 0.229;'
'Cldr_coeff = 0.0147;'
''
'% PITCH axis'
'Cmo_coeff = 0.10;'
'Cmalpha_coeff = -1.8;'
'Cmq_coeff = -12.4;'
'Cmde_coeff = -1.122;'
''
'% YAW axis'
'Cnbeta_coeff = 0.0205 / 0.3490;'
'Cnp_coeff = -0.0278;'
'Cnr_coeff = -0.0937;'
'Cnda_coeff = -0.0053;'
'Cndr_coeff = -0.0430;'
''
'alpha_rad = deg2rad(alpha_deg);'
'beta_rad = deg2rad(beta_deg);'
'elevator_rad = deg2rad(posElevator_deg);'
'aileron_rad = deg2rad(posAilerons_deg);'
'rudder_rad = deg2rad(posRudder_deg);'
''
'V_mps = norm(airspeedInBody_ftps) * 0.3048;'
'VforRates_mps = max(V_mps, 1.0);'
'pRate_radps = aircraftAngVelInBody_radps(1);'
'qRate_radps = aircraftAngVelInBody_radps(2);'
'rRate_radps = aircraftAngVelInBody_radps(3);'
'bi2vel = bRef_m / (2 * VforRates_mps);'
'ci2vel = cRef_m / (2 * VforRates_mps);'
''
'CDwbh = interp1(CDwbh_alpha_bkpts_rad, CDwbh_data, alpha_rad, ''linear'', ''extrap'');'
'CLwbh = interp1(CLwbh_alpha_bkpts_rad, CLwbh_data, alpha_rad, ''linear'', ''extrap'');'
'CYp_v = interp1(CYp_alpha_bkpts_rad, CYp_data, abs(alpha_rad), ''linear'', ''extrap'');'
'CYr_v = interp1(CYr_alpha_bkpts_rad, CYr_data, abs(alpha_rad), ''linear'', ''extrap'');'
'Clr_v = interp1(Clr_alpha_bkpts_rad, Clr_data, abs(alpha_rad), ''linear'', ''extrap'');'
''
'CD = CD0 + CDwbh + CDde_coeff * abs(elevator_rad) + CDbeta_coeff * abs(beta_rad);'
'CY = CYbeta_coeff * beta_rad + CYdr_coeff * rudder_rad + CYp_v * bi2vel * pRate_radps + CYr_v * bi2vel * rRate_radps;'
'CL = CLwbh + CLde_coeff * elevator_rad + CLq_coeff * ci2vel * qRate_radps;'
''
'rollMomentCoefficient_nd = ClBeta_coeff * beta_rad + Clp_coeff * bi2vel * pRate_radps + Clr_v * bi2vel * rRate_radps + ClDa_coeff * aileron_rad + Cldr_coeff * rudder_rad;'
'pitchMomentCoefficient_nd = Cmo_coeff + Cmalpha_coeff * alpha_rad + Cmq_coeff * ci2vel * qRate_radps + Cmde_coeff * elevator_rad;'
'yawMomentCoefficient_nd = Cnbeta_coeff * beta_rad + Cnp_coeff * bi2vel * pRate_radps + Cnr_coeff * bi2vel * rRate_radps + Cnda_coeff * aileron_rad + Cndr_coeff * rudder_rad;'
''
'% Wind-axes (CL,CD) -> body-axes (CX,CZ); CY needs no rotation at this fidelity level.'
'CXTotal = -CD * cos(alpha_rad) + CL * sin(alpha_rad);'
'CZTotal = -CD * sin(alpha_rad) - CL * cos(alpha_rad);'
''
'CF = [CXTotal; CY; CZTotal];'
'CM = [rollMomentCoefficient_nd; pitchMomentCoefficient_nd; yawMomentCoefficient_nd];'
'end' ...
    };
    chart.Script = strjoin(lines, newline);
end

%% ====================================================================
%% CONNECT
%% ====================================================================
fcnName = get_param(fcn_blk, 'Name');
add_line(modelName, 'alpha_deg/1', [fcnName '/1'], 'autorouting', 'on');
add_line(modelName, 'beta_deg/1', [fcnName '/2'], 'autorouting', 'on');
add_line(modelName, 'posElevator_deg/1', [fcnName '/3'], 'autorouting', 'on');
add_line(modelName, 'posAilerons_deg/1', [fcnName '/4'], 'autorouting', 'on');
add_line(modelName, 'posRudder_deg/1', [fcnName '/5'], 'autorouting', 'on');
add_line(modelName, 'airspeedInBody_ftps/1', [fcnName '/6'], 'autorouting', 'on');
add_line(modelName, 'aircraftAngVelInBody_radps/1', [fcnName '/7'], 'autorouting', 'on');
add_line(modelName, [fcnName '/1'], 'CF/1', 'autorouting', 'on');
add_line(modelName, [fcnName '/2'], 'CM/1', 'autorouting', 'on');

%% ====================================================================
%% CONFIG SET -- share the same external ConfigSetRef every other component in this pipeline
%% uses, same pattern as build_FixedwingPlaneAeroCoefficientsModel.m.
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
