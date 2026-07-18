# wheel_loader_simscape

Simscape adapter variant of `wheel_loader`. PX4 target `optimAeroLoader` (shares the
airframe with `wheel_loader`).

## Current physics (real, as of 2026-07-15)

Same finding as `ackermann_simscape`/`evtol_simscape`: this model was a clone of the
hexarotor's multirotor engine (wrong physics class for a steer+throttle machine) plus an
unconnected `simscape_adapter_diagnostics` placeholder network. It has been replaced with
a **direct copy of `wheel_loader.slx`'s real kinematic-bicycle plant** (see
`vehicle/wheel_loader/README.md` for the equations and parameters).

Parameters: identical to `wheel_loader` -- `dryMass_kg=4500`, `wheelBase_m=2.5`,
`maxSteerAngle_rad=0.5`, `maxSpeed_mps=4.0`, `tauSteer_s=0.4`, `tauThrottle_s=0.8`.

## Blocked future work

The external source project this adapter was meant to wrap
(`C:\AnelloSummer\new_vehicles\Wheel-Loader-Simscape`) does not exist on this machine --
confirmed by direct filesystem check. If that project becomes available, real
driveline/hydraulics Simscape physics should replace the kinematic plant above; candidate
source areas noted at the time (`Models\Vehicle\sm_wheel_loader_vehicle.slx`,
`Models\Vehicle\sm_wheel_loader_steer.slx`, `Models\Driveline\sm_wheel_loader_driveline.slx`,
CVT powertrain reference) are preserved here for whoever picks this up.

## Axis convention / gravity / units

Same as `wheel_loader`: NED world, FRD body, gravity sign not independently meaningful
for a ground vehicle at constant altitude, SI units throughout.

## Not modeled

Same gaps as `wheel_loader` (tire slip/suspension/hydraulics/bucket), plus the Simscape
swap-in above blocked on the missing source project.

## Validation evidence

`work/vehicle_test_results/vehicle_integration_matrix_20260715_040234.json` (final
17-vehicle regression, lote 2) -- `passed=1, plantSmokePassed=1`.
