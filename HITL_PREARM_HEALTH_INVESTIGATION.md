# HITL Arming Blocker: PX4 Preflight Health Failures

**Status: ORIGINAL BLOCKER RESOLVED.** `session_8` (with a fresh MATLAB restart, `is_armable` gate,
and a longer pymavlink connect timeout) achieved a full, real round trip: `arm()` accepted
(`COMMAND_ACK command=400 result=0`, `Armed by external command`), `takeoff()` accepted and
altitude reached (`rel_alt=2.61 m`), and the waypoint mission started executing
(`Executing Mission`, `mission progress 0/3`). This is the first time in this whole investigation
the vehicle has actually armed and flown. See section 7.

**New issue found downstream, not yet investigated:** shortly after mission execution begins, the
vehicle shows an erratic/excessive climb rate and PX4 starts cycling `Preflight Fail: Attitude
failure (roll/pitch)` + `Failsafe activated`, repeating roughly every 7-9s without the mission ever
completing. This is a different problem (flight dynamics / actuator mapping, not MAVLink transport
or arming) -- see section 7.2. Not yet root-caused.

**Also confirmed, reproducible (2-for-2 at time of writing) MATLAB crash** on the second
consecutive HITL session in the same MATLAB process (section 6.3) -- still unresolved, but worked
around by restarting MATLAB before every session that needs to reach `arm()`.
Real PX4 hardware involved; read section 6.3 before attempting another run.
**Date:** July 8, 2026
**Context:** `optimAeroPX4SIL` HIL bench, real PX4 (Cube Orange Plus) on serial COM4,
`CONTROLLER_RUNTIME=2` (HITL), no motors physically attached. Companion docs:
`CLAUDE_HITL.md` (HITL connector build story), `../newIMU_model/CLAUDE.md` (ANELLO X3 sensor
model, not in use here -- `INS_VARIANT=1`/generic is active for these tests).

---

## 1. How we got here: the MAVLink relay was completely broken, now isn't

Getting a real arm/mission attempt to even reach PX4 required fixing three independent bugs in
`HILDiagnostics/mavlink_system/`, in order of discovery:

