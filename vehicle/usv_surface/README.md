# usv_surface

Twin-thruster surface vessel. PX4 target `optimAeroUSV` (`10029_optimAeroUSV`),
twin-motor mixer class (`PWM_MAIN_FUNC1=101` left thruster, `PWM_MAIN_FUNC2=102` right
thruster) -- same mixer class as `differential_rover`/`tracked_vehicle`.

## Physics

Surge speed + yaw rate from differential thrust is the same math as differential-drive,
so `usv_surface.slx` reuses `differential_rover.slx`'s exact `DifferentialKinematics`
MATLAB Function block (see `vehicle/differential_rover/README.md` for the equations)
with its own constants. The vehicle stays at a constant NED reference altitude
(`referenceAltitude_m = 0`, sea level) -- heave/pitch/roll are not modeled.

Parameters (`setUpVehicle.m`/`setUpActuators.m` case `"usv_surface"`):

| Parameter | Value |
|---|---|
| `dryMass_kg` | 800 |
| `aircraftInertialBody_kgm2` | diag(200, 600, 700) |
| `trackWidth_m` | 2.0 (propeller separation) |
| `wheelRadius_m` | 0.1 |
| `maxSpeed_mps` | 3.0 |
| `tauMotor_s` | 1.0 (slower than the ground rovers -- added hydrodynamic mass) |
| `referenceAltitude_m` | 0 (sea level) |

**Fixed during Fase 3/final integration:** the reference altitude used inside the
`DifferentialKinematics` chart was originally hardcoded to `116*0.3048` (the hexarotor's
airport altitude, inherited via copy-paste from `differential_rover`) rather than read
from `vehicleParams`/workspace. This was corrected directly in the chart script so
`usv_surface` actually starts at sea level instead of silently inheriting the airport's
altitude.

## Axis convention / gravity / units

NED world, FRD body; gravity sign not independently meaningful at this fidelity level
(vehicle held at constant NED altitude); SI units, normalized `[-1,1]` per-thruster
commands.

## Not modeled (the important one for this vehicle)

**Horizontal plane only (surge + yaw). Heave/pitch/roll/depth are NOT modeled** -- same
simplification as "constant ground level" for the terrestrial rovers. No hydrodynamic
added mass, damping matrix, or restoring forces (Fossen/MSS formulation). The reference
this could be upgraded to already exists in this repo: `MSS/CRAFT/USV` (e.g.
`otter.m`/`SIMotter.m`) and ready-made Simulink templates in
`MSS/SIMULINK/mssSimulinkDemos/` (`demoOtterUSVHeadingControl.slx`,
`demoOtterUSVPathFollowingCourseControl.slx`). This port is deferred future work, not
attempted in this pass -- see `PLAN_CORRECCION_MULTIVEHICULO_SITL.md` Fase 3 step 4.

## Validation evidence

`work/vehicle_test_results/vehicle_integration_matrix_20260715_040234.json` (final
17-vehicle regression, lote 2) -- `passed=1, plantSmokePassed=1`.
