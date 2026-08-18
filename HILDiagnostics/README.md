# Guía Manual del Banco HIL (Cube Orange+ + Simulink)

Guía paso a paso para operar el banco HIL a mano: correr una sesión, flashear firmware, cambiar de
airframe, y qué cosas hay que tener en cuenta para no perder tiempo con problemas ya conocidos.

**Referencias relacionadas:**
- `C:\AS\HIL_ZERO_THRUST_FIX_CONFIRMED_AND_REPLICATION_GUIDE.md` — el fix del bug de thrust-cero y
  cómo reflashear si hace falta de nuevo.
- `C:\AS\HIL_ZERO_THRUST_AND_PARAM_RELIABILITY_NEXT_STEPS.md` — investigación completa, paso a paso,
  de todos los bugs encontrados en este banco.
- `C:\AS\claude_hil_instruction.md` — plan original de implementación del bench.
- `mavlink_system/README.md` — detalle específico del sistema MAVSDK + pymavlink.

---

## 0. Arquitectura en una línea

```
MATLAB/Simulink (VehicleSilSimulation.slx) <--serial COM4--> PX4 real (Cube Orange+)
                                            <--UDP 14550 (mirror)--> pymavlink (main.py)
                                                                          <--UDP 14540--> MAVSDK
```

Simulink habla con el board por serial (HIL_SENSOR/HIL_GPS de ida, HIL_ACTUATOR_CONTROLS de vuelta).
`main.py` (Python) espeja ese tráfico MAVLink a un puerto UDP para que MAVSDK pueda mandar
arm/takeoff/mission y para que pymavlink pueda loguear/inspeccionar todo en texto.

---

## 1. Requisitos antes de empezar

- Board Cube Orange+ conectado por USB, banco seguro (sin hélices si hay motores reales, kill
  switch a mano).
- Python 3.12 en Windows con el venv del proyecto:
  `C:\AS\optimAeroPX4SIL\.venv-hil\Lib\site-packages` (pymavlink, mavsdk, pyserial, pyyaml).
- MATLAB con el path del proyecto (`C:\AS\optimAeroPX4SIL`) y Simulink Coder listo (usa MinGW64).
- Confirmar en el Administrador de Dispositivos de Windows qué número de COM tiene el board
  (en este banco: **COM4** en modo normal).

---

## 2. Cómo correr una sesión HIL (uso normal, día a día)

### Paso 1 — Terminal 1: preflight + reboot + relay

```powershell
cd C:\AS\optimAeroPX4SIL\HILDiagnostics\mavlink_system
$env:PYTHONPATH = "C:\AS\optimAeroPX4SIL\.venv-hil\Lib\site-packages"
py -3.12 run_hitl_session.py --session-id <nombre_sesion> --mode relay --port COM4
```

Esto hace automáticamente, en orden:
1. Confirma que COM4 existe.
2. Libera los puertos UDP 14540/14550 si algún proceso viejo los tiene tomados.
3. Mata procesos huérfanos de `main.py`/`passive_listener.py`/`mavsdk_server.exe`.
4. **Reinicia el autopiloto directamente por serial, antes de que Simulink o MAVSDK toquen el
   puerto** (reiniciar a través de una conexión ya abierta rompe el USB — no lo hagas a mano).
5. Arranca `main.py` (el "relay": conecta MAVSDK, arma, despega, vuela la misión de
   `mission.yaml`, aterriza) y se queda bloqueado corriendo por el resto de la sesión.

Espera a ver en la terminal la línea `MAVSDK: connecting to udp://:14540` antes de tocar MATLAB —
esa línea confirma que el reboot ya terminó y el puerto está libre para Simulink.

**Modo alternativo (`--mode passive`):** solo escucha, no manda ningún comando (ni arma ni
despega). Útil para inspeccionar sin riesgo:
```powershell
py -3.12 run_hitl_session.py --session-id <nombre> --mode passive --duration 300
```

### Paso 2 — MATLAB: arrancar la simulación

