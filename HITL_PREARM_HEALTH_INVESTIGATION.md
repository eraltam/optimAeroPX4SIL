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

---

## 10. New failure mode found (session_9), root-caused, and fixed: stale PX4 board state carried across sessions

**Date:** July 9, 2026. Ran the section 8 reproduction procedure again (session_9), this time with
`visualizationType="FlightGear"` (which also enables `launchPreflightVisualization`, on by default --
FlightGear 2024.1 launched and the preflight TCP handshake completed before the sim ran; no code
change needed, `run_hil_automated_session.m` already threads `visualizationType` through). All
required toolboxes (Aerospace Blockset, UAV Toolbox, Simulink 3D Animation, etc.) were already
installed -- nothing to add there.

### 10.1 What happened

**Good news:** no MATLAB crash. This was the first `run_hil_automated_session` call in a fresh
MATLAB process, consistent with the section 6.3 workaround, and it completed cleanly to
`stopTime_s=120`.

**New failure:** the vehicle never armed. `main.py` timed out after 60s in `wait_until_ready()`
(`is_armable` never went `True`). PX4 logged recurring `Preflight Fail: High Accelerometer Bias`
plus a message not seen in sessions 1-8: `Preflight: GPS Vertical Pos Drift too high`. PX4's own
`GLOBAL_POSITION_INT` reported `alt~=7537 m` -- wildly wrong for a stationary bench vehicle.

