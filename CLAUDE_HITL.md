# CLAUDE.md - optimAeroPX4SIL Dual-Mode SITL/HITL Controller Plan

**Status:** Phase 1 implemented, compiles cleanly end-to-end for both `CONTROLLER_RUNTIME` values
via the real `initVehicleSIL` entry point, and now actually **runs** (not just compiles) without
hanging -- the phantom-SITL-connection-during-HITL issue is fixed (section 18.7). **Not yet run
against real PX4 hardware.** See section 18 ("Implementation results") for what's built, what's
verified, and what remains.
**Date:** June 30, 2026
**Primary goal:** Keep the current Simulink + PX4 SITL workflow working, but add a second path where the same Simulink plant runs against a **real PX4 controller on Cube Orange Plus** for controller-only HITL.

This file is the project brief for future Claude/Codex sessions working on HITL support in this repo.
Section 18 at the end has the implementation status; sections 1-17 are the original plan (left
intact, with inline corrections from section 7.1 onward, as a record of how the plan evolved).

## 1. What must be achieved

The repo already works as a Software-In-The-Loop system:

- Simulink runs the vehicle plant, environment, sensors, and visualization.
- PX4 runs as a POSIX SITL executable launched with commands such as:
  - `make px4_sitl_default optimAeroF16`
  - `make px4_sitl_default optimAeroHex`
- `initVehicleSIL.m` initializes the workspace and can launch the full SIL flow.

The new objective is to support **both** of these modes:

1. **SITL mode**  
   Keep the existing behavior unchanged.

2. **HITL mode, Phase 1**  
   Keep the Simulink plant and simulated sensors, but replace the simulated PX4 controller with a **real PX4 autopilot running on Cube Orange Plus**.

For Phase 1, the hardware PX4 should receive simulated sensor data from Simulink and return actuator commands back to Simulink. The plant stays in MATLAB/Simulink. The controller moves to the real flight controller.

## 2. Scope boundaries

These constraints are mandatory:

- Do **not** remove or break the existing SITL flow.
- Do **not** fully replace `initVehicleSIL`; extend it so the project can run in either SITL or HITL.
- Do **not** make Stage 2 changes yet.
- **Phase 1 only:** real PX4 hardware replaces the controller, but sensors still come from the Simulink simulation.
- **Stage 2 is deferred:** later, the repo may use sensors physically mounted on the PX4 hardware, but that is out of scope for this file.

## 3. Recommended user-facing behavior

The repo should expose an explicit mode selection in initialization, instead of forcing users to manually edit internals.

Recommended shape:

```matlab
initVehicleSIL( ...
    "launchFullSIL", false, ...
    "vehicleType", "hexarotor", ...
    "controllerRuntime", "SITL")
```

and

```matlab
initVehicleSIL( ...
    "launchFullSIL", false, ...
    "vehicleType", "hexarotor", ...
    "controllerRuntime", "HITL", ...
    "hardwareTarget", "CubeOrangePlus")
```

The exact argument names may change, but the final API must make the runtime mode obvious.

## 4. Current architecture in this repo

From `README.md` and `initVehicleSIL.m`, the current architecture is:

```text
VehicleSilSimulation.slx
  -> VehiclePlant.slx
  -> environment/*
  -> sensors/*
  -> PX4 interface / SIL connector
  -> visualization

initVehicleSIL.m
  -> loads buses, sensors, actuators, environment, vehicle params
  -> sets variant behavior
  -> optionally launches PX4 SITL in WSL
```

Important current facts:

- `initVehicleSIL.m` defaults to `vehicleType = "hexarotor"` and `controllerType = "PX4"`.
- SITL launches through WSL and calls:
  - `make px4_sitl_default optimAeroF16`
  - `make px4_sitl_default optimAeroHex`
- The POSIX launch helpers are defined in:
  - `PX4-Autopilot/src/modules/simulation/simulator_mavlink/CMakeLists.txt`
- The custom SITL airframes currently present are:
  - `ROMFS/px4fmu_common/init.d-posix/airframes/10020_optimAeroF16`
  - `ROMFS/px4fmu_common/init.d-posix/airframes/10021_optimAeroHex`

This means the current custom flow is explicitly POSIX/SITL-oriented.

## 5. Key HITL insight from the local PX4 tree

The local PX4 checkout already contains the pieces needed on the PX4 side for HIL messaging:

- `src/modules/mavlink/mavlink_receiver.cpp`
  - handles `HIL_SENSOR`
  - handles `HIL_GPS`
  - handles `HIL_STATE_QUATERNION`
  - updates PX4 gyro/accel/mag topics from those messages
- `src/modules/mavlink/streams/HIL_ACTUATOR_CONTROLS.hpp`
  - streams actuator commands from PX4 back to the simulator
- `src/modules/mavlink/mavlink_main.cpp`
  - enables HIL stream behavior when HIL is active

