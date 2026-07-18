# evtol_simscape

Simscape adapter variant of `evtol`. PX4 target `optimAeroEVTOL` (shares the airframe
with `evtol`).

## Current physics (real, as of 2026-07-15)

`evtol_simscape.slx` was a clone of `evtol.slx` (same hexarotor-engine reuse) plus an
unconnected `simscape_adapter_diagnostics` placeholder subsystem (confirmed via
`model_overview`: it does not appear in any signal connection). It now has the **same
real rigid-body physics as `evtol`** -- own `case` in `setUpVehicle.m`/
`setUpActuators.m` with identical geometry/mass (same airframe pending a future
powertrain swap-in), and its own `PX4OutputMappingEvtolSimscape.slx`/
`failureInputReadEvtolSimscape.slx` (renamed copies of `evtol`'s, same 6 motors).

Parameters: identical to `evtol` -- `dryMass_kg=45`, `aircraftInertialBody_kgm2=diag(6.5,
6.5, 13)`, `SRef_m2=0.5`.

## Blocked future work

The "high-fidelity" motor/propeller/powertrain Simscape physics that gives this vehicle
its name is still missing -- the external source project it was meant to wrap
(`C:\AnelloSummer\new_vehicles\eVTOL-Drone-Simscape`) does not exist on this machine,
confirmed by direct filesystem check. Candidate source models noted at the time
(`Components\Vehicle\AirframePowertrain.slx`, `Components\Motor\PropulsionMotor.slx`,
`Components\Propeller\PropellerSimscape.slx`, `Components\Battery\
BatteryPlantModel_Simple.slx`) are preserved here for whoever picks this up. If that
project appears, the adapter boundary should stay: `ActuatorCommandBus.MotorsCommandBus`
in (normalized motor commands), `VehicleBus` out (NED position/velocity, FRD body
velocity/rates, Euler attitude, body acceleration) -- frame conversions between the
Simscape source's own harness buses (`Vehicle_A_St.Xe`, `Ve`, `Euler`, `Vb`, `pqr`) and
this repo's `VehicleBus` must be explicit.

## Axis convention / gravity / units

Same as `evtol`: NED world, FRD body, standard gravity chain, SI units.

## Not modeled

Same tiltrotor/pusher-prop gap as `evtol`, plus the real motor/propeller/powertrain
Simscape physics blocked on the missing source project above.

## Validation evidence

`work/vehicle_test_results/vehicle_integration_matrix_20260715_022324.json` -- 11-vehicle
regression, `passed=1, plantSmokePassed=1`.