**But `session_9/matlab_summary.json` (Simulink's own ground truth) shows the injected data was
correct the entire run:**

```json
"coordinates": {
  "max_position_displacement_from_initial_m": 0,
  "max_abs_velocity_during_preflight_mps": 0,
  "was_airborne_at_any_point": false
},
"imu_noise": {
  "accel_mean_mps2": [0, 0, -9.7841378880139516]
},
"altitude_disagreement": { "max_abs_diff_m": 0.0016505171719316536 },
"actuator": { "armed_observed": false }
```

Zero displacement/velocity the whole 120s, correct gravity on Z, GPS/baro agreement within 1.6mm.
Same conclusion as section 5.1: **the sensor feed reaching PX4 was correct.** Yet PX4's internal
estimate diverged badly anyway -- a different symptom from anything sections 1-9 saw, and this time
not explained by anything on the Simulink side.

### 10.2 Root cause: the real board is never rebooted between sessions, and session_8 never landed

Section 6.3 already established that *MATLAB* needs a full restart before every session because
some MATLAB-side resource isn't released between `sim()` calls. **The real Cube Orange Plus was
never subject to an equivalent reset** -- nothing in the reproduction procedure (section 8) or
`main.py`/`controller_mavsdk.py` ever power-cycles or reboots the flight controller itself between
sessions. It just keeps running continuously, carrying its internal EKF2/arming state forward from
whatever the previous session left it in.

Session_8 (section 7.2) ended via `TimeoutError: MAVSDK: mission did not finish within 240.0s`
*while the vehicle was still cycling `Preflight Fail: Attitude failure` + `Failsafe activated` and
had just reported `ATTITUDE: roll=-0.75, pitch=0.83` rad (~45 deg)* -- i.e. session_8's `main.py`
process exited on an exception with the vehicle still armed, still in a failsafe loop, and never
called `land()`/`disarm()`. Session_9 started immediately after with no board-side reset in
between. `main.py`'s pymavlink listener received a heartbeat from the board *immediately* at
connection time in session_9, before Simulink's own `sim()` had even started -- confirming the
board had been running continuously since session_8, not freshly booted.

**This is the same class of bug as section 6.3 (stale state surviving a session boundary), just on
the hardware side instead of the software side.** A board left armed/mid-failsafe with a diverged
EKF2 estimate at the end of one session inherits that state into the next session's prearm checks,
producing exactly the spurious "High Accelerometer Bias" / "GPS Vertical Pos Drift" failures seen
in session_9, independent of whether the freshly-injected sensor data for that new session is
correct.

### 10.3 First fix attempt (v1) -- rebooting through the live MAVSDK/Simulink relay -- broke the session instead

The first fix added an automatic reboot step gated by `safety.allow_real_vehicle_commands`:
`MavsdkInstructor.reboot_autopilot_and_wait()` in `controller_mavsdk.py`, called from `main.py`'s
`run()` immediately after `instructor.connect()`, sending MAVSDK's `drone.action.reboot()`
(`MAV_CMD_PREFLIGHT_REBOOT_SHUTDOWN`) through the same live relay chain `arm()` already used in
session_8 (MAVSDK -> forward socket 14541 -> Simulink `UDP Receive`/`Switch` -> serial -> real PX4).

**This was validated against real hardware (session_10, same day) and it broke the session --
reverted, see section 10.4 for the actual fix.** After MATLAB was manually relaunched (a fresh
process, safe per section 6.3) and the section 8 preflight checks confirmed clean (COM4 present,
port 14540 free, no orphaned processes), session_10 ran `main.py --session-id 10` +
`run_hil_automated_session(..., "visualizationType","FlightGear", "sessionId","10")`. Two things
went wrong simultaneously, both traced to the same cause:

1. **MATLAB's own serial connection broke.** `sim()` aborted mid-run with `serialDataSet: WriteFile
   returned ERROR` inside `VehicleSilSimulation/PX4 HITL Interface/MAVLink Bridge Sink`
   (`px4.internal.block.MAVLinkSink`) -- not a `std::terminate()` crash (MATLAB itself stayed up,
   `SimulationStatus` cleanly `"stopped"` afterward), but the sim still aborted.
2. **`main.py` also failed**, with every MAVSDK RPC call after the reboot returning
   `AioRpcError: ... status = StatusCode.UNKNOWN, details = "Unexpected error in RPC handling"` --
   first `dump_params`, then `print_basic_telemetry`'s position stream, which finally killed the
   process with an unhandled exception.

**Root cause of the v1 failure:** the Cube Orange Plus's serial link *is* its USB connection --
`MAV_CMD_PREFLIGHT_REBOOT_SHUTDOWN` resets the flight-controller MCU, which means the USB CDC-ACM
device itself physically disconnects and re-enumerates (confirmed: COM4 reappeared with `Status: OK`
immediately after, per `Get-CimInstance ... Win32_PnPEntity`). Simulink's `MATLABSystem` serial
block had that COM4 handle open *before* the reboot and can't survive the underlying device
vanishing mid-`sim()` -- hence `WriteFile returned ERROR`. Simultaneously, `mavsdk_server` (the
native process MAVSDK's Python bindings talk to over gRPC) doesn't handle its vehicle disappearing
and reappearing mid-session cleanly either -- its internal RPC state stayed corrupted for the rest
of the process's life, well after the MAVLink heartbeat itself had resumed
(`MAVSDK: vehicle connected` / `MAVSDK: autopilot reconnected; settling 3.0s` both printed
successfully -- the protocol-level reconnect worked fine, `mavsdk_server`'s internal bookkeeping did
not).

**The reboot command itself worked correctly** -- this validates the section 10.2 root-cause
hypothesis (PX4 does need a clean boot between sessions) -- **it was just issued from the wrong
place**: mid-session, after Simulink and MAVSDK already had exclusive handles open on the vehicle.

Cleaned up afterward: confirmed COM4 re-enumerated with `Status: OK`, killed the orphaned
`mavsdk_server.exe` left holding UDP 14540 and the stale `python.exe`, confirmed port 14540 free
again -- same section 8 step 2/6 cleanup as every other session.

### 10.4 Fix applied (v2): reboot the autopilot as a standalone step, before Simulink or MAVSDK ever open the port

The v1 code (`reboot_autopilot_and_wait()` in `controller_mavsdk.py`, the call site in `main.py`,
and the `vehicle.reboot_*` keys in `config.yaml`) was **removed** -- it cannot work in this
USB-serial-HITL architecture, not just misconfigured.

**New standalone script: `HILDiagnostics/mavlink_system/reboot_autopilot.py`.** Connects directly to
the board's serial port with a plain `pymavlink.mavutil.mavlink_connection(port, baud=...)` (no
Simulink, no MAVSDK, no UDP relay), waits for a heartbeat, sends
`MAV_CMD_PREFLIGHT_REBOOT_SHUTDOWN`, closes the port immediately (does not wait for an ack -- the
board commonly resets before it can send one), sleeps `--settle-s` (default 15s) for the USB device
to re-enumerate, then reconnects once more to confirm a fresh heartbeat before exiting -- guaranteeing
the port is fully closed and the board is fully booted before anything else touches it. Usage:

```
py -3.12 reboot_autopilot.py --port COM4 --baud 921600
```

**Updated reproduction procedure (supersedes section 8 step 3 ordering):** run
`reboot_autopilot.py` first, standalone, *before* starting `main.py` and *before* running
`run_hil_automated_session.m` -- not concurrently with either. Section 8 has not yet been rewritten
to make this the canonical numbered step; treat this section as authoritative until it is.

### 10.5 v2 validated mechanically (session_11) -- reboot itself is now safe, but a separate bug masked whether it actually fixes arming

Ran `reboot_autopilot.py --port COM4 --baud 921600` standalone first (clean: heartbeat, reboot sent,
15s settle, reconnect confirmed, port closed). MATLAB was a genuinely fresh process (no
`VehicleSilSimulation` loaded, no leftover HITL workspace vars -- confirmed before proceeding).
Preflight checks (COM4 present, port 14540 free, no orphaned processes) all clean. Ran session_11
exactly like session_9/10 otherwise.

**The reboot mechanism itself is now proven safe:** MATLAB completed the full 120s run with no
crash and no serial error (`SimulationStatus` never faulted, no `WriteFile` error) -- the v1 failure
mode from section 10.3 is gone. This confirms the fix belongs where v2 put it (before Simulink opens
the port), not in the live relay.

**But the session still didn't arm, for a different, previously-unseen reason:** `main.py` hung
indefinitely inside `dump_params()` -- `MAVSDK -> PX4: PARAM_REQUEST_LIST` went out at the very
start (10:53:34) and the ~939-parameter response stream apparently never fully completed, so
`get_all_params()` (which has no internal timeout) blocked forever. `run_demo_sequence()` therefore
never even reached `wait_until_ready()`/`arm()` this session -- `main.py`'s console log has zero
`health pending`/`armable` lines, unlike sessions 1-3/8-10. After MATLAB's `sim()` hit its own
120s `StopTime` and closed the serial/UDP relay, the stuck `mavsdk_server`/`main.py` processes just
spammed `Sending message failed` forever into dead air; killed both manually and confirmed port
14540 freed again (same section 8 step 6 cleanup).

**Likely why the param dump stalled:** PX4 spent this entire session repeating
`Preflight Fail: High Accelerometer Bias` / `vertical velocity unstable` roughly every 11-13s for
the *whole* 120s -- unlike session_8's prearm failures, which cleared after ~25s. That's a lot of
sustained `STATUSTEXT` traffic sharing the same 921600-baud serial link with the parameter stream,
which is plausible enough to stall a large unbounded request, but not confirmed as the sole cause.
Also newly seen once, right after reboot: `Preflight Fail: ekf2 missing data` (10:53:34, never
repeated) -- consistent with EKF2 needing to reinitialize from scratch after a genuine cold boot,
which a continuously-running board (as in every prior session) never had to do.

`session_11/matlab_summary.json` shows the same clean ground truth as every prior session:
`accel_mean_mps2 z=-9.784`, zero displacement/velocity, `armed_observed: false` (unsurprising --
`arm()` was never even attempted this time).

**Fix applied:** `dump_params()` in `controller_mavsdk.py` now wraps `get_all_params()` in
`asyncio.wait_for(..., timeout=param_dump_timeout_s)` (new `config.yaml` key, default 30s) so a
slow/incomplete parameter stream can no longer block the rest of the session indefinitely.

### 10.6 Still not validated: does the reboot actually fix arming? Status and recommendation

Three sessions in a row today (9, 10, 11) each surfaced a *different* problem before the original
question -- does a clean-booted PX4 clear prearm checks and arm within a normal preflight window --
could actually be answered. The reboot mechanism itself (10.5) and the crash-causing v1 approach
(10.3) are both now resolved. The `dump_params` hang (10.5) is now fixed in code but **not yet
re-run against hardware**.

Per this document's own established practice (section 6.2: "do not retry a third time without
either that mitigation or explicit sign-off") -- now applied a second time, at three consecutive
imperfect hardware outcomes rather than two -- **stopping here for explicit sign-off before a
session_12 attempt** rather than proceeding automatically. Also worth deciding before the next
attempt: whether to keep `stopTime_s=120`, since session_11's prearm failures never cleared in that
window at all (versus session_8's ~25s), and a genuinely cold EKF2 boot may just need longer than a
warm one did.

### 10.7 session_12: `dump_params` fix confirmed working, but arming still didn't happen -- real bottleneck is `health_timeout_s`, not the reboot mechanism

Investigation note before this session: the user observed the vehicle appears to be sitting in open
ocean in the FlightGear preflight visualization rather than on solid ground. Traced to
`vehicle/common/setUpVehicle.m` lines 17-28 -- the hexarotor's reference position is deliberately
set to "Juancho E. Yrausquin Airport, helipad" on Saba (Dutch Caribbean), `17.64450, -63.21988`, a
real but extremely remote location known for being wedged between cliffs and ocean. Ruled out as a
contributor to the arming problem: the physics uses a flat local NED frame at a fixed reference
altitude (`aircraftInitialPosInNED_m = [0, 0, -referenceAltitude_m]`), not a real terrain/DEM lookup
tied to that lat/lon, and FlightGear here is a one-way display consumer
(`--fdm=null --native-fdm=socket,in,...` in `runFlightGear.m`) that cannot feed anything back into
the simulation. The ocean appearance is almost certainly just missing/uncached `--enable-terrasync`
scenery tiles for a remote island, not a physics or PX4 bug.

Ran `reboot_autopilot.py` standalone (clean), confirmed a genuinely fresh MATLAB process (started
*after* session_11 finished, confirmed via `Get-Process -Name MATLAB | Select StartTime`, not just
an unloaded model), all section 8 preflight checks clean, then session_12 exactly like session_11.

**The `dump_params` timeout fix (10.5) is confirmed working:** `MAVSDK: wrote 939 parameters to
...px4_params_before.txt` printed successfully this time, so `run_demo_sequence()` proceeded past
that point into `wait_until_ready()` -- unlike session_11, which never got that far.

**But the vehicle still never armed.** `wait_until_ready()` ran its full `health_timeout_s` (60s)
polling loop -- `MAVSDK: health pending global=True home=True armable=False` repeating the whole
time, `Preflight Fail: vertical velocity unstable` / `High Accelerometer Bias` recurring throughout
-- and then raised `TimeoutError: MAVSDK: timed out waiting for global/home position health and
armable status after 60.0s`, exactly as session_9 did. `main.py` exited on that exception (traceback,
clean process exit); MATLAB's `sim()` is independent of `main.py` and ran the full 120s regardless,
with `PX4`'s own `GLOBAL_POSITION_INT` still drifting at the very end
(`alt` climbing from ~35.36m toward ~38.3m, `vz` around -0.25 m/s) even though
`session_12/matlab_summary.json` again confirms the injected ground truth was perfectly static and
correct the entire run (`accel_mean_mps2 z=-9.784`, zero displacement/velocity) -- same pattern as
every prior session. Cleaned up the orphaned `mavsdk_server`/`python` afterward, port 14540 freed.

**Reassessing the reboot fix's actual contribution:** across all sessions so far, PX4 clearing its
prearm checks within 60s happened exactly once (session_8, ~25s, on an already-*warm*,
continuously-running board) and failed to clear within 60s three times (session_9, no reboot;
session_11, with reboot, EKF2 never observed at all due to the now-fixed hang; session_12, with
reboot, `wait_until_ready()` ran the complete 60s window). This no longer looks like "a stale/warm
board's leftover bad state blocks arming, so a cold reboot fixes it" -- a **freshly rebooted** board
(session_12) hit the identical persistent-failure pattern as an un-rebooted one (session_9). The
simpler, better-supported explanation at this point: PX4's EKF2/prearm settle time on this bench
setup is genuinely variable and sometimes exceeds 60s regardless of reboot state, and
`vehicle.health_timeout_s` (currently 60s in `config.yaml`) is just too tight -- not that the board's
prior-session state is the (or the only) culprit. The reboot-before-session step (10.4) is still a
reasonable practice to keep (it did prove itself safe, and a genuinely corrupted board state from an
abnormally-ended session, per section 10.2, is still a real possibility worth guarding against) --
but it is not, by itself, a confirmed fix for the arming timeout.

**Not yet tried: simply raising `health_timeout_s`.** No session has yet let `wait_until_ready()` run
past 60s to see whether "vertical velocity unstable"/"High Accelerometer Bias" eventually clear on
their own given enough time, the way section 7 documented once. That is the next, cheaper experiment
before investigating further -- e.g. bump `vehicle.health_timeout_s` in `config.yaml` to 180-300s and
rerun, watching whether the STATUSTEXT failures taper off (real convergence) or continue indefinitely
(a genuinely stuck EKF2 state, which would point back toward section 3's original open question --
the "vertical velocity unstable" check specifically, not accelerometer bias -- and section 5.5's
still-unaddressed lead: whether `HIL_GPS`'s `vd` field or `hexGroundContact` chatter is itself
injecting the instability the check is reacting to).

### 10.8 session_13: `health_timeout_s=240` result -- genuine divergence, not slow convergence; found (and ruled out) a real `pressure_alt` bug along the way

Bumped `vehicle.health_timeout_s` in `config.yaml` from 60s to 240s (kept the reasoning inline as a
comment). Re-ran the full procedure: fresh MATLAB process (confirmed via `Get-Process -Name MATLAB`
start time, not just an unloaded model), `reboot_autopilot.py` clean, preflight checks clean,
`stopTime_s=400` to leave headroom past the new 240s ceiling.

**Result: `wait_until_ready()` ran the complete 240s window and still timed out.** `armable` never
went `True`; `Preflight Fail: vertical velocity unstable` (14x) / `High Accelerometer Bias` (30x)
kept recurring the entire time. More telling: PX4's own `GLOBAL_POSITION_INT.alt` **climbed from
~35.4m to ~60.4m over the session** (`vz` reaching ~-0.77 m/s) while
`session_13/matlab_summary.json` again confirms the injected ground truth was perfectly static the
whole time. **This rules out "just needs more settle time"** -- a slow-to-converge filter would
approach a stable value, not drift monotonically further from the truth the longer it runs. This is
sustained divergence, not slow convergence, and directly contradicts the "stale board state from a
prior session" framing from section 10.2/10.6: session_13 followed a *fresh reboot* (10.4/10.5's v2
script, confirmed clean) and still diverged just as badly as un-rebooted sessions did.

**Investigated further while the arming test ran: found a real, previously-unknown bug in the
`HIL_SENSOR` construction, but ruled it out as the cause.** Exported `session_13`'s logged
`HIL_GPS_bytes`/`HIL_SENSOR_bytes` raw byte streams (Simulink's own `ToWorkspace`-equivalent
signal logs, see `run_hil_automated_session.m`'s `loggingTargets()`) out of MATLAB and decoded them
with `pymavlink`'s own `MAVLink.parse_buffer()` (avoids hand-guessing the wire format). Findings:

- `HIL_GPS.alt` = 35356mm constant, `HIL_GPS.vd` = 0 constant -- both correct, matching ground truth,
  for all 10,009 decoded HIL_GPS frames across the full 400s.
- `HIL_SENSOR.abs_pressure` transitions from 1013.25 hPa (cold-start default) to 1009.01 hPa and
  holds -- physically correct for ~35m elevation via the standard barometric formula.
- **`HIL_SENSOR.pressure_alt` is a constant `0.000` for all 50,001 decoded HIL_SENSOR frames across
  the full 400s**, despite `fields_updated=8191` marking it as a valid/updated field.

Traced this to `PX4HITLConnector.slx`'s (and the inlined "PX4 HITL Interface" in
`VehicleSilSimulation.slx`) `HIL Sensor/HIL_SENSOR Creation subsystem/Bus Assignment1` block: of the
15 total `HIL_SENSOR` fields, `Bus Assignment1` explicitly assigns 14 (`time_usec`, `xacc/yacc/zacc`,
`xgyro/ygyro/zgyro`, `xmag/ymag/zmag`, `abs_pressure`, `diff_pressure`, `fields_updated`,
`temperature`) but never assigns `pressure_alt` -- it silently keeps whatever the
`MAVLink Blank Message1` template block defaults it to (0). This block was copied unmodified from
MathWorks' own `uav_HITL_sample` reference model (per `build_PX4HITLConnector.m`'s header comment),
so this bug predates this repo's own changes.

**But this is very unlikely to be the arming root cause.** Grepped the vendored `PX4-Autopilot`
source: `SimulatorMavlink::handle_message_hil_sensor()`
(`src/modules/simulation/simulator_mavlink/SimulatorMavlink.cpp`) only reads `sensors.abs_pressure`
(converts hPa->Pa) and `sensors.temperature` from the incoming `HIL_SENSOR` message for its baro
handling -- `pressure_alt` is never read anywhere in PX4's sensor-ingestion path (only other repo
hits: the MAVLink XML schema itself, an outbound *telemetry* stream PX4 sends `TO` a GCS, and an
unrelated UAVCAN comment). PX4 computes its own baro altitude internally from `abs_pressure`, which
*is* being sent correctly. **Worth fixing for spec correctness regardless** (a `pressure_alt=0`
paired with `fields_updated` claiming it's valid could still confuse some other consumer, e.g.
QGroundControl's raw sensor view or a different simulator) -- not yet fixed, deferred pending
direction on priority.

**Also traced the actual PX4 arming-check source** (`estimatorCheck.cpp`) to understand what really
drives these two messages, since guessing was no longer productive:
- `"Preflight Fail: High Accelerometer Bias"` fires when EKF2's own internal
  `bias.accel_bias[axis_index]` estimate exceeds `EKF2_ABL_LIM` (`ekf_ab_test_limit`) --
  `checkSensorBias()`, around line 500.
- `"Preflight Fail: vertical velocity unstable"` fires on `estimator_status.pre_flt_fail_innov_vel_vert`
  -- an EKF2-internal innovation-consistency flag (`checkEstimatorStatus()`, line 151), i.e. EKF2's
  own prediction disagreeing with its fused measurement repeatedly, not a raw sensor threshold.

Neither of these is visible in anything currently captured -- `config.yaml`'s
`listener.message_types` does not include `ESTIMATOR_STATUS`, which carries the actual numeric
`vel_test_ratio`/`hgt_test_ratio`/`pre_flt_fail_*` flags driving these checks. Everything inferred
so far about "does it converge or diverge" has been indirect, via `GLOBAL_POSITION_INT` and
`STATUSTEXT` timing. **Next concrete step, not yet done:** add `ESTIMATOR_STATUS` to
`config.yaml`'s `listener.message_types` and rerun, to see the actual test ratios and
`pre_flt_fail_innov_vel_vert`/bias values over time instead of inferring divergence indirectly.

**Status check:** six real-hardware sessions today (8 through 13), one success (8). Stopping here to
report rather than continuing to iterate blindly -- this is a good point for the user to decide
whether to keep going now (add `ESTIMATOR_STATUS` capture, one more session) or pause the hardware
investigation for today.

### 10.9 session_14: `ESTIMATOR_STATUS` reveals a clean, regular sawtooth -- not noise, not slow convergence, a periodic reset-then-drift cycle

Added `ESTIMATOR_STATUS` to `config.yaml`'s `listener.message_types` (already part of PX4's default
MAVLink stream config at 0.5-5Hz across stream profiles -- confirmed by grepping
`mavlink_main.cpp`'s `configure_stream_local("ESTIMATOR_STATUS", ...)` calls, so no PX4-side
change was needed, just capturing what was already being sent). Ran session_14 the same way as
13 (fresh MATLAB, `reboot_autopilot.py` clean, preflight checks clean, `stopTime_s=300`).

**Found a bug while this ran:** `main.py` hung indefinitely after MATLAB's `sim()` ended at 300s.
Root cause: `wait_until_ready()`'s timeout was checked only inside the `async for health in
...health()` loop body -- i.e., only when a *new* health message arrived. When the underlying HITL
link died (Simulink's `sim()` ending closes the serial connection), the stream went silent and the
timeout check never re-fired, so the coroutine hung forever instead of raising after
`health_timeout_s`. Same failure class as the `dump_params` hang (10.5). **Fixed:**
`wait_until_ready()` now wraps the polling loop in `asyncio.wait_for(..., timeout=health_timeout_s)`
(`controller_mavsdk.py`), which enforces a hard wall-clock deadline independent of whether the
stream produces anything. Killed the stuck `mavsdk_server`/`python`, port 14540 freed.

**The data captured before the hang is the clearest signal this entire investigation has produced.**
Parsed all 239 `ESTIMATOR_STATUS` messages from `session_14/console.log`. `vel_ratio` and
`pos_vert_ratio` (the exact fields `estimatorCheck.cpp` compares against
`COM_ARM_EKF_VEL`/`COM_ARM_EKF_HGT` to produce the STATUSTEXT failures) are **not noisy and not
monotonically diverging -- they trace an almost perfectly regular sawtooth**: climb smoothly from
~0 up to the ratio's hard clamp at 2.0 over several seconds, then snap back down near 0 and start
climbing again. Reset points (`pos_vert_ratio` returning to ~0): `11:42:03, :18, :32, :45, :58,
11:43:14, :27, :41, :53, 11:44:07, :20, :33, :46, 11:45:00, :12` -- inter-reset gaps of 12-16s,
consistently clustered around 13-15s, for the entire ~4-minute window captured. `pos_vert_accuracy`
(EKF's own reported uncertainty) tracks the same cycle, tightening right after each reset (~0.10m)
and ballooning during each climb (up to ~0.4m).

**What this rules out:** not random sensor noise (too regular), not a slowly-converging startup
transient (10.7/10.8's hypothesis -- a real convergence would trend toward zero over time, not
repeat an unchanging cycle indefinitely), and not a simple GPS-update-rate artifact (13-15s is far
longer than any normal GPS or baro fusion interval). This is a **deterministic reset-then-drift
cycle**, and its ~13-15s period lines up with the `Preflight Fail` STATUSTEXT recurrence cadence
observed informally across *every* earlier session (session_9, 11, 12, 13 all showed failures
repeating roughly every 7-15s) -- consistent with PX4 itself periodically resetting some part of
its height/velocity estimator state while ungarmed and failing prearm checks, then the estimate
drifting away from truth again during each reset-to-reset window, rather than a one-time startup
issue.

**Not yet determined: what drives the drift *within* each ~13-15s window**, given
`session_14/matlab_summary.json` again confirms the injected ground truth was correct throughout
(static, `accel_mean_mps2 z=-9.785`, small residual chatter `accel_var_mps2_2 z=0.00306` from
`hexGroundContact`, consistent with every prior session). Candidates, not yet distinguished:
(a) this ~13-15s reset cycle is a normal, expected part of PX4's own EKF2 state machine while
grounded/unarmed and would occur even with a perfect sensor feed (i.e. not a bug in this repo at
all) -- would need reading PX4's own `EKF2`/`EKF` source for its reset-trigger conditions to
confirm; (b) the residual `hexGroundContact` accel-Z chatter (5.5 item 3, still never directly
correlated against these reset timestamps) is what each cycle's drift is actually built from;
(c) a timing/lockstep artifact between injected `HIL_SENSOR` timestamps and PX4's internal clock
causing a slow dead-reckoning error that gets corrected each reset. **A pure-SITL run of the
identical scenario** (`CONTROLLER_RUNTIME=1`, already proposed as 9.1.3 for the unrelated
attitude-failsafe investigation) would cheaply discriminate (a) from (b)/(c): if the same ~13-15s
sawtooth appears in SITL with no real hardware involved at all, the cause is shared
model/EKF2-config behavior, not anything specific to this HITL bench.

**Status check:** seven real-hardware sessions today (8 through 14), one success (8), but this is
the first session to produce a quantified, reproducible-shaped signal rather than a guess. Good
stopping point -- next step is either a source-level read of PX4's EKF2 reset logic, or a
pure-SITL comparison run, both of which don't require more real-hardware cycles today.

### 10.10 EKF2 source read: found the real reset mechanisms, definitively ruled out both candidate sensor-side causes -- points to PX4-intrinsic behavior, not this repo's data

Read the vendored `PX4-Autopilot/src/modules/ekf2/EKF/` source (no hardware involved) for what
actually drives a height/velocity reset:

- `Ekf::isHeightResetRequired()` (`ekf_helper.cpp:162`) returns true if either:
  - `continuous_bad_accel_hgt`: `_time_good_vert_accel` timed out against
    `bad_acc_reset_delay_us` = **0.5s** (`common.h:478`) -- driven by
    `estimateInertialNavFallingLikelihood()`/`bad_vert_accel` in `height_control.cpp`, i.e. the
    filter's own accel-based "am I falling" heuristic.
  - `hgt_fusion_timeout`: `_time_last_hgt_fuse` timed out against `hgt_fusion_timeout_max` = **5s**
    (`common.h:450`).
  - When true, `baro_height_control.cpp:127-136` resets both vertical position *and* velocity to
    the current baro/GPS measurement -- explains why `vel_ratio` and `pos_vert_ratio` reset
    together at the same instant, confirmed in 10.9's data.
- Separately, `Ekf::shouldResetGpsFusion()` (`gps_control.cpp:272`) resets GPS-sourced
  velocity/position after `reset_timeout_max` = **7s** (`common.h:448`) of failed horizontal
  aiding.

**None of these constants (0.5s / 5s / 7s) individually equal the observed ~13s period.** Re-derived
the reset timestamps precisely using `ESTIMATOR_STATUS.time_usec` (confirmed to be the same clock
basis as `HIL_GPS`/`HIL_SENSOR`'s own `time_usec`, i.e. directly comparable to Simulink sim-time in
seconds) instead of 1Hz-quantized wall-clock STATUSTEXT timestamps: resets at sim-time
`154.84, 169.83, 183.84, 196.83, 209.83, 225.83, 238.83, ...`, gaps of `15.00, 14.00, 13.00, 12.99,
16.01, 13.00, 26.01(=2x13.00), ...` -- clustering tightly on **exactly 13.00s**, with the 26s gaps
being a single missed detection (2x13), not a different mechanism. This is not "roughly 13-15s
noise", it's precisely, repeatably 13 seconds.

**Directly tested and ruled out both physical-layer candidates using already-captured data, no new
hardware:**

1. **Ground-contact chatter (the section 5.5/10.9 hypothesis): ruled out.** Pulled session_14's own
   logged `SensorsBus_final.INSSensorBus.AccelSensorBus.z_mps2` (Simulink's own signal, sim-time
   indexed) directly from `matlab_session.mat`. Checked 1-second windows around every one of the 13
   reset timestamps found above: **`std=0.00000` at every single one** (constant
   `-9.78507 m/s^2` to 6 decimal places). Checked the *entire* 300s signal: only 32 of 75,001
   samples deviate from the constant value at all, and every one of them is in the first 0.124s of
   the simulation (startup transient settling onto the ground) -- **zero variance for the entire
   rest of the session**. The earlier "`accel_var_mps2_2 z=0.003`" figures in every session's
   `matlab_summary.json` were measuring that one brief 32-sample startup blip diluted across
   50,000-75,000 total samples, not sustained chatter. There is no accelerometer noise anywhere
   near any reset window.
2. **`HIL_GPS` delivery timing/gaps: ruled out.** Re-decoded session_13's `hil_gps.bin` (same
   pymavlink approach as 10.8) and measured inter-message gaps directly from `time_usec`: **9,997 of
   10,008 gaps are exactly 0.040s (steady 25Hz), the other 11 are 0.008s, maximum gap over the
   entire 400s session is 0.040s** -- nowhere close to the 5-7s fusion-timeout thresholds above, and
   no irregularity of any kind.

**Conclusion: the sensor data Simulink sends to PX4 (accelerometer and GPS both) is clean, correct,
and delivered at a steady, expected rate for the entire session -- this rules out a data-quality or
data-timing explanation for the reset cycle a second time, this time with hard numbers instead of a
plausible-sounding hypothesis.** The ~13s sawtooth is very likely intrinsic to how PX4's EKF2 (or
its arming-check retry logic) behaves in this specific state -- HITL, grounded, continuously failing
prearm long enough to keep re-triggering some reset path -- independent of anything this repo's
Simulink model is doing. Not yet confirmed which exact code path produces the emergent 13s figure
(none of the three found constants equal it directly, so it's likely a combination/interaction, e.g.
an accel-bias-estimator adaptation time constant not yet located, or a possibility not yet
considered: this being *designed* PX4 behavior for an ungarmed vehicle that never satisfies arming,
not a "failure" in the EKF2 code sense at all).

**Best next step, still no real hardware required:** run the identical scenario in pure SITL
(`CONTROLLER_RUNTIME=1`, `initVehicleSIL`'s existing `PX4 Interface`/`pixhawk_sil_connector` path --
a simulated PX4 instance, not the physical Cube Orange Plus). If the same ~13s reset cycle appears
with literally the same Simulink sensor feed but no real board involved at all, that confirms this
is PX4/EKF2-intrinsic and not specific to this HITL bench -- and shifts the investigation from "what
is this repo doing wrong" to "is this expected PX4 behavior for a vehicle that sits ungarmed
indefinitely, and if not, which PX4 parameter/config controls it."

## 11. SITL comparison run: conclusive -- the ~13s reset cycle is HITL/hardware-specific, not PX4-intrinsic

Full details, four more real bugs found and fixed along the way, and the raw data are in
**`SITL_VS_HITL_ESTIMATOR_COMPARISON.md`** (new file). Summary:

Built and ran PX4 SITL (`optimAeroHex` target, this repo's own vendored PX4-Autopilot fork, no real
hardware) against the identical `VehicleSilSimulation.slx` model and scenario used for every HITL
session in this document. Captured `ESTIMATOR_STATUS` over a 226-second window via a WSL-side
listener (PX4 SITL's default MAVLink instances only bind to localhost within WSL's own network
namespace, so the listener has to run there, not on the Windows host).

**Result: `vel_ratio` and `pos_vert_ratio` stayed at essentially `0.000` for the entire captured
window -- no sawtooth, no resets, zero `STATUSTEXT` prearm failures of any kind.** PX4 SITL arms
cleanly and immediately with the same data that produces persistent "vertical velocity
unstable"/"High Accelerometer Bias" failures every time against the real Cube Orange Plus.

**This resolves the question section 10.10 left open.** Combined with 10.10's independent finding
(the actual accelerometer and GPS data reaching the real board were confirmed clean and correctly
timed via direct byte-level decoding), the sensor-injection path is now ruled out as a cause twice
over, and PX4/EKF2-intrinsic behavior is now also ruled out (same firmware codebase, same model,
healthy in SITL). **The ~13s reset cycle is specific to something about the real HITL bench** --
most likely something in the real serial link's timing characteristics or a `SYS_HITL`-gated
firmware code path that never activated in this SITL comparison (confirmed real and non-cosmetic:
`voted_sensors_update.cpp` widens sensor-voter timeouts and disables failover detection specifically
when `SYS_HITL=1`, though not obviously in a way that would directly cause this symptom -- see
section 5 of the comparison doc for the concrete next check: enabling `SYS_HITL=1` within SITL
itself to isolate firmware-branch effects from anything physical-hardware-specific, still without
touching the real board).

**Investigation status:** the original question ("why won't the vehicle arm") has been substantially
re-scoped over the course of this document -- from a suspected Simulink sensor-model bug (section 3,
disproven section 5.1), to a stale-board-state hypothesis (section 10.2, partially disproven by
session_12/13 still failing after a clean reboot), to "just needs more settle time" (disproven by
session_13's sustained divergence), to the current, evidence-backed position: **the cause is
somewhere in the interaction between PX4's `SYS_HITL=1` firmware behavior and the real Cube Orange
Plus / serial HITL link specifically**, not in this repo's Simulink model or sensor injection, which
has now been independently confirmed correct at every level checked (physics, accelerometer values,
GPS values and timing, and -- via this SITL comparison -- overall estimator convergence behavior).