```matlab
bdclose('all');   % si este MATLAB ya corrió una sesión HIL antes, mejor reiniciar MATLAB entero
addpath('HILDiagnostics');
addpath(genpath('C:\AS\optimAeroPX4SIL\sensors'));
addpath(genpath('C:\AS\optimAeroPX4SIL\signals'));
addpath(genpath('C:\AS\optimAeroPX4SIL\environment'));
addpath(genpath('C:\AS\optimAeroPX4SIL\PX4HITLConnector'));
addpath(genpath('C:\AS\optimAeroPX4SIL\PX4SILConnector'));
cd('C:/AS/optimAeroPX4SIL');
sessionDir = run_hil_automated_session("hitlSerialPort", "COM4", ...
    "visualizationType", "PassThrough", ...
    "stopTime_s", 300, ...
    "sessionId", "<nombre_sesion>");   % mismo nombre que en el paso 1
```

**`visualizationType` SIEMPRE debe ser `"PassThrough"`, nunca `"FlightGear"`** — FlightGear es la
causa confirmada de fallos de armado en este banco (ver `HIL_FLIGHTGEAR_ROOT_CAUSE_AND_FIX.md`).

`stopTime_s` es cuánto dura la simulación en segundos. Usa algo generoso (300s+) si vas a correr
una misión completa; algo corto (60-100s) si solo quieres una sonda rápida.

### Paso 3 — Revisar resultados

Todo queda en `HILDiagnostics/logs/session_<nombre_sesion>/`:

| Archivo | Qué tiene |
|---|---|
| `console.log` | Todo lo que imprimió `main.py`: conexión, arm/takeoff, telemetría, errores |
| `matlab_summary.json` | Resumen automático: `armed_observed`, `actuator_nonzero_after_arm`, `was_airborne_at_any_point`, ruido IMU, etc. — el primer archivo a mirar para saber si "funcionó" |
| `matlab_session.mat` | Todas las señales logueadas de Simulink (para análisis en profundidad) |
| `mavlink_raw_log.jsonl` | Cada mensaje MAVLink crudo, línea por línea |
| `takeoff_gate_probe_result.txt` | Sonda automática de `vehicle_constraints`/`trajectory_setpoint`/`flight_mode_manager` en la ventana de takeoff (ver sección 5) |
| `run_config.yaml` / `run_mission.yaml` | Copia de la config y la misión usadas en esa sesión |

Búsqueda rápida de éxito/fracaso:
```powershell
Select-String "MISSION SUCCESS|Disarmed by auto preflight|timed out" .\logs\session_<nombre>\console.log
```

### Paso 4 — Descargar el log de a bordo (opcional, después de que termine la sesión)

```powershell
cd C:\AS\optimAeroPX4SIL\HILDiagnostics\mavlink_system
$env:PYTHONPATH = "C:\AS\optimAeroPX4SIL\.venv-hil\Lib\site-packages"
py -3.12 download_latest_log.py "..\logs\session_<nombre>\board_log.ulg" "serial://COM4:921600"
```

El relay UDP (14550) solo funciona *durante* una sesión activa — para bajar el `.ulg` después hay
que conectarse por serial directo.

---

## 3. Cómo flashear firmware nuevo

Necesario cuando: cambiaste código de PX4, cambiaste el board config (`.px4board`), o el firmware
actual no tiene los módulos que necesitás (ver sección 6.1 para el caso ya confirmado de este banco).

### Paso 1 — Compilar en WSL

```bash
wsl.exe -d PX4Simulink -e bash -lc "cd /home/edison/PX4-Autopilot && make <target>"
```

`<target>` sigue el patrón `<vendor>_<board>_<config>`, ej. `cubepilot_cubeorangeplus_hil_hex`
(el config correcto para este hexarotor, ver sección 6.1). **Compilar siempre desde una ruta
nativa de WSL** (`/home/edison/PX4-Autopilot`, no `/mnt/c/Users/... /PX4-Autopilot` con espacios)
— el Makefile de NuttX se rompe con espacios en la ruta.

El resultado queda en:
`/home/edison/PX4-Autopilot/build/<target>/<target>.px4`

### Paso 2 — Copiar el `.px4` y el uploader a Windows

