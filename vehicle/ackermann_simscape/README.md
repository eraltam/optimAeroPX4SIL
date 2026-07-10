# ackermann_simscape

`ackermann_simscape` is a separate adapter entry for automotive/Ackermann Simscape sources. It
provides `ackermann_simscape.slx` as a separate `VehiclePlant.slx` plant variant, cloned from the
stable Ackermann rover plant with local copied component dependencies and a healthy
`simscape_adapter_diagnostics` Simscape network. It uses the `optimAeroAckermann` PX4 target.

Source project:

```text
C:\AnelloSummer\new_vehicles\Formula-Student-Vehicle-Simscape
```

Candidate source areas:

| Source area | Intended local use |
|---|---|
| `Libraries\Event\Scene\Plane_Grid` | Flat-scene setup |
| `Libraries\Event\Scene\Skidpad` | Low-speed closed-course reference |
| Formula Student vehicle libraries | Tire, suspension, and vehicle dynamics reference |
| `Optimize-Vehicle-Design-with-AI-and-Simscape` | Secondary parameter and workflow source |

First physical wrapper should use steering angle plus drive torque at low speed before adding
detailed tire/suspension effects.
