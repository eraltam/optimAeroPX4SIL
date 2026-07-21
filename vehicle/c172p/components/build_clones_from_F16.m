%% build_clones_from_F16.m
% Documents (and can re-run) how vehicle/c172p/{c172p.slx,
% components/{actuatorsC172p,aeroC172p}.slx} were derived from vehicle/F16's own files -- same
% pattern as vehicle/fixedwing_plane/components/build_clones_from_F16.m (see that file's header
% for why this clone-and-repoint approach is used instead of building from scratch: everything
% except the aero/engine Model References is generic and workspace-parametrized, confirmed by
% inspection, not assumed).
%
% Run from this folder (vehicle/c172p/components), or adjust the paths below. Idempotent: safe to
% re-run after a fresh `git checkout` of vehicle/F16 if F16's files ever change and need
% re-syncing (though at that point re-diff before blindly overwriting the c172p-specific patches
% below).
%
% PREREQUISITE: build_C172pAeroCoefficientsModel.m and build_enginePistonPropC172p.m must already
% have been run (this script repoints Model References at their .slx outputs).

clear; clc;

thisDir = fileparts(mfilename('fullpath'));                  % .../c172p/components
vehicleDir = fileparts(thisDir);                              % .../vehicle/c172p
f16Dir = fullfile(fileparts(vehicleDir), 'F16');               % .../vehicle/F16

%% ====================================================================
%% 1. Filesystem copies (generic, per-vehicle-folder-duplicated components -- see
%%    PLAN_CORRECCION_MULTIVEHICULO_SITL.md F3 on why these are cloned per vehicle folder instead
%%    of referenced from a shared location)
%% ====================================================================
copyfile(fullfile(f16Dir, 'components', 'addFM.slx'), fullfile(thisDir, 'addFM.slx'), 'f');
copyfile(fullfile(f16Dir, 'components', 'groundContact.slx'), fullfile(thisDir, 'groundContact.slx'), 'f');
copyfile(fullfile(f16Dir, 'components', 'actuators.slx'), fullfile(thisDir, 'actuatorsC172p.slx'), 'f');
copyfile(fullfile(f16Dir, 'components', 'aero.slx'), fullfile(thisDir, 'aeroC172p.slx'), 'f');
copyfile(fullfile(f16Dir, 'F16.slx'), fullfile(vehicleDir, 'c172p.slx'), 'f');
% gravity.slx / cgMassInertia.slx / inertialDynamics.slx are NOT copied -- they live in
% vehicle/common/components/ (a genuinely shared location) and are already on the path.
fprintf('Filesystem copies done.\n');

%% ====================================================================
%% 2. actuatorsC172p.slx patches
%% ====================================================================
mdl = 'actuatorsC172p';
if bdIsLoaded(mdl); close_system(mdl, 0); end
load_system(fullfile(thisDir, [mdl '.slx']));

% a) Swap the engine Model Reference: F16's turbojet -> our real piston + fixed-pitch-prop model.
set_param([mdl '/Model'], 'ModelFile', 'enginePistonPropC172p.slx');

% b) Same F16-specific ServosCommandBus sub-bus-name patch fixedwing_plane needed (see that
%    project's build_clones_from_F16.m comment) -- repoint at our own
%    ServosCommandFixedwingPlaneBus (defined in signals/ServosCommandBusDefinition.m's
%    {"fixedwing_plane","c172p"} case -- c172p reuses that exact bus, see plan §3 Fase B).
set_param([mdl '/Constant'], 'OutDataTypeStr', 'Bus: ServosCommandFixedwingPlaneBus');
set_param([mdl '/Bus Selector'], 'OutputSignals', ...
    'controllerArmed,ServosCommandBus.ServosCommandFixedwingPlaneBus,EngineCommandBus');

save_system(mdl, fullfile(thisDir, [mdl '.slx']));
fprintf('%s patched and saved.\n', mdl);

%% ====================================================================
%% 3. aeroC172p.slx patch
%% ====================================================================
mdl2 = 'aeroC172p';
if bdIsLoaded(mdl2); close_system(mdl2, 0); end
load_system(fullfile(thisDir, [mdl2 '.slx']));

% Swap the aero coefficient Model Reference: F16's Stevens & Lewis NASA wind-tunnel tables -> our
% ported JSBSim c172p.xml coefficient buildup. Everything else in aero.slx (wind-axes calc,
% dynamic pressure, sumForcesMoments) is generic and reads SRef_m2/bRef_m/cRef_m/refCG_m/
% aircraftCg_m straight from vehicleParams -- no other block needs to change.
set_param([mdl2 '/aeroModel'], 'ModelFile', 'C172pAeroCoefficientsModel.slx');

save_system(mdl2, fullfile(thisDir, [mdl2 '.slx']));
fprintf('%s patched and saved.\n', mdl2);

%% ====================================================================
%% 4. c172p.slx (top-level plant) patch
%% ====================================================================
mdl3 = 'c172p';
if bdIsLoaded(mdl3); close_system(mdl3, 0); end
load_system(fullfile(vehicleDir, [mdl3 '.slx']));

set_param([mdl3 '/actuators'], 'ModelFile', 'actuatorsC172p.slx');
set_param([mdl3 '/aero'], 'ModelFile', 'aeroC172p.slx');
% gravity/addFM/cgMassInertia/dynamics/ground Model References are left pointing at their
% original filenames -- they resolve correctly because this vehicle's own folder (with its own
% addFM.slx/groundContact.slx copies from step 1) is on the MATLAB path when this vehicle is
% active, and vehicle/common/components is always on the path for gravity/cgMassInertia/
% inertialDynamics.

save_system(mdl3, fullfile(vehicleDir, [mdl3 '.slx']));
fprintf('%s patched and saved.\n', mdl3);

fprintf(['\nAll clones built. Next: run BusDefinition(''c172p''), setUpVehicle, ' ...
    'setUpActuators, then set_param(''c172p'',''SimulationCommand'',''update'') to smoke-test ' ...
    'the plant standalone.\n']);
