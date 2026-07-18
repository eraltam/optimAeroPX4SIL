# unicycle_rover

Small skid/differential-drive rover -- despite the "unicycle" name, PX4's rover mixer has
no true single-wheel airframe type, so this vehicle uses the same twin-motor mixer class
as `differential_rover`. PX4 target `optimAeroUnicycle` (`10035_optimAeroUnicycle`),
`PWM_MAIN_FUNC1=101` left motor, `PWM_MAIN_FUNC2=102` right motor.

## Physics

Reuses `differential_rover.slx`'s exact `DifferentialKinematics` MATLAB Function block
and equations (see `vehicle/differential_rover/README.md`) with its own constants for a
small, fast rover.

Parameters (`setUpVehicle.m`/`setUpActuators.m` case `"unicycle_rover"`):

| Parameter | Value |
|---|---|
| `dryMass_kg` | 15 |
| `aircraftInertialBody_kgm2` | diag(1, 3, 3.5) |
| `trackWidth_m` | 0.35 |
| `wheelRadius_m` | 0.1 |
| `maxSpeed_mps` | 3.0 |
| `tauMotor_s` | 0.3 (fastest motor lag of the differential-drive family) |

## Axis convention / gravity / units

NED world, FRD body; gravity sign not independently meaningful (ground vehicle, constant
altitude); SI units, normalized `[-1,1]` per-side motor commands.

## Numerical validation

With left motor cmd=0.5, right motor cmd=0.9 for 1 s (250 steps at `Ts=0.004s`): left
wheel angular velocity converges to 1.448 rad/s (target
`0.5x(3.0/0.1)x(1-e^(-1/0.3))=1.446`), right wheel converges to 2.606 rad/s (target
`0.9x(3.0/0.1)x(1-e^(-1/0.3))=2.604`). No NaN/Inf.

## Not modeled

Wheel slip, suspension, ground-contact forces. Live PX4 loop closure not yet attempted
(Fase 6).

**Known metadata gap (not fixed here):** the PX4 airframe file previously declared
`@autostart 10032` while named/registered as `10035` -- corrected as part of Fase 5
(`PLAN_CORRECCION_MULTIVEHICULO_SITL.md` F8).

## Validation evidence

`work/vehicle_test_results/vehicle_integration_matrix_20260714_225616.json` (standalone)
and `..._231410.json` (full 7-vehicle regression) -- `passed=1, plantSmokePassed=1`.
