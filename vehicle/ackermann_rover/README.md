# ackermann_rover

Small Ackermann-steered ground rover (single front-steer axle). PX4 target
`optimAeroAckermann` (`10023_optimAeroAckermann`), steering+throttle mixer class
(`PWM_MAIN_FUNC1=201` servo, `PWM_MAIN_FUNC2=101` motor).

## Physics

Kinematic-bicycle plant, implemented as a single MATLAB Function block
(`AckermannKinematics`) inside `ackermann_rover.slx`. No tire/suspension/slip model --
this is a pure kinematic approximation valid at low speed.

```
posN' = v cos(psi)              yaw'      = (v / wheelBase) tan(delta)
posE' = v sin(psi)              v'        = (v_cmd - v) / tauThrottle
                                 delta'    = (delta_cmd - delta) / tauSteer
```

Parameters (`vehicle/common/setUpVehicle.m` case `"ackermann_rover"`, `setUpActuators.m`
case `"ackermann_rover"`):

| Parameter | Value | Source |
|---|---|---|
| `dryMass_kg` | 60 | order-of-magnitude placeholder for a small UGV |
| `aircraftInertialBody_kgm2` | diag(5, 15, 18) | placeholder |
| `wheelBase_m` | 2.0 | matches airframe `GND_WHEEL_BASE=2.0` |
| `maxSteerAngle_rad` | 0.6 | matches airframe `GND_MAX_ANG=0.6` |
| `maxSpeed_mps` | 8.0 | placeholder |
| `tauSteer_s` | 0.15 | steering servo lag |
| `tauThrottle_s` | 0.5 | drive motor lag |

`AircraftForcesMomentsBus` (`Aero`/`Engine`/`Ground`/`WeightForcesMomentsBus`) is left at
zero -- this plant does not simulate tire/traction forces, only commanded kinematics.

## Axis convention / gravity / units

World frame is NED, body frame is FRD, matching every other vehicle in this repo
(`optimAeroPX4SIL/CLAUDE.md` section on `DCM_be`). Gravity sign is not independently
meaningful for a ground vehicle held at constant NED altitude (no vertical dynamics
modeled). All rates/speeds in SI (rad/s, m/s); steering/throttle commands are
normalized `[-1, 1]`/`[0, 1]` fractions of `maxSteerAngle_rad`/`maxSpeed_mps`.

## Numerical validation (independent of the Simulink wiring)

With throttle=0.5, steering=0.2 for 1 s: velocity converges to 3.463 m/s (matches the
analytical first-order-lag prediction from `tauThrottle_s`), rover stays exactly at rest
under a zero command. No NaN/Inf.

## Not modeled (explicit gaps)

- Tire slip, suspension, weight transfer.
- Aerodynamic/ground-contact forces (`AircraftForcesMomentsBus` sub-buses are zero).
- Live PX4 loop closure (Fase 6 of `PLAN_CORRECCION_MULTIVEHICULO_SITL.md`) -- only
  compile + 1-second plant-smoke simulation verified so far.

## Validation evidence

`work/vehicle_test_results/vehicle_integration_matrix_20260714_173652.json` --
`runVehicleIntegrationMatrix` with `runPlantSmoke=true`, both INS variants, `passed=1`.
