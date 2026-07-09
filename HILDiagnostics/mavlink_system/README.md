# MAVSDK + pymavlink HIL Debug System

This folder separates command and debug responsibilities:

- `MAVSDK` is the flight instructor/controller.
- `pymavlink` is the low-level listener/debug logger.
- `QGroundControl` is optional visual backup.

This checkout is configured for a complete controlled HIL mission: arm, take off, fly the waypoints in `mission.yaml`, and land. Real vehicle commands are still gated by `safety.allow_real_vehicle_commands` in `config.yaml`.

## Install

```bash
pip install -r requirements.txt
```

## Hardware Routing

Use MAVProxy or mavlink-router so each client gets its own UDP port. Example MAVProxy shape:

```bash
mavproxy.py --master=/dev/ttyACM0 --baudrate 57600 \
  --out=udp:127.0.0.1:14540 \
  --out=udp:127.0.0.1:14550 \
  --out=udp:127.0.0.1:14551
```

On Windows, replace `/dev/ttyACM0` with the actual `COM` port if MAVProxy is running on Windows.

Default connections:

- MAVSDK: `udp://:14540`
- pymavlink: `udpin:0.0.0.0:14550`
- QGroundControl: `udp://127.0.0.1:14551`

## Run

### Normal First Run

Use this sequence when you want a repeatable HIL waypoint mission with MATLAB logs and MAVLink logs.

1. Install Python dependencies once:

```powershell
cd C:\AS\optimAeroPX4SIL\HILDiagnostics\mavlink_system
py -3.12 -m pip install -r requirements.txt
```

2. Start the MATLAB HIL simulation from MATLAB:

```matlab
cd C:\AS\optimAeroPX4SIL
sessionDir = run_hil_automated_session("hitlSerialPort","COM5","stopTime_s",360);
```

Change `COM5` to the Cube/Pixhawk COM port shown in Windows Device Manager.

3. In a second PowerShell terminal, start the Python MAVLink system:

```powershell
cd C:\AS\optimAeroPX4SIL\HILDiagnostics\mavlink_system
py -3.12 .\main.py
```

MATLAB writes `HILDiagnostics\logs\latest_session.txt` when it starts. With no `--session-dir`, `main.py` automatically uses that latest MATLAB session folder, so the MAVLink log and copied run config are stored beside `matlab_session.mat`.

The current `config.yaml` enables `auto_arm`, `auto_takeoff`, `auto_mission`, and `allow_real_vehicle_commands` for a controlled HIL run. During a successful run the Python terminal ends with:

```text
MAVSDK: MISSION SUCCESS: takeoff, waypoint mission, and landing completed
```

The session folder should contain:

- `matlab_session.mat`
- `matlab_summary.json`
- `matlab_diary.txt`
- `mavlink_raw_log.jsonl`
- `run_config.yaml`
- `run_mission.yaml`
- `px4_params_before.txt` / `px4_params_after.txt`

4. Once both MATLAB and Python have finished, cross-check the two logs against
   the coordinate and actuator/movement checklists from
   `HIL_MAVSDK_PYMAVLINK_AUTOMATED_DEBUG_PLAN.md` sections 8 and 9:

```powershell
py -3.12 .\analyze_session.py
```

With no `--session-dir`, the analyzer also uses the latest MATLAB session folder. This writes `session_report.json` in the session folder and prints a pass/fail
diagnosis (e.g. "PX4 armed but actuator outputs never moved -- check mixer/
failsafe" vs "HIL connector/wiring" vs "plant dynamics/motor mapping") so an
AI/coding agent can tell where the chain broke without opening
QGroundControl.

### Optional Session IDs

If you prefer stable numbered folders instead of timestamped folders, pass the same id to MATLAB and Python:

```matlab
sessionDir = run_hil_automated_session("hitlSerialPort","COM5","stopTime_s",360,"sessionId","1");
```

```powershell
py -3.12 .\main.py --session-id 1
py -3.12 .\analyze_session.py --session-id 1
```

This uses `C:\AS\optimAeroPX4SIL\HILDiagnostics\logs\session_1`.

### Python Only

If MATLAB is already running and you only want to start the MAVLink side:

```bash
py -3.12 .\main.py
```

Use a session directory if MATLAB is writing logs to the same run folder:

```bash
py -3.12 .\main.py --session-dir ..\logs\session_YYYYMMDD_HHMMSS
```

You can also use the latest MATLAB folder explicitly:

```bash
py -3.12 .\main.py --session-dir latest
```

## Command Safety Gates

The included `config.yaml` is set for a controlled HIL mission run:

```yaml
safety:
  allow_real_vehicle_commands: true
vehicle:
  auto_arm: true
  auto_takeoff: true
  auto_mission: true
```

To return to logging/dry-run mode, set:

```yaml
safety:
  allow_real_vehicle_commands: false
vehicle:
  auto_arm: false
  auto_takeoff: false
  auto_mission: false
```

Only enable real commands in a safe HIL test environment, with props removed if testing on a real drone and with a manual kill switch available.
