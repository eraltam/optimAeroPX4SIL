# wheel_loader

Small construction wheel loader. PX4 target `optimAeroLoader` (`10028_optimAeroLoader`),
steering+throttle mixer class (`PWM_MAIN_FUNC1=201` servo, `PWM_MAIN_FUNC2=101` motor) --
same mixer class as `ackermann_rover`/`bicycle_rover`.

## Physics

Same kinematic-bicycle equations as `ackermann_rover` (own MATLAB Function block, own
parameters -- see `vehicle/ackermann_rover/README.md` for the equations). Deliberately
heavier and slower than the small rovers.

Parameters (`setUpVehicle.m`/`setUpActuators.m` case `"wheel_loader"`):

| Parameter | Value | Note |
|---|---|---|
| `dryMass_kg` | 4500 | compact-loader placeholder |
| `aircraftInertialBody_kgm2` | diag(1200, 3500, 4000) | placeholder |
| `wheelBase_m` | 2.5 | airframe doesn't set `GND_WHEEL_BASE`; reasonable placeholder |
| `maxSteerAngle_rad` | 0.5 | airframe doesn't set `GND_MAX_ANG`; reasonable placeholder |
| `maxSpeed_mps` | 4.0 | placeholder |
| `tauSteer_s` | 0.4 | 2-3x slower than the small rovers |
| `tauThrottle_s` | 0.8 | 2x slower than `ackermann_rover` |

**Hydraulics and bucket are explicitly out of scope** at this fidelity level (matches
`SIMSCAPE_ADAPTER_HANDOFF.md`: "steering plus wheel torque on flat terrain; defer
hydraulics/bucket").

## Axis convention / gravity / units

NED world, FRD body; gravity sign not independently meaningful (ground vehicle, constant
altitude); SI units, normalized `[-1,1]`/`[0,1]` commands.

## Numerical validation

With steering=0.4, throttle=0.7 for 1 s: velocity converges to 2.000 m/s (target
`0.7x4.0x(1-e^(-1/0.8))=1.998`), steering converges to 0.1838 rad (target
`0.4x0.5x(1-e^(-1/0.4))=0.1836`). No NaN/Inf. Passed on the first build.

## Not modeled

Tire slip, suspension, hydraulics, bucket/lift mechanism, ground-contact forces. Live
PX4 loop closure not yet attempted (Fase 6).

## Validation evidence

`work/vehicle_test_results/vehicle_integration_matrix_20260714_205552.json` --
`passed=1, plantSmokePassed=1`.
