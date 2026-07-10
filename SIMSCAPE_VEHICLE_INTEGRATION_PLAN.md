# Simscape Vehicle Integration Plan

Date: 2026-07-10

## Goal

Use the vehicle examples in `C:\AnelloSummer\new_vehicles` as source material for higher-fidelity
plants inside the existing `VehicleSilSimulation.slx` + PX4 SITL workflow.

The target is not to run those external repositories separately. The target is to adapt useful
Simscape, Simulink, and MSS dynamics into this project so every vehicle still runs through:

```text
VehicleSilSimulation.slx
  -> PX4 SITL connector
  -> PX4 output mapping
  -> VehiclePlant.slx
  -> SensorsBus / VehicleBus
  -> PX4 estimator and controller
```

## Available Source Repositories

Local source folder:

```text
C:\AnelloSummer\new_vehicles
```

Useful sources found there:

| Source | Main use |
|---|---|
| `eVTOL-Drone-Simscape` | eVTOL airframe, battery, motor, propeller, powertrain ideas |
| `Formula-Student-Vehicle-Simscape` | Ackermann/ground vehicle parameters, driveline and tire ideas |
| `Tracked-Vehicles-Simscape` | tracked vehicle dynamics, track/contact examples, terrain interaction |
| `Wheel-Loader-Simscape` | wheel loader plant, driveline, tires, hydraulic/mechanical subsystems |
| `Optimize-Vehicle-Design-with-AI-and-Simscape` | optimization-oriented vehicle components and simulation workflows |
| `MSS` | marine USV/UUV equations, vessel models, navigation/dynamics references |

## Core Design Rule

Do not directly copy whole external repositories into `VehicleSilSimulation.slx`.

Instead, create a project-local plant wrapper for each vehicle. The wrapper must expose the same
external contract as the current plant:

```text
ActuatorCommandBus + EnvironmentBus + FailureBus
  -> vehicle plant internals
  -> VehicleBus
```

The internals may be Simscape, Simscape Multibody, Simscape Driveline, plain Simulink equations, or
MSS equations. The boundary must stay stable so PX4, sensors, logging, and tests do not need a new
architecture for every vehicle.

## Required Adapter Layer

Each Simscape vehicle needs an adapter layer between PX4 and the physical model.

### Inputs

Convert PX4 output mapping into plant inputs:

| Vehicle class | PX4/control output | Simscape plant input |
|---|---|---|
| Multirotor/eVTOL | motor normalized outputs | motor speed, torque, or thrust commands |
| Fixed wing | actuator surfaces + throttle | elevator, aileron, rudder, throttle/engine force |
| Ackermann rover | steering + throttle | steering angle, wheel torque, brake |
| Differential/tracked rover | left/right command or velocity | left/right track or wheel torque |
| Wheel loader | throttle, steering, hydraulic commands | driveline torque, steering articulation, bucket controls |
| USV | thrust/yaw commands | propeller/thruster force and moment |
| UUV | thruster commands | 6DOF force/moment input |

### Outputs

Convert plant states into this repo's `VehicleBus` fields:

- Position in NED, meters.
- Velocity in body frame, meters per second.
- Velocity in NED, meters per second.
- Euler attitude, radians.
- Body angular velocity, radians per second.
- Body acceleration, meters per second squared.
- Any actuator state needed by visualization or sensors.

### Frame Conventions

This is critical.

PX4 and the current SIL path expect NED/FRD-style conventions. Many Simscape and robotics examples
use world/ENU or CAD/body frames. Every wrapper must explicitly document and test:

- World frame to NED conversion.
- Body frame to FRD conversion.
- Gravity sign.
- Yaw sign.
- Angular-rate axis order.
- Units for position, speed, acceleration, magnetic field, and actuator commands.

## Vehicle Integration Priority

### 1. eVTOL

Source:

```text
C:\AnelloSummer\new_vehicles\eVTOL-Drone-Simscape
```

Recommended first Simscape target because it is closest to the existing multirotor architecture.

