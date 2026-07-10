# Simscape Adapter Handoff

Date: 2026-07-10

This file summarizes the Simscape/PX4 integration work completed in this change set and what is
still missing. It is intended as the first document another AI agent or engineer should read before
continuing.

## Current State

The project now has separate Simscape adapter vehicle entries for:

| Vehicle key | Adapter model | PX4 target | Baseline cloned from |
|---|---|---|---|
| `evtol_simscape` | `vehicle/evtol_simscape/evtol_simscape.slx` | `optimAeroEVTOL` | `vehicle/evtol/evtol.slx` |
| `tracked_vehicle_simscape` | `vehicle/tracked_vehicle_simscape/tracked_vehicle_simscape.slx` | `optimAeroTracked` | `vehicle/tracked_vehicle/tracked_vehicle.slx` |
| `wheel_loader_simscape` | `vehicle/wheel_loader_simscape/wheel_loader_simscape.slx` | `optimAeroLoader` | `vehicle/wheel_loader/wheel_loader.slx` |
| `ackermann_simscape` | `vehicle/ackermann_simscape/ackermann_simscape.slx` | `optimAeroAckermann` | `vehicle/ackermann_rover/ackermann_rover.slx` |

Each adapter model is selected by `VehiclePlant.slx` through a new variant choice under
`VehiclePlant/plant`.

Each adapter model contains a contained subsystem named:

```text
simscape_adapter_diagnostics
```

That subsystem contains a small Simscape network:

- Solver Configuration
- Electrical Reference
- Simulink-PS Converter
- Controlled Voltage Source
- Resistor load
- Voltage Sensor
- PS-Simulink Converter
- Terminator

This is not yet the final physical vehicle plant. It proves that each adapter model reference can
host Simscape blocks and compile inside the existing SIL plant contract.

## Main Files Changed

### Simulink Models

```text
VehiclePlant.slx
vehicle/evtol_simscape/evtol_simscape.slx
vehicle/tracked_vehicle_simscape/tracked_vehicle_simscape.slx
vehicle/wheel_loader_simscape/wheel_loader_simscape.slx
vehicle/ackermann_simscape/ackermann_simscape.slx
```

`VehiclePlant.slx` now has four new model-reference variant choices:

```text
evtol_simscape
tracked_vehicle_simscape
wheel_loader_simscape
ackermann_simscape
```

Their variant controls are keyed by:

```matlab
strcmpi(vehicleParams.type, "<vehicle_key>")
```

### MATLAB Glue

```text
vehicle/vehicleRegistry.m
initVehicleSIL.m
```

Important behavior:

- `vehicleRegistry.m` maps the `*_simscape` vehicle keys to the new adapter model names through
  `PlantModel` and `CompileModel`.
- `initVehicleSIL.m` sets `vehicleParams.type` from `vehicleDefinition.PlantModel`, so the adapter
  key activates the matching `VehiclePlant.slx` variant.
- `initVehicleSIL.m` has `vehicleBasePath` cases for the new adapter folders.

### Adapter Metadata And Contracts

Each adapter folder contains:

```text
README.md
data/parameters_<vehicle_key>.m
data/setUpVehicle_<vehicle_key>.m
adapters/*AdapterContract.m
testing/*PlantSmokeTest.m
testing/*WaypointSmokeTest.m
components/
```

The `components/` folders were copied from the corresponding baseline vehicle folder so the cloned
adapter model references resolve locally.

## Validation Completed

All four adapter variants passed the matrix checks with:

- `INS_VARIANT = 1`
- `INS_VARIANT = 2`
- PX4 target registration
- PX4 airframe registration
- `VehiclePlant` one-second smoke simulation

Evidence files:

```text
work/vehicle_test_results/evtol_simscape_matrix_20260710_114734.json
work/vehicle_test_results/tracked_vehicle_simscape_matrix_20260710_115335.json
work/vehicle_test_results/wheel_loader_simscape_matrix_20260710_120001.json
work/vehicle_test_results/ackermann_simscape_matrix_20260710_120529.json
```

PX4 target startup checks were also run with bounded `timeout 75s` launches. Each target started
PX4 and reached the expected wait-for-simulator state on TCP port `4560`.

Evidence files:

```text
work/px4_sitl_logs/optimAeroEVTOL_20260710_122658.log
work/px4_sitl_logs/optimAeroTracked_20260710_122821.log
work/px4_sitl_logs/optimAeroLoader_20260710_122938.log
work/px4_sitl_logs/optimAeroAckermann_20260710_123055.log
```

