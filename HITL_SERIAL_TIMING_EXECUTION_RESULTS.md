# HITL Serial Timing Plan Execution Results

Execution date: 2026-07-09

## What Was Executed

Plan file:

- `HITL_SERIAL_TIMING_HYPOTHESIS_TEST_PLAN.md`

Executed first discriminator route:

1. Preflight process checks for UDP 14540/14550 and MATLAB/Python helpers.
2. Standalone autopilot reboot on `COM4` at `921600`.
3. Passive `PassThrough` HITL run without `main.py` / MAVSDK command relay.
4. Post-run MATLAB summary analysis.
5. Direct post-run COM4 telemetry responsiveness check.

## Environment Deviations

- `matlab -batch` could not start. It failed with:
  - `Fatal Startup Error`
  - `System Error: File system inconsistency`
- The 300 second run was therefore executed through the existing MATLAB MCP session, not a fresh `matlab -batch` process.
- A second short probe run through the MATLAB MCP session crashed MATLAB with `std::terminate()` in Simulink mask/block parameter evaluation. Crash dump path reported by MATLAB:
  - `C:\Users\EDISON~1\AppData\Local\Temp\matlab_crash_dump.47552-1`
- The passive listener process did not produce `mavlink_raw_log.jsonl`, so this execution did not capture `ESTIMATOR_STATUS` or `STATUSTEXT` during the 300 second run.

## Completed Session

Session:

- `HILDiagnostics\logs\session_passive_passthrough_exec_01`

MATLAB artifacts:

- `matlab_session.mat`
- `matlab_summary.json`
- `session_report.json`
- `execution_note.txt`

Important `matlab_summary.json` results:

- `CONTROLLER_RUNTIME`: `2` (`HITL`)
- `hitlParams.serialPort`: `COM4`
- `hitlParams.baudRate`: `921600`
- `hitlParams.qgcRemotePortNumber`: `14550`
- Simulation duration: `300 s`
- HIL Sensor samples: `37501`
- HIL GPS samples: `37501`
- HIL Sensor bytes/sample: `77`
- HIL GPS bytes/sample: `51`
- HIL timestamp `min_step`: `8000 us`
- HIL timestamp `max_step`: `8000 us`
- `armed_observed`: `false`
- `px4_controls.nonzero_at_any_point`: `false`
- `max_position_displacement_from_initial_m`: `0`
- `was_airborne_at_any_point`: `false`

Interpretation from completed artifacts:

- Simulink generated the HIL payloads cleanly for the full 300 second passive `PassThrough` run.
- There was no MATLAB-side evidence of arming, actuator output, or simulated vehicle movement.
- This run cannot answer whether the PX4 estimator saw the previous `vel_ratio` / `pos_vert_ratio` sawtooth, because the concurrent MAVLink telemetry log is missing.

## Post-Run COM4 Check

Command:

```powershell
$env:PYTHONPATH=(Resolve-Path '.venv-hil\Lib\site-packages').Path
python HILDiagnostics\live_monitor.py COM4 921600 8
```

Result:

- Board responded on `COM4`.
- `HEARTBEAT`, `SYS_STATUS`, `GPS_RAW_INT`, and `EXTENDED_SYS_STATE` were observed.
- `ESTIMATOR_STATUS`, `ATTITUDE`, `HIGHRES_IMU`, `LOCAL_POSITION_NED`, and `SERVO_OUTPUT_RAW` were stale/not observed during that post-run window.

Interpretation:

- The board remained reachable after the passive run.
- This post-run state is not equivalent to concurrent HITL estimator evidence because Simulink had already released the serial port and stopped feeding HIL data.

## Current Conclusion

The first route was only partially executed.

Confirmed:

- Autopilot reboot path works on `COM4` at `921600`.
- Passive `PassThrough` Simulink HITL generation is clean for 300 seconds.
- No arming/takeoff/actuator activity occurred in the passive run.

Not confirmed:

