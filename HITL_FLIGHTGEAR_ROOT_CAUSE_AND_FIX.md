# HITL Arming Blocker: Root Cause Found (`visualizationType`), Fix Validated, What's Still Open

**Status:** Practical fix validated against real hardware (two clean runs, one a complete flight).
**Not** root-caused at the mechanism level -- this is a confirmed workaround, not a confirmed fix of
whatever FlightGear actually does wrong.
**Date:** July 9, 2026

**Reads alongside:** `HITL_PREARM_HEALTH_INVESTIGATION.md` (full chronological investigation,
sections 1-12), `SITL_VS_HITL_ESTIMATOR_COMPARISON.md` (the SITL comparison that ruled out
PX4/firmware causes), `HITL_SERIAL_TIMING_HYPOTHESIS_TEST_PLAN.md` /
`HITL_SERIAL_TIMING_EXECUTION_RESULTS.md` (the test plan that found this). This file is the
short "what do we actually know now" summary.

---

## 1. Bottom line

**Confirmed:** the vehicle arms and flies a complete real mission against the real Cube Orange Plus
when `visualizationType="PassThrough"` is used instead of `"FlightGear"`. Everything else about the
setup -- command relay, MAVSDK, mission upload, EKF2, PX4 parameters -- was held identical to every
prior *failing* session.

**Not confirmed:** *why* `FlightGear` breaks it. No mechanism-level instrumentation was done (no
serial write timestamps captured, no confirmation of which specific FlightGear-side activity is the
culprit). This is a validated correlation with a very clean signal (2/2 passes with `PassThrough`,
5/5 fails with `FlightGear` across sessions 9/11/12/13/14 -- see section 3), not a root-cause fix.

**Not tested:** `visualizationType="Matlab"` against real hardware. Every failing session used
`"FlightGear"` specifically; `"Matlab"` was never actually tried on the real board in this
investigation, only inferred by association (see section 4).

---

## 2. What actually happened, in order

1. Sessions 8-14 (documented in `HITL_PREARM_HEALTH_INVESTIGATION.md`) all used
   `visualizationType="FlightGear"`. Session 8 armed once (~25s settle); sessions 9, 11, 12, 13, 14
   all failed to arm, showing a strikingly regular ~13-second `ESTIMATOR_STATUS.vel_ratio`/
   `pos_vert_ratio` sawtooth (climb to the 2.0 clamp, snap back to ~0, repeat) that never cleared.
2. Every plausible software/data cause was independently ruled out first: the injected Simulink
   sensor data (accelerometer, GPS) was confirmed byte-for-byte clean and correctly timed; a pure
   SITL run of the identical model armed instantly with a perfectly flat estimator; explicitly
   setting `SYS_HITL=1` inside that same SITL run changed nothing. (Full detail: investigation doc
   sections 10-11, comparison doc sections 5-6.)
3. A systematic test plan (`HITL_SERIAL_TIMING_HYPOTHESIS_TEST_PLAN.md`) proposed isolating
   visualization load and MAVSDK command-relay traffic as the two remaining live variables, via a
   *passive* telemetry-only run (no MAVSDK at all) with `visualizationType="PassThrough"`.
4. **Test 1** (`session_passive_passthrough_02`): `PassThrough`, zero MAVSDK/command traffic (new
   `HILDiagnostics/mavlink_system/passive_listener.py`, a truly read-only listener -- `main.py` was
   never actually passive, since it always runs MAVSDK's `connect()`/`dump_params()`/
   `print_basic_telemetry()` regardless of `auto_arm`/`auto_takeoff`/`auto_mission` settings).
   Result: `vel_ratio`/`pos_vert_ratio` flat at `~0.000` for the full 300s, zero `STATUSTEXT`
   failures, zero reset events.
5. **Test 2** (`session_relay_passthrough_01`): `PassThrough` again, but this time with `main.py`'s
   **normal** command relay (`auto_arm`/`auto_takeoff`/`auto_mission` all `true` -- identical
   config to every failing session). Result: **complete mission success** -- armed, took off to
   2.4m, flew all 3 waypoints (`MISSION_ITEM_REACHED` fired for each), loitered, landed,
   `"MISSION SUCCESS: takeoff, waypoint mission, and landing completed"`. Zero
   `vertical velocity unstable`/`High Accelerometer Bias` failures anywhere in the session (only
   the same benign one-time `ekf2 missing data` transient right after reboot, seen in every prior
   session too).