Expected PX4 launch behavior in these bounded logs:

```text
[0/1] launching px4 <target> (SYS_AUTOSTART=<id>)
px4 starting.
INFO  [px4] startup script: /bin/sh etc/init.d-posix/rcS 0
INFO  [simulator_mavlink] Waiting for simulator to accept connection on TCP port 4560
```

Exit code `124` from those checks means the external `timeout` stopped PX4 after startup. It should
not be interpreted as a PX4 startup failure by itself.

## Known Warnings

The matrix runs emit many existing warnings from the ANELLO INS path, mostly bus element-name
mismatches such as:

```text
ins_anello/AccelSensorBus_Creator
ins_anello/GyroSensorBus_Creator
ins_anello/MagSensorBus_Creator
ins_anello/INSSensorBus_Creator
```

These warnings were present during passing runs and did not block the adapter validation.

There are also pressure extrapolation warnings from the sensor/ADC path during short plant smoke
simulations. These did not block the passing matrix runs.

## What Is Still Missing

The adapter models currently contain diagnostic Simscape networks, not command-coupled physical
vehicle dynamics.

The next real implementation step is to replace the diagnostic subsystem in each adapter model with
the minimum useful physical plant subsystem while preserving the existing SIL bus contract:

```text
ActuatorCommandBus + EnvironmentBus + FailureBus
  -> adapter plant internals
  -> VehicleBus + ActuatorBus
```

Recommended first physical subsystems:

| Vehicle key | First physical subsystem to add |
|---|---|
| `evtol_simscape` | Motor/propeller/powertrain response from normalized motor commands |
| `tracked_vehicle_simscape` | Reduced left/right track force or torque model |
| `wheel_loader_simscape` | Steering plus wheel torque on flat terrain; defer hydraulics/bucket |
| `ackermann_simscape` | Steering angle plus drive torque at low speed |

Do not start by importing entire external Simscape example repositories into `VehiclePlant.slx`.
Keep the wrapper small and make it pass update/smoke tests first.

## Live Loop Closure Status

Fully automated headless live loop closure was attempted but did not complete.

Attempted approach:

1. Start PX4 target from WSL in the background.
2. Run MATLAB batch:

   ```matlab
   initVehicleSIL("launchFullSIL", false, ...
       "vehicleType", "evtol_simscape", ...
       "controllerRuntime", "SITL", ...
       "visualizationType", "Matlab", ...
       "launchPreflightVisualization", false)
   sim("VehicleSilSimulation", "StopTime", "1")
   ```

Observed result:

- The simulation did not return before the shell timeout.
- The batch MATLAB process had to be stopped.
- No persistent MATLAB/PX4 batch process was left running afterward.

Logs from the attempted live loop are under:

```text
work/live_loop_logs/
```

The next agent should treat live loop closure as still open. The static/update/smoke layer is done;
the remaining proof is simultaneous PX4 plus Simulink connection, finite PX4 sensor topics, actuator
feedback into Simulink, and actual movement response.

## Recommended Continuation Order

1. Open MATLAB interactively, not headless batch, for live loop closure.
2. Start with `evtol_simscape` only.
3. Run:

   ```matlab
   cd("C:\AnelloSummer\optimAeroPX4SIL")
   initVehicleSIL("launchFullSIL", false, ...
       "vehicleType", "evtol_simscape", ...
       "controllerRuntime", "SITL", ...
       "visualizationType", "Matlab", ...
       "launchPreflightVisualization", false)
   ```

4. Start `VehicleSilSimulation` from Simulink.
5. In WSL:

   ```bash
   cd /mnt/c/AnelloSummer/optimAeroPX4SIL/PX4-Autopilot
   make px4_sitl_default optimAeroEVTOL
   ```

6. Confirm PX4 connects instead of only waiting on TCP `4560`.
7. Check PX4 topics:

   ```text
   listener sensor_accel
   listener sensor_gyro
   listener sensor_mag
   listener vehicle_attitude
   listener vehicle_local_position
   listener actuator_outputs
   ```

8. Only after eVTOL loop closure works, repeat for:

   ```text
   tracked_vehicle_simscape
   wheel_loader_simscape
   ackermann_simscape
   ```

