# HITL Serial Timing Hypothesis Test Plan

## Objective

Find the smallest reproducible difference between a failing real-board HITL run and a stable run, then route the fix to the correct layer:

- Simulink visualization or pacing
- MAVSDK command relay / companion traffic
- MathWorks MAVLink Bridge serial timing
- Windows USB / physical serial path
- PX4 real-board HIL ingestion / EKF fusion
- PX4 parameters or firmware behavior

The current leading symptom is a recurring estimator failure in HITL on the real Cube Orange Plus path: `ESTIMATOR_STATUS.vel_ratio` and `pos_vert_ratio` climb toward the PX4 failure threshold, then reset/drop on a regular roughly 13 second cycle. The prearm failure reports vertical velocity instability. Treat that as an estimator health failure, not as something to bypass with relaxed arming checks.

## Known Constraints

- The Simulink support package MAVLink Bridge block hardcodes serial baud at `921600`.
- Changing `hitlBaudRate` alone is not a valid baud test unless the underlying bridge implementation is also changed or bypassed.
- `initVehicleSIL.m` already patches the serial port on both MAVLink Sink and MAVLink Source blocks.
- `SYS_HITL=1`, `EKF2_HGT_REF=1`, `EKF2_GPS_CTRL=7`, `USB_MAV_MODE=2`, and `MAV_0_RATE=1200` have appeared in the failing real-board runs.
- Session 8 armed, but it is not a clean success baseline because it lacks `ESTIMATOR_STATUS` and includes early position/movement anomalies.
- `main.py` is not a passive telemetry-only test path. It starts MAVSDK and runs the instructor sequence after the listener connects.

## Controls For Every Run

Use these controls unless the test explicitly changes them.

1. Remove props and restrain the vehicle.
2. Close MATLAB, QGC, Python helpers, MAVSDK server, and any process holding COM4.
3. Check for leftover UDP and helper processes:

   ```powershell
   Get-NetUDPEndpoint -LocalPort 14540
   Get-NetUDPEndpoint -LocalPort 14550
   Get-Process | Where-Object {$_.ProcessName -match 'python|mavsdk|MATLAB'}
   ```

4. Reboot the autopilot before the run:

   ```powershell
   py -3.12 HILDiagnostics\mavlink_system\reboot_autopilot.py --port COM4 --baud 921600
   ```

5. Use the same vehicle, cable, power setup, `config.yaml`, and mission configuration unless the test explicitly changes one.
6. Run long enough to observe at least 3 estimator cycles. Use `stopTime_s=300` unless the run becomes clearly unsafe or clearly failed.
7. Preserve these artifacts for every session:

   - `run_config.yaml`
   - `matlab_summary.json`
   - `console.log`
   - `mavlink_raw_log.jsonl`
   - PX4 params before/after
   - ULog if enabled for that test

## Pass / Fail Measurements

For each run, record:

- Whether the vehicle becomes armable without disabling checks.
- First prearm failure text and timestamp.
- Max `ESTIMATOR_STATUS.vel_ratio`.
- Max `ESTIMATOR_STATUS.pos_vert_ratio`.
- Reset/drop timestamps for those ratios.
- Mean reset period and period range.
- Whether the period scales with load, baud, visualization, or transport changes.
- HIL message timing from `matlab_summary.json`: `hil_timestamp.min_step`, `hil_timestamp.max_step`, HIL Sensor count, HIL GPS count.
- If instrumented, max serial write gap and max serial read gap.

Failure signature for the current bug:

- `vel_ratio` or `pos_vert_ratio` repeatedly approaches or reaches about `2.0`.
- PX4 reports vertical velocity / estimator instability.
- Ratio drops repeat on a roughly regular period.
- Arm remains blocked by estimator health.

## Data Sheet

Create one row per run.

| Session ID | Date | Changed Variable | Visualization | Command Relay | Serial Bridge | Baud | SampleTime | GPS Freq | Port Path | EKF Params | ULog | Reset Period | Max vel_ratio | Max pos_vert_ratio | Prearm Text | Arm Result | Conclusion |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| baseline_01 |  | none | FlightGear | on | MathWorks | 921600 | 0.008 | current | Cube USB COM4 | current | no |  |  |  |  |  |  |

## Recommended Test Order

### 0. Reproduce Baseline

Purpose: confirm the current failure still reproduces under controlled startup.

Setup:

- Use Cube USB `COM4`.
- Use MathWorks MAVLink Bridge.
- Use the current `config.yaml`.
- Use the current default visualization and command path.