```bash
wsl.exe -d PX4Simulink -e bash -lc "
cp /home/edison/PX4-Autopilot/Tools/px_uploader.py \
   /home/edison/PX4-Autopilot/build/<target>/<target>.px4 \
   '/mnt/c/AS/optimAeroPX4SIL/HILDiagnostics/firmware/'
"
```

### Paso 3 — Cerrar todo lo que tenga el puerto abierto

Antes de flashear, asegurate de que nada tenga COM4 abierto: cierra `main.py`,
`run_hitl_session.py`, cualquier sesión MATLAB con HIL corriendo. Revisa procesos huérfanos:

```powershell
Get-CimInstance Win32_Process | Where-Object { $_.CommandLine -match 'main\.py|run_hitl_session\.py' } |
  Select-Object ProcessId,CommandLine
```

Si aparece algo, mátalo con `Stop-Process -Id <pid> -Force`.

### Paso 4 — Flashear

```powershell
cd C:\AS\optimAeroPX4SIL\HILDiagnostics\firmware
$env:PYTHONPATH = "C:\AS\optimAeroPX4SIL\.venv-hil\Lib\site-packages"
py -3.12 px_uploader.py --port COM3,COM4 --baud-flightstack 921600 <target>.px4
```

**Muy importante:** pasar **ambos** puertos como lista (`COM3,COM4`), no solo uno. El board
aparece en un número de COM distinto según el modo (bootloader vs. normal) — si solo le das el
puerto "normal", el uploader se queda pegado para siempre reintentando abrir un puerto que ya no
existe. Con ambos puertos ofrecidos, encuentra el que esté activo en cada momento.

Salida esperada:
```
Found board id: ... on COM3
Erase  : [====================] 100.0%
Program: [====================] 100.0%
Verify : [====================] 100.0%
Rebooting. Elapsed Time XX.X
```

Espera ~10-15s a que el board reinicie con el firmware nuevo antes de usarlo.

### Paso 5 — Verificar que el flash funcionó (sin armar, seguro)

Cualquier chequeo por `listener`/`param show`/`cat` sobre el shell NSH (vía MAVLink `SERIAL_CONTROL`)
es de solo lectura y no arma nada:

```powershell
cd C:\AS\optimAeroPX4SIL\HILDiagnostics
py -3.12 probe_boot_config.py COM4 921600
```

Confirmá:
- `param show SYS_AUTOSTART` → el airframe que esperás (ej. `1003`)
- Los archivos `rc.*` que tu airframe necesita existen (`cat /etc/init.d/rc.mc_defaults` no debe
  dar "No such file or directory" si es un airframe multirrotor puro — ver sección 6.1)

---

## 4. Cómo poner otro airframe

### 4.1 Airframes disponibles en este firmware

```
1001_rc_quad_x.hil          — quad genérico
1002_standard_vtol.hil       — VTOL estándar
1003_optim_aero_hex.hil      — hexarotor de este proyecto (el que se usa normalmente)
1100_rc_quad_x_sih.hil       — quad SIH (simulación interna, no HITL real)
1101_rc_plane_sih.hil        — avión ala fija SIH
1102_tailsitter_duo_sih.hil  — tailsitter SIH
```

Cada uno corresponde a un valor de `SYS_AUTOSTART` (el número al principio del nombre de archivo).

### 4.2 Cambiar el airframe activo

**No uses el picker de airframes de QGroundControl ni la página de Parameters de QGC para esto** —
confirmado en este banco (2026-07-06): el combo-box de QGC para `SYS_AUTOSTART` solo conoce los
airframes de su propia lista bundleada; un ID custom como `1003` no se escribe de forma confiable
aunque parezca que sí en la UI.

**Forma que sí funciona: escribir el parámetro directo por MAVLink.**

1. Editá `HILDiagnostics/force_set_params.py` — el diccionario `TO_SET` tiene los params
   específicos del airframe 1003 (`CA_ROTOR_COUNT`, `HIL_ACT_FUNC1-6`, etc.). Si cambiás a otro
   airframe, ajustá `SYS_AUTOSTART` al nuevo número y los demás params según lo que ese airframe
   necesite (mirá el propio archivo `.hil` del airframe en
   `PX4-Autopilot/ROMFS/px4fmu_common/init.d/airframes/` para saber qué params usa).

