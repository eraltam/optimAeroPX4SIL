# tracked_vehicle_simscape

Simscape adapter variant of `tracked_vehicle`. PX4 target `optimAeroTracked` (shares the
airframe with `tracked_vehicle`).

## Current physics (real, as of 2026-07-15)

Same finding as the other `_simscape` adapters: this model was a clone of the
hexarotor's multirotor engine plus an unconnected `simscape_adapter_diagnostics`
placeholder network. It has been replaced with a **direct copy of
`tracked_vehicle.slx`'s real kinematic plant** (same `DifferentialKinematics` MATLAB
Function block and constants -- see `vehicle/tracked_vehicle/README.md`).

Parameters: identical to `tracked_vehicle` -- `dryMass_kg=2500`, `trackWidth_m=1.5`,
`wheelRadius_m=0.3`, `maxSpeed_mps=3.0`, `tauMotor_s=0.6`.

## Why this one is the most promising Simscape candidate

Unlike `ackermann_simscape`/`evtol_simscape`/`wheel_loader_simscape`, the Simscape source
this adapter was meant to wrap **does exist** in this checkout:
`Tracked-Vehicles-Simscape/Models/{Excavator_Tracks,Tractor_Tracks}` at the repo root.
(An earlier version of this README pointed at a stale/incorrect path,
`C:\AnelloSummer\new_vehicles\Tracked-Vehicles-Simscape` -- that path is wrong, not the
repo being missing.) A full bus-contract/frame-conversion integration against that repo
is still out of scope for this pass; see `SIMSCAPE_VEHICLE_INTEGRATION_PLAN.md` and
`PLAN_CORRECCION_MULTIVEHICULO_SITL.md` Fase 3 step 2 for the plan already written.

## Axis convention / gravity / units

Same as `tracked_vehicle`: NED world, FRD body, gravity sign not independently meaningful
for a ground vehicle at constant altitude, SI units throughout.

## Not modeled

Real track/terrain contact and slip (same gap as `tracked_vehicle`), blocked pending the
Simscape port above rather than a missing source project.

## Validation evidence

`work/vehicle_test_results/vehicle_integration_matrix_20260715_040234.json` (final
17-vehicle regression, lote 2) -- `passed=1, plantSmokePassed=1`.