6. Downloaded the actual PX4 onboard flight log (`session_relay_passthrough_01.ulg`, via MAVLink
   FTP -- `python -m pymavlink.mavftp ... get /fs/microsd/log/sess106/log100.ulg`) for independent,
   authoritative confirmation, replayable in QGroundControl's Log Replay / Log Analysis tools.

---

## 3. Why this counts as a real isolation, not a coincidence

| Visualization | MAVSDK/command relay | Result | Sessions |
|---|---|---|---|
| `FlightGear` | on | **FAIL** (persistent ~13s sawtooth, never arms) | 9, 11, 12, 13, 14 |
| `PassThrough` | off (`passive_listener.py`) | **PASS** (flat estimator) | passive_passthrough_02 |
| `PassThrough` | on (`main.py`, normal) | **PASS** (full mission flown) | relay_passthrough_01 |

Command-relay traffic is present in both a failing configuration (row 1) and a passing one (row 3)
-- ruled out as the cause. The only variable that changed between every failing session and both
passing tests is `visualizationType`. This is as clean an isolation as this investigation produced
for anything, including a full physical mission as confirmation, not just a cleared health check.

**One test not run:** the plan's Test 3 (`FlightGear` + *no* command relay) would have closed the
last cell of the 2x2 matrix explicitly. Not done, because Test 2 already held command relay
constant while flipping only visualization, which is sufficient to isolate the variable -- Test 3
would only add confirmation, not new information. Cheap to run later if more certainty is wanted.

---

## 4. What's still missing / not yet answered

**The actual mechanism.** Nothing above says *why* FlightGear disrupts things. Candidate
explanations, none confirmed:
- FlightGear's TCP-send/rendering work runs on (or blocks) the same Simulink execution thread that
  needs to write to the HITL serial port on a tight, consistent cadence, and the resulting jitter is
  just barely enough to perturb PX4's EKF2 innovation checks without being visible in the *payload*
  content checks already done (section 10.10 of the investigation doc checked accelerometer/GPS
  *values*, not serial *write timing*).
- Something more specific to FlightGear's own TCP connection/handshake behavior interacting badly
  with the serial write scheduling -- as opposed to visualization overhead in general.
- `initVehicleSIL.m` itself hints generically at pacing (`"When using Matlab visualization the SIL
  simulator runs slower than FlightGear. Recommend setting simulink model to accelerator mode."`)
  but this is about SITL, written before this HITL-specific failure mode was known, and don't
  conflate the two -- the direction of the effect isn't obviously the same (that warning says Matlab
  viz is *slower* than FlightGear; if raw speed were what mattered, Matlab would be expected to fail
  *worse*, but this was never tested against real hardware to check).

None of `HITL_SERIAL_TIMING_NEXT_STEPS.md`'s deeper instrumentation ideas (actual serial write
timestamp capture, USB latency timer, baud rate scaling) were run, since the practical fix
(switch visualization types) was found first via the higher-level isolation tests and the
investigation stopped there. If the *mechanism* matters (e.g. to fix FlightGear support properly
rather than just avoid it), that instrumentation work is still fully available to do -- see that
file's section 2.

**`visualizationType="Matlab"` against real hardware -- never tested.** Everything failing in this
investigation used `"FlightGear"` specifically. Whether `"Matlab"` visualization has the same
problem, a different problem, or no problem at all is unknown. Don't assume it's safe just because
it isn't FlightGear -- Matlab visualization also does real work (a live 3D animation window) on
what's presumably the same execution thread, so the same class of risk plausibly applies. If a
visualization *is* needed live during a HITL run (rather than relying on QGroundControl, which
receives its own MAVLink telemetry over UDP 14550 regardless of `visualizationType` and needs
nothing from this investigation to keep working), this would need its own isolation test before
trusting it.

