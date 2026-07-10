# SITL vs. HITL Comparison: Isolating the ~13s EKF2 Reset Cycle

**Status:** In progress -- SITL build running, comparison not yet executed.
**Date:** July 9, 2026
**Parent investigation:** `HITL_PREARM_HEALTH_INVESTIGATION.md` section 10 (in particular 10.9/10.10).

---

## 1. What this test is for

`HITL_PREARM_HEALTH_INVESTIGATION.md` section 10 found that PX4 never clears its prearm checks
during real HITL bench sessions against the Cube Orange Plus. Section 10.9 captured `ESTIMATOR_STATUS`
telemetry and found the failing checks (`vel_ratio`, `pos_vert_ratio`) trace an almost perfectly
regular **~13.00 second sawtooth**: climb smoothly to the ratio's clamp at 2.0, snap back near 0,
repeat -- not noise, not a one-time startup transient, a sustained periodic cycle.

Section 10.10 read PX4's own EKF2 source (`src/modules/ekf2/EKF/*.cpp`) for what actually triggers
a height/velocity reset, and found three real candidate timeout constants:

| Trigger | Constant | Value |
|---|---|---|
| `Ekf::isHeightResetRequired()` -- bad-accel ("falling") detection | `bad_acc_reset_delay_us` | 0.5 s |
| `Ekf::isHeightResetRequired()` -- height fusion timeout | `hgt_fusion_timeout_max` | 5 s |
| `Ekf::shouldResetGpsFusion()` -- horizontal aiding timeout | `reset_timeout_max` | 7 s |

None of these equal 13s directly. Section 10.10 then used data already captured from prior HITL
sessions (no new hardware) to test the two most plausible physical-layer explanations directly:

- **Ground-contact numerical chatter on the accelerometer feed:** ruled out. Session_14's own
  logged accel-Z signal is provably flat (`std=0.00000`) at every one of the 13 reset windows
  measured, and for the entire session after a 0.124s startup transient.
- **`HIL_GPS` delivery timing:** ruled out. Session_13's decoded `HIL_GPS` stream shows a rock-steady
  25Hz delivery rate (40ms gaps, max gap 40ms across the whole 400s session) -- nowhere near the
  5-7s fusion-timeout thresholds.

**Both of this repo's sensor-injection paths are confirmed clean.** That leaves two live
possibilities:

1. **The ~13s cycle is intrinsic to PX4/EKF2** (or the arming-check retry logic) while grounded and
   continuously failing prearm -- i.e. this would happen with *any* correctly-injected static sensor
   feed, real hardware or not, and isn't a bug in this project at all.
