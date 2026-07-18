# bicycle_rover

Small single-track steer+throttle rover. PX4 target `optimAeroBicycle`
(`10034_optimAeroBicycle`), same steering+throttle mixer class as `ackermann_rover`
(`PWM_MAIN_FUNC1=201` servo, `PWM_MAIN_FUNC2=101` motor).

## Physics

Same kinematic-bicycle equations as `ackermann_rover` (own MATLAB Function block, own
parameters -- see `vehicle/ackermann_rover/README.md` for the equations):

```
posN' = v cos(psi)              yaw'      = (v / wheelBase) tan(delta)
posE' = v sin(psi)              v'        = (v_cmd - v) / tauThrottle
                                 delta'    = (delta_cmd - delta) / tauSteer
```

Parameters (`setUpVehicle.m`/`setUpActuators.m` case `"bicycle_rover"`):

| Parameter | Value | Source |
|---|---|---|
| `dryMass_kg` | 25 | order-of-magnitude placeholder, lighter than ackermann_rover |
| `aircraftInertialBody_kgm2` | diag(2, 6, 7) | placeholder |
| `wheelBase_m` | 1.2 | matches airframe `GND_WHEEL_BASE=1.2` |
| `maxSteerAngle_rad` | 0.6 | matches airframe `GND_MAX_ANG=0.6` |
| `maxSpeed_mps` | 6.0 | placeholder |
| `tauSteer_s` | 0.15 | steering servo lag |
| `tauThrottle_s` | 0.4 | drive motor lag (faster than ackermann_rover) |

## Axis convention / gravity / units

NED world, FRD body; gravity sign not independently meaningful (ground vehicle, constant
altitude); SI units, normalized `[-1,1]`/`[0,1]` commands.

## Numerical validation

With throttle=0.6, steering=0.3 for 1 s: velocity converges to 3.308 m/s (target
`0.6x6.0=3.6`, `3.6x(1-e^(-1/0.4))=3.305` -- matches), steering converges to 0.1798 rad
(target `0.3x0.6=0.18` -- matches). No NaN/Inf. Passed on the first build (no mechanical
port-width/type fixes needed, unlike `ackermann_rover`'s pilot pass).

## Not modeled

Tire slip, suspension, ground-contact forces (`AircraftForcesMomentsBus` left at zero).
Live PX4 loop closure not yet attempted (Fase 6).

**Known metadata gap (not fixed here):** the PX4 airframe file previously declared
`@autostart 10031` while named/registered as `10034` -- corrected as part of Fase 5
(`PLAN_CORRECCION_MULTIVEHICULO_SITL.md` F8).

## Validation evidence

`work/vehicle_test_results/vehicle_integration_matrix_20260714_201813.json` --
`passed=1, plantSmokePassed=1`.