**Sample size is small.** Two clean `PassThrough` runs total (one passive, one full mission). Not
yet stress-tested across many repeated sessions the way the *failing* configuration was observed
5 times. Given how mechanically clean both passing runs were (flat estimator, real mission,
matching QGC-downloadable flight log), this reads as a strong result -- but "always passes" hasn't
been established with the same repetition "always fails" was for `FlightGear`.

**Related bugs found along the way, not yet fixed:**
- `utilities/getWSLIP.m` looks for a network adapter literally named `'vEthernet (WSL)'`; on this
  machine it's actually `'vEthernet (WSL (Hyper-V firewall))'` -- breaks `initVehicleSIL`'s
  `simHostIP="auto"` for any future SITL work. Worked around by resolving the IP manually each
  time; the file itself is unpatched.
- The vendored `PX4-Autopilot` fork has no git tags, which broke the SITL build until a local-only
  `git tag v1.15.0` was added (not pushed anywhere). Worth deciding whether to push a real tag.
- `matlab -batch` fails immediately with `Fatal Startup Error: System Error: File system
  inconsistency` (surfaced in `HITL_SERIAL_TIMING_EXECUTION_RESULTS.md`'s first execution attempt).
  Never root-caused -- every session in this investigation used an interactive MATLAB MCP session
  instead, restarted before every single `sim()` call. This blocks any *unattended*/scripted
  execution of further HITL sessions until fixed.
- `HIL_SENSOR.pressure_alt` is a constant `0.0` in every HITL session (confirmed via byte-level
  decode, `HITL_PREARM_HEALTH_INVESTIGATION.md` section 10.10) -- the `HIL_SENSOR Creation
  subsystem`'s `Bus Assignment1` block never assigns that field. Confirmed *not* the arming root
  cause (PX4 doesn't read it), but it's still a real spec-compliance bug worth fixing for any other
  consumer that might read it (e.g. QGroundControl's raw sensor inspector).
- A real typo bug in `ROMFS/px4fmu_common/init.d-posix/airframes/10021_optimAeroHex` (`para
  set-default MPC_THR_HOVER 32` missing the `m` in `param`) was found and fixed during the SITL
  comparison work -- `MPC_THR_HOVER`'s intended default was silently never applied in every SITL
  run before this fix. Not yet confirmed whether this also affected any HITL session (HITL uses the
  real board's already-flashed parameters, not this airframe script, so it's SITL-specific and
  likely irrelevant to the arming investigation -- but worth double-checking if hover-throttle
  behavior ever looks off in a SITL context).

**The separate, still-unresolved section 7.2 issue** (excessive climb rate / attitude-failsafe loop,
first seen in session_8 -- the one session that *did* arm) was never explicitly re-tested here.
`session_relay_passthrough_01`'s clean full-mission flight (no failsafe STATUSTEXT, coherent
attitude/position throughout) is encouraging circumstantial evidence that it didn't recur, but
session_8's own `visualizationType` was never confirmed one way or the other, so no causal claim
should be made connecting that older issue to this fix.

---

## 5. Practical recommendation

- **For any HITL session that needs to actually arm and fly: use `visualizationType="PassThrough"`,
  not `"FlightGear"`.** This is validated, not speculative.
- Live situational awareness during a real HITL flight doesn't require FlightGear -- QGroundControl
  connects to the same UDP 14550 MAVLink mirror `setUpHITLConnection.m` always sets up, independent
  of `visualizationType`, and shows a live map/attitude/telemetry view regardless.
- Treat `visualizationType="Matlab"` as unverified for HITL until it's actually tested against the
  real board the same way `PassThrough` was here.
- `HILDiagnostics/run_hil_automated_session.m` and `HITL_PREARM_HEALTH_INVESTIGATION.md` section 8's
  documented reproduction steps should be updated to default to `PassThrough` for HITL, and should
  say explicitly *why* (not just "avoids a FlightGear dependency," which undersells what's actually
  going on).
- If the underlying mechanism ever needs to be understood (e.g. to restore FlightGear support
  properly rather than just avoid it), start with `HITL_SERIAL_TIMING_NEXT_STEPS.md` section 2's
  instrumentation ideas -- none of that work has been done yet.
