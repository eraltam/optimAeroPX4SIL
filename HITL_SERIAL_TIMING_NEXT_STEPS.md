# HITL Arming Investigation: Status and Next Steps (Real-Hardware Serial Timing)

**Status:** Paused for hand-off. Every no-hardware avenue has been exhausted; further progress
requires the real Cube Orange Plus bench again.
**Date:** July 9, 2026
**Reads alongside:** `HITL_PREARM_HEALTH_INVESTIGATION.md` (full chronological log, sections 1-11)
and `SITL_VS_HITL_ESTIMATOR_COMPARISON.md` (the SITL comparison test record). This file is the
short "what do we actually know, what do we do next" summary -- read those two for the evidence
behind each claim below.

---

## 1. What's confirmed, in one table

| # | Hypothesis | Status | Evidence |
|---|---|---|---|
| 1 | Simulink sensor-model bug (wrong gravity sign/magnitude, double-counted gravity, etc.) | **Disproven** | `HITL_PREARM_HEALTH_INVESTIGATION.md` section 5.1 -- ground-truth accel matches `-9.81 m/s^2` on Z exactly, `0` on X/Y, across every session logged since |
| 2 | Stale PX4 board state carried over from a prior session (never rebooted between HITL runs) | **Disproven as sole cause** | section 10.2 hypothesis, but session_12/13 (immediately after a confirmed clean `reboot_autopilot.py` reboot) still failed identically -- section 10.7 |
| 3 | "Just needs more settle time" (prearm checks eventually clear given a longer timeout) | **Disproven** | section 10.8 -- `health_timeout_s` raised 60s -> 240s; `GLOBAL_POSITION_INT.alt` *diverged further* over the longer window (35m -> 60m), not converged |
| 4 | Ground-contact numerical chatter or `HIL_GPS` delivery gaps injecting noise into the EKF | **Disproven** | section 10.10 -- direct byte-level decode of what Simulink actually sent: accel-Z flat to 6 decimal places at every reset window; `HIL_GPS` delivered at a rock-steady 25Hz, max gap 40ms |
| 5 | PX4/EKF2 just behaves this way (periodic reset cycle) while grounded and unarmed, regardless of hardware | **Disproven** | section 11 / comparison doc section 5 -- identical model run against SITL (no real board) arms cleanly, `vel_ratio`/`pos_vert_ratio` flat at `~0.000` the entire run, zero `STATUSTEXT` failures |
| 6 | A `SYS_HITL=1`-gated PX4 firmware code branch causes it | **Disproven** | comparison doc section 6 -- `SYS_HITL=1` set explicitly inside the same SITL setup, zero behavioral change from baseline |

**What's actually observed** (`HITL_PREARM_HEALTH_INVESTIGATION.md` section 10.9): against the real
board, `ESTIMATOR_STATUS.vel_ratio`/`pos_vert_ratio` trace an almost perfectly regular sawtooth --
smooth climb to the 2.0 clamp, snap back to ~0, repeat, on a **~13.00 second period**, indefinitely,
never clearing. This is what actually blocks arming (`High Accelerometer Bias` / `vertical velocity
unstable` prearm failures).

**Conclusion: the cause is physical to the real HITL bench specifically** -- something that only
exists when the real Cube Orange Plus and its actual serial link are in the loop, with no software
or firmware-parameter explanation left to rule out.

---

## 2. Candidate next steps (real hardware required)

None of these have been tried yet. Roughly ordered by how cheap/fast they are to test:

1. **Try a different baud rate.** Currently 921600 (`hitlBaudRate` in
   `run_hil_automated_session.m`/`initVehicleSIL.m`). If the ~13s period scales proportionally with
   baud rate (e.g. roughly doubles at 460800), that's strong evidence the cause is serial
   throughput/buffering-related, not a fixed timer. If the period stays ~13s regardless of baud
   rate, that points away from raw throughput and toward a fixed timeout/polling interval somewhere
   in the stack.

2. **Check the Windows USB-CDC serial driver's "Latency Timer" setting** for the Cube Orange Plus's
   COM port (Device Manager -> Ports -> [port] -> Port Settings -> Advanced -> BM Options ->
   Latency Timer). Default is commonly 16ms, sometimes causing USB-serial devices to batch/delay
   small writes in a way that can produce periodic jitter under certain traffic patterns. Cheap to
   test: lower it (e.g. to 1ms) and rerun a session, see if the sawtooth period changes or
   disappears.