Run:

```matlab
run_hil_automated_session("hitlSerialPort","COM4","visualizationType","FlightGear","stopTime_s",300,"sessionId","baseline_01")
```

Expected result:

- If the known failure reproduces, continue.
- If it does not reproduce, repeat once after a cold power cycle. If still clean, the earlier failure may depend on process state, startup order, thermal/power state, or a stale COM/UDP owner.

Interpretation:

- This establishes the reference run for all later comparisons.

### 1. Passive No-Command + PassThrough

Purpose: remove visualization load and remove MAVSDK command traffic at the same time. This gives the cleanest real-board HIL transport baseline.

Setup:

- Use `visualizationType="PassThrough"`.
- Do not run `main.py`.
- Use only a passive telemetry observer. Options:
  - QGC MAVLink Inspector with no mission/actions.
  - A listener-only wrapper around `PymavlinkListener`.
  - A minimal `pymavlink` logger.

Run:

```matlab
run_hil_automated_session("hitlSerialPort","COM4","visualizationType","PassThrough","stopTime_s",300,"sessionId","passive_passthrough_01")
```

Expected result:

- Stable estimator: the root cause is likely visualization load, command relay traffic, startup interaction, or companion-side traffic.
- Same failure: the root cause is below those layers, likely serial bridge, USB path, PX4 HIL ingestion, or EKF fusion.

Interpretation:

- If this fails, do not spend more time on MAVSDK until serial/PX4 paths are isolated.
- If this passes, restore one removed variable at a time using tests 2 and 3.

### 2. PassThrough + Normal Command Relay

Purpose: test whether MAVSDK command relay / companion traffic contributes while visualization remains removed.

Setup:

- Use `visualizationType="PassThrough"`.
- Run the normal command relay path exactly as used in failing runs.

Run:

```matlab
run_hil_automated_session("hitlSerialPort","COM4","visualizationType","PassThrough","stopTime_s",300,"sessionId","relay_passthrough_01")
```

Expected result:

- Fails only when command relay is on: inspect MAVSDK traffic, UDP relay gating, heartbeat rate, command retries, and any repeated stale command packets.
- Still passes: command relay is probably not primary.

Interpretation:

- If command relay causes the failure, keep the Simulink UDP receive gate and add logging for every relayed MAVLink frame length, sequence, message id, and write timestamp.

### 3. FlightGear + No Command Relay

Purpose: test whether visualization load or visualization pacing contributes when command traffic is absent.

Setup:

- Use `visualizationType="FlightGear"`.
- Do not run `main.py`.
- Use only passive telemetry observation.

Run:

```matlab
run_hil_automated_session("hitlSerialPort","COM4","visualizationType","FlightGear","stopTime_s",300,"sessionId","passive_flightgear_01")
```

Expected result:

- Fails with FlightGear but passes with PassThrough: visualization or timing load is a major contributor.
- Passes: visualization alone is not sufficient.

Interpretation:

- If this identifies FlightGear as the trigger, keep PassThrough for HITL validation and profile Simulink execution/wall-clock pacing separately.

### 4. PX4 ULog / Internal EKF Evidence

Purpose: determine whether PX4 sees missing/delayed HIL IMU/GPS, bad height fusion, bad vertical velocity innovation, CPU load, or scheduler stalls.

Setup:

- Enable ULog for the same scenario that reproduces the failure.
- Log at least:
  - `estimator_status`
  - `estimator_innovations`
  - `vehicle_local_position`
  - `vehicle_global_position`
  - `sensor_combined`
  - `vehicle_gps_position`
  - `cpuload`
  - `vehicle_status`

Run:

```matlab
run_hil_automated_session("hitlSerialPort","COM4","visualizationType","PassThrough","stopTime_s",300,"sessionId","ulog_passthrough_01")
```

Expected result:

- ULog timestamps show whether the estimator ratio cycles align with sensor gaps, GPS updates, height reset events, CPU load, or logging/streaming bursts.

Interpretation:

- If ULog shows PX4 input gaps while MATLAB reports clean 8 ms generation, the gap is between Simulink output and PX4 uORB ingestion.
- If ULog shows clean sensor ingestion but bad vertical innovations, focus on EKF source configuration and HIL data consistency.

### 5. MathWorks Bridge Bypass With Python Serial Bridge

Purpose: isolate the MathWorks MAVLink Bridge Sink/Source timing and buffering.

Setup:

- Simulink emits the exact HIL MAVLink frames to UDP instead of writing serial through the MathWorks bridge.
- A Python bridge writes those frames to `COM4` at `921600` and logs:
  - message id
  - frame length
  - receive timestamp
  - serial write start/end timestamp
  - write duration
  - write gap
- The same Python bridge forwards PX4 telemetry back to whatever observer/relay is required.

Run:

```matlab
run_hil_automated_session("hitlSerialPort","COM4","visualizationType","PassThrough","stopTime_s",300,"sessionId","python_bridge_01")
```

Expected result:

- Python bridge passes while MathWorks bridge fails: root cause is the MathWorks bridge implementation, buffering, serial scheduling, or the way the block is called.
- Python bridge fails the same way: root cause is likely PX4, USB path, EKF configuration, or HIL data semantics.

Interpretation:

- If Python bridge fixes it, make the Python bridge the production HITL transport or replace the MathWorks serial block with an instrumented/custom transport block.

### 6. HIL Message Rate / Load Sweep

Purpose: determine whether the estimator cycle scales with HIL message load or queue pressure.

Only run this after baseline reproduction is stable.

Test matrix:

| Session ID | SampleTime | GPS Freq | Expected Meaning |
| --- | --- | --- | --- |
| rate_16ms_gps_current | 0.016 | current | Half IMU rate; should reduce serial load |
| rate_8ms_gps_current | 0.008 | current | Current baseline |
| rate_4ms_gps_current | 0.004 | current | Double IMU rate; should increase load |
| rate_8ms_gps10 | 0.008 | 10 | Lower GPS load |
| rate_8ms_gps50 | 0.008 | 50 | Higher GPS load |

Expected result:

- Reset period scales with message rate or failure worsens at higher load: transport queueing, serial write pacing, PX4 MAVLink ingestion, or CPU load is implicated.
- Reset period unchanged: look harder at EKF fusion timing, PX4 timeout logic, or periodic internal reset behavior.

Interpretation:

- If load scaling is clear, prioritize bridge instrumentation and physical serial path tests.

### 7. True Baud And Physical Serial Path Sweep

Purpose: distinguish USB CDC / driver / physical path problems from firmware or estimator problems.

Important:

- Do not treat `hitlBaudRate` changes as valid unless the actual serial writer baud changes.
- A true baud test requires either patching the support package bridge, using a custom transport, or using the Python serial bridge.

Test matrix:

| Session ID | Physical Path | True Baud | Notes |
| --- | --- | --- | --- |
| usb_cdc_921600 | Cube native USB COM4 | 921600 | Current path |
| usb_cdc_460800 | Cube native USB COM4 | 460800 | Only valid with actual writer change |
| telem_uart_921600 | TELEM port via USB-UART | 921600 | Requires PX4 serial config |
| telem_uart_460800 | TELEM port via USB-UART | 460800 | Requires PX4 serial config |
| other_pc_usb_921600 | Different PC / USB controller | 921600 | Driver/controller discriminator |

Expected result:

- Native USB fails but TELEM UART passes: Windows USB CDC or Cube USB path is implicated.
- Lower true baud passes: burst/driver buffering or PX4 serial ingestion is implicated.
- All physical paths fail the same: move toward PX4/EKF/HIL data semantics.

Interpretation:

- Check Windows latency timer only for adapters that expose it, such as FTDI-style USB-UART devices. Native Cube USB CDC may not expose that option.

### 8. EKF Height Source / Aid Source Sweep

Purpose: test whether the current HIL GPS-height reference path is causing the vertical estimator instability.

Current important values:

- `EKF2_HGT_REF=1` means GPS height reference.
- `EKF2_GPS_CTRL=7` enables GPS aiding modes.

Setup:

- Change one EKF parameter at a time.
- Reboot after changing any parameter marked reboot-required.
- Use QGC or a general parameter-set helper. Do not use `HILDiagnostics\force_set_params.py` blindly because it hardcodes a specific parameter set.

Test matrix:

| Session ID | EKF2_HGT_REF | EKF2_GPS_CTRL | Expected Meaning |
| --- | --- | --- | --- |
| ekf_hgt_gps | 1 | current | Current baseline |
| ekf_hgt_baro | 0 | current | Tests GPS-height dependency |
| ekf_hgt_current_gpsctrl_min | current | reduced/controlled | Tests GPS aiding interaction |

Expected result:

- `EKF2_HGT_REF=0` stabilizes vertical ratios: issue is likely GPS-height fusion, HIL GPS altitude semantics, or GPS/baro consistency.
- No EKF parameter changes affect the cycle: timing/transport/PX4 ingestion remains more likely.