2. Con el board conectado y **nada más usando el puerto**, corré:
   ```powershell
   cd C:\AS\optimAeroPX4SIL\HILDiagnostics
   $env:PYTHONPATH = "C:\AS\optimAeroPX4SIL\.venv-hil\Lib\site-packages"
   py -3.12 force_set_params.py COM4 921600
   ```
   Esto escribe cada param, verifica el `readback`, y si todo coincide reinicia el vehículo solo.

3. Confirmá con:
   ```powershell
   py -3.12 preflight_check.py COM4 921600
   ```
   Debe decir `SYS_AUTOSTART = 1003` (o el que pusiste) con `PASS`, y `OVERALL: PASS`.

### 4.3 Crear un airframe nuevo desde cero

1. Crear el archivo `.hil` en `ROMFS/px4fmu_common/init.d/airframes/<numero>_<nombre>.hil`
   (copiar la estructura de uno existente como plantilla — típicamente empieza sourceando
   `rc.mc_defaults`/`rc.fw_defaults`/`rc.vtol_defaults` según el tipo de vehículo, seguido de una
   lista de `param set-default`).
2. Agregarlo a `ROMFS/px4fmu_common/init.d/airframes/CMakeLists.txt` en la lista de archivos
   `.hil` (si no está ahí, el build no lo incluye en el ROMFS aunque el archivo exista).
3. Recompilar y reflashear (sección 3).
4. Poner `SYS_AUTOSTART` al nuevo número (sección 4.2) y reiniciar.

---

## 5. Qué tener en cuenta (lecciones aprendidas en este banco)

- **`visualizationType` debe ser `"PassThrough"`, nunca `"FlightGear"`.** Causa confirmada de
  fallos de armado.
- **Nunca reiniciar el board a través de una conexión MAVSDK/Simulink ya abierta.** Rompe el
  enumerado USB a mitad de sesión. Siempre reiniciar standalone antes (ya lo hace
  `run_hitl_session.py` automáticamente).
- **El board cambia de número de COM entre modo bootloader y modo normal.** Cualquier script que
  hable con el board (flasheo, reboot) debería tolerar ambos, o al menos volver a chequear la
  lista de puertos si algo falla con "no se encuentra el puerto".
- **Nunca abras un segundo proceso escuchando en el mismo puerto UDP `14550`** mientras `main.py`
  ya tiene un listener corriendo ahí. Confirmado que esto le roba/duplica el tráfico al primero y
  puede tirar abajo toda la conexión (ningún heartbeat llega, el vehículo ni arma). Si necesitás
  una sonda extra durante una sesión, integrala en el proceso de `main.py` (reusar el mismo socket
  vía `PymavlinkListener.send_shell_command()`/`drain_shell_output()`), no un script aparte.
- **`main.py`'s `dump_params()` está deshabilitado a propósito** (comentario con fecha en
  `main.py`): causaba una tormenta de `PARAM_REQUEST_READ` que colgaba `set_takeoff_altitude()`
  indefinidamente. No lo reactives sin arreglar antes el manejo del timeout (ver
  `HIL_ZERO_THRUST_AND_PARAM_RELIABILITY_NEXT_STEPS.md` sección 1.3).
- **PX4 se autodesarma solo ~10s después de armar si nunca despega** (`COM_DISARM_PRFLT`, default
  10.0s). Si estás probando algo que necesita una ventana armada más larga, subilo temporalmente
  (`param set COM_DISARM_PRFLT <segundos>`) y **revertilo después** — es un parámetro de seguridad
  real, no lo dejes alto en una sesión con riesgo real de actuación.
- **Los parámetros (incluyendo `SYS_AUTOSTART`) sobreviven un reflasheo de firmware** — se guardan
  en un sector de flash separado. No hace falta reconfigurar todo después de reflashear, pero
  siempre conviene confirmar con `preflight_check.py`.