Plan:

1. Keep the current `evtol` registry entry as the fast baseline.
2. Add an `evtol_simscape` plant variant.
3. Start by adapting motor/propeller/powertrain pieces, not the entire full example.
4. Use the existing multirotor PX4 output mapping first.
5. Validate hover, actuator response, and gravity sign before trying waypoint flight.

### 2. Wheel Loader

Source:

```text
C:\AnelloSummer\new_vehicles\Wheel-Loader-Simscape
```

High value but likely heavier because of driveline, tires, terrain, and possible hydraulics.

Plan:

1. Start with simplified driveline + chassis.
2. Defer bucket/hydraulic control until vehicle movement works.
3. Use a flat terrain model first.
4. Map PX4 rover throttle/steering to wheel torque and articulation/steering.
5. Add terrain/contact complexity only after fixed-step compile and basic SITL loop closure pass.

### 3. Tracked Vehicle

Source:

```text
C:\AnelloSummer\new_vehicles\Tracked-Vehicles-Simscape
```

Useful for tracked robot dynamics, but contact and track models can be expensive.

Plan:

1. Start with a reduced track model or equivalent left/right track force model.
2. Use the full track/contact Simscape model only after the simplified model is connected.
3. Map PX4 rover outputs into left/right track commands.
4. Verify turning in place, straight-line motion, and terrain contact stability.

### 4. Ackermann / Formula Student

Sources:

```text
C:\AnelloSummer\new_vehicles\Formula-Student-Vehicle-Simscape
C:\AnelloSummer\new_vehicles\Optimize-Vehicle-Design-with-AI-and-Simscape
```

Good source for ground vehicle parameters, tires, and driveline ideas.

Plan:

1. Keep the current `ackermann_rover` as the fast kinematic baseline.
2. Add an `ackermann_simscape` variant later.
3. Map PX4 rover steering/throttle to steering angle and drive torque.
4. Validate low-speed waypoint following before adding high-speed tire dynamics.

### 5. USV / UUV

Source:

```text
C:\AnelloSummer\new_vehicles\MSS
```

For marine vehicles, MSS equations may be a better first step than Simscape.

Plan:

1. Implement an MSS-derived 3DOF USV plant first.
2. Implement an MSS-derived 6DOF UUV plant second.
3. Wrap the equations behind the same `VehicleBus` contract.
4. Map PX4 marine/rover-style commands to surge/yaw or thruster force.
5. Add Simscape only if physical actuator or hydrodynamic subsystem fidelity is needed.

## Implementation Steps

### Step 1. Keep Baselines Working

Before adding Simscape internals, run:

```matlab
cd("C:\AnelloSummer\optimAeroPX4SIL")
addpath("utilities")
runVehicleIntegrationMatrix("insVariants", [1 2])
```

Expected:

- All registered vehicles compile.
- `INS_VARIANT = 1` passes.
- `INS_VARIANT = 2` passes.
- PX4 targets and airframes are registered.

### Step 2. Add a New Vehicle Registry Entry

Do not replace the fast baseline entry immediately. Add a separate entry, for example:

```text
evtol_simscape
wheel_loader_simscape
tracked_vehicle_simscape
ackermann_simscape
usv_mss
uuv_mss
```

This keeps a working fallback while the high-fidelity plant is still being debugged.

### Step 3. Create the Plant Wrapper

For each vehicle, create a local project folder:

```text
vehicle/<vehicle_key>/
  README.md
  data/
  testing/
  adapters/
  models/
```

The wrapper model should accept the current plant inputs and output `VehicleBus`.

Suggested naming:

```text
vehicle/<vehicle_key>/models/<vehicle_key>Plant.slx
vehicle/<vehicle_key>/adapters/<vehicle_key>CommandAdapter.slx
vehicle/<vehicle_key>/adapters/<vehicle_key>StateAdapter.slx
```

### Step 4. Add a `VehiclePlant.slx` Variant

