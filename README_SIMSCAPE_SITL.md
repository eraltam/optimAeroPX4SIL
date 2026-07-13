# Tutorial: vehículos Simscape con PX4 SITL

Esta guía explica cómo validar y ejecutar los nuevos adaptadores de vehículos Simscape del
proyecto `optimAeroPX4SIL` usando MATLAB/Simulink en Windows y PX4 SITL dentro de WSL.

> [!IMPORTANT]
> Los adaptadores Simscape actuales compilan dentro del contrato de la planta SIL, pero todavía
> no contienen la dinámica física completa de cada vehículo. Cada modelo incluye una red
> `simscape_adapter_diagnostics` y conserva como base una planta ya estable. La conexión física
> entre comandos PX4, actuadores Simscape y movimiento del vehículo sigue en desarrollo.

## 1. Arquitectura y alcance actual

En una ejecución participan:

```text
PX4 SITL en WSL
       |
       | TCP 4560 / MAVLink
       v
VehicleSilSimulation.slx
       |
       +-- VehiclePlant.slx
       |       +-- un adaptador *_simscape
       |
       +-- sensores y modelo ANELLO
       |
       +-- entorno y visualización
```

El registro selecciona **un vehículo por ejecución**. Esta integración agrega más tipos de
vehículo, pero no ejecuta varios vehículos simultáneamente.

### Vehículos Simscape disponibles

| `vehicleType` en MATLAB | Target PX4 SITL | Modelo de planta |
|---|---|---|
| `evtol_simscape` | `optimAeroEVTOL` | `vehicle/evtol_simscape/evtol_simscape.slx` |
| `tracked_vehicle_simscape` | `optimAeroTracked` | `vehicle/tracked_vehicle_simscape/tracked_vehicle_simscape.slx` |
| `wheel_loader_simscape` | `optimAeroLoader` | `vehicle/wheel_loader_simscape/wheel_loader_simscape.slx` |
| `ackermann_simscape` | `optimAeroAckermann` | `vehicle/ackermann_simscape/ackermann_simscape.slx` |

Se recomienda cerrar primero el lazo con `evtol_simscape` y después probar los demás.

## 2. Requisitos

### Windows y MATLAB

- Windows 10 u 11 con WSL2.
- Ubuntu 22.04 en WSL.
- MATLAB, Simulink y Simscape.
- Aerospace Toolbox.
- Aerospace Blockset.
- UAV Toolbox.
- Instrument Control Toolbox.
- Simulink Test, si se ejecutarán todas las pruebas del repositorio.
- MATLAB Support for MinGW-w64 C/C++/Fortran Compiler.
- UAV Toolbox Support Package for PX4 Autopilots.

En la computadora donde se escribió esta guía se detectó MATLAB R2026a con todos los productos y
paquetes anteriores. La distribución WSL preparada para PX4 se llama `PX4Simulink` y usa Ubuntu
22.04; la distribución predeterminada `Ubuntu` usa 24.04 y no debe usarse para este fork antiguo.
Los modelos originales fueron creados con MATLAB R2024a. R2026a puede mostrar
avisos de actualización; si solo se está validando el sistema, no guardar los modelos actualizados
sin decidir primero si el repositorio migrará oficialmente de versión.

### Ruta usada por esta copia

```text
C:\SITL\AnelloSummer\optimAeroPX4SIL
```

En WSL, la misma carpeta se ve como:

```text
/mnt/c/SITL/AnelloSummer/optimAeroPX4SIL
```

Algunos documentos anteriores mencionan `C:\AnelloSummer`; esa no es la ruta de esta copia.

## 3. Instalar y preparar WSL

### 3.1 Instalar Ubuntu 22.04

Abrir PowerShell **como administrador** y ejecutar:

```powershell
wsl --install -d Ubuntu-22.04
```

Reiniciar Windows si lo solicita. Después, abrir Ubuntu desde el menú Inicio y completar la
creación del usuario Linux.

Comprobar la instalación desde PowerShell:

```powershell
wsl --list --verbose
```

La distribución debe aparecer con versión `2`.

Si aparece con versión `1`, convertirla:

```powershell
wsl --set-version Ubuntu-22.04 2
```

### 3.2 Instalar el toolchain de PX4

Dentro de Ubuntu/WSL (en esta computadora, abrir `PX4Simulink`):

