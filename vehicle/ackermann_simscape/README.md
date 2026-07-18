# ackermann_simscape

Simscape adapter variant of `ackermann_rover`. PX4 target `optimAeroAckermann` (shares the
airframe with `ackermann_rover`).

## Current physics (real, as of 2026-07-15)

`ackermann_simscape.slx` was originally a clone of the hexarotor's multirotor engine
(wrong physics class entirely for a steer+throttle car) plus an unconnected
`simscape_adapter_diagnostics` placeholder network. It has been replaced with a **direct
copy of `ackermann_rover.slx`'s real kinematic-bicycle plant** (same `AckermannKinematics`
MATLAB Function block, same equations -- see `vehicle/ackermann_rover/README.md`) and the
matching `PX4OutputMappingAckermannRover.slx`/`failureInputReadAckermannRover.slx`
mapping. Same car as `ackermann_rover`, pending a future tire/suspension Simscape
swap-in.

Parameters (`setUpVehicle.m`/`setUpActuators.m` case `"ackermann_simscape"`): identical
to `ackermann_rover` -- `dryMass_kg=60`, `wheelBase_m=2.0`, `maxSteerAngle_rad=0.6`,
`maxSpeed_mps=8.0`, `tauSteer_s=0.15`, `tauThrottle_s=0.5`.

## Blocked future work

The external source project this adapter was meant to wrap
(`C:\AnelloSummer\new_vehicles\Formula-Student-Vehicle-Simscape`) does not exist on this
machine -- confirmed by direct filesystem check, not a scope decision. If that project
becomes available, real tire/suspension Simscape physics should replace the kinematic
plant above; candidate source areas noted at the time (`Libraries\Event\Scene\Plane_Grid`,
`Libraries\Event\Scene\Skidpad`, Formula Student vehicle libraries for tire/suspension
reference) are preserved here for whoever picks this up.

## Axis convention / gravity / units

Same as `ackermann_rover`: NED world, FRD body, gravity sign not independently meaningful
for a ground vehicle at constant altitude, SI units throughout.

## Not modeled

Same gaps as `ackermann_rover` (no tire slip/suspension/traction forces), plus the
Simscape swap-in above blocked on the missing source project.

## Validation evidence

`work/vehicle_test_results/vehicle_integration_matrix_20260715_023036.json` --
`passed=1, plantSmokePassed=1`.
