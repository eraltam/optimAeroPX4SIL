<#
.SYNOPSIS
    Launch the WSL relay (if not already running), a fresh PX4 SITL instance, and the
    Windows-side mission harness for a c130JSBSim SIL session -- everything except MATLAB.
    Clone of start_mission.ps1 (c172pJSBSim), pointed at launch_c130_px4_wsl.sh /
    optimAeroC130JSBSim instead -- see PLAN_JSBSIM_SFUNCTION_F22_C130.md Phase F.

.PARAMETER SessionId
    Unique name for this run, e.g. "run1". Logs land in HILDiagnostics\logs\session_<SessionId>.

.PARAMETER Config
    Harness config file (in HILDiagnostics\mavlink_system): config_c130JSBSim.yaml (default,
    straight departure-corridor mission).

.PARAMETER WindowsHost
    This machine's IP as seen from WSL (the vEthernet (WSL) adapter address from `ipconfig`).
    Re-verify after a reboot -- see runbook.

.PARAMETER WslDistro
    WSL distro that has PX4-Autopilot-optimAero checked out under ~/SITLV2.

.EXAMPLE
    .\start_mission_c130.ps1 -SessionId run1
#>
param(
    [Parameter(Mandatory = $true)][string]$SessionId,
    [string]$Config = "config_c130JSBSim.yaml",
    [string]$WindowsHost = "172.31.224.1",
    [string]$WslDistro = "PX4Simulink",
    [switch]$SkipRelay
)

$ErrorActionPreference = "Stop"

$repoRoot   = "C:\Users\Edison Altamirano\SITLV2\AnelloSummer\optimAeroPX4SIL"
$hilRoot    = Join-Path $repoRoot "HILDiagnostics"
$mavlinkDir = Join-Path $hilRoot "mavlink_system"
$sessionDir = Join-Path $hilRoot "logs\session_$SessionId"
$pythonExe  = "C:\Users\Edison Altamirano\AppData\Local\Programs\Python\Python312\python.exe"

# PX4 instance 0, TCP 4560, and the MAVLink ports are process-global resources.
# Refuse to launch over a live mission: the old unconditional stale-PX4 cleanup
# could silently kill a healthy flight started by another terminal/agent.
$activeMissionProcesses = Get-CimInstance Win32_Process | Where-Object {
    ($_.Name -match '^(python|matlab)(\.exe)?$') -and
    ($_.CommandLine -match 'main\.py|run_current_mission_from_env')
}
if ($activeMissionProcesses) {
    $details = ($activeMissionProcesses | ForEach-Object {
        "PID=$($_.ProcessId) $($_.CommandLine)"
    }) -join [Environment]::NewLine
    throw "Another optimAero mission is active; refusing to replace its PX4 instance.`n$details"
}

# Close the startup race before Python/MATLAB exist. Directory creation is atomic.
$missionLockDir = Join-Path $hilRoot ".mission.lock"
try {
    New-Item -ItemType Directory -Path $missionLockDir -ErrorAction Stop | Out-Null
}
catch {
    $owner = Join-Path $missionLockDir "owner.txt"
    $ownerText = if (Test-Path $owner) { Get-Content $owner -Raw } else { "unknown owner" }
    throw "Another optimAero launch owns the mission lock; refusing concurrent startup.`n$ownerText"
}
"session=$SessionId`nvehicle=c130`ncreated_utc=$([DateTime]::UtcNow.ToString('o'))" |
    Set-Content -LiteralPath (Join-Path $missionLockDir "owner.txt") -Encoding UTF8

# Wipe any prior contents so reusing a SessionId (e.g. "run1") always starts from a clean
# session dir -- otherwise leftover files from an earlier, unrelated run (or even a different
# vehicle, since px4_console.log/mavlink_raw_log_*.jsonl names can differ per vehicle config)
# stick around and make the logs confusing to read.
if (Test-Path $sessionDir) {
    Remove-Item -Recurse -Force -Path $sessionDir
}
New-Item -ItemType Directory -Force -Path $sessionDir | Out-Null

