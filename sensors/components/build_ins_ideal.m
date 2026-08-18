%% build_ins_ideal.m
% Programmatically creates sensors/components/ins_ideal.slx -- INS_VARIANT==3,
% a third drop-in INS choice alongside Generic (ins.slx) and AnelloX3
% (ins_anello.slx): a genuine zero-error, pass-through IMU. This is the "D1"
% baseline from the HIL validation plan (see
% ../../HIL_TEST_STATUS_AND_NEXT_PRIORITIES.md, section 3/4/5 -- "mission
% with ideal IMU... never a raw pass-through of plant truth with zero sensor
% error"), used to separate "IMU noise-model effect" from "any sensor noise
% vs. none at all" in the existing pairwise (fase_c7_c8) comparison.
%
% Built by copying ins.slx (same aerolibnav "Three-axis Inertial Measurement
% Unit" block, same aircraftAccelbe_mps2/gravity/DCM_be wiring, so the
% sign/kinematics convention is guaranteed identical to Generic -- no need to
% re-derive the specific-force formula from scratch) and then:
%   - disabling second-order sensor dynamics for both accel and gyro (dtype_a/dtype_g -> off)
%   - disabling the block's internal noise generator (i_rand -> off)
%   - zeroing accel/gyro bias and g-sensitive bias (inlined as literal zeros,
%     not references to setUpSensors.m's `parameters` struct, so this variant
%     stays "ideal" even if the Generic model's bias parameters ever change)
%   - setting scale-factor/cross-coupling to identity
%   - widening saturation limits so they can never bind
%   - zeroing the external magnetometer noise gain (ins.slx's ad hoc `Gain`
%     block feeding the mag Add junction, upstream of the Band-Limited White
%     Noise source)
%
% Interface contract (must match ins.slx/ins_anello.slx exactly so it can be
% used as a Model Reference choice for the same Goto/From-routed signals in
% sensors.slx):
%   Inputs:  EnvironmentBus (Bus: EnvironmentBus), BodyStatesBus (Bus: BodyStateBus)
%   Outputs: INSSensorBus (Bus: INSSensorBus)
%
% Prerequisites (run from optimAeroPX4SIL root, or run initVehicleSIL first):
%   cd(optimAeroPX4SIL root)
%   addpath(genpath('sensors')); addpath(genpath('signals')); addpath(genpath('data'));
%   constants
%   BusDefinition('hexarotor')   % or whichever vehicle type -- defines the bus objects
%   sensors/setUpSensors          % defines stepSize_s-independent parameters (not required by
%                                  % this variant, but ins.slx's mask still references
%                                  % stepSize_s for i_Ts)

rootDir   = fileparts(mfilename('fullpath'));   % .../optimAeroPX4SIL/sensors/components
modelName = 'ins_ideal';
savePath  = fullfile(rootDir, [modelName '.slx']);

if bdIsLoaded(modelName)
    close_system(modelName, 0);
end
if bdIsLoaded('ins')
    close_system('ins', 0);
end

%% ====================================================================
%% COPY ins.slx -> ins_ideal (guarantees identical interface + kinematics)
%% ====================================================================
copyfile(fullfile(rootDir, 'ins.slx'), savePath, 'f');
load_system(savePath);
open_system(modelName);

%% ====================================================================
%% STRIP ALL SENSOR ERROR FROM THE IMU BLOCK
%% ====================================================================
subsystems = find_system(modelName, 'SearchDepth', 1, 'BlockType', 'SubSystem');
imuBlk = subsystems{~cellfun(@isempty, strfind(subsystems, 'Inertial'))}; %#ok<STRCL1>

set_param(imuBlk, ...
    'dtype_a', 'off', ...              % disable 2nd-order accel dynamics (no lag)
    'dtype_g', 'off', ...              % disable 2nd-order gyro dynamics (no lag)
    'i_rand',  'off', ...              % disable internal white-noise generator
    'a_bias',  '[0 0 0]', ...
    'g_bias',  '[0 0 0]', ...
    'g_sens',  '[0 0 0]', ...
    'a_sf_cc', 'eye(3)', ...
    'g_sf_cc', 'eye(3)', ...
    'a_sat',   '[-1e6 -1e6 -1e6 1e6 1e6 1e6]', ...
    'g_sat',   '[-1e6 -1e6 -1e6 1e6 1e6 1e6]');

%% ====================================================================
%% STRIP THE AD HOC MAGNETOMETER NOISE
%% ====================================================================
set_param([modelName '/Gain'], 'Gain', '0');

%% ---- Save ----------------------------------------------------------
Simulink.BlockDiagram.arrangeSystem(modelName);
save_system(modelName, savePath);
fprintf('\nModel saved: %s\n', savePath);
