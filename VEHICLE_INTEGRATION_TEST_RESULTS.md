# Vehicle Integration Test Results

Date: 2026-07-10

## Scope

Validated the registered vehicle matrix against:

- `initVehicleSIL(..., "controllerRuntime", "SITL")`
- `VehicleSilSimulation` update diagram
- `INS_VARIANT = 1` generic INS
- `INS_VARIANT = 2` ANELLO X3 INS
- PX4 airframe file registration
- PX4 `simulator_mavlink` custom target registration
- Shared PX4 SITL binary build with `make px4_sitl_default`

These checks do not prove a live waypoint flight. They prove the Simulink side initializes and
compiles for each vehicle and that PX4 has the matching launch target and airframe entry.

## Commands Run

```matlab
cd("C:\AnelloSummer\optimAeroPX4SIL")
addpath("utilities")

runVehicleIntegrationMatrix( ...
    "vehicleTypes", [string("hexarotor"), string("F16")], ...
    "insVariants", [1 2])

runVehicleIntegrationMatrix( ...
    "vehicleTypes", [string("quadrotor"), string("octarotor"), string("evtol"), string("ackermann_rover")], ...
    "insVariants", [1 2])

runVehicleIntegrationMatrix( ...
    "vehicleTypes", [string("differential_rover"), string("tracked_vehicle"), string("wheel_loader"), string("usv_surface")], ...
    "insVariants", [1 2])

runVehicleIntegrationMatrix( ...
    "vehicleTypes", [string("uuv_subsea"), string("bicycle_rover"), string("unicycle_rover")], ...
    "insVariants", [1 2])
```

```powershell
wsl bash -lc "cd /mnt/c/AnelloSummer/optimAeroPX4SIL/PX4-Autopilot && make px4_sitl_default"
```

## Results

| Vehicle | PX4 target | INS 1 update | INS 2 update | PX4 target registered | Airframe registered |
|---|---|---:|---:|---:|---:|
| `F16` | `optimAeroF16` | pass | pass | pass | pass |
| `hexarotor` | `optimAeroHex` | pass | pass | pass | pass |
| `quadrotor` | `optimAeroQuad` | pass | pass | pass | pass |
| `octarotor` | `optimAeroOcta` | pass | pass | pass | pass |
| `evtol` | `optimAeroEVTOL` | pass | pass | pass | pass |
| `ackermann_rover` | `optimAeroAckermann` | pass | pass | pass | pass |
| `differential_rover` | `optimAeroDiffRover` | pass | pass | pass | pass |
| `tracked_vehicle` | `optimAeroTracked` | pass | pass | pass | pass |
| `wheel_loader` | `optimAeroLoader` | pass | pass | pass | pass |
| `usv_surface` | `optimAeroUSV` | pass | pass | pass | pass |
| `uuv_subsea` | `optimAeroUUV` | pass | pass | pass | pass |
| `bicycle_rover` | `optimAeroBicycle` | pass | pass | pass | pass |
| `unicycle_rover` | `optimAeroUnicycle` | pass | pass | pass | pass |

PX4 shared SITL build result: pass.

## Simscape Adapter Results

Additional adapter variants were added after the baseline matrix:

| Vehicle | Adapter plant model | PX4 target | INS 1 update | INS 2 update | Plant smoke | PX4 launch |
|---|---|---|---:|---:|---:|---:|
| `evtol_simscape` | `evtol_simscape.slx` | `optimAeroEVTOL` | pass | pass | pass | starts, waits on TCP 4560 |
| `tracked_vehicle_simscape` | `tracked_vehicle_simscape.slx` | `optimAeroTracked` | pass | pass | pass | starts, waits on TCP 4560 |
| `wheel_loader_simscape` | `wheel_loader_simscape.slx` | `optimAeroLoader` | pass | pass | pass | starts, waits on TCP 4560 |
| `ackermann_simscape` | `ackermann_simscape.slx` | `optimAeroAckermann` | pass | pass | pass | starts, waits on TCP 4560 |

Adapter matrix evidence:

- `work/vehicle_test_results/evtol_simscape_matrix_20260710_114734.json`
- `work/vehicle_test_results/tracked_vehicle_simscape_matrix_20260710_115335.json`
- `work/vehicle_test_results/wheel_loader_simscape_matrix_20260710_120001.json`
- `work/vehicle_test_results/ackermann_simscape_matrix_20260710_120529.json`

PX4 bounded launch evidence:

- `work/px4_sitl_logs/optimAeroEVTOL_20260710_122658.log`
- `work/px4_sitl_logs/optimAeroTracked_20260710_122821.log`
- `work/px4_sitl_logs/optimAeroLoader_20260710_122938.log`
- `work/px4_sitl_logs/optimAeroAckermann_20260710_123055.log`

The `PX4 launch` result is intentionally bounded by `timeout 75s`; exit code `124` means the
timeout stopped PX4 after it started and waited for a simulator connection. It is not a startup
failure by itself.

Local JSON evidence:

- `work/vehicle_test_results/vehicle_integration_matrix_20260710_093412.json`
- `work/vehicle_test_results/vehicle_integration_matrix_20260710_094434.json`
- `work/vehicle_test_results/vehicle_integration_matrix_20260710_095455.json`
- `work/vehicle_test_results/vehicle_integration_matrix_20260710_100341.json`

## Remaining Live Tests

The next validation layer is live SITL loop closure per vehicle:

1. Start `VehicleSilSimulation` for one vehicle.
2. Launch the matching PX4 helper, for example `make px4_sitl_default optimAeroQuad`.
3. Confirm PX4 connects to Simulink on TCP `4560`.
4. Verify `listener sensor_accel`, `listener sensor_gyro`, `listener sensor_mag`, `listener vehicle_attitude`, and actuator output topics.
5. Execute a simple mode-specific waypoint or movement command and compare PX4 state against Simulink plant state.

The current waypoint smoke utility still records a placeholder `finalWaypointError = 0`; do not treat
those waypoint JSON files as proof of live waypoint tracking.