9. After live loop closure, replace the diagnostic Simscape networks with command-coupled physical
   subsystem slices one vehicle at a time.

## Useful Commands

Run one adapter matrix:

```matlab
cd("C:\AnelloSummer\optimAeroPX4SIL")
addpath("utilities")
runVehicleIntegrationMatrix( ...
    "vehicleTypes", string("evtol_simscape"), ...
    "insVariants", [1 2], ...
    "runPlantSmoke", true, ...
    "runPx4Build", false)
```

Run all four adapter matrices individually rather than as one long batch, because first-time model
reference compilation can take several minutes per vehicle:

```matlab
vehicleTypes = [
    "evtol_simscape"
    "tracked_vehicle_simscape"
    "wheel_loader_simscape"
    "ackermann_simscape"
];

for ii = 1:numel(vehicleTypes)
    runVehicleIntegrationMatrix( ...
        "vehicleTypes", vehicleTypes(ii), ...
        "insVariants", [1 2], ...
        "runPlantSmoke", true, ...
        "runPx4Build", false);
end
```

Bounded PX4 startup check:

```powershell
wsl bash -lc "cd '/mnt/c/AnelloSummer/optimAeroPX4SIL/PX4-Autopilot' && timeout 75s make px4_sitl_default optimAeroEVTOL"
```

## Related Docs

Read these next:

```text
SIMSCAPE_VEHICLE_INTEGRATION_PLAN.md
VEHICLE_INTEGRATION_TEST_RESULTS.md
vehicle/evtol_simscape/README.md
vehicle/tracked_vehicle_simscape/README.md
vehicle/wheel_loader_simscape/README.md
vehicle/ackermann_simscape/README.md
```

## Current Worktree Changes To Commit

This section records the current `git status --short` state at the time this handoff was written.
The worktree already contained unrelated/generated changes before some of this work started, so do
not assume every listed file was intentionally authored in the Simscape adapter change. Review before
committing.

### Primary Simscape Adapter Changes

These are the core files and folders from this handoff:

```text
 M VehiclePlant.slx
 M initVehicleSIL.m
?? SIMSCAPE_ADAPTER_HANDOFF.md
?? SIMSCAPE_VEHICLE_INTEGRATION_PLAN.md
?? VEHICLE_INTEGRATION_TEST_RESULTS.md
?? vehicle/ackermann_simscape/
?? vehicle/evtol_simscape/
?? vehicle/tracked_vehicle_simscape/
?? vehicle/vehicleRegistry.m
?? vehicle/wheel_loader_simscape/
```

Notes:

- `VehiclePlant.slx` contains the new `*_simscape` variant model references.
- `initVehicleSIL.m` contains the adapter path/type resolution changes.
- `vehicle/vehicleRegistry.m` contains the registry entries for all baseline and adapter vehicles.
- The four `vehicle/*_simscape/` folders contain adapter models, copied local component dependencies,
  metadata, contract files, tests, and READMEs.

### Prior Or Related Vehicle Additions Present In Worktree

These vehicle folders are also untracked in the current worktree. They appear related to the broader
multi-vehicle integration effort, but they were not all created in the final Simscape adapter step:

```text
?? vehicle/ackermann_rover/
?? vehicle/bicycle_rover/
?? vehicle/differential_rover/
?? vehicle/evtol/
?? vehicle/octarotor/
?? vehicle/quadrotor/
?? vehicle/tracked_vehicle/
?? vehicle/unicycle_rover/
?? vehicle/usv_surface/
?? vehicle/uuv_subsea/
?? vehicle/wheel_loader/
```

### Test Utilities And Backups Present In Worktree

```text
?? VehiclePlant.slx.backup_20260709_190338
?? VehiclePlant.slx.r2024a
?? VehicleSilSimulation.slx.backup_20260709_190338
?? utilities/makeVehicleSmokeMission.m
?? utilities/runVehicleIntegrationMatrix.m
?? utilities/runVehicleWaypointSmokeTest.m
```

The matrix utility is required to reproduce the validation evidence in this handoff:

```text
utilities/runVehicleIntegrationMatrix.m
```

### Modified Existing Files Not Primarily Part Of The Adapter Models

These are modified in the current worktree and should be reviewed before committing:

```text
 m PX4-Autopilot
 M VehicleSilSimulation.slx
 M VehicleSilSimulation.slx.original
 M testVehicleSIL.m
 M utilities/getWSLIP.m
 M vehicle/common/setUpActuators.m
 M vehicle/common/setUpVehicle.m
 D testResultsF16.pdf
 D testResultshexarotor.pdf
```

`PX4-Autopilot` is a submodule-style entry shown as modified by git. Inspect inside that folder
before committing parent-repo metadata.

### Generated Simulink Build And Cache Artifacts

The current worktree also contains many modified generated artifacts under `work/`. These were
created or updated by Simulink model-reference builds, smoke simulations, and matrix validation.

Representative modified generated files include:

```text
 M work/VehiclePlant.slxc
 M work/VehicleSilSimulation.slxc
 M work/PX4OutputMappingHex.slxc
 M work/PX4OutputMappingHex_msf.mexw64
 M work/cgMassInertia.slxc
 M work/cgMassInertia_msf.mexw64
 M work/environment.slxc
 M work/failureInputReadHex.slxc
 M work/failureInputReadHex_msf.mexw64
 M work/gravity.slxc
 M work/gravity_msf.mexw64
 M work/hexActuator.slxc
 M work/hexActuator_msf.mexw64
 M work/hexAddFm.slxc
 M work/hexAddFm_msf.mexw64
 M work/hexAero.slxc
 M work/hexAero_msf.mexw64
 M work/hexAeroCoefficientsModel.slxc
 M work/hexAeroCoefficientsModel_msf.mexw64
 M work/hexGroundContact.slxc
 M work/hexGroundContact_msf.mexw64
 M work/hexMotorModel.slxc
 M work/hexMotorModel_msf.mexw64
 M work/hexarotor.slxc
 M work/inertialDynamics.slxc
 M work/inertialDynamics_msf.mexw64
 M work/sensors.slxc
```

There are also many generated files under:

```text
work/slprj/_cgxe/
work/slprj/sim/PX4OutputMappingHex/
work/slprj/sim/_sharedutils/
work/slprj/sim/cgMassInertia/
work/slprj/sim/failureInputReadHex/
work/slprj/sim/gravity/
work/slprj/sim/hexActuator/
work/slprj/sim/hexAddFm/
work/slprj/sim/hexAero/
work/slprj/sim/hexAeroCoefficientsModel/
work/slprj/sim/hexGroundContact/
work/slprj/sim/hexMotorModel/
work/slprj/sim/inertialDynamics/
work/slprj/sim/varcache/
```

These generated artifacts are usually not the right files to commit unless this repository
intentionally tracks Simulink caches/build outputs. Confirm the repo policy before including them.

### Validation Logs Produced By This Work

The following evidence files are referenced by the docs but may not appear in `git status` if ignored:

```text
work/vehicle_test_results/evtol_simscape_matrix_20260710_114734.json
work/vehicle_test_results/tracked_vehicle_simscape_matrix_20260710_115335.json
work/vehicle_test_results/wheel_loader_simscape_matrix_20260710_120001.json
work/vehicle_test_results/ackermann_simscape_matrix_20260710_120529.json
work/px4_sitl_logs/optimAeroEVTOL_20260710_122658.log
work/px4_sitl_logs/optimAeroTracked_20260710_122821.log
work/px4_sitl_logs/optimAeroLoader_20260710_122938.log
work/px4_sitl_logs/optimAeroAckermann_20260710_123055.log
work/live_loop_logs/
```

### Suggested Commit Inclusion

Likely include:

```text
VehiclePlant.slx
initVehicleSIL.m
SIMSCAPE_ADAPTER_HANDOFF.md
SIMSCAPE_VEHICLE_INTEGRATION_PLAN.md
VEHICLE_INTEGRATION_TEST_RESULTS.md
utilities/runVehicleIntegrationMatrix.m
vehicle/vehicleRegistry.m
vehicle/evtol_simscape/
vehicle/tracked_vehicle_simscape/
vehicle/wheel_loader_simscape/
vehicle/ackermann_simscape/
```

Review separately before including:

```text
VehicleSilSimulation.slx
VehicleSilSimulation.slx.original
testVehicleSIL.m
utilities/getWSLIP.m
vehicle/common/setUpActuators.m
vehicle/common/setUpVehicle.m
vehicle/<baseline vehicle folders>/
PX4-Autopilot
```

Likely exclude unless the repo intentionally tracks generated outputs:

```text
work/*.slxc
work/*.mexw64
work/slprj/
```
