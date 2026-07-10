# wheel_loader_simscape

`wheel_loader_simscape` is a separate adapter entry for the wheel-loader Simscape source. It
provides `wheel_loader_simscape.slx` as a separate `VehiclePlant.slx` plant variant, cloned from
the stable wheel-loader plant with local copied component dependencies and a healthy
`simscape_adapter_diagnostics` Simscape network. It uses the `optimAeroLoader` PX4 target while
the command-coupled physical Simscape wrapper is staged.

Source project:

```text
C:\AnelloSummer\new_vehicles\Wheel-Loader-Simscape
```

Candidate source models:

| Source model | Intended local use |
|---|---|
| `Models\Vehicle\sm_wheel_loader_vehicle.slx` | Vehicle body/reference plant |
| `Models\Vehicle\sm_wheel_loader_steer.slx` | Steering/articulation reference |
| `Models\Driveline\sm_wheel_loader_driveline.slx` | Driveline reference |
| `Models\CVT\ssc_hydromech_power_split_cvt_engine.slx` | Powertrain reference, deferred |

First physical wrapper should use steering plus wheel torque on flat terrain. Bucket and hydraulic
functions should remain deferred until PX4 loop closure is stable.