Interpretation:

- A stabilizing EKF parameter is a diagnostic result, not automatically the final fix. The final fix should preserve realistic HITL sensor semantics where possible.

### 9. PX4 Firmware / Scheduler Instrumentation

Purpose: investigate if the failure only exists inside real-board PX4 HIL scheduling or MAVLink ingestion.

Setup:

- Build a PX4 firmware variant with added logging around:
  - MAVLink `HIL_SENSOR` receive timestamp
  - MAVLink `HIL_GPS` receive timestamp
  - uORB publish timestamp for synthetic sensors
  - EKF height reset and velocity innovation failure flags
  - MAVLink RX buffer pressure if available

Expected result:

- Periodic gaps or bursts near the estimator reset period: firmware scheduling/MAVLink RX path.
- Clean ingestion but bad fusion: EKF/HIL data semantics.

Interpretation:

- Only go here after bridge bypass and ULog fail to identify the layer.

### 10. Hardware State / Reboot / Power Sweep

Purpose: check whether the failure depends on board state, cold boot, software reboot, or USB/power conditions.

Test matrix:

| Session ID | Startup | Power | Expected Meaning |
| --- | --- | --- | --- |
| reboot_soft_01 | MAVLink reboot | current | Baseline controlled startup |
| reboot_cold_01 | full power cycle | current | Detect stale board state |
| powered_hub_01 | full power cycle | powered hub | Detect USB power/controller issue |
| battery_usb_01 | full power cycle | normal battery + USB | Detect power-source issue |

Expected result:

- Only cold boot passes: stale board/driver state or startup sequencing.
- Only powered hub / different power passes: electrical or USB controller path.

Interpretation:

- If hardware state changes the result, lock down a deterministic startup procedure before more software tests.

### 11. HIL Payload Content Guard Rails

Purpose: ensure later changes do not accidentally change the payload content and create false conclusions.

For every altered run, compare:

- HIL Sensor frame size and count.
- HIL GPS frame size and count.
- HIL timestamp step min/max.
- GPS altitude and velocity ranges.
- IMU acceleration/gyro ranges.

Expected result:

- Payload contents remain equivalent when the test is only meant to change transport or visualization.

Interpretation:

- If payload contents change unexpectedly, discard the run as a transport/estimator discriminator.

## Decision Routes

Use these routes after the tests.

| Observation | Route |
| --- | --- |
| Passive PassThrough fails like baseline | Skip MAVSDK/FlightGear. Test bridge bypass and ULog. |
| Passive PassThrough passes, command relay fails | Fix MAVSDK traffic, UDP relay gating, command retry behavior, or stale packet repeats. |
| Passive PassThrough passes, FlightGear-only fails | Keep PassThrough for validation; profile Simulink/FlightGear pacing. |
| Python serial bridge passes, MathWorks bridge fails | Replace or bypass MathWorks bridge for HITL serial transport. |
| Python serial bridge fails like MathWorks | PX4 ingestion, USB path, EKF parameters, or HIL data semantics. |
| Failure period scales with SampleTime/GPS frequency | Transport or PX4 MAVLink ingestion load problem. |
| Failure disappears on TELEM UART or other PC | Native USB CDC / driver / physical serial path problem. |
| Failure disappears with `EKF2_HGT_REF=0` | GPS-height fusion or HIL GPS altitude semantics problem. |
| ULog shows PX4 sensor gaps | Path between Simulink generation and PX4 uORB ingestion. |
| ULog shows clean sensor timing but bad vertical innovations | EKF source/data consistency problem. |

## Invalid Or Low-Value Tests

- Changing only `hitlBaudRate` in the current setup and calling it a baud test.
- Disabling prearm checks as a solution.
- Repeating SITL `SYS_HITL=1` as the main discriminator; SITL has already been a negative control.
- Treating session 8 as a clean success baseline without repeating it with `ESTIMATOR_STATUS`.
- Running multiple changes at once after the initial clean passive baseline.

## Recommended First Solution Route

The most direct route is:

1. Run `passive_passthrough_01`.
2. If it fails, run `python_bridge_01`.
3. If the Python bridge passes, replace the MathWorks serial bridge path.
4. If the Python bridge fails, use ULog and physical serial path tests to decide between PX4/EKF and USB/driver causes.

This route avoids wasting time on parameter tuning before proving whether PX4 is receiving cleanly timed HIL data.