- Whether passive `PassThrough` fixes the PX4 estimator sawtooth.
- Whether `ESTIMATOR_STATUS.vel_ratio` or `pos_vert_ratio` stayed below threshold.
- Whether `STATUSTEXT` prearm failures disappeared.

The missing discriminator is concurrent MAVLink telemetry capture during the passive run.

## Required Next Step (superseded -- see session_passive_passthrough_02 below)

Before continuing with the rest of the hypothesis matrix, fix the execution harness:

1. Restore a reliable fresh MATLAB execution path.
   - Fix `matlab -batch` startup failure, or use a known-clean MATLAB desktop session after restarting MATLAB manually.
2. Restore concurrent telemetry capture.
   - Verify that a passive listener can bind and receive on the UDP mirror port before starting a long run.
   - If `PassThrough` does not mirror telemetry to UDP 14550, add or configure a second explicit UDP telemetry mirror port and point the listener there.
3. Repeat:

   ```matlab
   run_hil_automated_session("hitlSerialPort","COM4","visualizationType","PassThrough","stopTime_s",300,"sessionId","passive_passthrough_02")
   ```

4. Only after `mavlink_raw_log.jsonl` exists with `ESTIMATOR_STATUS`, decide the route:
   - If estimator ratios are stable, reintroduce command relay and FlightGear one at a time.
   - If estimator ratios still sawtooth, go directly to MathWorks bridge bypass / Python serial bridge.

**Diagnosis of the harness gap:** `visualizationType="PassThrough"` does *not* disable the UDP
telemetry mirror -- `PX4HITLConnector/setUpHITLConnection.m` sets up `hitlParams.qgcRemote*`
(port 14550) unconditionally whenever `controllerRuntime="HITL"`, regardless of visualization type.
The missing `mavlink_raw_log.jsonl` in `passive_passthrough_exec_01` was a listener bug, not a
PassThrough side effect: `main.py` was never truly passive to begin with (it always starts MAVSDK
and runs `connect()`/`dump_params()`/`print_basic_telemetry()` even with `auto_arm`/`auto_takeoff`/
`auto_mission` all `false`), and no other listener was actually bound and confirmed live before the
run started.

**Fix applied:** new `HILDiagnostics/mavlink_system/passive_listener.py` -- reuses the existing,
already-proven `PymavlinkListener` class directly, with `forward_to_mavsdk` and
`relay_commands_to_simulink` both forced `False` regardless of `config.yaml` (zero MAVSDK, zero
outbound traffic of any kind), and blocks in `connect()` until it confirms a live heartbeat before
returning -- so the caller knows capture is actually live before starting the Simulink run (this
was the exact sequencing gap that produced the empty log in `_exec_01`).

## session_passive_passthrough_02: Test 1 PASSES cleanly -- sawtooth eliminated

Ran with `passive_listener.py` started first (confirmed heartbeat before proceeding), then
`run_hil_automated_session("hitlSerialPort","COM4","visualizationType","PassThrough","stopTime_s",300,"sessionId","passive_passthrough_02")`
in a fresh MATLAB process (confirmed via `Get-Process -Name MATLAB` start time), after a clean
standalone `reboot_autopilot.py` reboot and full preflight checks (COM4 present, UDP 14540/14550
free, no orphaned processes).

