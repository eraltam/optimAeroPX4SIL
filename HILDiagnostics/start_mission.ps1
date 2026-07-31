<#
.SYNOPSIS
    Launch the WSL relay (if not already running), a fresh PX4 SITL instance, and the
    Windows-side mission harness for a c172pJSBSim SIL session -- everything except MATLAB.

.PARAMETER SessionId
    Unique name for this run, e.g. "run1". Logs land in HILDiagnostics\logs\session_<SessionId>.

.PARAMETER Config
    Harness config file (in HILDiagnostics\mavlink_system): config_c172pJSBSim.yaml (straight),
    config_c172pJSBSim_racetrack.yaml, config_c172pJSBSim_box.yaml, config_c172pJSBSim_zigzag.yaml.

.PARAMETER WindowsHost
    This machine's IP as seen from WSL (the vEthernet (WSL) adapter address from `ipconfig`).
    Re-verify after a reboot -- see runbook.

.PARAMETER WslDistro
    WSL distro that has PX4-Autopilot-optimAero checked out under ~/SITLV2.

.EXAMPLE
    .\start_mission.ps1 -SessionId run1
    .\start_mission.ps1 -SessionId run2 -Config config_c172pJSBSim_racetrack.yaml
#>
param(
    [Parameter(Mandatory = $true)][string]$SessionId,
    [string]$Config = "config_c172pJSBSim.yaml",
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

New-Item -ItemType Directory -Force -Path $sessionDir | Out-Null

# 1) UDP relay -- one instance is enough for the whole day; skip if already running in WSL.
if (-not $SkipRelay) {
    $relayRunning = wsl -d $WslDistro -e bash -lc "pgrep -f wsl_udp_relay.py" 2>$null
    if (-not $relayRunning) {
        Write-Host "Starting UDP relay..." -ForegroundColor Cyan
        Start-Process -FilePath "wsl.exe" -ArgumentList @(
            "-d", $WslDistro, "-e", "bash", "-lc",
            "`"cd '/mnt/c/Users/Edison Altamirano/SITLV2/AnelloSummer/optimAeroPX4SIL/HILDiagnostics/mavlink_system' && python3 wsl_udp_relay.py --windows-host $WindowsHost`""
        )
        Start-Sleep -Seconds 2
    }
    else {
        Write-Host "Relay already running in WSL, leaving it alone." -ForegroundColor DarkGray
    }
}

# 2) Fresh PX4 SITL instance for this session.
Write-Host "Starting PX4 SITL for session '$SessionId'..." -ForegroundColor Cyan
Start-Process -FilePath "wsl.exe" -ArgumentList @(
    "-d", $WslDistro, "-e", "bash", "-lc",
    "`"bash '/mnt/c/Users/Edison Altamirano/SITLV2/AnelloSummer/optimAeroPX4SIL/HILDiagnostics/launch_c172_px4_wsl.sh' $SessionId $WindowsHost`""
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

# 3) Mission harness (own window so its console.log mirror is visible live).
Write-Host "Starting mission harness (config=$Config)..." -ForegroundColor Cyan
Start-Process -FilePath $pythonExe -ArgumentList @("main.py", "--config", $Config, "--session-id", $SessionId) -WorkingDirectory $mavlinkDir

Write-Host ""
Write-Host "Done. In MATLAB now run:" -ForegroundColor Green
Write-Host "  cd(`"$repoRoot`"); runMissionSIL(`"$SessionId`")"
Write-Host ""
Write-Host "Add `"visualizationType`", `"FlightGear`" as extra runMissionSIL args for the FlightGear view."

# Reset exit status: earlier native calls (e.g. the relay's `pgrep` probe) legitimately return
# non-zero when nothing is found yet, and that stale $LASTEXITCODE would otherwise leak out as
# this script's own exit code even though everything above succeeded.
exit 0
