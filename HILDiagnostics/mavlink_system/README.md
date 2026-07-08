# MAVSDK + pymavlink HIL Debug System

This folder separates command and debug responsibilities:

- `MAVSDK` is the flight instructor/controller.
- `pymavlink` is the low-level listener/debug logger.
- `QGroundControl` is optional visual backup.

By default, the system is conservative and does not arm or take off. Real vehicle commands are blocked unless `safety.allow_real_vehicle_commands` is set to `true` in `config.yaml`.

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

Use this sequence when you want a repeatable HIL debug session with MATLAB logs and MAVLink logs.

1. Install Python dependencies once:

```powershell
cd C:\Users\Usuario\AnelloSummer\optimAeroPX4SIL\HILDiagnostics\mavlink_system
pip install -r requirements.txt
```

2. Start the MATLAB HIL simulation from MATLAB:

```matlab
cd C:\Users\Usuario\AnelloSummer\optimAeroPX4SIL
sessionDir = run_hil_automated_session("hitlSerialPort","COM5","stopTime_s",200);
```

Change `COM5` to the Cube/Pixhawk COM port shown in Windows Device Manager.

3. In a second PowerShell terminal, start the Python MAVLink system. Use the `sessionDir` path printed by MATLAB:

```powershell
cd C:\Users\Usuario\AnelloSummer\optimAeroPX4SIL\HILDiagnostics\mavlink_system
python main.py --session-dir "C:\Users\Usuario\AnelloSummer\optimAeroPX4SIL\HILDiagnostics\logs\session_YYYYMMDD_HHMMSS"
```

For the first run, leave `config.yaml` unchanged. The default behavior is telemetry/logging only: no arm, no takeoff, no real flight commands.

The session folder should contain:

- `matlab_session.mat`
- `matlab_summary.json`
- `matlab_diary.txt`
- `mavlink_raw_log.jsonl`
- `run_config.yaml`
- `px4_params_before.txt` / `px4_params_after.txt`

4. Once both MATLAB and Python have finished, cross-check the two logs against
   the coordinate and actuator/movement checklists from
   `HIL_MAVSDK_PYMAVLINK_AUTOMATED_DEBUG_PLAN.md` sections 8 and 9:

```powershell
python analyze_session.py --session-dir "C:\Users\Usuario\AnelloSummer\optimAeroPX4SIL\HILDiagnostics\logs\session_YYYYMMDD_HHMMSS"
```

This writes `session_report.json` in the session folder and prints a pass/fail
diagnosis (e.g. "PX4 armed but actuator outputs never moved -- check mixer/
failsafe" vs "HIL connector/wiring" vs "plant dynamics/motor mapping") so an
AI/coding agent can tell where the chain broke without opening
QGroundControl.

### Python Only

If MATLAB is already running and you only want to start the MAVLink side:

```bash
python main.py
```

Use a session directory if MATLAB is writing logs to the same run folder:

```bash
python main.py --session-dir ..\logs\session_YYYYMMDD_HHMMSS
```

## Enable Real Commands

Edit `config.yaml`:

```yaml
safety:
  allow_real_vehicle_commands: true
```

Also set the requested behavior:

```yaml
vehicle:
  auto_arm: true
  auto_takeoff: true
```

Only enable real commands in a safe HIL test environment, with props removed if testing on a real drone and with a manual kill switch available.
