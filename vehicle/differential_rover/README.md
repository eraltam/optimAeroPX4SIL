# differential_rover

Small skid/differential-drive ground rover (twin independently-driven wheels/sides). PX4
target `optimAeroDiffRover` (`10026_optimAeroDiffRover`), twin-motor mixer class
(`PWM_MAIN_FUNC1=101` left, `PWM_MAIN_FUNC2=102` right).

## Physics

Differential-drive kinematic plant, implemented as a MATLAB Function block
(`DifferentialKinematics`) inside `differential_rover.slx`. This is the base
implementation reused (copied, with per-vehicle constants) by `tracked_vehicle`,
`tracked_vehicle_simscape`, `usv_surface`, `uuv_subsea`, and `unicycle_rover` -- see each
of those READMEs for why the same math applies to tracks and twin-thruster marine
vehicles at low speed.

```
omegaLeft'  = (cmdLeft  * maxSpeed/wheelRadius - omegaLeft)  / tauMotor
omegaRight' = (cmdRight * maxSpeed/wheelRadius - omegaRight) / tauMotor
v        = wheelRadius * (omegaLeft + omegaRight) / 2
yawRate  = wheelRadius * (omegaRight - omegaLeft) / trackWidth
posN'    = v cos(psi)      posE' = v sin(psi)      yaw' = yawRate
```

Parameters (`setUpVehicle.m`/`setUpActuators.m` case `"differential_rover"`):

| Parameter | Value |
|---|---|
| `dryMass_kg` | 50 |
| `aircraftInertialBody_kgm2` | diag(4, 12, 14) |
| `trackWidth_m` | 0.6 |
| `wheelRadius_m` | 0.15 |
| `maxSpeed_mps` | 5.0 |
| `tauMotor_s` | 0.4 |

`PropulsionBus` for this vehicle has 2 elements (left/right motor angular velocity packed
as a vector inside one `motorAngVel_radps` element, not two scalar elements) -- see
`PLAN_CORRECCION_MULTIVEHICULO_SITL.md` "F10 (retracted)" for why the element *count*
matters (each active vehicle's own `PropulsionBus` must be internally consistent) but the
specific 2-vs-6-element shape does not need to match hexarotor's.

## Axis convention / gravity / units

NED world, FRD body; gravity sign not independently meaningful (ground vehicle, constant
altitude); SI units, normalized `[-1,1]` per-side motor commands.

## Not modeled

Wheel slip, suspension, ground-contact forces (`AircraftForcesMomentsBus` left at zero).
Live PX4 loop closure not yet attempted (Fase 6).

## Validation evidence

`work/vehicle_test_results/vehicle_integration_matrix_20260714_192847.json` --
`passed=1, plantSmokePassed=1`. See also the "F10 (retracted)" section of
`PLAN_CORRECCION_MULTIVEHICULO_SITL.md` for the debugging story behind this vehicle's
implementation (a `save_system` failure that looked like a Simulink platform bug but
wasn't).
