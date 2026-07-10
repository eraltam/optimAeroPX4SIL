# tracked_vehicle_simscape

`tracked_vehicle_simscape` is a separate adapter entry for the tracked vehicle Simscape source.
It provides `tracked_vehicle_simscape.slx` as a separate `VehiclePlant.slx` plant variant, cloned
from the stable tracked vehicle plant with local copied component dependencies and a healthy
`simscape_adapter_diagnostics` Simscape network. It uses the `optimAeroTracked` PX4 target while
documenting the physical-model source and adapter contract.

Source project:

```text
C:\AnelloSummer\new_vehicles\Tracked-Vehicles-Simscape
```

Candidate source models:

| Source model | Intended local use |
|---|---|
| `Libraries\Belt\sm_trackV_lib_belt_track_s45.slx` | Belt-track dynamics reference |
| `Libraries\Chain\sm_trackV_lib_chain_track_s46.slx` | Chain-track dynamics reference |
| `Libraries\Scene\sm_trackV_lib_terrain_contact_s46.slx` | Terrain/contact reference |
| `Libraries\Chassis\sm_excv_chassis_lib.slx` | Chassis/reference geometry |
| `Models\Excavator_Tracks\*.slx` | Full examples, reference only for first pass |

First physical wrapper should use a reduced left/right track force model before full contact.