# 1) UDP relays. PX4 publishes the normal stream on 18570 (pymavlink/14550)
# and the Onboard stream on 14580 (MAVSDK/14540). Both must cross WSL2.
if (-not $SkipRelay) {
    $telemetryRelay = wsl -d $WslDistro -e bash -lc "ps -eo args | grep -F 'wsl_udp_relay.py' | grep -v -- '--listen-port 14540' | grep -v grep" 2>$null
    if (-not $telemetryRelay) {
        Write-Host "Starting telemetry UDP relay (18570 <-> 14550)..." -ForegroundColor Cyan
        Start-Process -FilePath "wsl.exe" -ArgumentList @(
            "-d", $WslDistro, "-e", "bash", "-lc",
            "`"cd '/mnt/c/Users/Edison Altamirano/SITLV2/AnelloSummer/optimAeroPX4SIL/HILDiagnostics/mavlink_system' && python3 wsl_udp_relay.py --windows-host $WindowsHost`""
        )
    }
    $mavsdkRelay = wsl -d $WslDistro -e bash -lc "ps -eo args | grep -F 'wsl_udp_relay.py' | grep -F -- '--listen-port 14540' | grep -v grep" 2>$null
    if (-not $mavsdkRelay) {
        Write-Host "Starting MAVSDK UDP relay (14580 <-> 14540)..." -ForegroundColor Cyan
        Start-Process -FilePath "wsl.exe" -ArgumentList @(
            "-d", $WslDistro, "-e", "bash", "-lc",
            "`"cd '/mnt/c/Users/Edison Altamirano/SITLV2/AnelloSummer/optimAeroPX4SIL/HILDiagnostics/mavlink_system' && python3 wsl_udp_relay.py --windows-host $WindowsHost --listen-port 14540 --px4-port 14580`""
        )
    }
    Start-Sleep -Seconds 2
}

# 2) Clear any stale PX4 SITL processes left running in WSL from a previous session that wasn't
#    cleanly stopped (e.g. only the harness/PX4 console windows were closed, not Ctrl+C'd) --
#    the leftover build chain (make/cmake/ninja) and/or the px4 binary itself otherwise blocks
#    every subsequent launch with "PX4 server already running for instance 0" and
#    "ninja: build stopped: subcommand failed", while PX4 never actually restarts and MATLAB's
#    TCP 4560 connection just times out against the stuck old instance. `px4_sitl_default` (the
#    build output directory) appears in every stage's command line -- make, cmake --build, ninja,
#    and the final px4 binary invocation -- so one pattern covers the whole chain.
Write-Host "Clearing any stale PX4 SITL processes in WSL..." -ForegroundColor Cyan
wsl -d $WslDistro -e bash -lc "pkill -9 -f 'px4_sitl_default' 2>/dev/null; sleep 1" | Out-Null

# 3) Fresh PX4 SITL instance for this session.
Write-Host "Starting PX4 SITL for session '$SessionId'..." -ForegroundColor Cyan
Start-Process -FilePath "wsl.exe" -ArgumentList @(
    "-d", $WslDistro, "-e", "bash", "-lc",
    "`"bash '/mnt/c/Users/Edison Altamirano/SITLV2/AnelloSummer/optimAeroPX4SIL/HILDiagnostics/launch_c130_px4_wsl.sh' $SessionId $WindowsHost`""
)

# Poll px4_console.log for the "waiting for simulator" line instead of a fixed sleep.
$px4Log = Join-Path $sessionDir "px4_console.log"
Write-Host "Waiting for PX4 to boot (up to 60s)..." -ForegroundColor Cyan
$deadline = (Get-Date).AddSeconds(60)
$ready = $false
while ((Get-Date) -lt $deadline) {
    if (Test-Path $px4Log) {
        $content = Get-Content $px4Log -Raw -ErrorAction SilentlyContinue
        if ($content -match "Waiting for simulator") { $ready = $true; break }
    }
    Start-Sleep -Milliseconds 500
}
if ($ready) {
    Write-Host "PX4 is up and waiting for Simulink on TCP 4560." -ForegroundColor Green
}
else {
    Write-Host "Timed out waiting for PX4's boot message -- check its window manually." -ForegroundColor Yellow
}

# 4) Mission harness (own window so its console.log mirror is visible live).
Write-Host "Starting mission harness (config=$Config)..." -ForegroundColor Cyan
Start-Process -FilePath $pythonExe -ArgumentList @("main.py", "--config", $Config, "--session-id", $SessionId) -WorkingDirectory $mavlinkDir

Write-Host ""
Write-Host "Done. In MATLAB now run:" -ForegroundColor Green
Write-Host "  cd(`"$repoRoot`"); runMissionSILC130(`"$SessionId`")"
Write-Host ""
Write-Host "Add `"visualizationType`", `"FlightGear`" as extra runMissionSILC130 args for the FlightGear view."

# Reset exit status: earlier native calls (e.g. the relay's `pgrep` probe) legitimately return
# non-zero when nothing is found yet, and that stale $LASTEXITCODE would otherwise leak out as
# this script's own exit code even though everything above succeeded.
exit 0