**Result: `mavlink_raw_log.jsonl` captured cleanly (33.7MB, 300 `ESTIMATOR_STATUS` rows, full
300s+ window). `vel_ratio` and `pos_vert_ratio` stayed at essentially `0.000` for the entire
capture (`max vel_ratio=0.000994`, `max pos_vert_ratio=0.000119`). Zero reset events detected.
Zero `STATUSTEXT` messages of any kind (0 rows) -- no `Preflight Fail`, nothing.** `flags` settled
at a stable `959` (with brief early transients `1152`/`1189` during startup, matching the pattern
already seen in every prior session's first ~10-20s).

**Test 1 passes decisively.** Per the plan's decision route ("Passive PassThrough passes, the root
cause is likely visualization load, command relay traffic, startup interaction, or companion-side
traffic" -- restore one removed variable at a time via Tests 2/3): proceeding to Test 2
(`PassThrough` + normal command relay via `main.py`) to test whether MAVSDK/command traffic alone
reproduces the failure with visualization still removed.

## session_relay_passthrough_01: Test 2 -- FULL MISSION SUCCESS, root cause isolated to FlightGear visualization

Fresh MATLAB restart (required -- Test 1 was already the one `sim()` call in that process), clean
reboot, full preflight checks. Ran `main.py --session-id relay_passthrough_01` (**normal** command
relay path -- `auto_arm`/`auto_takeoff`/`auto_mission` all `true`, exactly as in every prior failing
session) together with
`run_hil_automated_session("hitlSerialPort","COM4","visualizationType","PassThrough","stopTime_s",300,"sessionId","relay_passthrough_01")`.

**Result: complete, clean success end to end.** From `console.log`:

```
MAVSDK: position health is OK and vehicle is armable
MAVSDK: sending arm()          -> COMMAND_ACK result=0, "Armed by external command"
MAVSDK: sending takeoff()      -> COMMAND_ACK result=0, "Takeoff detected"
MAVSDK: takeoff altitude reached rel_alt=2.40 m
MAVSDK: starting waypoint mission -> "Executing Mission"
MAVSDK: mission progress 0/3, 1/3, 2/3, 3/3 -- all waypoints reached
MAVSDK: sending land() -> "Landing detected" -> "landing confirmed ON_GROUND"
MAVSDK: MISSION SUCCESS: takeoff, waypoint mission, and landing completed
```

Zero occurrences of `vertical velocity unstable` or `High Accelerometer Bias` anywhere in the
session (`grep -c` confirms `0`) -- the only `Preflight Fail` message in the entire log is a single
`ekf2 missing data` at the very start (16:48:04, before `MAVSDK: vehicle connected`), matching the
same benign one-time post-reboot transient already seen and dismissed in earlier sessions (e.g.
session_11). `HIL_ACTUATOR_CONTROLS` shows real, evolving, nonzero commands throughout flight, and
`LOCAL_POSITION_NED` shows genuine commanded movement (~20m lateral displacement during the
mission) -- this is a real, physically-coherent flight, not a stuck/faulted state that happened to
avoid the specific STATUSTEXT strings.

**Root cause isolated.** Comparing the three data points now available:

| Visualization | Command relay (`main.py`) | Result |
|---|---|---|
| FlightGear | on | **FAIL** (sessions 9, 11, 12, 13, 14 -- the entire section 10 investigation) |
| PassThrough | off (`passive_listener.py`) | **PASS** (Test 1, this document) |
| PassThrough | on (normal `main.py`) | **PASS** (Test 2, this session) |

Command relay traffic is present in both a passing run (Test 2) and every failing run -- ruled out.
The only variable that changed between every failing session and both passing tests is
**visualization type: `FlightGear` -> `PassThrough`**. This satisfies the plan's Test 3 decision
route in advance ("Fails with FlightGear but passes with PassThrough: visualization or timing load
is a major contributor") without needing to run Test 3 separately, since Test 2 already holds
command relay constant (on) while only visualization changed, and it passed.

**Recommended production fix:** use `visualizationType="PassThrough"` (or profile/fix whatever in
`initVehicleSIL.m`'s `visualizationType="FlightGear"` path perturbs real-time pacing enough to
disrupt the actual HITL serial timing) for any HITL session that needs to reach `arm()`. This
directly explains `initVehicleSIL.m`'s own existing warning
(`"When using Matlab visualization the SIL simulator runs slower than FlightGear..."`) -- FlightGear
was presumably chosen historically as the *faster* visualization option, not realizing that
"faster but not what PX4's HIL lockstep timing actually needs" was itself the problem. Not yet
root-caused *why* FlightGear specifically disrupts timing (options: rendering/TCP-send overhead on
the main Simulink thread stealing cycles from the HITL serial write, or some other pacing
interaction) -- but the practical fix (switch visualization types) is already fully validated by two
independent clean 300s+ runs, one of which flew a complete real mission.