1. **MAVSDK had no telemetry at all.** In HITL mode, `PX4HITLConnector`'s `MAVLink Bridge
   Sink/Source` blocks only mirror MAVLink to UDP `127.0.0.1:14550` (where `pymavlink` listens);
   nothing sent to MAVSDK's port (`14540`), so `instructor.connect()` hung forever.
   **Fix:** `listener_pymavlink.py` now re-sends every message it receives to
   `vehicle.mavsdk_address` (`forward_to_mavsdk` config flag).
2. **MAVSDK's replies had nowhere to go.** MAVSDK addresses its outbound commands back to
   whatever ephemeral port sent it packets -- i.e. our forward socket -- and nothing read them
   there, so arm/takeoff/mission commands never reached the real board over serial.
   **Fix:** added a new `UDP Receive` block to `PX4HITLConnector.slx` and the inlined `PX4 HITL
   Interface` in `VehicleSilSimulation.slx` (port `14541`), feeding a 4th input into
   `Combine_MAVLink_Streams` (which already feeds `MAVLink Bridge Sink`'s serial write). Python
   side: `listener_pymavlink.py`'s `_relay_commands_to_simulink()` background thread relays
   MAVSDK's replies there (`relay_commands_to_simulink` / `vehicle.simulink_command_address`).
3. **Two bugs found only by testing the above:**
   - `UDP Receive` does **not** zero-pad between packets -- confirmed empirically (a scratch
     model showed one sent packet replayed on 216+ consecutive 8ms ticks). Every MAVSDK command
     was getting fed into PX4's serial input continuously at 125Hz until the next command
     replaced it, which manifested as duplicate `MISSION_ACK`s and a `PROTOCOL_ERROR` mid mission
     upload. **Fix:** gated the `UDP Receive` output through a `Switch` block keyed on the block's
     second output (`y2` = bytes received *this tick only*, 0 otherwise), so PX4 sees each command
     exactly once.
   - Windows-specific `ConnectionResetError` (`WinError 10054`): our first `sendto()` to MAVSDK's
     port raced ahead of `mavsdk_server` actually binding it, triggering an ICMP port-unreachable
     that Windows delivers as `ECONNRESET` on the *next* `recvfrom()` -- which killed the relay
     thread permanently a few ms into the session. **Fix:** catch and ignore
     `ConnectionResetError` in the relay loop instead of treating it as fatal.

All four fixes are in `HILDiagnostics/mavlink_system/listener_pymavlink.py`, `config.yaml`,
`PX4HITLConnector.slx`, and `VehicleSilSimulation.slx` (already saved). With all four in place, a
real end-to-end round trip was observed: MAVSDK sent `MISSION_COUNT`, mission upload completed,
and an `arm()` `COMMAND_LONG` reached PX4 and got a real `COMMAND_ACK` back -- proof the transport
layer is no longer the problem.

---

## 2. The current blocker

```
15:46:50 COMMAND_ACK: command=400, result=1
15:46:50 STATUSTEXT: severity=2, text=Arming denied: Resolve system health failures firs
15:46:50 STATUSTEXT: severity=2, text=t
MAVSDK: arm failed: COMMAND_DENIED: 'Command Denied'; origin: arm(); params: ()
```

(`command=400` = `MAV_CMD_COMPONENT_ARM_DISARM`, `result=1` = `MAV_RESULT_TEMPORARILY_REJECTED`.)

Earlier in the same session, PX4 logged the *specific* failing checks (not repeated at the actual
arm attempt, which only gave the generic summary above):

```
15:46:30 STATUSTEXT: severity=2, text=Preflight Fail: vertical velocity unstable
15:46:30 STATUSTEXT: severity=2, text=Preflight Fail: High Accelerometer Bias
15:46:49 STATUSTEXT: severity=2, text=Preflight Fail: vertical velocity unstable
```

Both are EKF2 sanity checks, and both point at the **injected HIL sensor data**, not at MAVLink
transport. MAVSDK's own basic health gate (`is_global_position_ok`, `is_home_position_ok`) already
passed (`MAVSDK: position health is OK`) before this -- PX4's internal arming checks are stricter
and broader than what MAVSDK's Python API exposes directly.

---

## 3. Root-cause investigation so far

### 3.1 This matches an already-documented, never-verified risk

`optimAeroPX4SIL/CLAUDE.md` (ANELLO X3 IMU integration doc) already lists, as an unresolved
"Next Steps" item:

> **Gravity sign validation** -- Critical blocker for PX4 integration. Static test: vehicle level,
> `A_CG = 0`, should measure `a_z = -g` (or `+g` depending on convention). Compare with PX4
> MAVLink `sensor_accel` expected sign.

That test was never actually run against real hardware. An accelerometer bias estimate that grows
implausibly large during EKF2 initialization -- exactly what "High Accelerometer Bias" means -- is
the textbook symptom of a sign, magnitude, or double-counting error in the fed accelerometer data.

**Important caveat:** this HITL bench run used `INS_VARIANT=1` (the **generic** Aerospace
Blockset IMU model in `sensors/components/ins.slx`), not the ANELLO X3 model the above doc
discusses. The concern is the same class of bug, but in a different piece of code.

### 3.2 Traced the actual accelerometer math (confirmed correct in isolation)

`ins.slx` (`INS_VARIANT=1`, the active path) wires the Aerospace Blockset library block
`aerolibnav/Three-axis Inertial Measurement Unit` with:

- `u1` (`A_b`) = `aircraftAccelbe_mps2` (from `BodyStatesBus`)
- `u5` (`g`) = `g_body = DCM_be * [0; 0; gravityScalar_mps2]`

Traced the block's internals down to `aerolibnav/Three-axis Accelerometer`'s `Sum7` block
directly (`get_param(..., 'Inputs')` = `|+-`, i.e. two real ports: `+` and `-`):

```
Sum7 = Ab_scaled(+) - g_scaled(-)          -- i.e. specific force = Ab - g
Sum  = Sum7(+) + centripetal(+) + tangential(+)   -- lever-arm terms, CG=[0,0,0] here so these are ~0
```

This is the textbook-correct formula: `A_meas = A_b - g + omega x (omega x r) + omegadot x r`.
With `gravityScalar_mps2` positive (~9.81) and a level vehicle (`DCM_be` ~ identity for the Z
component), `g_body ~ [0, 0, +9.81]`. For a static vehicle at rest (`A_b = 0`):

```
A_meas = 0 - [0,0,9.81] = [0, 0, -9.81]
```

**This matches PX4's expected convention** (FRD, Z-down: at rest, accelerometer reads the
upward support-force reaction, i.e. `accel_z ~ -9.81 m/s^2`). So the IMU block's own math, in
isolation, is not obviously wrong.

### 3.3 The open, unresolved question

The correctness of section 3.2 hinges entirely on **`aircraftAccelbe_mps2` genuinely excluding
gravity** (i.e. being pure `F_aero+thrust/m`, not the total kinematic `d(V)/dt` which would already
include gravity's contribution). If it actually *includes* gravity, the block's `A_b - g` would be
**double-subtracting gravity**, producing a `2g` error -- more than enough to explain both
"High Accelerometer Bias" (EKF trying to compensate for a huge, wrong DC offset) and "vertical
velocity unstable" (EKF's vertical channel fighting a badly wrong accel input).

`ins.slx` itself carries a developer annotation flagging exactly this uncertainty, unresolved:

> `TODO: Why does AircraftAccelbe work better than accel in body?`

i.e. someone already tried at least one alternative signal for this exact input and picked
`aircraftAccelbe_mps2` empirically, without fully nailing down *why* -- consistent with this being
a live, unresolved ambiguity rather than a settled design choice.

**Not yet traced:** where `aircraftAccelbe_mps2` is actually computed inside the vehicle 6DOF
plant (`VehiclePlant.slx` and/or a nested hexarotor-specific model reference) -- a `find_system`
sweep of `VehiclePlant.slx`'s top level found no matching block/signal name, meaning it's produced
inside a nested Model Reference not yet identified this session.

### 3.4 Checked whether switching to the ANELLO X3 model would sidestep this -- it wouldn't

The ANELLO X3 IMU (`INS_VARIANT=2`, `sensors/components/ins_anello.slx`, not active in this
session's tests) is a datasheet-accurate sensor model with real noise/bias/temperature
characteristics -- see `../newIMU_model/CLAUDE.md` -- and was raised as a possible alternative
worth checking. Traced its gravity handling in
`sensors/components/anelloX3/ANELLO_X3_IMU_Vehicle_fcn_SIL.m`:

```matlab
A_IMU = Ab + cross(omegaDot, r) + cross(omega, cross(omega, r));   % lever-arm terms ADDED to Ab
f_imu_true = mp.R_sensor_body * A_IMU + mp.gravitySign .* g_body;  % gravitySign = -1 (anelloX3MountParams.m)
```

With `gravitySign = -1`, this is `f = A_IMU - g_body` -- **algebraically identical** to the
generic model's `A_meas = A_b - g + lever-arm terms` from section 3.2 (`ANELLO_X3_IMU_fcn_SIL.m`,
which layers the sensor-error model on top, does no further gravity/sign handling -- confirmed by
grepping it for `sign`/`gravity`, no matches beyond the docstring).

**Conclusion: both IMU paths use the exact same gravity-sign convention**, fed by the exact same
`aircraftAccelbe_mps2`/`g_body` upstream signals. This means:

- **Switching `INS_VARIANT` to 2 (ANELLO) would not fix a gravity-sign/double-counting bug** if
  section 3.3's hypothesis is correct -- the bug, if real, lives upstream of both sensor models
  (in how `aircraftAccelbe_mps2` itself is computed), not inside either IMU formula. Fixing it in
  one place (or in the shared `g_body`/`Ab` construction) would fix both paths at once.
- The one real *difference* between the two paths: ANELLO models a nonzero mounting lever arm
  (`r_IMU_CG = [0.10; 0.02; -0.03]` m) via the `cross(omegaDot,r)` / `cross(omega,cross(omega,r))`
  terms, while the generic path hardcodes `center_of_gravity = [0,0,0]` (no lever arm). For a
  static, non-rotating vehicle (`omega ~ 0`, `omegaDot ~ 0` at rest, which is the case here) these
  terms are ~0 either way, so this difference is not expected to matter for the *current* arming
  failure -- but would matter once the vehicle is actually moving/rotating.
- If section 3.3's ground-truth check (`HIL_SENSOR.zacc`) comes back wrong, the fix belongs in the
  shared upstream signal, and `anelloX3MountParams.m`'s `gravitySign` should **not** be flipped
  independently to "compensate" -- that would silently paper over the real bug for the ANELLO path
  while leaving the generic path (and the underlying signal) still broken.

### 3.5 Second, possibly unrelated symptom: wild altitude/velocity spike

An earlier raw log (captured *before* the fixes in section 1, during active flooding from the
"hold instead of zero-pad" bug) showed:

```
LOCAL_POSITION_NED: x=-5.06, y=1214.4, z=108572.18, vx=-0.05, vy=12.01, vz=1077.01
```

`z=108572m` (108km) and `vz=1077 m/s` are wildly implausible for a static hexarotor. This was very
likely a *downstream consequence* of PX4 receiving a continuous flood of garbage/duplicate MAVLink
commands (per bug #3 above) rather than a separate root cause -- **needs re-checking now that the
flooding bug is fixed** to confirm it doesn't still occur.

---

## 5. Executed: ground-truth check (session_3)

Ran `HILDiagnostics/run_hil_automated_session.m` directly (`hitlSerialPort="COM4"`,
`visualizationType="Matlab"` to avoid a FlightGear dependency, `stopTime_s=30`) -- this reads
`SensorsBus_final.INSSensorBus.AccelSensorBus`/`GyroSensorBus` straight out of Simulink's own
signal logging, upstream of MAVLink entirely, so it's a direct ground-truth check independent of
`main.py`/MAVSDK (not run this time -- not needed for this specific question).

**First attempt crashed MATLAB** (`std::terminate()`, fatal error, full process exit) partway
through `initVehicleSIL`/`configureLogging`, stack trace deep in Simulink's mask/parameter
evaluation (`sl_mask.dll` -> `BlockEvalParams`/`slFullSetParam`). Cause not conclusively
identified -- possibly related to the new blocks added to `VehicleSilSimulation.slx` earlier this
session, possibly an unrelated hardware/driver-level hiccup on serial connect. Power-cycling the
Cube Orange Plus and retrying (fresh MATLAB session, same command) completed cleanly with no
further issue -- **if this recurs, revisit whether the new `UDP Receive`/`Switch`/`Reshape`
blocks are involved**, but one clean run after a hardware reset isn't enough to rule it in or out.

### 5.1 Gravity sign/magnitude: CONFIRMED CORRECT -- section 3.3's hypothesis is disproven

`session_3/matlab_summary.json`, `imu_noise.accel_mean_mps2` (mean over the full 30s run, vehicle
static and level the whole time):

```json
"accel_mean_mps2": [0, 0, -9.78133930918135]
```

This is **exactly** the physically-correct value predicted in section 3.2 (`~-9.81 m/s^2` on Z,
`0` on X/Y) -- the small difference from the textbook `9.80665` constant is expected (real gravity
models vary with latitude/altitude, not a red flag). **The accelerometer data being fed to PX4 is
correct.** Section 3.3's "does `aircraftAccelbe_mps2` double-count gravity" question is answered:
no. Section 3.4's conclusion (both `ins.slx` and the ANELLO path share this same, now-confirmed
correct, convention) stands unchanged.

### 5.2 New finding: gyro noise is exactly zero, not just low

```json
"gyro_mean_radps": [0, 0, 0],
"gyro_var_radps2": [0, 0, 0],
"accel_var_mps2_2": [0, 0, 0.030568310846743125]
```

Gyro shows **literally zero** variance -- not small noise, a perfectly deterministic signal with
no randomness at all. Accelerometer Z does have real noise (var `0.0306`, std `~0.175 m/s^2`).
This asymmetry suggests the generic `ins.slx` sensor model's gyro noise path isn't actually
configured/enabled (its accelerometer noise clearly is), separate from whatever is blocking
arming. Worth fixing for realism regardless, but see 5.3 for why it's probably not *this* bug.

### 5.3 Checked EKF2's own noise expectations -- no obvious mismatch in the "too noisy" direction

From `session_1/px4_params_before.txt` (939 params, dumped via MAVSDK earlier in this
investigation):

```
EKF2_ACC_NOISE   = 0.35      # PX4's expected accel measurement noise std [m/s^2]
EKF2_ACC_B_NOISE = 0.003     # accel bias random-walk noise
EKF2_ABIAS_INIT  = 0.20      # initial accel bias uncertainty [m/s^2]
EKF2_ABL_LIM     = 0.40      # accel bias LIMIT -- likely what "High Accelerometer Bias" checks against
EKF2_GYR_NOISE   = 0.015     # PX4's expected gyro measurement noise std [rad/s]
```

Injected accel Z noise (`~0.175 m/s^2` std) is **lower** than what PX4 expects (`0.35`), not
higher -- ruling out the simplest "we're feeding it noisier data than it's tuned for" explanation
for the accelerometer-bias check specifically. The zero gyro noise (5.2) is a bigger mismatch
against `EKF2_GYR_NOISE=0.015`, but in the "too clean" direction, which is a less obvious way to
break an EKF than "too noisy."

### 5.4 Narrowed the actual blocker: re-examined message timing

Re-checked `session_1/console.log` timestamps precisely:

```
15:46:30  Preflight Fail: vertical velocity unstable   <- ~ start of session
15:46:30  Preflight Fail: High Accelerometer Bias       <- ~ start of session, NEVER repeats
15:46:49  Preflight Fail: vertical velocity unstable    <- 19s later, right before the arm attempt
15:46:50  arm() -> COMMAND_DENIED (generic "resolve system health failures")
```

**"High Accelerometer Bias" only ever appeared once, at the very start of the session, and never
recurred** -- consistent with a normal EKF2 startup transient that had already cleared by the time
arming was attempted 19+ seconds later. **"vertical velocity unstable" recurred right before the
failed arm attempt** -- this, specifically, looks like the actual active blocker, not accelerometer
bias.

### 5.5 Next steps, re-prioritized given 5.1-5.4

1. **Focus specifically on "vertical velocity unstable", not accelerometer bias.** This PX4 EKF2
   health check compares GPS/baro vertical velocity against the filter's prediction. Given 5.1
   confirms the accelerometer feed is correct, look at `HIL_GPS`'s vertical velocity field
   (`vd`, derived from `aircraftVelInNED_mps` in `HIL_GPS` construction) for noise/inconsistency --
   not yet inspected this session. `run_hil_automated_session.m`'s own `buildCoordinateSummary`
   already confirms the *true* physics velocity is exactly zero throughout
   (`max_abs_velocity_during_preflight_mps: 0`), so any instability has to be introduced by the
   `HIL_GPS`/`HIL_SENSOR` construction itself, not the underlying vehicle state.
2. **Try a longer settle time before arming.** 19 seconds may simply not be enough for PX4's EKF2
   to fully converge its vertical velocity covariance on real hardware. `main.py`'s
   `wait_until_ready()` already gates on MAVSDK's basic health flags (which passed quickly) --
   consider adding an explicit longer fixed delay, or gating on the *absence* of recent
   `STATUSTEXT: Preflight Fail` messages, before calling `arm()`.
3. ~~Fix the zero-gyro-noise gap (5.2)~~ **Correction (section 6.1): not actually a config bug.**
   `ins/Three-axis Inertial Measurement Unit`'s own mask shows `i_pow = zeros(1,6)` -- the block's
   white-noise generator is hardcoded off for **both** accel and gyro equally. Base workspace
   `parameters.accel.noisePower_mps2` / `parameters.gyro.noisePower_radps` are both `[1e-7 1e-7
   1e-7]` (tiny, and NOT wired to `i_pow` -- likely vestigial/unused). This means the observed
   accel-Z variance (5.2, `0.0306`) isn't coming from a "sensor noise" model at all -- it's most
   likely real numerical chatter from the vehicle's ground-contact dynamics (`hexGroundContact`),
   faithfully passed through by an otherwise-noiseless sensor path. Nothing to "fix" in the noise
   config; if anything, worth checking whether that ground-contact chatter is itself contributing
   to "vertical velocity unstable".
4. Confirm the section 3.5 altitude/velocity spike (108km/1077 m/s) no longer reproduces -- not
   re-checked this session since `main.py` wasn't run against `session_3`.
5. Consider running PX4's own `commander check` (or QGroundControl's Vehicle Setup > Summary) for
   the full, unabridged list of failing health flags -- MAVLink `STATUSTEXT` only surfaces what PX4
   chooses to broadcast, which may not be the complete picture, and would directly confirm/refute
   5.4's "only vertical velocity is still failing" read.

---

## 6. Fix applied: `is_armable` gate

### 6.1 The actual gap

`controller_mavsdk.py`'s `wait_until_ready()` only ever waited on
`health.is_global_position_ok` and `health.is_home_position_ok` before letting
`run_waypoint_mission()` proceed to `arm()`. MAVSDK's `Telemetry.Health` struct
(`mavsdk.telemetry.Health`) also exposes `is_armable` -- a direct aggregate "PX4 currently allows
arming" flag that reflects PX4's *actual* prearm check result (including EKF2 checks like
"vertical velocity unstable"), not just the two narrow position flags. `wait_until_ready()` was
never checking it, so it would return "ready" and let `arm()` fire while PX4 was still internally
unhappy -- explaining the `COMMAND_DENIED` seen in section 2 despite `MAVSDK: position health is
OK` printing first.

**Fix (`controller_mavsdk.py`, `wait_until_ready()`):** now also requires `health.is_armable`
before returning, polling at the existing 1s interval up to the existing `health_timeout_s`
(default 60s). No new config keys -- reuses the existing gate/timeout.

### 6.2 Validation attempt hit a reproducible MATLAB crash -- unresolved, real hardware involved

Tried to validate end-to-end: started `main.py --session-id 4` in the background, then
`run_hil_automated_session("hitlSerialPort","COM4","stopTime_s",90,"sessionId","4")` immediately
after. **MATLAB crashed again** (`std::terminate()`, same signature as section 5's first crash),
this time even earlier -- the diary shows the crash happened before *any* of the script's own
`fprintf` calls executed (not even `"HIL automated session directory: ..."`, the very first line
after `diary()` is turned on), versus section 5's first crash which got as far as
`configureLogging`'s `set_param` calls. `main.py`'s pymavlink listener timed out 30s later with no
heartbeat ever received, confirming the Simulink side never got far enough to produce anything.

**Pattern so far, in order:**
1. First `run_hil_automated_session` call this investigation: crashed mid-`configureLogging`.
2. Power-cycled the board, retried: succeeded cleanly (session_3, the section 5 ground-truth data).
3. Immediately after, without a power-cycle, tried again (session_4): crashed even earlier than #1.

This "works once, then crashes again on the next attempt" pattern is more consistent with a
**stale resource from the previous session not being released** than with a one-off hardware
fluke -- most likely the `MAVLink Bridge Sink`/`Source` MATLABSystem blocks' serial connection to
COM4 not being fully closed/released when session_3's `sim()` call returned, so session_4's
attempt to re-open the same port crashed instead of erroring cleanly. Not confirmed -- just the
best-fitting hypothesis from the timing. Also cleaned up two orphaned `mavsdk_server.exe`
processes and two stray `python.exe` processes left over from the crashed/killed `main.py`
attempts (both crash attempts left port `14540` bound after the fact) -- worth checking for these
manually (`Get-NetUDPEndpoint -LocalPort 14540`) before any future retry, since a bound port
causes a *different*, non-crashing failure (`bind error`) that's easy to confuse with this one.

**Not yet done:** confirmed reproduction with a controlled wait between sessions (e.g. a full
`bdclose('all')` + several seconds' pause between HITL runs, or restarting MATLAB entirely between
every session) to see if that avoids the crash -- would help confirm/refute the stale-serial-handle
hypothesis. Given two crashes in a row against real hardware, **do not retry a third time without
either that mitigation or explicit sign-off**, per the earlier agreement to flag this rather than
loop on it blindly.

### 6.3 Pattern now CONFIRMED, 2-for-2: kill/restart MATLAB itself (not just the board) between HITL runs

Attempting to force a MATLAB restart by killing the crashed process directly (`Stop-Process` on
the two `MATLAB.exe` PIDs matching the crash timestamp) did **not** trigger an auto-relaunch --
the MCP tooling only auto-respawns MATLAB after an actual crash (`std::terminate()`), not after an
explicit kill. Needed a genuine manual relaunch (user reopened MATLAB normally).

**After the manual relaunch, ran the exact same test twice:**

- **Attempt A** (`session_5`): `main.py --session-id 5` in background, then
  `run_hil_automated_session(...,"stopTime_s",90,"sessionId","5")` immediately after. **Succeeded
  cleanly**, no crash -- but `main.py`'s pymavlink listener had already timed out (fixed 30s
  default in `PymavlinkListener.connect()`, unrelated to the crash bug) before the Simulink build
  even finished, so it couldn't validate the `is_armable` fix this round. **Fixed**: `main.py`'s
  `listener_main()` now passes `timeout_s=vehicle.connect_timeout_s` (default 60s, reusing the
  existing config key) instead of the method's hardcoded 30s default -- MATLAB's own compile/init
  overhead genuinely needs more than 30s of slack when orchestrated as two separate processes.
- **Attempt B** (`session_6`): immediately after A, same MATLAB session, no restart in between --
  **crashed again**, same signature, before any `fprintf`. `main.py` (now with the 60s timeout)
  confirmed no heartbeat ever arrived within the full 60s window.

**This makes it 2-for-2, unambiguous: the first `run_hil_automated_session` call in a fresh MATLAB
session always succeeds; the second consecutive call in that same session always crashes**,
regardless of the board's power state (attempt B ran with no board power-cycle in between, and
crashed exactly the same way attempt with-power-cycle did in section 6.2). This rules out a
board/hardware-state explanation entirely and strongly confirms the stale-MATLAB-side-resource
hypothesis (most likely the serial `MATLABSystem` connection object from the prior `sim()` call).
**Practical implication until root-caused: treat MATLAB as needing a full restart before every
single HITL session that needs to actually reach `arm()`** -- do not attempt two
`run_hil_automated_session` calls back-to-back in the same MATLAB session.

**Real root-cause fix, not yet attempted:** find where `MAVLink Bridge Sink`/`Source`'s underlying
serial connection object gets released. Likely candidates: explicitly call
`clear <mask-instance-var>` / release the `MATLABSystem` objects after `sim()` returns inside
`run_hil_automated_session.m` itself (e.g. `bdclose('VehicleSilSimulation')` or an explicit
`release()` call on the relevant System objects), or file this against
`px4.internal.block.MAVLinkBridgeBase` if it's a genuine support-package cleanup bug outside this
repo's control.

---

## 7. Validated: `is_armable` fix works -- arm/takeoff/mission-start all succeeded (session_8)

After a fourth MATLAB restart and one more orchestration-timing fix
(`vehicle.connect_timeout_s: 60 -> 180` in `config.yaml`, since `main.py`'s pymavlink connect was
still timing out even at 60s purely due to startup overhead between launching `main.py` and
Simulink actually producing its first `HEARTBEAT` -- unrelated to the crash bug, just orchestration
slack), ran `main.py --session-id 8` + `run_hil_automated_session(...,"stopTime_s",120,"sessionId","8")`
back-to-back on a fresh MATLAB session. No crash. Full real round trip, confirmed from `main.py`'s
raw output:

```
MAVSDK: health pending global=False home=True armable=False   (repeats ~25s)
MAVSDK: position health is OK and vehicle is armable
MAVSDK: mission upload accepted
MAVSDK: sending arm()
COMMAND_ACK: command=400, result=0                             (ARM accepted)
STATUSTEXT: severity=6, text=Armed by external command
MAVSDK: arm accepted
MAVSDK: sending takeoff()
COMMAND_ACK: command=22, result=0                               (TAKEOFF accepted)
STATUSTEXT: severity=6, text=Takeoff detected
MAVSDK: takeoff altitude reached rel_alt=2.61 m
MAVSDK: starting waypoint mission
STATUSTEXT: severity=6, text=Executing Mission
MAVSDK: mission progress 0/3
```

This is the first time in this entire investigation the vehicle has actually armed and begun
flying. **Section 6.1's `is_armable` fix is confirmed correct** -- PX4 took ~25s of real time
after `position health is OK`-equivalent-flags to actually clear its internal prearm checks and
report armable, exactly matching the "just needs more settle time" theory from section 5.5 item 2.
The earlier "vertical velocity unstable"/"High Accelerometer Bias" failures were transient startup
conditions, not a sensor data bug -- consistent with section 5.1's ground-truth confirmation that
the injected accelerometer data was correct all along.

### 7.1 Orchestration notes for future sessions (not bugs, just practical constraints)

- **MATLAB must be restarted before every session that needs to reach `arm()`**, per section 6.3 --
  unresolved root cause, but the workaround is reliable (4-for-4: every first-run-after-restart
  succeeded, every second-consecutive-run crashed).
- **Start `main.py` and the Simulink HITL session close together**, and give pymavlink's connect
  a generous timeout (now 180s via `vehicle.connect_timeout_s`) -- there's real, somewhat variable
  overhead in `initVehicleSIL`/model build before Simulink produces its first `HEARTBEAT`.
- **Check for orphaned `mavsdk_server.exe` holding UDP port 14540** before every retry
  (`Get-NetUDPEndpoint -LocalPort 14540`) -- both the crash and a `main.py` timeout leave it
  behind, and a bound port produces a different, non-crashing `bind error` failure that's easy to
  mistake for the crash bug.
- `run_hil_automated_session`'s `visualizationType="Matlab"` (not the script's own default,
  `"FlightGear"`) avoids an external FlightGear dependency for headless diagnostic runs like these.

### 7.2 New issue, not yet investigated: erratic/excessive climb rate + attitude-failure/failsafe loop

Shortly after `Executing Mission` begins (climbing to the first waypoint's 5m target), PX4 starts
cycling, repeating roughly every 7-9s for the rest of the session:

```
STATUSTEXT: Preflight Fail: Attitude failure (roll)     (and, separately, "(pitch)")
STATUSTEXT: Failsafe activated
STATUSTEXT: Executing Mission
STATUSTEXT: Climb to 5.0 meters above home
```

`LOCAL_POSITION_NED` during this window is erratic, not a smooth climb -- e.g. one sample jumps to
`z=-66.18` (66m, vz=-22.6 m/s) then the next several samples are back around `z=-10` to `-13` and
climbing more gradually (`vz` around -22 to -23 m/s throughout, which is still a very fast climb
rate for a "climb to 5m" mission item). Roll/pitch angles were small (`~0.01-0.02 rad`, ~1 degree)
in the samples checked right at the *first* attitude-failure trigger -- the check doesn't
obviously correspond to a large attitude excursion at that point, more likely an
innovation/consistency check reacting to the abnormal climb dynamics. `HIL_ACTUATOR_CONTROLS`
shows real, varying, nonzero actuator values being commanded by PX4 (`[0.49, 0.0, 0.27, 0.41,
0.44, 0.21, 0, ...]`) -- confirms the actuator read-back path is genuinely live and PX4 is
actively trying to control the vehicle, not just outputting garbage.

**By the end of the session it was no longer a false alarm**: `main.py`'s own log (near the very
end, right before it gave up with `TimeoutError: MAVSDK: mission did not finish within 240.0s`)
shows `ATTITUDE: roll=-0.75, pitch=0.83` rad (**~43 deg / ~47 deg**) -- the vehicle genuinely lost
attitude control over the course of the session, tumbling far beyond the ~1 degree seen at the
first failsafe trigger. So the failsafe loop isn't just PX4 being oversensitive early on; whatever
is wrong compounds into an actual loss of control given enough time.

**This is a different class of problem than everything else in this document** -- flight
dynamics / actuator-to-thrust mapping, not MAVLink transport or PX4 arming logic. Candidate areas,
none yet investigated: `PX4OutputMappingHex.slx` (hex output/motor mixing), `hexMotorModel`/
`hexActuator` (thrust-per-actuator-command scaling), or vehicle mass/inertia parameters being
mismatched against what PX4's attitude/rate controllers (tuned for a specific vehicle) expect.
Worth its own dedicated investigation, likely starting from `matlab_summary.json`'s
`actuator`/`coordinates` fields for `session_8` (not yet pulled) plus a fresh look at the
`LOCAL_POSITION_NED`/`HIL_ACTUATOR_CONTROLS` timeline around the first failsafe trigger.

(Checked one candidate lead and ruled it out: `HIL_ACTUATOR_CONTROLS.mode` went from `53` pre-arm
to `189` once armed. Decoded as a `MAV_MODE_FLAG` bitmask, `189 = 128(ARMED) + 32(HIL) +
16(STABILIZE) + 8(GUIDED) + 4(AUTO) + 1(CUSTOM_MODE)` -- entirely normal armed/auto-mission flags,
not evidence of a HIL lockstep or control-mode mismatch.)

---

## 8. How to reproduce this experiment

Prerequisites: real PX4 (Cube Orange Plus) connected via USB, enumerated as a COM port (check
`Get-CimInstance -ClassName Win32_PnPEntity | Where-Object { $_.Name -match 'COM\d+' }` in
PowerShell -- look for the Pixhawk/PX4 device name, not just any COM port; it was `COM4` for this
investigation but can shift after a re-enumeration).

1. **Restart MATLAB if it has already run a HITL session this session** (section 6.3 -- a second
   consecutive `run_hil_automated_session` call in the same MATLAB process reliably crashes it).
   A plain relaunch is sufficient; no special flags needed.
2. **Check nothing is already bound to UDP 14540** (leftover from a previous crashed/killed run):
   ```powershell
   $ep = Get-NetUDPEndpoint -LocalPort 14540 -ErrorAction SilentlyContinue
   if ($ep) { Stop-Process -Id $ep.OwningProcess -Force }
   ```
3. **Start `main.py` in the background**, using a fresh session number N (its logs and MATLAB's
   land in the same shared directory, `HILDiagnostics/logs/session_N/`, because both tools resolve
   `--session-id`/`sessionId` to the identical path convention):
   ```
   cd HILDiagnostics/mavlink_system
   py -3.12 main.py --session-id N
   ```
4. **Immediately** (don't wait -- there's already ~20-90s of unavoidable startup overhead before
   Simulink produces its first `HEARTBEAT`; `vehicle.connect_timeout_s` in `config.yaml` is
   currently `180` to absorb this), run the HITL session in MATLAB, same session number:
   ```matlab
   addpath('<repo>/HILDiagnostics');
   cd('<repo>');
   run_hil_automated_session("hitlSerialPort", "COM4", "visualizationType", "Matlab", ...
       "stopTime_s", 120, "sessionId", "N");
   ```
   (`visualizationType="Matlab"` avoids needing FlightGear running; `stopTime_s=120` gives enough
   real time for PX4's prearm checks to clear -- observed taking ~25s -- plus arm/takeoff/mission
   start. This call blocks until the simulated time elapses, roughly matching real time since HITL
   is bottlenecked by genuine serial I/O with the board.)
5. **Check the results** in `HILDiagnostics/logs/session_N/`:
   - `console.log` (from `main.py`, if run in the foreground) or the background task's captured
     output -- grep for `MAVSDK:`, `COMMAND_ACK`, `STATUSTEXT` to follow the arm/takeoff/mission
     sequence.
   - `mavlink_raw_log.jsonl` -- full structured record of every MAVLink message in both
     directions (see `config.yaml`'s `listener.message_types` for what's captured; add more types
     there if something needed isn't showing up).
   - `matlab_summary.json` -- Simulink-side ground truth (`imu_noise`, `actuator`, `coordinates`,
     etc.), independent of whether MAVLink transport worked at all.
6. **Clean up afterward regardless of outcome** -- check for orphaned `mavsdk_server.exe`/stray
   `python.exe` processes (`Get-Process | Where-Object { $_.ProcessName -match 'python|mavsdk' }`)
   and kill any left over before the next attempt, per step 2.

---

## 9. Proceeding with the new problem: attitude-failure / excessive-climb-rate

### 9.1 Expected tests, in order

1. **Pull `session_8`'s `matlab_summary.json`** (not yet done) -- specifically `actuator.*` (does
   `max_command_magnitude` look sane for a hexarotor?) and `coordinates.max_position_displacement_from_initial_m`/
   velocity fields, to get Simulink's own ground-truth view of how far/fast the vehicle actually
   moved, independent of the noisy `LOCAL_POSITION_NED` MAVLink samples already looked at.
2. **Extract the full `HIL_ACTUATOR_CONTROLS` + `LOCAL_POSITION_NED` + `ATTITUDE` timeline** from
   `session_8`'s `mavlink_raw_log.jsonl` (structured, easier to analyze than grepping the console
   log) from mission start to the first `Attitude failure` STATUSTEXT, and plot/tabulate actuator
   values vs. climb rate vs. attitude over that window -- confirms *when* the divergence starts
   relative to actuator commands, not just that it happens.
3. **Compare against a pure-SITL run of the identical mission** (`CONTROLLER_RUNTIME=1`, i.e. the
   existing `PX4 Interface`/`pixhawk_sil_connector` path, not HITL). `PX4OutputMappingHex.slx` is
   shared between both paths (per `optimAeroPX4SIL/CLAUDE.md` -- "same model reference the
   existing SITL 'PX4 Interface' already uses"), so **if SITL flies normally with the same
   mission, the bug is specific to something HITL-only** (real actuator command values/timing
   from real PX4 hardware vs. simulated PX4), not the shared output-mapping model itself. If SITL
   *also* shows excessive climb, the bug is in the shared mapping/vehicle model and would need
   fixing regardless of `CONTROLLER_RUNTIME`.
4. **Check PX4's own actuator/mixer parameters** (`px4_params_before.txt`, already dumped
   every session) for anything HITL-specific that could scale outputs differently than SITL
   expects -- e.g. `CA_*` (control allocation), `PWM_*`/`HITL_ACT_FUNC*`, or `SYS_HITL` itself.
5. **Check vehicle mass/inertia** (`cgMassInertia` model reference, `parameters` struct in base
   workspace) against what would produce a *sane* hover throttle. Rough sanity check: if PX4 is
   commanding ~0.4-0.49 normalized throttle (observed) and that produces a ~22 m/s climb rate
   rather than a controlled hover/gentle climb, either the thrust-per-unit-command gain is too
   high, or the vehicle mass is too low, or both -- back-of-envelope of thrust vs. weight from the
   model's own `hexMotorModel`/`cgMassInertia` parameters would confirm which.

### 9.2 Candidate fixes, ranked by how well they fit the evidence so far

1. **Thrust/actuator scaling mismatch in `hexMotorModel`/`hexActuator`/`PX4OutputMappingHex.slx`**
   -- best fit if step 9.1.3 shows SITL *also* climbs too fast with the same mission (shared
   model, shared bug). Fix: retune the actuator-command-to-thrust gain/curve against the vehicle's
   actual mass, or verify the model was ever validated against a realistic hover throttle in the
   first place.
2. **HITL-specific actuator command interpretation mismatch** -- best fit if step 9.1.3 shows SITL
   flies fine but HITL doesn't. Real PX4 firmware and the SITL simulator's own PX4 instance could
   plausibly output actuator commands in subtly different normalized ranges/conventions for the
   same commanded thrust; would need comparing actual `HIL_ACTUATOR_CONTROLS` values between a
   SITL run and this HITL run for the same mission phase.
3. **Vehicle mass/inertia parameters unrealistic for this airframe** -- best fit if step 9.1.5's
   back-of-envelope thrust-vs-weight math comes out wildly imbalanced. Fix: correct
   `cgMassInertia`'s parameters to a realistic hexarotor mass/inertia.
4. **EKF2/controller tuning mismatch, not a Simulink-side bug at all** -- if actuator commands and
   resulting climb rate both look individually reasonable but PX4's attitude controller still
   can't track them (e.g. genuinely under-tuned rate/attitude gains for this simulated airframe's
   effective dynamics), the fix would be PX4 parameter tuning (`MC_*` gains), not a Simulink model
   change. Least likely given the climb rate itself already looks excessive before attitude even
   diverges, but worth keeping in mind if 9.1-9.2's Simulink-side checks all come back clean.
