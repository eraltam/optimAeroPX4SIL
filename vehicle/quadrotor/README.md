# quadrotor

X-quad multirotor. PX4 target `optimAeroQuad` (`10022_optimAeroQuad`).

## Physics

Unlike the rovers/marine vehicles, `quadrotor` **reuses the hexarotor's real 6DOF
rigid-body engine unmodified** (`hexActuator.slx`/`hexMotorModel.slx` -- confirmed by
inspection that the `For Each Subsystem1` block inside `hexMotorModel.slx` is already
parametric in rotor count via `vehicleParams.rotor.motorLocs`/`spinDirectionCW_isTrue`).
Only rotor geometry, mass, and inertia are vehicle-specific.

Rotor layout (`setUpActuators.m` case `"quadrotor"`): X-quad, taken directly from
`PX4-Autopilot/.../airframes/10022_optimAeroQuad` (`CA_ROTORn_PX/PY/KM`), arm length
1.0 m, same motor/prop/ESC coefficients as hexarotor (T-Motor-class generic, not
mass-specific: `rotor.diameter_m=1.016`, `rotor.maxRPM=3473`).

Mass/inertia (`setUpVehicle.m` case `"quadrotor"`): scaled from the hexarotor's own
values by rotor-count ratio (4/6) -- an order-of-magnitude placeholder for a quad on the
same physical scale as the existing hex, not a specific catalog vehicle.

| Parameter | Value |
|---|---|
| `dryMass_kg` | (4/6) x 180 lbs = 54.4 kg |
| `aircraftInertialBody_kgm2` | (4/6) x diag(11.8, 11.8, 23.5) |
| `SRef_m2` | 0.35 |

**Only shared-file change required:** `hexMotorModel.slx`'s "armed/disarmed" `Constant`
block was hardcoded to `zeros(1,6)`; corrected to
`zeros(1, numel(vehicleParams.rotor.spinDirectionCW_isTrue))` so it works for any rotor
count. Verified against a full 10-vehicle regression that this did not change hexarotor's
behavior.

## Axis convention / gravity / units

Same as hexarotor: NED world, FRD body, gravity applied through the standard
`gravity.slx`/`cgMassInertia.slx` chain (unchanged from hexarotor). SI units.

## Not modeled

Live PX4 loop closure not yet attempted (Fase 6). Otherwise same fidelity level as
hexarotor (same rigid-body/aero/actuator engine), just with quad-specific geometry.

## Validation evidence

`work/vehicle_test_results/vehicle_integration_matrix_20260715_014859.json` (final
10-vehicle regression including the LADAC family) -- `passed=1, plantSmokePassed=1`.