Add the new vehicle wrapper as a plant variant. The variant must:

- Compile with fixed-step settings.
- Use the existing bus definitions or a compatible extension.
- Avoid changing PX4 connector or sensor architecture.
- Emit finite states at `t = 0`.

### Step 5. Connect PX4 Target/Airframe

Each vehicle needs a PX4 SITL target:

```text
make px4_sitl_default <PX4Target>
```

The target must be registered in:

```text
PX4-Autopilot/src/modules/simulation/simulator_mavlink/CMakeLists.txt
PX4-Autopilot/ROMFS/px4fmu_common/init.d-posix/airframes/CMakeLists.txt
PX4-Autopilot/ROMFS/px4fmu_common/init.d-posix/airframes/<id>_<PX4Target>
```

### Step 6. Run Compile Tests

For the new entry:

```matlab
runVehicleIntegrationMatrix( ...
    "vehicleTypes", string("<vehicle_key>"), ...
    "insVariants", [1 2], ...
    "runPlantSmoke", true)
```

Pass criteria:

- `initVehicleSIL` completes.
- `VehicleSilSimulation` update diagram passes.
- `INS_VARIANT = 1` and `2` pass.
- One-second `VehiclePlant` smoke simulation runs without NaN/Inf or solver errors.

### Step 7. Run Live SITL Loop Closure

This is the first test that proves the vehicle is connected to PX4, not just compilable.

Process:

1. Initialize the vehicle in MATLAB:

   ```matlab
   initVehicleSIL("launchFullSIL", false, ...
       "vehicleType", "<vehicle_key>", ...
       "controllerRuntime", "SITL", ...
       "visualizationType", "Matlab", ...
       "simHostIP", "<WINDOWS_IP>")
   ```

2. Start `VehicleSilSimulation`.

3. In WSL:

   ```bash
   cd /mnt/c/AnelloSummer/optimAeroPX4SIL/PX4-Autopilot
   export PX4_SIM_HOSTNAME=<WINDOWS_IP>
   make px4_sitl_default <PX4Target>
   ```

4. Check PX4 topics:

   ```text
   listener sensor_accel
   listener sensor_gyro
   listener sensor_mag
   listener vehicle_attitude
   listener vehicle_local_position
   listener actuator_outputs
   ```

Pass criteria:

- PX4 connects to Simulink.
- Sensors publish finite values.
- Attitude and local position are finite.
- Actuator outputs change when PX4 is armed or commanded.
- Simulink plant state responds to actuator commands.

## Solver And Performance Notes

Full Simscape models can be too slow for PX4 SITL if used directly.

Recommended strategy:

- Start with abstract or simplified Simscape variants.
- Disable unnecessary detailed visualization/contact during SITL.
- Use Simscape local solver where appropriate.
- Keep the SIL fixed-step sample time compatible with `stepSize_s = 0.004`.
- Only enable detailed tire/contact/terrain/hydraulics after loop closure is proven.

If a high-fidelity model cannot run fast enough, keep two variants:

| Variant | Purpose |
|---|---|
| simplified plant | PX4 SITL loop closure and waypoint tests |
| high-fidelity Simscape plant | offline dynamics and design validation |

## Test Levels

| Level | Purpose | Required before next level |
|---|---|---|
| Registry check | Vehicle key resolves and has PX4 target | Yes |
| Simulink update | `VehicleSilSimulation` compiles | Yes |
| Plant smoke | Plant runs briefly without solver/state errors | Yes |
| PX4 build | `make px4_sitl_default` works | Yes |
| PX4 target launch | `make px4_sitl_default <target>` starts | Yes |
| Sensor loop | PX4 receives finite simulated sensors | Yes |
| Actuator loop | Simulink receives PX4 actuator outputs | Yes |
| Movement test | Plant state changes correctly | Yes |
| Waypoint test | Vehicle follows a simple command/mission | Final |

## Current Status

As of 2026-07-10:

- The registered non-Simscape baseline vehicles compile through `VehicleSilSimulation`.
- All registered vehicles passed update diagram checks with `INS_VARIANT = 1` and `INS_VARIANT = 2`.
- PX4 target and airframe registration checks passed for all registered vehicles.
- `make px4_sitl_default` passed in WSL.
- `VehiclePlant.slx` now has separate plant variant choices for:
  - `evtol_simscape` -> `vehicle/evtol_simscape/evtol_simscape.slx`, `optimAeroEVTOL`
  - `tracked_vehicle_simscape` -> `vehicle/tracked_vehicle_simscape/tracked_vehicle_simscape.slx`, `optimAeroTracked`
  - `wheel_loader_simscape` -> `vehicle/wheel_loader_simscape/wheel_loader_simscape.slx`, `optimAeroLoader`
  - `ackermann_simscape` -> `vehicle/ackermann_simscape/ackermann_simscape.slx`, `optimAeroAckermann`
- Each adapter model contains a structurally healthy `simscape_adapter_diagnostics` subsystem with
  a Simscape solver configuration, electrical reference, controlled source, load, voltage sensor,
  Simulink-PS converter, and PS-Simulink converter. This proves the adapter model references can
  host Simscape networks under the current plant contract.
- The adapter models are cloned from the validated baseline plant models and include copied local
  component model dependencies so the adapter folders are selectable as independent plant variants.
- `initVehicleSIL` and `vehicle/vehicleRegistry.m` now select the adapter plant model when a
  `*_simscape` vehicle key is requested.
- All four Simscape adapter variants passed `runVehicleIntegrationMatrix` with `INS_VARIANT = 1`,
  `INS_VARIANT = 2`, PX4 target/airframe registration, and `runPlantSmoke = true`:
  - `work/vehicle_test_results/evtol_simscape_matrix_20260710_114734.json`
  - `work/vehicle_test_results/tracked_vehicle_simscape_matrix_20260710_115335.json`
  - `work/vehicle_test_results/wheel_loader_simscape_matrix_20260710_120001.json`
  - `work/vehicle_test_results/ackermann_simscape_matrix_20260710_120529.json`
- Bounded PX4 target launches were run for the four adapter PX4 targets. Each target started PX4,
  selected the expected `SYS_AUTOSTART`, and waited for simulator TCP connection on port `4560`:
  - `work/px4_sitl_logs/optimAeroEVTOL_20260710_122658.log`
  - `work/px4_sitl_logs/optimAeroTracked_20260710_122821.log`
  - `work/px4_sitl_logs/optimAeroLoader_20260710_122938.log`
  - `work/px4_sitl_logs/optimAeroAckermann_20260710_123055.log`
- Baseline `evtol` and `hexarotor` were spot-checked after the registry/initialization changes and
  still passed with both INS variants.
- Live per-vehicle waypoint tests are still open.
- The current waypoint smoke utility is a placeholder and should not be treated as proof of flight.

## Recommended Next Work

1. Replace the contained diagnostic Simscape networks with command-coupled physical subsystems:
   - eVTOL: motor/propeller/powertrain pieces.
   - tracked vehicle: reduced left/right track force model before full contact.
   - wheel loader: steering plus wheel torque on flat terrain; defer hydraulics/bucket.
   - Ackermann: steering angle plus drive torque at low speed before tire/suspension fidelity.
2. Implement the command and state adapter subsystems described in the adapter contract files under
   each `vehicle/*_simscape/adapters/` folder.
3. Re-run `runVehicleIntegrationMatrix("vehicleTypes", "<vehicle_key>", "insVariants", [1 2],
   "runPlantSmoke", true)` for each adapter entry after structural model edits.
4. Run one live PX4 SITL movement test with `VehicleSilSimulation` running at the same time for
   each adapter target. The bounded PX4-only launches already show that PX4 starts and waits for
   the simulator; the remaining proof is sensor/actuator loop closure and movement.
5. Repeat the pattern for MSS-based USV/UUV.
