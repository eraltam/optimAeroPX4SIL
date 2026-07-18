# uuv_subsea

Twin-thruster subsea vehicle. PX4 target `optimAeroUUV` (`10033_optimAeroUUV`),
twin-motor mixer class (`PWM_MAIN_FUNC1=101` left thruster, `PWM_MAIN_FUNC2=102` right
thruster) -- same mixer class as `differential_rover`/`tracked_vehicle`/`usv_surface`.

## Physics

Same reuse rationale as `usv_surface`: horizontal-plane (yaw + surge) kinematics only,
via `differential_rover.slx`'s `DifferentialKinematics` MATLAB Function block (see
`vehicle/differential_rover/README.md` for the equations). The vehicle stays at a
constant reference depth (`referenceAltitude_m = -5`, i.e. 5 m below sea level in NED).

Parameters (`setUpVehicle.m`/`setUpActuators.m` case `"uuv_subsea"`):

| Parameter | Value |
|---|---|
| `dryMass_kg` | 300 |
| `aircraftInertialBody_kgm2` | diag(50, 150, 170) |
| `trackWidth_m` | 1.2 (thruster separation) |
| `wheelRadius_m` | 0.08 |
| `maxSpeed_mps` | 2.0 (slower than `usv_surface` -- greater submerged added-mass/drag) |
| `tauMotor_s` | 1.5 (longer thruster lag than `usv_surface`) |
| `referenceAltitude_m` | -5 (constant reference depth) |

**Fixed during Fase 3/final integration:** same hardcoded-altitude bug as `usv_surface`
(the `DifferentialKinematics` chart's reference altitude was hardcoded to the
hexarotor's airport altitude, inherited via copy-paste) -- corrected directly in the
chart script so `uuv_subsea` actually starts at -5 m instead of the airport's altitude.

## Axis convention / gravity / units

NED world, FRD body; gravity sign not independently meaningful at this fidelity level
(vehicle held at constant reference depth); SI units, normalized `[-1,1]` per-thruster
commands.

## Not modeled (the important one for this vehicle)

**Depth/heave/pitch/roll are NOT modeled** -- the vehicle stays at a fixed reference
depth rather than actually diving/surfacing. No hydrodynamic added mass, damping matrix,
or restoring forces. The reference this could be upgraded to already exists in this
repo: `MSS/CRAFT` (`remus100`-class 6DOF AUV models) and the AUV folder alongside it.
This port is deferred future work, not attempted in this pass -- see
`PLAN_CORRECCION_MULTIVEHICULO_SITL.md` Fase 3 step 5.

## Validation evidence

`work/vehicle_test_results/vehicle_integration_matrix_20260715_040234.json` (final
17-vehicle regression, lote 2) -- `passed=1, plantSmokePassed=1`.
