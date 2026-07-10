"""One-shot preflight + reboot + relay launcher for a HITL session.

Consolidates everything HITL_FLIGHTGEAR_ROOT_CAUSE_AND_FIX.md / the rest of this investigation
found had to be done by hand, in order, before every session:

  1. Confirm the board's COM port is actually present.
  2. Confirm UDP 14540/14550 aren't already held by a stale process, and clean up if they are.
  3. Confirm no orphaned main.py/passive_listener.py/mavsdk_server processes are still running,
     and clean up if they are.
  4. Reboot the autopilot standalone, BEFORE Simulink or MAVSDK ever touch the serial port
     (rebooting through a live connection breaks it -- see HITL_PREARM_HEALTH_INVESTIGATION.md
     section 10.3).
  5. Launch main.py (normal arm/takeoff/mission relay) or passive_listener.py (read-only, no
     commands sent), which then blocks in the foreground for the rest of the session.

Deliberately does NOT touch MATLAB -- start this first, wait for the "ready" banner, then run
run_hil_automated_session(...) with visualizationType="PassThrough" in MATLAB yourself. See
HITL_FLIGHTGEAR_ROOT_CAUSE_AND_FIX.md section 5 -- do not use "FlightGear", it's the confirmed
cause of every arming failure in this investigation.

Usage:
    py -3.12 run_hitl_session.py --session-id my_session --mode relay
    py -3.12 run_hitl_session.py --session-id my_session --mode passive --duration 300
"""

from __future__ import annotations

import argparse
import subprocess
import sys
import time
from pathlib import Path

SCRIPT_DIR = Path(__file__).resolve().parent


def run_powershell(command: str) -> str:
    result = subprocess.run(
        ["powershell", "-NoProfile", "-Command", command],
        capture_output=True,
        text=True,
    )
    return (result.stdout or "").strip()


def step(title: str) -> None:
    print(f"\n=== {title} ===")


def check_com_port(port: str) -> None:
    step(f"Checking {port} is present")
    out = run_powershell(
        "Get-CimInstance -ClassName Win32_PnPEntity | "
        f"Where-Object {{ $_.Name -match '\\({port}\\)' }} | "
        "Select-Object -ExpandProperty Name"
    )
    if not out:
        print(f"ERROR: {port} not found in Device Manager. Is the board plugged in and powered?")
        sys.exit(1)
    print(f"OK: {out}")


def free_udp_ports(ports: list[int]) -> None:
    step(f"Checking UDP ports {ports} are free")
    for port in ports:
        pids = run_powershell(
            f"Get-NetUDPEndpoint -LocalPort {port} -ErrorAction SilentlyContinue | "
            "Select-Object -ExpandProperty OwningProcess"
        )
        if not pids:
            print(f"port {port}: free")
            continue
        for pid in pids.splitlines():
            pid = pid.strip()
            if not pid:
                continue
            name = run_powershell(
                f"(Get-Process -Id {pid} -ErrorAction SilentlyContinue).ProcessName"
            )
            print(f"port {port}: held by PID {pid} ({name or 'unknown'}) -- stopping it")
            run_powershell(f"Stop-Process -Id {pid} -Force -ErrorAction SilentlyContinue")
        time.sleep(1)
        still = run_powershell(
            f"Get-NetUDPEndpoint -LocalPort {port} -ErrorAction SilentlyContinue"
        )
        print(f"port {port}: {'still bound, check manually' if still else 'now free'}")


def kill_orphaned_helpers() -> None:
    step("Checking for orphaned main.py / passive_listener.py / mavsdk_server processes")
    # Targeted by command line / image name -- never touches unrelated python.exe processes
    # elsewhere on the machine.
    matches = run_powershell(
        "Get-CimInstance Win32_Process | "
        "Where-Object { $_.CommandLine -match 'main\\.py|passive_listener\\.py' -or "
        "$_.Name -eq 'mavsdk_server.exe' } | "
        "Select-Object -ExpandProperty ProcessId"
    )
    if not matches:
        print("none found")
        return
    for pid in matches.splitlines():
        pid = pid.strip()
        if not pid:
            continue
        print(f"stopping orphaned helper PID {pid}")
        run_powershell(f"Stop-Process -Id {pid} -Force -ErrorAction SilentlyContinue")
    time.sleep(1)


def reboot_autopilot(port: str, baud: int) -> None:
    step("Rebooting the autopilot (standalone, before Simulink/MAVSDK touch the port)")
    result = subprocess.run(
        [sys.executable, str(SCRIPT_DIR / "reboot_autopilot.py"), "--port", port, "--baud", str(baud)],
        cwd=SCRIPT_DIR,
    )
    if result.returncode != 0:
        print("ERROR: reboot_autopilot.py failed -- check the board/COM port before continuing.")
        sys.exit(1)


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--session-id", required=True, help="Same value to pass as sessionId in MATLAB")
    parser.add_argument("--port", default="COM4")
    parser.add_argument("--baud", type=int, default=921600)
    parser.add_argument(
        "--mode",
        choices=["relay", "passive"],
        default="relay",
        help="relay = main.py, normal arm/takeoff/mission (default). "
        "passive = passive_listener.py, read-only, sends nothing.",
    )
    parser.add_argument(
        "--duration",
        type=float,
        default=300.0,
        help="Only used for --mode passive (passive_listener.py has no natural end otherwise)",
    )
    args = parser.parse_args()

    check_com_port(args.port)
    free_udp_ports([14540, 14550])
    kill_orphaned_helpers()
    reboot_autopilot(args.port, args.baud)

    step("Starting the relay/listener -- this will block for the rest of the session")
    print(
        "\nOnce you see MAVSDK/pymavlink connect below, switch to MATLAB and run:\n\n"
        "  addpath('HILDiagnostics'); cd('C:/AS/optimAeroPX4SIL');\n"
        "  run_hil_automated_session(\"hitlSerialPort\", \"" + args.port + "\", ...\n"
        "      \"visualizationType\", \"PassThrough\", ...\n"
        "      \"stopTime_s\", 300, \"sessionId\", \"" + args.session_id + "\");\n\n"
        "(visualizationType MUST be \"PassThrough\", not \"FlightGear\" -- see\n"
        "HITL_FLIGHTGEAR_ROOT_CAUSE_AND_FIX.md.)\n"
    )

    if args.mode == "relay":
        subprocess.run(
            [sys.executable, str(SCRIPT_DIR / "main.py"), "--session-id", args.session_id],
            cwd=SCRIPT_DIR,
        )
    else:
        subprocess.run(
            [
                sys.executable,
                str(SCRIPT_DIR / "passive_listener.py"),
                "--duration",
                str(args.duration),
                "--session-id",
                args.session_id,
            ],
            cwd=SCRIPT_DIR,
        )


if __name__ == "__main__":
    main()
