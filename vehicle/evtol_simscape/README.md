# evtol_simscape

`evtol_simscape` is the first high-fidelity vehicle integration entry for the Simscape vehicle
plan. It deliberately remains separate from the fast `evtol` baseline.

Current implementation status:

- Registered as a separate vehicle key.
- Adds `evtol_simscape.slx` as a separate `VehiclePlant.slx` plant variant.
- The adapter model is cloned from the stable `evtol` plant, includes local copied component model
  dependencies, and contains a healthy `simscape_adapter_diagnostics` Simscape network.
- Uses the `optimAeroEVTOL` PX4 SITL target.
- Adds local metadata for the eVTOL Simscape source project.
- Adds explicit command/state adapter contract notes for the future physical plant.
- Provides smoke-test wrappers so the vehicle can be included in the integration matrix.

Source project:

```text
C:\AnelloSummer\new_vehicles\eVTOL-Drone-Simscape
```

Key source models selected for future adapter work:

| Source model | Intended local use |
|---|---|
| `Components\Vehicle\AirframePowertrain.slx` | Candidate powertrain/airframe subsystem |
| `Components\Vehicle\AirframePowertrainMotorSDL.slx` | Candidate lower-level motor/driveline variant |
| `Components\Motor\PropulsionMotor.slx` | Motor dynamics reference |
| `Components\Propeller\PropellerSimscape.slx` | Simscape propeller dynamics reference |
| `Components\Propeller\PropulsionSystem.slx` | Combined propulsion unit reference |
| `Components\Battery\BatteryPlantModel_Simple.slx` | Initial battery source for runtime-friendly model |
| `Model\EVTOLTiltrotor.slx` | Full example, reference only for first pass |

The next structural step is to replace the contained diagnostic Simscape network with a
command-coupled motor/propeller/powertrain subsystem while preserving the existing SIL bus
boundary.

## Adapter Boundary

Input side:

```text
ActuatorCommandBus.MotorsCommandBus
  -> normalized motor commands
  -> eVTOL motor speed / torque / thrust references
```

Output side:

```text
eVTOL Simscape/world states
  -> NED position and velocity
  -> FRD body velocity and body rates
  -> Euler attitude
  -> body acceleration
  -> VehicleBus
```

Frame conversions must be explicit. The eVTOL Simscape source uses its own harness buses with
fields such as `Vehicle_A_St.Xe`, `Ve`, `Euler`, `Vb`, `pqr`, and rotor parameters. Those must be
mapped to this repo's `VehicleBus` contract before PX4 loop-closure testing.

## Validation

Run:

```matlab
cd("C:\AnelloSummer\optimAeroPX4SIL")
addpath("utilities")
runVehicleIntegrationMatrix( ...
    "vehicleTypes", string("evtol_simscape"), ...
    "insVariants", [1 2], ...
    "runPlantSmoke", true)
```

This validation proves registry, initialization, adapter model selection, INS variant
compatibility, PX4 target registration, and one-second plant smoke execution.