This is the main reason the repo can support controller-only HITL without rewriting PX4 estimator/controller logic.

## 6. Hardware target note

For Cube Orange Plus, the local PX4 tree includes:

- `boards/cubepilot/cubeorangeplus/`

So the hardware firmware target is expected to be based on:

```text
cubepilot_cubeorangeplus_default
```

That is a **hardware build/flash target**, not a SITL target.

Important distinction:

- Current SITL command:
  - `make px4_sitl_default optimAeroHex`
- Expected hardware build family:
  - `make cubepilot_cubeorangeplus_default`

Do not try to reuse the existing `optimAeroHex` POSIX launch helper as the hardware flashing strategy. HITL needs a separate hardware-oriented path.

## 7. Reference project to study first

Use this local project as the primary reference for controller-only HITL behavior:

`C:\Users\Usuario\AnelloSummer\uav_HITL_sample`

Most relevant files (paths corrected -- the models live under `VTOLApp\models\`, not directly in
`VTOLApp\`):

- `uav_HITL_sample/TuneControlDesignForUAVInHoverExample/VTOLApp/steps/setupHITLConfiguration.m`
- `uav_HITL_sample/TuneControlDesignForUAVInHoverExample/VTOLApp/steps/setupHITLController.m`
- `uav_HITL_sample/TuneControlDesignForUAVInHoverExample/VTOLApp/steps/openHITLController.m`
- `uav_HITL_sample/TuneControlDesignForUAVInHoverExample/VTOLApp/models/HITL_Controller_top.slx`
- `uav_HITL_sample/TuneControlDesignForUAVInHoverExample/VTOLApp/models/HITL_Plant_top.slx`
- `uav_HITL_sample/TuneControlDesignForUAVInHoverExample/VTOLApp/codegen_HITL_clean/codegen/HITL_Controller_top_ert_rtw/*`

Why this sample matters:

- it shows a PX4-compatible HITL controller path,
- it shows how MathWorks structures HIL setup scripts,
- it shows generated controller code that already includes PX4/uORB topic headers,
- it is the best local example for how to send simulated sensor data to PX4 and receive commands back.

### 7.1 Verified: this sample answers most of section 12's open questions

Inspected `HITL_Plant_top.slx` directly (`model_overview`/`model_read`, not guessing). Findings:

- The plant-side HIL packing/unpacking is built entirely from a MathWorks library called
  **`px4MAVLinkBridgelib`**, part of the **UAV Toolbox Support Package for PX4 Autopilots** (already
  installed in this MATLAB environment -- confirmed via `matlab.addons.installedAddons`, version
  26.1.0, enabled). Key blocks and their annotations (taken verbatim from the model):
  - **`MAVLink Bridge Source`**: *"Reads MAVLink messages from the PX4 hardware connected on
    **serial** and parses the MAVLink data stream as array."*
  - **`MAVLink Bridge Sink`**: *"Reads MAVLink data and sends it to PX4 hardware via **serial
    connection**."*
  - `HIL_SENSOR Creation subsystem`, `HIL_GPS Subsystem`, `Heartbeat_Serialization` build the raw
    MAVLink byte streams from plant truth states, all under `VTOL Plant > Send Plant Feedback`.
  - On the return path: `Read actuator output` + `Split actuator command` parse the incoming MAVLink
    byte stream from `MAVLink Bridge Source` back into `MotorCommands`/`CtrlSurfaceCommands`.
- **This answers section 12, question 1 directly: the transport is serial MAVLink**, via
  `px4MAVLinkBridgelib`'s Source/Sink blocks -- not UDP, and not a hand-rolled S-function.
- **This answers section 12, question 2: the pack/unpack lives inside the Simulink plant model
  itself**, as a sibling subsystem pattern to today's `VehicleSilSimulation.slx` SIL connector (not
  inside `PX4SILConnector`, and not as a new standalone connector -- reuse the library's blocks).
- **Caveat found while checking this:** `px4MAVLinkBridgelib` did **not** resolve in a fresh
  `evaluate_matlab_code` session here (`exist('px4MAVLinkBridgelib','file')` returned `0`, and
  opening the model threw `Cannot find library called 'px4MAVLinkBridgelib'`) even though the
  support package shows as installed/enabled. **First implementation-session action item:** confirm
  the library actually resolves (`which px4MAVLinkBridgelib`, or browse it in the Simulink Library
  Browser) before building anything on top of it -- this repo's own `initVehicleSIL.m` calls
  `restoredefaultpath` on every run, which is already known (see `optimAeroPX4SIL/CLAUDE.md` section
  7) to strip custom/support-package toolbox paths in this environment, so this may just need a path
  re-registration, not a reinstall.
- Update rate used by the reference example, for context against section 12 question 6: plant
  `SampleTime = 0.02` (50 Hz), `gpsFreq = 25` Hz, `heartbeatFreq = 1` Hz
  (`setupHITLConfiguration.m`). This repo's SIL currently runs at `stepSize_s = 0.004` (250 Hz) --
  a real rate mismatch to resolve deliberately (probably run the HIL link slower than the base
  plant step, not force the whole plant down to 50 Hz), not copy blindly.

## 8. Desired end-state architecture

Phase 1 target architecture:

```text
QGroundControl
  <-> MAVLink telemetry
PX4 on Cube Orange Plus
  <-> HIL MAVLink sensor/actuator exchange
Simulink VehicleSilSimulation.slx
  -> plant dynamics
  -> simulated sensors
  -> HIL message pack/unpack layer
```

Data flow:

```text
VehiclePlant truth states
  -> sensor models in Simulink
  -> HIL_SENSOR / HIL_GPS / related messages
  -> PX4 on Cube Orange Plus
  -> estimator + controller on hardware
  -> HIL_ACTUATOR_CONTROLS or equivalent outputs
  -> Simulink actuator mapping
  -> VehiclePlant
```

The user should see nearly the same closed-loop behavior as current SITL, except the controller is now running on the real flight controller.

## 9. Implementation strategy

Preferred strategy:

1. Keep the existing SITL connector path intact.
2. Add a second connector/runtime path for HITL.
3. Make runtime selection happen in `initVehicleSIL.m`.
4. Use model variants or a clean configuration switch rather than manual block rewiring every run.

Recommended structure:

- Existing path:
  - `controllerRuntime = "SITL"`
  - use current PX4 SITL launch path
  - use current SIL connector behavior

- New path:
  - `controllerRuntime = "HITL"`
  - do not launch WSL SITL
  - initialize hardware/HIL comms instead
  - send sensor MAVLink to PX4 hardware
  - receive actuator MAVLink from PX4 hardware

## 10. Files most likely to change

- `initVehicleSIL.m`
  - add runtime mode selection
  - separate SITL launch logic from HITL setup logic
  - add hardware-specific config inputs

- `VehicleSilSimulation.slx`
  - add or refactor interface variant for SITL vs HITL controller path

- `PX4SILConnector/*`
  - **Verified, not just "to inspect": cannot be reused for HITL.** Read `SILConnector.h` and
    `pixhawk_sil_connector.cpp` directly. The connector is a C++ S-function wrapping an `asio` TCP
    **server** hardcoded to listen on `0.0.0.0:4560` (`mexPrintf("Waiting for PX4 to connect on TCP
    port 4560...")`) and `accept()`-blocks for an incoming connection. This is the exact protocol
    PX4's POSIX SITL `simulator_mavlink` module uses when it runs as a TCP **client** connecting back
    to the simulator -- it does not exist on regular flight-controller firmware. Cube Orange Plus
    running normal (non-SITL) PX4 firmware talks MAVLink over its physical UART/USB ports via the
    standard `mavlink` module, not `simulator_mavlink`, so it will never dial out to
    `tcp://localhost:4560`. **Build a new HITL connector using `px4MAVLinkBridgelib`'s serial
    Source/Sink blocks (section 7.1) instead of trying to retarget this one.**

- `README.md`
  - document the new initialization mode and hardware flashing flow

- PX4 config/support files
  - only if needed for custom actuator mapping or startup behavior on hardware

## 11. Things to avoid

- Do not hardcode HITL assumptions into the existing SITL-only path.
- Do not remove the `optimAeroF16` / `optimAeroHex` SITL launch helpers.
- Do not assume the POSIX airframe helper files are sufficient for the hardware flow.
- Do not jump to Stage 2 sensor-on-hardware work.
- Do not make the user manually toggle many blocks in Simulink if a mode argument can handle it.

## 12. Open technical questions to resolve during implementation

These questions must be answered by inspecting the sample project and local models, not by guessing:

1. ~~What exact transport is best for Phase 1 HITL in this repo?~~ **ANSWERED (section 7.1):
   serial MAVLink, via `px4MAVLinkBridgelib`'s `MAVLink Bridge Source`/`MAVLink Bridge Sink` blocks
   (UAV Toolbox Support Package for PX4 Autopilots, already installed).** Not UDP, not the existing
   `PX4SILConnector` (that one is TCP:4560, SITL-only -- see section 10).

2. ~~Where should the HITL pack/unpack live?~~ **ANSWERED (section 7.1): inside the Simulink plant
   model**, as a sibling subsystem to today's SIL connector wiring in `VehicleSilSimulation.slx` --
   built from `px4MAVLinkBridgelib` blocks (`HIL_SENSOR Creation subsystem`, `HIL_GPS Subsystem`,
   `Heartbeat_Serialization`, `Read actuator output`), not a new from-scratch S-function and not
   bolted onto `PX4SILConnector`.

3. Which messages are sufficient for Phase 1? **Largely answered (section 7.1) by what
   `uav_HITL_sample` actually sends:** `HIL_SENSOR`, `HIL_GPS`, `HEARTBEAT`, plus a custom "Rotor
   parameter" message (not a standard HIL message -- worth understanding what it carries before
   assuming it's required; this repo's hexarotor/F-16 plants may not need an equivalent). No
   `HIL_STATE_QUATERNION` block was found in the reference plant -- drop it from the Phase 1 list
   unless something in this repo specifically needs it. Actuator return path confirmed: parsed via
   `Read actuator output` + `Split actuator command` from the `MAVLink Bridge Source`'s raw byte
   stream.

4. How should the actuator mapping be reused from current SITL?
   - ideally the plant/actuator mapping stays identical between SITL and HITL

5. What hardware-side startup or parameter loading is required?
   - firmware target
   - flash process
   - autostart / airframe / mixer / params
   - HIL enablement parameters

6. What update rate should the HIL link run at?
   - current Simulink fixed step is `0.004 s`
   - do not silently degrade timing without validating controller behavior

## 13. Expected phases

### Phase 1 - Controller-only HITL

Deliver this first:

- mode-selectable SITL vs HITL init path,
- Simulink simulated sensors to PX4 hardware,
- PX4 hardware actuator outputs back to Simulink,
- same plant and same vehicle behavior as close as practical.

### Phase 2 - Real onboard sensors on PX4

Do not implement yet, but keep the design compatible with it:

- PX4 hardware sensors may replace some or all simulated sensor feeds later,
- this will require a different architecture than Phase 1,
- do not let Phase 2 assumptions distort the Phase 1 implementation.

## 14. Validation checklist

HITL Phase 1 is not complete until these are checked:

- `initVehicleSIL` can still run the old SITL path.
- A new explicit HITL path exists.
- The Cube Orange Plus firmware can be built/flashed with the correct hardware target.
- QGroundControl can connect during HITL runs.
- PX4 shows that HIL sensor data is being consumed.
- Simulink receives actuator commands back from PX4 hardware.
- The vehicle response is qualitatively similar to current SITL for the same mission/setup.
- SITL regression is acceptable after the changes.

## 15. Useful MATLAB toolkits/plugins for this work

The user reported these MATLAB-oriented plugins are already installed and relevant:

- `aerospace@matlab-agentic-toolkits`
- `model-based-design-core@matlab-agentic-toolkits`
- `robotics-and-autonomous-systems@matlab-agentic-toolkits`
- `matlab-core@matlab-agentic-toolkits`
- `test-and-measurement@matlab-agentic-toolkits`

When working in MATLAB/Simulink, prefer those capabilities for:

- model inspection,
- interface tracing,
- variant management,
- test harness creation,
- HIL comms inspection.

## 16. Minimum deliverables for the first implementation session

If asked to implement this plan, the first coding session should at minimum produce:

1. A runtime switch in `initVehicleSIL.m` for SITL vs HITL.
2. A documented HITL initialization branch.
3. A clear separation between SITL launch and hardware/HIL setup.
4. A documented hardware build target path for Cube Orange Plus.
5. A short README update describing how the user chooses SITL or HITL.

## 17. Summary for future agents

The mission is **not** to replace the repo's SITL system. The mission is to make the repo dual-mode:

- same Simulink plant,
- same vehicle model,
- same user workflow,
- but with a selectable controller backend:
  - PX4 SITL in WSL, or
  - PX4 on real Cube Orange Plus hardware.

For now, only move the controller to hardware. Leave plant and sensors in Simulink.

---

## 18. Implementation results (this session)

### 18.1 What changed

| File | Status | Role |
|---|---|---|
| `initVehicleSIL.m` | modified | Added `controllerRuntime` ("SITL"\|"HITL"), `hardwareTarget` ("CubeOrangePlus"), `hitlSerialPort`, `hitlBaudRate`, `hitlQGCPort`. SITL path unchanged when `controllerRuntime` omitted. Sets `CONTROLLER_RUNTIME` (1 or 2) in the base workspace and calls `setUpHITLConnection` unconditionally (see 18.3). |
| `PX4HITLConnector/setUpHITLConnection.m` | new | Locates `px4MAVLinkBridgelib` on disk (it does **not** reliably resolve just because the support package shows installed -- see 18.2) and pushes HITL link parameters (`hitlParams.*`, `SampleTime`, `gpsFreq`, `heartbeatFreq`, `MAVLink_Input_Read_Size`) into the base workspace. |
| `PX4HITLConnector/build_PX4HITLConnector.m` | new | Programmatic builder for `PX4HITLConnector.slx` (Simulink API script, same pattern as `newIMU_model`'s `build_ANELLO_X3_IMU.m`). Re-run if the model needs rebuilding. |
| `PX4HITLConnector/PX4HITLConnector.slx` | new | The HITL connector itself. Interface matches the existing "PX4 Interface" (SITL) exactly: in `SensorsBus`/`VehicleBus`/`EnvironmentBus`, out `ActuatorCommandBus`. Compiles standalone. |
| `PX4HITLConnector/wire_px4_hitl_selector.m` | new | Wires `PX4HITLConnector.slx`'s contents into `VehicleSilSimulation.slx` as an Enable-gated alternative to "PX4 Interface", selected by `CONTROLLER_RUNTIME`. Re-run if `VehicleSilSimulation.slx` needs rebuilding (only after `build_PX4HITLConnector.m`). |
| `VehicleSilSimulation.slx` | modified | Added "PX4 HITL Interface" (inlined `PX4HITLConnector` contents, Enable-gated on `CONTROLLER_RUNTIME==2`) and `PX4_Connector_Select` (Multiport Switch) choosing between it and the existing "PX4 Interface" output. |
| `README.md` | modified | New "HITL (Hardware-in-the-Loop) Mode" section: prerequisites, flash command, run command, known limitation. Updated required-toolboxes list. |

### 18.2 Verified, not guessed: `px4MAVLinkBridgelib` configuration

Everything below was confirmed by directly reading the UAV Toolbox Support Package for PX4
Autopilots' source (`px4.internal.block.MAVLinkSource`/`MAVLinkSink`/`MAVLinkBridgeBase`, found
under `C:\ProgramData\MATLAB\SupportPackages\<release>\toolbox\target\supportpackages\px4\core\`)
and by reading `uav_HITL_sample`'s actual model internals -- not inferred from documentation alone.

- **Path gotcha (real, not hypothetical):** `px4MAVLinkBridgelib` does not resolve in a fresh MATLAB
  session just because `matlab.addons.installedAddons` lists the support package as installed. Its
  library (`px4MAVLinkBridgelib.slx`) and the `+px4` namespace package containing the actual System
  object classes (`MAVLinkSource.m`/`.p`, `MAVLinkSink.m`/`.p`, `MAVLinkBridgeBase.m`/`.p`) live
  under `SupportPackages\<release>\toolbox\target\supportpackages\px4\core\` (and `\core\blocks\`
  for the .slx) -- neither is on the MATLAB path by default. `setUpHITLConnection.m` searches for
  this directory (globbing the release folder name, not hardcoding it) rather than assuming it's
  already resolvable.
- **`initVehicleSIL.m`'s own `restoredefaultpath` strips this path on every call** -- same root
  cause as the `model_check`/`model_overview` MCP tool breakage documented in
  `optimAeroPX4SIL/CLAUDE.md` section 7. `setUpHITLConnection` must run (and therefore
  re-add the path) on every `initVehicleSIL` call, not just once per MATLAB session.
- **Real mask parameter names** (not the System object's raw property names, which is what's
  guessable from reading `MAVLinkBridgeBase.m`): the MAVLink Bridge Source/Sink blocks' actual
  Simulink mask exposes `PixhawkSerialPorttemp` ("Specify manually" vs. an auto-detect mode),
  `PixhawkSerialPortManually` (the actual COM port string), and `FieldTable` (a combined
  port-type/IP/port-number table for relaying the stream to QGroundControl/a companion computer
  over UDP or TCP) -- not `PixhawkSerialPort`/`RemotePortType`/`RemoteIpAddress`/`RemotePortNumber`
  directly, which is what the underlying System object's property names would suggest. Confirmed by
  reading the *instantiated* block's `MaskNames`/`MaskValues` (the abstract library block's
  `MaskNames` came back empty when queried directly, which is what led to the wrong guess
  initially).
- **Baud rate: 921600 on Windows** (230400 on Linux, set automatically by the block itself based on
  `ispc`). Per the support package's own source comment: *"Used 921600 as baud-rate as it is also
  used by jMAVSim for HITL mode in PX4."* Not user-configurable through the mask -- the
  `hitlBaudRate` argument in `initVehicleSIL.m` is informational only (kept visible in case
  MathWorks changes this in a future release).
- **QGroundControl relay is automatic:** the Source/Sink blocks can simultaneously relay the raw
  MAVLink byte stream to a UDP/TCP endpoint while talking to the board over serial -- with special-
  cased local ports 14570 (QGC) and 14580 ("OBC"/companion computer) baked into the support
  package's connection-sharing logic. This repo's connector uses port 14550 (QGC's standard MAVLink
  port) for that relay, configured via the `FieldTable` mask parameter.
- **`HIL_ACTUATOR_CONTROLS` confirmed as the actuator readback message** by reading
  `pixhawk_sil_connector.cpp` directly: its raw 16-element actuator output (`PX4_Output_Commands`,
  what `PX4OutputMappingHex.slx` already consumes) is literally
  `hil_actuator_controls[0..15]` copied straight through (`pwm[i] = hil_actuator_controls[i];`).
  This is the same standard MAVLink message `uav_HITL_sample`'s own `MAVLink Deserializer` block is
  configured for (`messageType = 'HIL_ACTUATOR_CONTROLS'`, dialect `common.xml`). That match is what
  let this session reuse `PX4OutputMappingHex.slx` -- this repo's own, already-validated hexarotor
  actuator mapping -- instead of inventing a new one.
- **Sensor units verified by reading the reference subsystems' internal Gain blocks**, not assumed:
  Acc `[m/s^2]`, Gyro `[rad/s]`, Pressure/DiffPressure `[Pa]` (the subsystem itself converts to hPa
  internally), LLA `[deg, deg, m]`, Velocity (NED) `[m/s]`, GndSpeed `[m/s]`, Course `[deg]` -- all
  match this repo's `AccelSensorBus`/`GyroSensorBus`/`BaroSensorBus`/`DiffPressureSensorBus`/
  `GPSSensorBus` fields directly, **except Mag**: the subsystem wants **microtesla**, but
  `MagSensorBus` is in **Gauss** -- `PX4HITLConnector.slx` applies a x100 conversion
  (1 Gauss = 100 uT) that isn't needed for any of the other signals.

### 18.3 Architecture decisions forced by what was discovered mid-implementation

These are real constraints found by trying to build the thing, not foreseeable from the plan alone:

1. **Not a true Variant Subsystem.** Attempted first (per the original plan's intent). This
   MATLAB release's Variant Subsystem block requires every variant choice's port set to match the
   parent exactly; mismatches are fixed via a GUI "synchronize ports" one-click action with no
   discoverable scriptable equivalent (`Simulink.SubSystem.convertToVariant` does not exist in this
   release; nothing else in `methods('Simulink.SubSystem')`/`methods('Simulink.VariantManager')`
   does the job either). Fell back to a Multiport Switch (same pattern already used for
   `INS_VARIANT` in the ANELLO X3 sensor integration -- see `CLAUDE.md`).
2. **Inlined, not Model-Referenced.** `px4MAVLinkBridgelib`'s blocks declare
   `getSimulateUsingImpl = "Interpreted execution"` only (in `MAVLinkSource.m`). This is
   incompatible with being inside a Model Reference block *at all*, even with
   `SimulationMode="Normal"` -- confirmed by a direct compile error ("the
   `px4.internal.block.MAVLinkBridgeBase` class does not support code generation"), since Model
   blocks still attempt a sim-target build even in Normal mode. Fixed by inlining
   `PX4HITLConnector`'s contents directly into `VehicleSilSimulation.slx` via
   `Simulink.BlockDiagram.copyContentsToSubSystem`, matching how the existing "PX4 Interface"
   (built around the custom `pixhawk_sil_connector` S-Function) is *also* a plain Subsystem rather
   than a Model Reference -- this turned out to be established precedent in this repo for exactly
   this kind of reason, not an inconsistency.
3. **The HITL side needed an Enable port; this is NOT the same situation as `INS_VARIANT`.** A
   Multiport Switch computes *both* branches every step and only switches the output -- harmless
   for `INS_VARIANT` (pure math, no side effects either way), but `px4MAVLinkBridgelib`'s blocks
   *open a real serial port* on their first step. Without gating, a pure-SITL user would get a
   spurious serial-port error despite never asking for HITL. Fixed by wrapping "PX4 HITL Interface"
   in an Enabled Subsystem gated on `CONTROLLER_RUNTIME==2`. This works because MATLABSystem blocks
   connect *lazily on first `step()`* (confirmed by reading `MAVLinkSource.m`'s `setupImpl`), and
   Enabled Subsystem content simply never steps while disabled -- verified both ways compile and
   that disabling genuinely skips execution (not just output selection).
4. **Required support package became unconditional**, not HITL-only. Because of (1) -- both
   connectors always present in the compiled model -- `px4MAVLinkBridgelib` must resolve for *any*
   compile of `VehicleSilSimulation.slx`, SITL included. `requiredSupportPackages` and the
   `setUpHITLConnection` path-setup call in `initVehicleSIL.m` were changed from
   HITL-conditional to unconditional accordingly.
5. **Internal `SampleTime` had to move off the reference project's default.** `uav_HITL_sample`
   defaults the HIL link to `SampleTime = 0.01` (100 Hz). This repo's fixed-step solver requires
   every sample time in the model to be an integer multiple of `stepSize_s = 0.004` (250 Hz); 0.01s
   isn't. Changed to `0.008s` (125 Hz, 2x the base step) in `setUpHITLConnection.m`.
6. **Several named workspace-variable dependencies had to be made local.** Blocks copied verbatim
   from `uav_HITL_sample` reference workspace variables defined by *that* project's own setup
   scripts (`uT_to_gauss`, `m_to_mm`, `ms_to_cms`, `gpsFreq`, `heartbeatFreq`,
   `MAVLink_Input_Read_Size`). Rather than depending on that sibling project's data files at
   runtime, the unit-conversion constants were hardcoded as literals directly on the copied Gain
   blocks, and the remaining rate/size variables are defined in `setUpHITLConnection.m`.
7. **`DataStoreMemory` declarations live one level up from where they're used.** The copied
   "Compute Timestamp"/"HIL_GPS"/"Send Heartbeat" subsystems read/write named data stores
   ("Timestamp", "Heartbeat", "GPS") declared as *sibling* blocks in the reference project's
   hierarchy, not inside the subsystems themselves -- had to copy those three `DataStoreMemory`
   blocks too, or the data store names don't resolve.

### 18.4 Deliberate gaps (documented, not oversights)

- **`controllerArmed` is hardcoded `false`.** The MAVLink Deserializer's `status` output (which
  might indicate "new message received") has no documentation available in this session, and
  `uav_HITL_sample`'s own reference implementation just terminates it unused. Guessing at its
  polarity risks getting it backwards -- for an *armed* flag, guessing wrong fails **unsafe** (stuck
  armed) rather than safe. Hardcoded `false` until someone with the MAVLink
  `HIL_ACTUATOR_CONTROLS`/deserializer-status spec, or real hardware to test against, verifies the
  right condition.
- **No "Rotor parameter" message.** `uav_HITL_sample`'s VTOL-specific custom MAVLink message
  (not a standard one) was dropped rather than faked with zeros -- PX4 wouldn't recognize it anyway
  (unknown message IDs are simply ignored by MAVLink-compliant receivers), and there's no reason to
  carry VTOL-specific baggage into a hexarotor/F-16 connector.
- **GPS quality fields (eph/epv/satellite count) are the reference project's hardcoded defaults**
  (0.3, 0.4, 10) inside the copied `HIL_GPS Subsystem`, not wired from this repo's own
  `GPSSensorBus.eph_m`/`epv_m`/`satellites_used_nd`. Low-risk to wire up properly later; left alone
  to minimize changes to a proven, copied subsystem.
- **`EngineCommandBus` is zero**, matching the existing SITL "PX4 Interface" hexarotor path's own
  default (`Constant=0`) -- not a new gap introduced here.

### 18.5 What's still missing before HITL is usable end-to-end

1. ~~The existing SITL connector is not gated, HITL runs hang waiting for a phantom SITL
   connection~~ **RESOLVED -- see section 18.7.** `pixhawk_sil_connector.cpp`'s blocking connect
   was moved from `mdlStart` to a lazy first-`mdlOutputs` connect, and "PX4 Interface" is now
   Enable-gated on `CONTROLLER_RUNTIME==1` exactly like "PX4 HITL Interface" is on `==2`. Verified
   empirically, not just by compiling: ran the model with `CONTROLLER_RUNTIME=2` and it failed
   after ~20s trying to open the (nonexistent, no real hardware attached) `COM5` port -- no
   `"Waiting for PX4 to connect on TCP port 4560..."` was ever printed.
2. **No simulation has actually been run against anything real** -- only against nonexistent
   hardware/connections (which correctly fail fast now, per item 1, rather than hang). Compiling
   and running clean up to the point of "can't find the hardware" means the model is internally
   consistent and all blocks can resolve their types/sizes/side effects at the right time; it does
   **not** mean the data flowing through it is correct (e.g. the gravity-sign/frame-convention
   question already flagged for the ANELLO X3 sensor work in `CLAUDE.md` applies equally here, since
   this connector consumes the same `SensorsBus`). This is now the single most important remaining
   item.
3. **Never connected to real PX4 hardware.** Everything in section 18.2's "verified" claims is
   verified against MathWorks' source code and the reference example's structure -- not against an
   actual Cube Orange Plus. The serial port name/COM port enumeration, `SYS_HITL` parameter
   behavior, and whether QGroundControl actually shows live telemetry over the 14550 relay are all
   unverified.
4. **`controllerArmed` polarity** (section 18.4) needs resolving with the MAVLink spec or hardware
   in hand before this could ever safely drive real actuators.

### 18.7 Fixing the phantom-SITL-connection hang (follow-up session)

The user's question was direct: "wouldn't an Enable fix this?" Answer: yes for the HITL side (see
section 18.3), but not for the SITL side by itself -- confirmed by reading
`PX4SILConnector/pixhawk_sil_connector.cpp` directly. Its blocking `m_acceptor.accept(...)` call
was in `mdlStart` (line ~122), which Simulink calls once at simulation start **for every block
instance present in the compiled diagram**, regardless of Enabled Subsystem state -- Enable only
gates `mdlOutputs`/per-step execution, not initialization. So wrapping "PX4 Interface" in an
Enabled Subsystem alone would compile fine but not change runtime behavior at all.

**Fix applied:** modified `pixhawk_sil_connector.cpp` to defer the connect from `mdlStart` to the
first `mdlOutputs` call (mirroring how `px4MAVLinkBridgelib`'s MATLABSystem blocks already behave):

- `mdlStart` now just does `ssSetPWorkValue(S, 0, nullptr)` -- marks "not yet connected", nothing
  blocking.
- `mdlOutputs` checks the PWork pointer; on the first real call (`nullptr`), it heap-allocates a
  `new SILConnector(...)`, prints the same `"Waiting for PX4 to connect..."` message, calls
  `->open()` (the actual blocking `accept()`), and stores the pointer. Subsequent calls reuse it.
- `mdlTerminate` now `delete`s the heap-allocated connector (the original code pointed at a
  function-local `static` object that didn't need explicit cleanup; heap allocation was needed
  here so each block instance owns its own connector, freed properly).
- "PX4 Interface" then got the same Enable-port treatment as "PX4 HITL Interface"
  (`wire_px4_hitl_selector.m` now adds this to both), gated on `CONTROLLER_RUNTIME==1`.

**Recompiling hit two unrelated, pre-existing gaps in this repo's build setup**, both fixed along
the way:
1. `PX4SILConnector/includes/` (asio + MAVLink C headers) wasn't present -- has to be extracted
   from `PX4SILConnector/includes.zip` first (this *is* documented in the main README's "SIL
   Connector Notes", just easy to miss). It's `.gitignore`d, so every fresh clone needs this step
   before `pixhawk_sil_connector.cpp` can be recompiled.
2. `PX4SILConnector/make.m`'s plain `mex -I./includes pixhawk_sil_connector.cpp` fails to link on
   this toolchain -- undefined references to `WSAStartup`/`accept`/`closesocket`/etc. (asio's
   Windows socket calls). Needed `-lws2_32 -lmswsock` plus an explicit `-L` to the MinGW lib
   directory bundled with the MATLAB Support for MinGW-w64 compiler (`mex` doesn't find
   `libws2_32.a` on its default search path with just `-l`). Full working command is now in
   `README.md`'s HITL section.

**Verification:** ran the actual model (not just `update_diagram`) with `CONTROLLER_RUNTIME=2` via
`sim('VehicleSilSimulation')`. It built (22 referenced models, ~5-12s) and then failed within
another ~15-20s with `"Failed to open serial port '\\.\COM5'"` -- the expected, fast failure for
"no real hardware attached," and critically, `"Waiting for PX4 to connect on TCP port 4560..."`
never printed. This is the first actual *run* (not just compile) performed in this whole HITL
effort, and it behaved exactly as intended.

**One more real bug found and fixed while verifying this:** `PixhawkSerialPortManually` (the COM
port mask parameter, section 18.2) turned out to not evaluate `hitlParams.serialPort` as an
expression at all -- `MaskVariables` shows it as `&2` (Simulink's "take this string literally"
notation), not `@2` ("evaluate as expression"). The model was actually trying to open a port
literally named `\\.\hitlParams.serialPort`. Fixed by having `initVehicleSIL.m` call `set_param`
directly with the real port string (`char(opts.hitlSerialPort)`) on both MAVLink Bridge blocks
after loading `VehicleSilSimulation.slx`, rather than relying on the mask parameter to resolve a
workspace variable reference. Re-verified after the fix: it correctly tried `'\\.\COM5'`.

### 18.6 Validation actually performed

- `model_check` (structural: unconnected ports/lines, Stateflow lint) on `PX4HITLConnector.slx`
  standalone: healthy.
- `update_diagram` compile check on `PX4HITLConnector.slx` standalone: passes.
- `model_check` on `VehicleSilSimulation.slx` after wiring: one pre-existing warning (a dangling
  line inside "PX4 Interface" -- confirmed present in the original, unmodified, git-committed
  version of the file too, via `git show HEAD:VehicleSilSimulation.slx`, so not introduced by this
  work). Found and fixed three *new* dangling line branches this work did introduce (artifacts of
  an intermediate Model-block-then-delete-then-reconnect step taken before settling on the inlined
  Subsystem approach) -- confirmed clean after.
- `update_diagram` compile check on the **full** `VehicleSilSimulation.slx`, via the real
  `initVehicleSIL(...)` entry point (not a shortcut/bypass), for **both**
  `controllerRuntime="SITL"` (default) and `controllerRuntime="HITL"`: both pass.
- **Actual `sim()` run** (not just compile) with `CONTROLLER_RUNTIME=2`, via the real
  `initVehicleSIL(...)` entry point: fails fast and correctly on the missing hardware, with no
  hang -- see section 18.7 for the full story. This is the first real-execution validation in this
  effort, as opposed to structural/compile-only checks.
