# tracked_vehicle

Skid-steer tracked vehicle. PX4 target `optimAeroTracked` (`10027_optimAeroTracked`),
twin-motor mixer class (`PWM_MAIN_FUNC1=101` left track, `PWM_MAIN_FUNC2=102` right
track).

## Physics

At low speed, a skid-steer track's kinematics are differential-drive-equivalent (left/
right track speed -> forward velocity + yaw rate via an effective track width), so
`tracked_vehicle.slx` reuses `differential_rover.slx`'s exact `DifferentialKinematics`
MATLAB Function block (see `vehicle/differential_rover/README.md` for the equations)
with its own constants for a heavy, slow vehicle.

Parameters (`setUpVehicle.m`/`setUpActuators.m` case `"tracked_vehicle"`):

| Parameter | Value |
|---|---|
| `dryMass_kg` | 2500 |
| `aircraftInertialBody_kgm2` | diag(400, 1200, 1400) |
| `trackWidth_m` | 1.5 |
| `wheelRadius_m` | 0.3 |
| `maxSpeed_mps` | 3.0 |
| `tauMotor_s` | 0.6 |

## Axis convention / gravity / units

NED world, FRD body; gravity sign not independently meaningful (ground vehicle, constant
altitude); SI units, normalized `[-1,1]` per-track commands.

## Not modeled (the important one for this vehicle)

**Real track/terrain contact and slip are NOT modeled** -- this is a kinematic
approximation, not a Simscape Multibody track model. The full-fidelity reference this
could be upgraded to already exists in this repo at
`Tracked-Vehicles-Simscape/Models/{Excavator_Tracks,Tractor_Tracks}` (belt/chain track
dynamics, terrain contact, multiple fidelity levels from abstract `*_float` variants to
detailed point-cloud contact `*_ptcld` variants). That port is deferred future work, not
attempted in this pass -- see `PLAN_CORRECCION_MULTIVEHICULO_SITL.md` Fase 3 step 2 and
`SIMSCAPE_VEHICLE_INTEGRATION_PLAN.md` section "3. Tracked Vehicle" for the plan already
written for it.

## Validation evidence

`work/vehicle_test_results/vehicle_integration_matrix_20260715_034801.json` (final
17-vehicle regression, lote 1) -- `passed=1, plantSmokePassed=1`.
