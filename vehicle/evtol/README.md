# evtol

Lightweight eVTOL demonstrator, simulated as a 6-motor multirotor. PX4 target
`optimAeroEVTOL` (`10025_optimAeroEVTOL`).

## Physics

Same reuse pattern as `quadrotor`/`octarotor`: reuses the hexarotor's real 6DOF
rigid-body engine unmodified (`hexActuator.slx`/`hexMotorModel.slx`), with the exact same
rotor geometry as hexarotor (no distinct `CA_ROTORn_PX/PY` given in the airframe file
either) but its own, much lighter mass/inertia -- a small electric VTOL demonstrator
class, distinct from the heavy-lift hex.

**Important fidelity gap:** the PX4 airframe is declared `@type VTOL Tiltrotor`, but its
actual `PWM_MAIN_FUNC1-6` assignment (101-106, all plain motor functions) only wires a
6-motor multirotor mixer -- there is no distinct tilt/pusher actuator channel. **True
tiltrotor transition, pusher-prop, and fixed-wing aerodynamics are NOT modeled** at this
fidelity level (same documented-gap pattern as the kinematic rovers' "no tire slip"). If
this vehicle needs real transition physics, treat this as a from-scratch Phase-3-style
task, not a parameter tweak.

Mass/inertia (`setUpVehicle.m` case `"evtol"`): own class, not scaled from hexarotor.

| Parameter | Value |
|---|---|
| `dryMass_kg` | 45 |
| `aircraftInertialBody_kgm2` | diag(6.5, 6.5, 13) |
| `SRef_m2` | 0.5 |

## Axis convention / gravity / units

Same as hexarotor: NED world, FRD body, standard gravity chain, SI units.

## Not modeled

Tiltrotor transition/pusher-prop/fixed-wing aero (see above). Live PX4 loop closure not
yet attempted (Fase 6).

## Validation evidence

`work/vehicle_test_results/vehicle_integration_matrix_20260715_014859.json` (final
10-vehicle regression including the LADAC family) -- `passed=1, plantSmokePassed=1`.
