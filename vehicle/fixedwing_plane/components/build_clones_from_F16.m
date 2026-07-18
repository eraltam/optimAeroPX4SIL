%% build_clones_from_F16.m
% Documents (and can re-run) how vehicle/fixedwing_plane/{fixedwing_plane.slx,
% components/{actuatorsFixedwingPlane,aeroFixedwingPlane}.slx} were derived
% from vehicle/F16's own files. Unlike the two build_*.m scripts in this same
% folder (which build brand-new models from scratch), these three files are
% near-verbatim FILE COPIES of F16's actuators.slx / aero.slx / F16.slx, each
% with only the Model Reference block(s) that actually differ between a
% turbojet fighter and a light electric ala-fija swapped -- everything else
% (gravity/cgMassInertia/inertialDynamics 6DOF integration, addFM force
% summation, groundContact, servo.slx actuator dynamics, wind-axes/dynamic-
% pressure aero math) is generic and workspace-parametrized, confirmed by
% inspecting each file's block structure with model_read before cloning --
% see PLAN_VEHICULOS_AEREOS_Y_MARINOS_SITL.md Fase A-D writeup.
%
% Run from this folder (vehicle/fixedwing_plane/components), or adjust the
% paths below. Idempotent: safe to re-run after a fresh `git checkout` of
% vehicle/F16 if F16's files ever change and need re-syncing (though at that
% point re-diff before blindly overwriting the fixedwing_plane-specific
% patches below).

clear; clc;

thisDir = fileparts(mfilename('fullpath'));                  % .../fixedwing_plane/components
vehicleDir = fileparts(thisDir);                              % .../vehicle/fixedwing_plane
f16Dir = fullfile(fileparts(vehicleDir), 'F16');               % .../vehicle/F16

%% ====================================================================
%% 1. Filesystem copies (generic, per-vehicle-folder-duplicated components --
%%    see PLAN_CORRECCION_MULTIVEHICULO_SITL.md F3 on why these are cloned
%%    per vehicle folder instead of referenced from a shared location)
%% ====================================================================
copyfile(fullfile(f16Dir, 'components', 'addFM.slx'), fullfile(thisDir, 'addFM.slx'), 'f');
copyfile(fullfile(f16Dir, 'components', 'groundContact.slx'), fullfile(thisDir, 'groundContact.slx'), 'f');
copyfile(fullfile(f16Dir, 'components', 'actuators.slx'), fullfile(thisDir, 'actuatorsFixedwingPlane.slx'), 'f');
copyfile(fullfile(f16Dir, 'components', 'aero.slx'), fullfile(thisDir, 'aeroFixedwingPlane.slx'), 'f');
copyfile(fullfile(f16Dir, 'F16.slx'), fullfile(vehicleDir, 'fixedwing_plane.slx'), 'f');
% gravity.slx / cgMassInertia.slx / inertialDynamics.slx are NOT copied --
% they live in vehicle/common/components/ (a genuinely shared location, not
% per-vehicle-folder duplicated) and are already on the path via vehicle/common.
fprintf('Filesystem copies done.\n');

%% ====================================================================
%% 2. actuatorsFixedwingPlane.slx patches
%% ====================================================================
mdl = 'actuatorsFixedwingPlane';
if bdIsLoaded(mdl); close_system(mdl, 0); end
load_system(fullfile(thisDir, [mdl '.slx']));

% a) Swap the engine Model Reference: F16's turbojet -> our electric motor +
%    tractor propeller (same PropulsionBus-producing interface contract).
set_param([mdl '/Model'], 'ModelFile', 'enginePropFixedwing.slx');

% b) The F16-specific ServosCommandBus sub-bus name is hardcoded (not read
%    generically) in two places inside actuators.slx -- a Constant block's
%    OutDataTypeStr (used for the "controller not armed" failsafe pass-
%    through) and a Bus Selector's OutputSignals path string. Both must be
%    repointed at our own ServosCommandFixedwingPlaneBus (defined in
%    signals/ServosCommandBusDefinition.m's "fixedwing_plane" case) --
%    otherwise compilation fails with "Unable to find selected signal
%    'ServosCommandBus.ServosCommandF16Bus' in the input bus".
set_param([mdl '/Constant'], 'OutDataTypeStr', 'Bus: ServosCommandFixedwingPlaneBus');
set_param([mdl '/Bus Selector'], 'OutputSignals', ...
    'controllerArmed,ServosCommandBus.ServosCommandFixedwingPlaneBus,EngineCommandBus');

save_system(mdl, fullfile(thisDir, [mdl '.slx']));
fprintf('%s patched and saved.\n', mdl);

%% ====================================================================
%% 3. aeroFixedwingPlane.slx patch
%% ====================================================================
mdl2 = 'aeroFixedwingPlane';
if bdIsLoaded(mdl2); close_system(mdl2, 0); end
load_system(fullfile(thisDir, [mdl2 '.slx']));

% Swap the aero coefficient Model Reference: F16's Stevens & Lewis NASA
% wind-tunnel tables -> our linear stability-derivative model. Everything
% else in aero.slx (wind-axes calc, dynamic pressure, sumForcesMoments) is
% generic and reads SRef_m2/bRef_m/cRef_m/refCG_m/aircraftCg_m straight from
% vehicleParams -- no other block needs to change.
set_param([mdl2 '/aeroModel'], 'ModelFile', 'FixedwingPlaneAeroCoefficientsModel.slx');

save_system(mdl2, fullfile(thisDir, [mdl2 '.slx']));
fprintf('%s patched and saved.\n', mdl2);

%% ====================================================================
%% 4. fixedwing_plane.slx (top-level plant) patch
%% ====================================================================
% Loading a copied .slx file by its new FILENAME names the loaded model after
% the file (not whatever name it was internally saved under before copying),
% so no explicit rename step is needed here.
mdl3 = 'fixedwing_plane';
if bdIsLoaded(mdl3); close_system(mdl3, 0); end
load_system(fullfile(vehicleDir, [mdl3 '.slx']));

set_param([mdl3 '/actuators'], 'ModelFile', 'actuatorsFixedwingPlane.slx');
set_param([mdl3 '/aero'], 'ModelFile', 'aeroFixedwingPlane.slx');
% gravity/addFM/cgMassInertia/dynamics/ground Model References are left
% pointing at their original filenames (gravity.slx, addFM.slx,
% cgMassInertia.slx, inertialDynamics.slx, groundContact.slx) -- they resolve
% correctly because this vehicle's own folder (with its own addFM.slx/
% groundContact.slx copies from step 1) is what's on the MATLAB path when
% this vehicle is active, and vehicle/common/components is always on the
% path for gravity/cgMassInertia/inertialDynamics.

save_system(mdl3, fullfile(vehicleDir, [mdl3 '.slx']));
fprintf('%s patched and saved.\n', mdl3);

fprintf(['\nAll clones built. Next: run BusDefinition(''fixedwing_plane''), ' ...
    'setUpVehicle, setUpActuators, then set_param(''fixedwing_plane'',' ...
    '''SimulationCommand'',''update'') to smoke-test the plant standalone.\n']);