- **No usar `VTOL.px4board` como base para un airframe multirrotor puro** — aunque tenga las
  banderas de simulación HIL activadas, no trae `rc.mc_defaults`/`rc.mc_apps` (usa su propia
  cadena `rc.vtol_apps`), y por eso el thrust se queda en cero para siempre en un airframe MC
  normal. Usar siempre una base `multicopter.px4board` para vehículos que no sean VTOL (ver
  `hil_hex.px4board`, ya resuelto — detalle completo en
  `HIL_ZERO_THRUST_FIX_CONFIRMED_AND_REPLICATION_GUIDE.md`).
- **`INS_VARIANT`** (en `sensors/setUpSensors.m`, actualmente `2`) y **`WIND_SOURCE`** (en
  `environment/setUpEnvironment.m`, actualmente `2`) son los dos interruptores de fidelidad de
  sensor/entorno de este proyecto: `INS_VARIANT`: `1`=IMU genérica, `2`=modelo ANELLO X3
  datasheet-accurate. `WIND_SOURCE`: `1`=viento sintético (Dryden), `2`=viento real replay de un
  `.ulg`. Cambialos ahí si necesitás la config genérica/sintética para aislar un problema.

---

## 6. Diagnóstico rápido / scripts útiles (todos en `HILDiagnostics/`)

| Script | Para qué |
|---|---|
| `run_hitl_session.py` | Arranca una sesión completa (preflight + reboot + relay), ver sección 2 |
| `probe_boot_config.py <COM> <baud>` | Sonda de solo lectura del estado de boot: `pwm_out_sim status`, airframes presentes, `param show SYS_AUTOSTART`, `dmesg` |
| `probe_takeoff_gate.py` | Sonda manual (para correr *durante* una sesión activa) de `vehicle_constraints`/`trajectory_setpoint`/`flight_mode_manager` — hoy integrado automáticamente en `main.py`'s `takeoff()`, este script queda para pruebas ad-hoc |
| `mavlink_shell_probe.py` | Cliente de shell MAVLink no interactivo (`listener <topic> -n 1`, etc.) — base de los demás scripts de sonda |
| `probe_top.py` | `top once` — lista de tareas/workqueues corriendo, útil para confirmar que un módulo no crasheó al boot |
| `force_set_params.py` | Fuerza params por MAVLink cuando la UI de QGC no confía (ver sección 4.2) |
| `preflight_check.py` | Verifica el subset estático de params esperados antes de una sesión |
| `reboot_autopilot.py` | Reinicia el board standalone, por serial, antes de que nadie más toque el puerto |
| `zero_ekf2_leverarm.py` | Fuerza `EKF2_IMU_POS_*`/`EKF2_GPS_POS_*` a cero (diagnóstico, actualmente aplicado en el board — ver el doc de investigación completo para revertirlo) |
| `set_gps_check.py` | Fuerza `EKF2_GPS_CHECK=1023` (fix permanente ya aplicado también en el archivo del airframe) |
| `mavlink_system/analyze_session.py` | Cruza los logs de MATLAB y MAVLink de una sesión y da un diagnóstico pass/fail |
| `mavlink_system/download_latest_log.py` | Baja el `.ulg` de a bordo después de que termine una sesión |

**Nota de PowerShell:** rutas de dispositivo Windows crudas (`\\.\COM4`) se rompen en Git-Bash
(pierde una barra). Usá la tool de PowerShell, no Bash, para cualquier script que tome un argumento
de ese estilo.

---

## 7. Cómo reproducir la matriz de 4 sesiones (misión reconstruida, IMU × viento)

Esto documenta cómo se corrieron las sesiones `session_wind_matched_generic_01` (S1),
`session_wind_synth_generic_01` (S2), `session_wind_matched_anello_01` (S3),
`session_wind_synth_anello_01` (S4) — la comparación real-vs-HIL con la misión de 55
waypoints reconstruida de `nav_hil_ground_truth.ulg` (ver `PLAN_RECONSTRUCCION_MISION_HIL_Y_COMPARACION_4VIAS.md`
y `AnalysisIMU/four_way_wind_comparison/`). Sirve para volver a correr la matriz completa o
una sesión suelta más adelante.

### 7.1 Configuración ya persistida (no hace falta rehacerla, solo confirmar)

