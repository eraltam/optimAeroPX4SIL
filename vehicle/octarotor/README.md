# octarotor

Flat-X octocopter. PX4 target `optimAeroOcta` (`10024_optimAeroOcta`).

## Physics

Same reuse pattern as `quadrotor`: reuses the hexarotor's real 6DOF rigid-body engine
unmodified (`hexActuator.slx`/`hexMotorModel.slx`, parametric in rotor count). Only rotor
geometry, mass, and inertia are vehicle-specific.

Rotor layout (`setUpActuators.m` case `"octarotor"`): 8 rotors at 45-degree spacing,
alternating spin direction, arm length 1.0 m matching hex's scale. **Note:** the PX4
airframe (`10024_optimAeroOcta`) declares `CA_ROTOR_COUNT=8` but does not specify
`CA_ROTORn_PX/PY` (uses PX4's internal default octa-X geometry, which this repo cannot
inspect) -- this repo's flat-octo-X layout is our own explicit, physically valid choice,
not lifted from the airframe file. Same motor/prop/ESC coefficients as hexarotor.

Mass/inertia (`setUpVehicle.m` case `"octarotor"`): scaled from hexarotor by rotor-count
ratio (8/6).

| Parameter | Value |
|---|---|
| `dryMass_kg` | (8/6) x 180 lbs = 108.9 kg |
| `aircraftInertialBody_kgm2` | (8/6) x diag(11.8, 11.8, 23.5) |
| `SRef_m2` | 0.65 |

## Axis convention / gravity / units

Same as hexarotor: NED world, FRD body, standard gravity chain, SI units.

## Not modeled

Live PX4 loop closure not yet attempted (Fase 6). The `CA_ROTORn_PX/PY` geometry gap
noted above (same class of documentation gap as the `@autostart` mismatches, F8) means
this vehicle's simulated rotor layout has not been cross-checked against whatever
geometry PX4's internal default octa-X mixer actually assumes.

## Validation evidence

`work/vehicle_test_results/vehicle_integration_matrix_20260715_014859.json` (final
10-vehicle regression including the LADAC family) -- `passed=1, plantSmokePassed=1`.