```bash
cd /mnt/c/SITL/AnelloSummer/optimAeroPX4SIL/PX4-Autopilot
./Tools/setup/ubuntu.sh
```

El script puede solicitar la contraseña del usuario Linux. Al terminar, cerrar Ubuntu, ejecutar
`wsl --shutdown` desde PowerShell y volver a abrir Ubuntu:

```powershell
wsl --shutdown
```

### 3.3 Confirmar los archivos PX4

En WSL:

```bash
cd /mnt/c/SITL/AnelloSummer/optimAeroPX4SIL/PX4-Autopilot
for airframe in \
    10025_optimAeroEVTOL \
    10027_optimAeroTracked \
    10028_optimAeroLoader \
    10023_optimAeroAckermann; do
    test -f "ROMFS/px4fmu_common/init.d-posix/airframes/$airframe" || exit 1
done
echo "Airframes Simscape encontrados"
```

El último comando debe imprimir `Airframes Simscape encontrados`.

## 4. Validar las plantas antes de iniciar PX4

Esta prueba no necesita una sesión PX4 activa. Comprueba:

- registro del vehículo;
- selección de la variante de `VehiclePlant`;
- presencia del target y airframe PX4;
- actualización de `VehicleSilSimulation` con las variantes INS 1 y 2;
- simulación de humo de la planta durante un segundo.

### 4.1 Validar primero eVTOL

Abrir MATLAB y ejecutar:

```matlab
cd("C:\SITL\AnelloSummer\optimAeroPX4SIL")
addpath("utilities")

results = runVehicleIntegrationMatrix( ...
    "vehicleTypes", "evtol_simscape", ...
    "insVariants", [1 2], ...
    "runPlantSmoke", true, ...
    "runPx4Build", false);
```

La estructura `results.vehicles` debe indicar `passed = 1`.

Los resultados JSON se guardan en:

```text
work/vehicle_test_results/
```

### 4.2 Validar los cuatro adaptadores

Ejecutarlos individualmente evita que la primera compilación de referencias consuma demasiado
tiempo o memoria en una sola operación:

```matlab
cd("C:\SITL\AnelloSummer\optimAeroPX4SIL")
addpath("utilities")

vehicleTypes = [
    "evtol_simscape"
    "tracked_vehicle_simscape"
    "wheel_loader_simscape"
    "ackermann_simscape"
];

for k = 1:numel(vehicleTypes)
    results = runVehicleIntegrationMatrix( ...
        "vehicleTypes", vehicleTypes(k), ...
        "insVariants", [1 2], ...
        "runPlantSmoke", true, ...
        "runPx4Build", false);

    assert(results.vehicles(1).passed, ...
        "Falló la validación de %s", vehicleTypes(k));
end
```

Pueden aparecer avisos existentes del modelo ANELLO relacionados con nombres de elementos de bus
o extrapolación de presión. Si `passed = 1`, esos avisos no bloquearon esta validación.

## 5. Primera ejecución completa: método manual recomendado

El método manual permite ver claramente cuál lado está esperando y facilita el diagnóstico. Se
utilizan dos ventanas: MATLAB y Ubuntu/WSL.

### 5.1 Inicializar MATLAB y Simulink

En MATLAB:

```matlab
cd("C:\SITL\AnelloSummer\optimAeroPX4SIL")

initVehicleSIL( ...
    "launchFullSIL", false, ...
    "vehicleType", "evtol_simscape", ...
    "controllerRuntime", "SITL", ...
    "visualizationType", "Matlab", ...
    "launchPreflightVisualization", false);

open_system("VehicleSilSimulation")
```

Presionar **Run** en `VehicleSilSimulation.slx`. El modelo puede mostrar que está inicializando o
esperando la conexión en el puerto TCP `4560`. Dejar MATLAB abierto.

### 5.2 Iniciar PX4 en WSL

En una terminal Ubuntu separada:

```bash
cd /mnt/c/SITL/AnelloSummer/optimAeroPX4SIL/PX4-Autopilot
export PX4_SIM_HOSTNAME=$(ip route show default | awk '{print $3}')
make px4_sitl_default optimAeroEVTOL
```

La primera compilación puede tardar varios minutos. Una vez compilado, PX4 debe conectarse al
simulador y mostrar el prompt:

```text
pxh>
```

### 5.3 Confirmar el intercambio de datos

En el prompt `pxh>`, ejecutar uno por uno:

```text
listener sensor_accel
listener sensor_gyro
listener sensor_mag
listener vehicle_attitude
listener vehicle_local_position
listener actuator_outputs
```

Los mensajes deben contener valores finitos y timestamps que se actualicen. Para observar varias
muestras, se puede usar, por ejemplo:

```text
listener sensor_accel 5
```

### 5.4 Cerrar la sesión correctamente

El orden de cierre es importante:

1. Presionar `Ctrl+C` en la terminal PX4/WSL.
2. Detener la simulación en Simulink.
3. Cerrar el modelo si se desea.

Si se intenta cerrar primero Simulink mientras el conector sigue activo, MATLAB puede quedar
bloqueado.

## 6. Ejecutar los demás vehículos

Repetir la sección anterior cambiando `vehicleType` en MATLAB y el target del comando `make`.

### Tracked Vehicle

MATLAB:

```matlab
initVehicleSIL( ...
    "launchFullSIL", false, ...
    "vehicleType", "tracked_vehicle_simscape", ...
    "controllerRuntime", "SITL", ...
    "visualizationType", "Matlab", ...
    "launchPreflightVisualization", false);
open_system("VehicleSilSimulation")
```

WSL:

```bash
make px4_sitl_default optimAeroTracked
```

### Wheel Loader

MATLAB:

```matlab
initVehicleSIL( ...
    "launchFullSIL", false, ...
    "vehicleType", "wheel_loader_simscape", ...
    "controllerRuntime", "SITL", ...
    "visualizationType", "Matlab", ...
    "launchPreflightVisualization", false);
open_system("VehicleSilSimulation")
```

WSL:

```bash
make px4_sitl_default optimAeroLoader
```

### Ackermann

MATLAB:

```matlab
initVehicleSIL( ...
    "launchFullSIL", false, ...
    "vehicleType", "ackermann_simscape", ...
    "controllerRuntime", "SITL", ...
    "visualizationType", "Matlab", ...
    "launchPreflightVisualization", false);
open_system("VehicleSilSimulation")
```

WSL:

```bash
make px4_sitl_default optimAeroAckermann
```

Antes de cambiar de vehículo, detener PX4 y Simulink y volver a ejecutar `initVehicleSIL`.

## 7. Inicio automático después de validar el método manual

Cuando WSL, el target PX4 y el modelo ya funcionen manualmente, se puede solicitar que MATLAB
inicie toda la sesión:

```matlab
cd("C:\SITL\AnelloSummer\optimAeroPX4SIL")

initVehicleSIL( ...
    "launchFullSIL", true, ...
    "vehicleType", "evtol_simscape", ...
    "controllerRuntime", "SITL", ...
    "visualizationType", "Matlab", ...
    "launchPreflightVisualization", false, ...
    "PX4InWSL", false, ...
    "wslDistro", "PX4Simulink");
```

Con `PX4InWSL=false`, `PX4-Autopilot` está en la partición Windows dentro del repositorio actual.
No se debe cambiar a `true` salvo que exista otra copia del repositorio PX4 dentro del directorio
home de Linux. Con `wslDistro="auto"` (valor predeterminado), `initVehicleSIL` prefiere
`PX4Simulink` y después `Ubuntu-22.04`; indicar el nombre explícitamente hace la ejecución
reproducible cuando hay varias distribuciones instaladas.

Si el inicio automático no conecta, regresar al método manual para observar por separado la salida
de Simulink y PX4.

## 8. QGroundControl opcional

QGroundControl no es necesario para la prueba de humo, pero ayuda a observar telemetría, estado de
armado y parámetros.

La configuración histórica del proyecto utiliza una conexión UDP hacia el puerto `18570` de WSL.
La IP actual de WSL puede consultarse con:

```bash
hostname -I
```

Si QGroundControl no muestra el vehículo, primero confirmar que PX4 y Simulink ya se conectaron y
que los comandos `listener` reciben datos. Después revisar el enlace UDP de QGroundControl y las
reglas del firewall de Windows.

## 9. Solución de problemas

### `wsl --list --verbose` indica que no hay distribuciones

Instalar Ubuntu y reiniciar:

```powershell
wsl --install -d Ubuntu-22.04
```

### MATLAB no puede obtener la IP `vEthernet (WSL)`

Primero iniciar Ubuntu. Desde PowerShell, consultar la dirección:

```powershell
Get-NetIPAddress -AddressFamily IPv4 |
    Where-Object InterfaceAlias -Like 'vEthernet (WSL*' |
    Select-Object -First 1 -ExpandProperty IPAddress
```

Luego pasar el valor explícitamente:

```matlab
initVehicleSIL( ...
    "launchFullSIL", false, ...
    "vehicleType", "evtol_simscape", ...
    "simHostIP", "DIRECCION_OBTENIDA", ...
    "visualizationType", "Matlab", ...
    "launchPreflightVisualization", false)
```

### PX4 muestra `Waiting for simulator to accept connection on TCP port 4560`

Esto es normal mientras Simulink todavía no está ejecutándose. Confirmar que:

- se ejecutó `initVehicleSIL` con el mismo vehículo;
- `VehicleSilSimulation.slx` está en ejecución;
- `PX4_SIM_HOSTNAME` apunta al host Windows accesible desde WSL;
- el firewall no está bloqueando MATLAB o el puerto `4560`.

### El target PX4 no existe

Confirmar que el comando se ejecuta en esta copia:

```bash
cd /mnt/c/SITL/AnelloSummer/optimAeroPX4SIL/PX4-Autopilot
```

No usar por accidente otra instalación o clon estándar de PX4, porque no contendrá los targets
`optimAero*` agregados en este repositorio.

### Error de dependencias Python durante la compilación

Volver a ejecutar:

```bash
cd /mnt/c/SITL/AnelloSummer/optimAeroPX4SIL/PX4-Autopilot
./Tools/setup/ubuntu.sh
```

Este fork antiguo de PX4 puede requerir `empy==3.3.4` si aparece un error relacionado con
`em.RAW_OPT`:

```bash
python3 -m pip install --user empy==3.3.4
```

Aplicar esa versión solamente si aparece el error correspondiente.

### Simulink falla después de cambios de modelos o targets

Primero intentar una inicialización que limpie la caché de Simulink:

```matlab
initVehicleSIL( ...
    "launchFullSIL", false, ...
    "vehicleType", "evtol_simscape", ...
    "visualizationType", "Matlab", ...
    "launchPreflightVisualization", false, ...
    "clearSLCache", true)
```

Usar `makeClean=true` solo después de cambiar configuraciones PX4 o si una compilación incremental
quedó inconsistente, porque fuerza una reconstrucción más lenta:

```matlab
initVehicleSIL( ...
    "launchFullSIL", true, ...
    "vehicleType", "evtol_simscape", ...
    "visualizationType", "Matlab", ...
    "launchPreflightVisualization", false, ...
    "makeClean", true)
```

### MATLAB queda bloqueado al cerrar

Detener primero PX4 con `Ctrl+C` y después Simulink. Si MATLAB ya quedó bloqueado, cerrar el proceso
WSL/PX4 antes de intentar finalizar MATLAB desde el Administrador de tareas.

## 10. Criterios mínimos para considerar una ejecución exitosa

- La matriz del vehículo devuelve `passed = 1` para INS 1 y 2.
- `VehiclePlant` completa la prueba de humo de un segundo.
- PX4 carga el airframe correcto y deja de esperar en TCP `4560`.
- `sensor_accel`, `sensor_gyro` y `sensor_mag` se actualizan con valores finitos.
- `vehicle_attitude` y `vehicle_local_position` se publican.
- `actuator_outputs` se publica y llega al lado Simulink.
- PX4 se puede detener con `Ctrl+C` y Simulink cierra sin bloquear MATLAB.

Estos criterios prueban el registro, la compilación y el intercambio básico PX4-Simulink. No
prueban todavía que la dinámica Simscape física responda correctamente a los comandos. Esa prueba
será válida cuando el subsistema diagnóstico sea reemplazado por la planta física conectada.

## 11. Archivos de referencia

- `README.md`: instalación y ejecución histórica del SIL.
- `SIMSCAPE_ADAPTER_HANDOFF.md`: estado actual y trabajo pendiente de los adaptadores.
- `SIMSCAPE_VEHICLE_INTEGRATION_PLAN.md`: plan completo de integración.
- `VEHICLE_INTEGRATION_TEST_RESULTS.md`: evidencia de validaciones anteriores.
- `vehicle/vehicleRegistry.m`: nombres, alias, targets PX4 y modelos registrados.
- `utilities/runVehicleIntegrationMatrix.m`: matriz automatizada de validación.
- `initVehicleSIL.m`: inicialización y lanzamiento del sistema.