- `mavlink_system/config.yaml`:
  - `vehicle.mission_file: C:\AS\AnalysisIMU\mission_reconstruction\run_mission_reconstructed_from_real.yaml`
    (55 waypoints, no el `mission.yaml` de 19 waypoints original).
  - `vehicle.connect_timeout_s: 600` (subido de 180s — esta misión tarda más en compilar en
    Simulink que el modelo de build/compile que asumía el timeout viejo).
  - `safety.max_mission_altitude_m: 2400.0` (la misión reconstruida usa altitudes relativas
    reales de hasta ~2280m en el campo `relative_altitude_m`; no es una altura física real
    del banco, es solo el límite de validación de contenido de misión).
- `run_mission_reconstructed_from_real.yaml`: `mission_timeout_s: 1200`, `land_timeout_s: 150`
  (subidos con margen de seguridad de hardware respecto a los defaults, ver plan sección Fase B).

### 7.2 Los dos interruptores que definen cada sesión de la matriz

| Sesión | `INS_VARIANT` (`sensors/setUpSensors.m`) | `WIND_SOURCE` (`environment/setUpEnvironment.m`) |
|---|---|---|
| S1 — generic + viento real | `1` | `2` (replay real desde `.ulg`) |
| S2 — generic + viento sintético | `1` | `1` (Dryden + gust) |
| S3 — ANELLO + viento real | `2` | `2` |
| S4 — ANELLO + viento sintético | `2` | `1` |

Editar esos dos archivos (una línea cada uno) **antes** de arrancar el paso 2 de la sección 2
(MATLAB), para cada sesión de la matriz. `INS_VARIANT`/`WIND_SOURCE` no son argumentos de
`run_hil_automated_session` — son variables de workspace que esos dos scripts `setUp*.m`
definen, así que el cambio tiene que estar guardado en el archivo antes de que
`initVehicleSIL`/`run_hil_automated_session` los ejecute.

### 7.3 Procedimiento por sesión

1. Editar `INS_VARIANT` y `WIND_SOURCE` según la tabla de 7.2.
2. Seguir la sección 2 normal (Terminal 1: `run_hitl_session.py --session-id <nombre_de_la_tabla>`,
   Terminal 2: MATLAB `run_hil_automated_session(..., "sessionId", "<mismo_nombre>")`). Usar
   `stopTime_s` generoso (esta misión completa tarda ~550-580s de vuelo real, más el tiempo de
   preflight/arm — 900s de margen es lo que se usó).
3. **Reiniciar MATLAB por completo entre sesiones** (`bdclose('all')` no es suficiente para
   garantizar que no queda estado viejo de Simulink — se perdió una corrida completa así una vez,
   ver `matlab_session.mat` de la sesión y confirmar `armed_observed: true` /
   `was_airborne_at_any_point: true` en `matlab_summary.json` antes de dar la sesión por buena).
4. Confirmar éxito: `Select-String "MISSION SUCCESS" .\logs\session_<nombre>\console.log` y
   `mission_result.seq_reached == mission_result.seq_total` al final.
5. Bajar el log de a bordo (paso 4 de la sección 2) — necesario para el análisis de innovaciones
   EKF (Fase C.6), que solo está en el `.ulg` descargado de la placa, no en el relay en vivo.
6. Repetir para las 4 combinaciones. Al terminar, `INS_VARIANT`/`WIND_SOURCE` quedan en lo último
   que se haya corrido — revisar antes de asumir cuál es la config "activa" para una sesión nueva.

### 7.4 Reanálisis

Los scripts de extracción/alineación/comparación de las 4 sesiones están en
`C:\AS\AnalysisIMU\four_way_wind_comparison\scripts\` (`build_manifest_4way.py`,
`export_simulink_sessions_4way.m`, `fase_c2_events_and_alignment.py` ... `fase_c9_trajectory_figures.py`),
pensados para correrse en orden sobre las 4 carpetas de sesión nuevas. Ver
`AnalysisIMU/four_way_wind_comparison/comparison_report_4way.md` para el resumen de resultados y
`C:\AS\ANELLO_X3_HIL_vs_RealGroundTruth_Summary.tex` para la versión concisa con tablas/figuras.