3. **Instrument the actual write cadence of `MAVLink Bridge Sink`** (`PX4HITLConnector.slx` /
   `VehicleSilSimulation.slx`'s "PX4 HITL Interface"). Confirm bytes are actually being written to
   the serial port at the expected fixed tick rate with no periodic stalls -- e.g. wrap or trace the
   `MATLABSystem`'s `stepImpl` calls, or capture serial write timestamps directly, and check for any
   ~13s-periodic gap or burst. If Simulink itself is stalling/bursting on that cadence (independent
   of PX4), that would point at MATLAB/Simulink real-time pacing rather than PX4 or the USB stack.

4. **Re-check whether `visualizationType` affects this.** `initVehicleSIL.m` explicitly warns
   `"When using Matlab visualization the SIL simulator runs slower than FlightGear. Recommend
   setting simulink model to accelerator mode."` Every HITL session captured with `ESTIMATOR_STATUS`
   data (9, 11, 12, 13, 14) used `visualizationType="FlightGear"`. Not yet tested: does the sawtooth
   period or its presence change with `visualizationType="Matlab"` in Accelerator mode, or with
   `visualizationType="PassThrough"` (no visualization overhead at all)? If real-time pacing
   consistency is the culprit, removing visualization entirely is the cleanest way to isolate it.

5. **Check for a periodic gap or burst in the `UDP Receive`/`Switch`-gated command-relay path**
   specifically (`PX4HITLConnector.slx` -> `Combine_MAVLink_Streams`'s 4th input, the path added to
   fix the original MAVSDK-command-relay bug in
   `HITL_PREARM_HEALTH_INVESTIGATION.md` section 1). Even though `main.py` isn't actively arming
   during most of a failed session, MAVSDK's connection still round-trips `PING`/`HEARTBEAT`
   messages on this path constantly (visible throughout every session's `console.log`) -- if this
   relay periodically hiccups (e.g. every ~13s), it could plausibly perturb PX4's serial read timing
   even while nominally unrelated to the sensor stream itself.

6. **Check Cube Orange Plus's own hardware clock/timing config** -- e.g. whether `SYS_HITL=1` (or
   HITL mode generally) causes PX4 firmware to rely on a different internal timing source when
   actually running on real hardware (vs. SITL's `posix` clock), and whether that source has any
   known ~13s-scale characteristic (unlikely, but cheap to grep PX4 firmware sources for any
   `hrt_absolute_time`/watchdog-adjacent constant near that magnitude if the above tests don't
   converge on an answer first).

---

## 3. Practical notes carried over from the rest of the investigation

- **Restart MATLAB before every HITL or SITL session that calls `sim()`.** Confirmed for both
  `PX4HITLConnector`'s serial connector and `pixhawk_sil_connector`'s TCP connector -- a second
  `sim()` call in the same MATLAB process reliably hangs or crashes (section 6.3 and comparison doc
  section 4). Always verify via `Get-Process -Name MATLAB | Select StartTime` plus
  `bdIsLoaded('VehicleSilSimulation') == 0` before proceeding, not just the second check alone.
- **Reboot the autopilot standalone before every session**, via
  `HILDiagnostics/mavlink_system/reboot_autopilot.py --port COM4 --baud 921600`, *before* starting
  `main.py` or MATLAB -- never through the live MAVSDK/Simulink relay (breaks the USB connection
  mid-session, section 10.3).
- **Check for orphaned processes/ports before every attempt**: `Get-NetUDPEndpoint -LocalPort 14540`
  and `Get-Process | Where-Object {$_.ProcessName -match 'python|mavsdk'}`.
- `HILDiagnostics/mavlink_system/config.yaml`'s `vehicle.health_timeout_s` is currently `240` (was
  60) -- fine to leave, doesn't affect diagnosis, just gives more observation window per session if
  needed.
- `utilities/getWSLIP.m` is broken on this machine's current WSL networking (looks for adapter alias
  `'vEthernet (WSL)'`, actual name is `'vEthernet (WSL (Hyper-V firewall))'`) -- not yet fixed
  in the file; resolve the IP manually and pass `simHostIP` explicitly if doing any more SITL work
  (`SITL_VS_HITL_ESTIMATOR_COMPARISON.md` section 2).
- The vendored `PX4-Autopilot` fork has no git tags (broke the SITL build's version-string parsing)
  -- worked around with a local-only `git tag v1.15.0`, not pushed anywhere.