2. **Something specific to the real HITL bench** (the physical Cube Orange Plus, the serial
   relay/lockstep timing, or a real hardware sensor/clock quirk not visible in Simulink's own logs)
   is still the cause, and simply isn't visible from the angles checked so far.

**A pure-SITL run of the identical scenario is the cheapest way to tell these apart, and requires no
real hardware at all.** If the same ~13s sawtooth appears in `ESTIMATOR_STATUS` with a simulated PX4
process (no Cube Orange Plus involved), that confirms (1) -- the investigation reframes from "what is
this repo doing wrong" to "is this expected PX4 behavior, and which parameter controls it." If SITL
arms cleanly (or shows a materially different pattern), that points back toward (2) and the real
board specifically.

---

## 2. Prerequisites and a real environment bug found along the way

Same repo, same Simulink model (`VehicleSilSimulation.slx`), just `CONTROLLER_RUNTIME=1` (the
existing `PX4 Interface`/`pixhawk_sil_connector` path already used elsewhere in this project) instead
of `CONTROLLER_RUNTIME=2` (HITL). No real PX4 hardware, no serial port, no `reboot_autopilot.py`.

**Found a real, separate bug while setting this up:** `utilities/getWSLIP.m` (used when
`initVehicleSIL`'s `simHostIP="auto"`, the default) looks for a network adapter literally named
`'vEthernet (WSL)'` via `Get-NetIPAddress -InterfaceAlias 'vEthernet (WSL)'`. On this machine's
current WSL install, that adapter is named **`vEthernet (WSL (Hyper-V firewall))`** instead --
`getWSLIP.m` finds nothing and throws `"Could not retrieve WSL vEthernet IP"`. WSL is confirmed
running in NAT mode (`wslinfo --networking-mode` -> `nat`), just with the newer adapter naming
convention that `getWSLIP.m` predates. **Not yet fixed in `getWSLIP.m`** -- worked around here by
resolving the IP manually and passing `simHostIP` explicitly instead of `"auto"`:

```powershell
Get-NetIPAddress -InterfaceAlias 'vEthernet (WSL (Hyper-V firewall))' | Where-Object {$_.AddressFamily -eq 'IPv4'} | Select-Object -ExpandProperty IPAddress
# -> 172.23.32.1 (this machine, this session -- will differ on other machines/reboots)
```

**Recommended real fix for `getWSLIP.m`** (not applied yet, flagging for follow-up): match on
`'vEthernet (WSL*'` (wildcard) instead of the exact literal string, e.g.:
```matlab
Get-NetIPAddress | Where-Object {$_.InterfaceAlias -like 'vEthernet (WSL*'} | Where-Object {$_.AddressFamily -eq 'IPv4'} | Select-Object -First 1 -ExpandProperty IPAddress
```

**Build dependencies:** the WSL distro (`PX4Simulink`) already had `cmake`/`ninja`/`ccache`/`git`
from previous HITL-firmware build work, but was missing some SITL-specific Python packages
(`empy` among them). Fixed with:
```bash
pip3 install --user -r /mnt/c/AS/optimAeroPX4SIL/PX4-Autopilot/Tools/setup/requirements.txt
```

**No prior SITL build existed** in either PX4-Autopilot checkout on this machine (WSL home's
`~/PX4-Autopilot` doesn't even have the `optimAeroHex`/`optimAeroF16` custom airframes this repo
adds -- it's a separate, unrelated clone, seemingly only used for the HITL hardware-flash build,
`cubepilot_cubeorangeplus_VTOL`). The real SITL target has to be built from **this repo's own**
`PX4-Autopilot/` (the Windows-side checkout, reached from WSL via `/mnt/c/AS/optimAeroPX4SIL/PX4-Autopilot`,
per `initVehicleSIL.m`'s default `PX4InWSL=false` behavior), confirmed to have `optimAeroHex`
configured (`ROMFS/px4fmu_common/init.d-posix/airframes/CMakeLists.txt`,
`src/modules/simulation/simulator_mavlink/CMakeLists.txt`). This is a from-scratch build across the
Windows/WSL filesystem boundary (`/mnt/c/...`), which is slower than a native WSL build -- no
existing `build/` cache, so first build time is unknown but expected to take a while.

**Second real bug found: the vendored `PX4-Autopilot` fork has zero git tags.** First build attempt
failed immediately at CMake configure time:
```
CMake Error at CMakeLists.txt:129 (list): list index: 1 out of range (-1, 0)
CMake Error at CMakeLists.txt:131 (list): list index: 2 out of range (-1, 0)
CMake Error at CMakeLists.txt:132 (string): string sub-command REPLACE requires at least four arguments.
-- PX4 version: fd9e16a0a2 (fd9e16a0a2..NOTFOUND)
```
Root cause: top-level `CMakeLists.txt:115-133` runs `git describe --exclude ext/* --always --tags`
to derive `PX4_VERSION_MAJOR`/`MINOR`/`PATCH` by splitting the result on `.`. This repo's PX4
fork (`origin` = `https://github.com/eraltam/PX4-Autopilot-optimAero.git`) has **`git tag` return
completely empty** -- confirmed not a shallow-clone issue (`git rev-parse --is-shallow-repository`
-> `false`, full history present) and confirmed `git fetch --tags origin` brings in nothing either
(the fork itself was apparently created/pushed without carrying any tags over). With no tag
reachable, `git describe --tags` falls back to a bare commit hash with no `.` characters, so
splitting it produces a 1-element list and every subsequent `list(GET ... 1/2)` goes out of range.
**Fixed here with a local-only tag** (`git tag v1.15.0`, at HEAD, not pushed anywhere) so
`git describe` resolves to a clean `X.Y.Z`-parseable string -- purely cosmetic (affects the
version string PX4 reports via `ver`, not firmware behavior), and safe/reversible (`git tag -d
v1.15.0` removes it). Worth deciding later whether to push a real tag to the fork so this doesn't
need re-doing locally on every fresh checkout.

---

## 3. How to run it (once the build finishes)

```bash
# 1. Build (one-time; ccache speeds up any rebuild afterward). Run in WSL:
cd /mnt/c/AS/optimAeroPX4SIL/PX4-Autopilot
export PX4_SIM_HOSTNAME=172.23.32.1   # or whatever `vEthernet (WSL*` currently resolves to
make px4_sitl_default optimAeroHex
# This both builds AND launches the px4 process, which then waits for Simulink to connect
# on port 4560 ("Initializing.." / listening).
```

```matlab
% 2. In MATLAB, launch the Simulink side (SITL, not HITL -- controllerRuntime defaults to "SITL"):
initVehicleSIL("launchFullSIL", true, "vehicleType", "hexarotor", ...
    "visualizationType", "Matlab", "simHostIP", "172.23.32.1", "PX4InWSL", false)
```

Then capture the same `ESTIMATOR_STATUS` telemetry used in HITL section 10.9 -- either via
`HILDiagnostics/mavlink_system/main.py` pointed at the SITL instance's MAVLink port (would need a
config variant, since `main.py`/`config.yaml` currently assume the HITL relay topology -- UDP 14550
listener, HITL's port-14541 command relay, etc. -- most of which doesn't apply to a direct SITL
connection), or a simpler standalone pymavlink/MAVSDK script connecting directly to whatever UDP port
SITL's `mavlink start` config exposes (commonly `udp://:14540` by default for the first instance,
already MAVSDK's default `vehicle.mavsdk_address` in `config.yaml` -- may just work with `main.py`
largely as-is, minus the HITL-specific reboot/relay steps). **Not yet tested which is actually needed
-- first priority is getting the build to finish and PX4 to actually start.**

---

## 4. Build and launch notes (two more real bugs found)

Build eventually succeeded after the two fixes in section 2. Two further, separate problems showed
up getting an actual comparison run going:

**Third bug: PX4's interactive `pxh` shell spins uncontrollably with no real TTY attached.**
Launching via `make px4_sitl_default optimAeroHex` (the README's documented method, `USES_TERMINAL`
in `CMakeLists.txt`) through a piped `wsl -d PX4Simulink -e bash -c "..."` invocation (no real pty)
caused the `pxh>` prompt to redraw in an uncontrolled tight loop -- the captured output hit **5.06 GB**
before being killed. `USES_TERMINAL` presumably works fine when `make` is run from an actual
interactive terminal (as the README's instructions assume, e.g. via MATLAB's `system('start wsl
bash -c "..."')`, which opens a real console window), but not when piped. **Fixed:** run the `px4`
binary directly with the `-d` (daemon mode, no `pxh` shell) flag instead of through the `make`
target's terminal-wrapped launch:
```bash
cd /mnt/c/AS/optimAeroPX4SIL/PX4-Autopilot/build/px4_sitl_default/rootfs
PX4_SYS_AUTOSTART=10021 PX4_SIM_HOSTNAME=<host-ip> \
  /mnt/c/AS/optimAeroPX4SIL/PX4-Autopilot/build/px4_sitl_default/bin/px4 -d
```
(`PX4_SYS_AUTOSTART=10021` and the rootfs working directory both taken from what the `optimAeroHex`
`add_custom_target` actually does in `CMakeLists.txt`.) With `-d`, a full 300s Simulink `sim()` run
completed cleanly on the first attempt, confirming this was the actual cause, not a deeper
simulator-protocol problem.

**Fourth bug: `pixhawk_sil_connector` doesn't survive a second `sim()` call in the same MATLAB
process -- the exact same failure class already documented for HITL's `MAVLink Bridge
Sink/Source`.** After the first successful 300s SITL run, `sim()` returned cleanly and PX4's daemon
stayed alive. Started a WSL-side `ESTIMATOR_STATUS` listener (`sitl_estimator_listener.py`, new file
-- necessary because PX4's default MAVLink instances only bind to localhost *within WSL's own
network namespace*; a listener on the Windows host can't reach them without setting
`MAV_{i}_BROADCAST=1`, not yet tried). The listener itself failed harmlessly (no heartbeat within
60s -- started before Simulink's second connection resumed PX4's lockstep ticking, so nothing was
being broadcast yet; a sequencing issue, not investigated further). **But the second `sim()` call
itself hung MATLAB completely** -- `evaluate_matlab_code` timed out after 1800s waiting for the
`sim()` call to return, and a subsequent trivial `disp('ping')` call *also* timed out, confirming
MATLAB's command engine was genuinely blocked, not just a slow simulation. Killed the WSL-side PX4
daemon; MATLAB itself needs a manual restart (same constraint documented for HITL in
`HITL_PREARM_HEALTH_INVESTIGATION.md` section 6.3 -- killing the MATLAB process programmatically
does not trigger the MCP tooling's auto-respawn, only an actual crash does).

**This generalizes section 6.3's finding beyond HITL's serial connector:** both
`PX4HITLConnector`'s MAVLink Bridge blocks *and* `pixhawk_sil_connector`'s TCP-based S-function
apparently leak some resource across `sim()` calls that isn't released when the model stops, and
both manifest as "first run in a fresh MATLAB process works, second one hangs/crashes." Worth
treating as one root cause investigation rather than two separate ones -- likely something common to
how this project's PX4 connector blocks (S-Function / MATLABSystem) release their sockets/handles
when a Simulink run ends, not something specific to serial vs. TCP transport.

## 5. Results: SITL is completely healthy -- the reset cycle is HITL/hardware-specific, not PX4-intrinsic

After a MATLAB restart (per section 4), re-ran cleanly: launched PX4 in daemon mode, started the
Simulink sim **asynchronously** (`set_param(model, 'SimulationCommand', 'start')` instead of the
blocking `sim()` -- lets the tool calling this interleave other work while the sim runs, and avoids
needing a second `sim()` call in the process at all), confirmed connection via PX4's own log
(`Simulator connected on TCP port 4560`), *then* started `sitl_estimator_listener.py` inside WSL
(fixing the earlier sequencing bug -- starting the listener before Simulink connects means PX4 is
still lockstep-paused and sends nothing). Captured 110 `ESTIMATOR_STATUS` messages over the first
~226s of the 300s run (`HILDiagnostics/logs/sitl_estimator_status.jsonl`). Simulink's `sim()`
completed cleanly to `SimulationStatus == 'stopped'` at the full 300s with no errors.

**Result: `vel_ratio` and `pos_vert_ratio` sit at essentially `0.000` for the entire ~226s
captured** (values like `0.000`-`0.003`, never approaching the arming-check thresholds, let alone
the 2.0 clamp HITL sessions hit repeatedly). `pos_vert_accuracy` stays a tight, stable
~0.19-0.22m throughout, no growth. **Zero `STATUSTEXT` messages of any kind were received** --
no `Preflight Fail`, nothing -- across the whole window. `flags` stays constant at `831` the entire
time (versus HITL's `959`/`1983`/`1189`/etc. flapping between values as different estimator aiding
sources dropped in and out every reset cycle).

**This directly answers section 1's question.** With the identical Simulink model, identical
scenario, identical injected sensor values (same static hexarotor, same reference position, same
`stepSize_s`), and a simulated PX4 instead of the real Cube Orange Plus: **PX4's EKF2 converges
essentially immediately and stays converged indefinitely -- no ~13s sawtooth, no periodic resets, no
prearm failures at all.** Combined with section 10.10's HITL-side finding (accelerometer and GPS
data reaching the *real* board were independently confirmed clean and correctly timed, ruling out a
Simulink-side data problem twice over), this now **rules out possibility (1) from section 1**
(EKF2-intrinsic behavior while grounded) just as firmly as the sensor-data hypotheses were already
ruled out. **The ~13s reset cycle is specific to the real HITL bench** -- something about the
physical Cube Orange Plus, the HITL serial relay/lockstep timing, or a real hardware clock/sensor
quirk that has no counterpart in SITL's TCP-based simulator protocol.

**Reframed next question, for `HITL_PREARM_HEALTH_INVESTIGATION.md`:** given the same model behaves
perfectly in SITL, what is different about the real board's HITL path specifically? Candidates not
yet checked: (a) the serial link's own timing characteristics (baud-rate-limited throughput vs.
SITL's TCP, byte-level jitter, or the `UDP Receive`/`Switch` gating logic in
`PX4HITLConnector.slx`/`VehicleSilSimulation.slx`'s "PX4 HITL Interface"); (b) `SYS_HITL`-specific
PX4 firmware code paths that don't exist/activate in the plain SITL build (some sensor/EKF
initialization logic is explicitly gated on `SYS_HITL=1`, not exercised by this SITL comparison at
all since `optimAeroHex`'s airframe config does not set it); (c) real board-specific timing
(actual clock drift/jitter on the Cube Orange Plus's own hardware clock vs. lockstep-perfect SITL).
(b) is the most promising to check first, structurally: `SYS_HITL` is precisely the parameter that
distinguishes the two paths and is known (`HITL_PREARM_HEALTH_INVESTIGATION.md` section 9.1.4) to
gate real PX4 firmware behavior differences, not just a cosmetic flag.

**Confirmed (b) is real, not speculative:** `src/modules/sensors/voted_sensors_update.cpp` takes a
`hil_enabled` constructor argument (from `SYS_HITL`) and, when true: widens the gyro/accel sensor
voter timeout from its default to `500000` us (500ms) with the comment `// HIL has less accurate
timing so increase the timeouts a bit`, and explicitly skips `checkFailover()` entirely
(`!_hil_enabled` guard) -- HIL mode intentionally disables PX4's normal sensor-failover detection.
This confirms PX4 has real, deliberate code paths that treat real-hardware sensor timing as
fundamentally less reliable than SITL's lockstep-perfect delivery. This specific change (widening a
timeout) wouldn't by itself *cause* instability -- if anything it's more forgiving -- but it proves
the hypothesis that HIL-specific firmware branches exist and differ from what this SITL comparison
exercised (this comparison never set `SYS_HITL=1`, so none of this code activated). Worth explicitly
setting `SYS_HITL=1` in a SITL run as a cheap next check: if the sawtooth reappears with `SYS_HITL=1`
enabled even in SITL (no real board), that would isolate the cause to this parameter's firmware
branches specifically, rather than anything about the physical Cube Orange Plus or the real serial
link.

## 6. `SYS_HITL=1` test result: negative -- also ruled out

Added `param set-default SYS_HITL 1` to `ROMFS/px4fmu_common/init.d-posix/airframes/10021_optimAeroHex`
(also fixed a real, separate typo bug found in the same file while there: line 64 had `para
set-default MPC_THR_HOVER 32` -- missing the `m` in `param`, matching a `"para: not found"` shell
warning seen in every SITL boot log throughout this comparison; `MPC_THR_HOVER`'s intended default
was silently never applied). Cleared the rootfs's saved `parameters.bson`/`parameters_backup.bson`
and `eeprom/` to guarantee a clean boot picks up the new default (`param set-default` only takes
effect if the parameter hasn't already been explicitly saved). Re-ran the identical 300s SITL
comparison, using the same async `SimulationCommand=start` + WSL-listener-after-connection sequencing
established in section 5, in a fresh MATLAB process (per section 4's finding: never reuse a MATLAB
process across `sim()` calls with either PX4 connector).

**Result: no change at all.** `vel_ratio`/`pos_vert_ratio` stayed at essentially `0.000` for the
entire ~226s captured, `flags` stayed constant at `831` (identical to the `SYS_HITL=0` baseline run
in section 5, same values at matching timestamps to within noise), zero `STATUSTEXT` messages.
**`SYS_HITL=1` alone, without the real hardware, produces no observable difference in estimator
behavior.**

**This rules out hypothesis (b) from section 5 as the sole cause.** The `voted_sensors_update.cpp`
timeout-widening/failover-disabling code confirmed to exist under `SYS_HITL=1` either isn't relevant
to this symptom, or isn't sufficient by itself -- something about it requires the actual physical
hardware/serial path to manifest. **Remaining live hypothesis: (c) -- something physical to the real
HITL bench specifically** (the real serial link's byte-level timing/jitter at 921600 baud, the
`UDP Receive`/`Switch` gating logic in `PX4HITLConnector.slx` mixing HITL command traffic into the
serial stream, or genuine clock drift/jitter on the Cube Orange Plus's own hardware clock that
lockstep-perfect SITL has no counterpart for). This can only be investigated with the real board --
there is no further no-hardware experiment obviously available from where this comparison leaves
off. See `HITL_PREARM_HEALTH_INVESTIGATION.md` section 11 for the consolidated status and candidate
next real-hardware checks.
