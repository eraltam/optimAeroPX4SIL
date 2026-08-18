"""Identify the real Cube Orange+ COM port by USB device identity, never by a hardcoded port
number.

Found the hard way (2026-07-23, `v2_shakedown_3001`): `reboot_autopilot.py` and
`run_hil_automated_session.m` both defaulted to a literal `"COM4"`/`"COM5"` string. That happened
to be correct on that boot, but Windows assigns COM numbers per USB enumeration order/driver
state -- a reboot, a different USB hub, or another CH340/CDC-ACM device attached first can hand
the Cube a different number next time with no error, just a session that silently talks to the
wrong port (or a bind/serial failure that looks like a hardware fault but isn't). The relay ports
already avoid this correctly (`relay_map.json` keys them by hwid+location, not a fixed COM
number) -- this module gives the flight controller the same treatment.

Matches on the Cube Orange+'s own USB vendor/product ID (`VID_2DAE&PID_1058`, CubePilot's
enumeration as a MAVLink-native CDC-ACM device -- confirmed directly via
`Get-CimInstance Win32_PnPEntity` on 2026-07-23, Name `"...ORANGEMAVPX4..." (COM4)`), not on the
port number or a device Name substring alone (Name strings can vary by driver/firmware build;
the VID:PID pair is the actual stable identity).

Usage (prints the resolved port and appends a timestamped record to
`HILDiagnostics/logs/cube_port_log.csv` -- every session that calls this gets its own row, so a
port change is visible in the record rather than silently assumed unchanged):

    py -3.12 resolve_cube_port.py
    py -3.12 resolve_cube_port.py --vid-pid VID_2DAE&PID_1058

Importable: `from resolve_cube_port import resolve_cube_port` returns the port string (e.g.
"COM4") or raises `RuntimeError` if no matching device is currently enumerated.
"""

from __future__ import annotations

import argparse
import csv
import datetime as dt
import re
import subprocess
import sys
from pathlib import Path

LOG_PATH = Path(r"C:\AS\optimAeroPX4SIL\HILDiagnostics\logs\cube_port_log.csv")
DEFAULT_VID_PID = "VID_2DAE&PID_1058"
LOG_FIELDS = ["timestamp_utc", "vid_pid", "port", "device_name"]


def _query_pnp_entities() -> list[dict[str, str]]:
    result = subprocess.run(
        [
            "powershell", "-NoProfile", "-Command",
            "Get-CimInstance -ClassName Win32_PnPEntity | "
            "Where-Object { $_.Name -match 'COM\\d+' } | "
            "Select-Object Name, DeviceID | ConvertTo-Csv -NoTypeInformation",
        ],
        capture_output=True,
        text=True,
    )
    lines = [ln for ln in (result.stdout or "").splitlines() if ln.strip()]
    if len(lines) < 2:
        return []
    reader = csv.DictReader(lines)
    return list(reader)


def resolve_cube_port(vid_pid: str = DEFAULT_VID_PID, log: bool = True) -> str:
    """Return the COM port currently enumerated by the device matching vid_pid.

    Raises RuntimeError if zero or more than one match is found -- an ambiguous match is not
    safe to silently pick from, it must be resolved by a human before proceeding.
    """
    entities = _query_pnp_entities()
    matches = [e for e in entities if vid_pid.upper() in e.get("DeviceID", "").upper()]

    if not matches:
        raise RuntimeError(
            f"resolve_cube_port: no currently-enumerated device matches {vid_pid}. "
            "Is the Cube relay powered on and has it finished enumerating? "
            "(see HILDiagnostics/power_on_sequence.py)"
        )
    if len(matches) > 1:
        raise RuntimeError(
            f"resolve_cube_port: {len(matches)} devices match {vid_pid}, ambiguous: {matches}. "
            "Resolve by hand before proceeding -- do not guess which one is the flight controller."
        )

    name = matches[0]["Name"]
    port_match = re.search(r"\((COM\d+)\)", name)
    if not port_match:
        raise RuntimeError(f"resolve_cube_port: matched device but couldn't parse a COM port from '{name}'")
    port = port_match.group(1)

    if log:
        LOG_PATH.parent.mkdir(parents=True, exist_ok=True)
        write_header = not LOG_PATH.exists()
        with LOG_PATH.open("a", newline="", encoding="utf-8") as f:
            writer = csv.DictWriter(f, fieldnames=LOG_FIELDS)
            if write_header:
                writer.writeheader()
            writer.writerow({
                "timestamp_utc": dt.datetime.now(dt.timezone.utc).isoformat(timespec="seconds"),
                "vid_pid": vid_pid,
                "port": port,
                "device_name": name,
            })

    return port


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--vid-pid", default=DEFAULT_VID_PID)
    parser.add_argument("--no-log", action="store_true")
    args = parser.parse_args()

    try:
        port = resolve_cube_port(args.vid_pid, log=not args.no_log)
    except RuntimeError as exc:
        print(f"resolve_cube_port: FAILED: {exc}", file=sys.stderr)
        sys.exit(1)

    print(port)
    if not args.no_log:
        print(f"resolve_cube_port: logged to {LOG_PATH}", file=sys.stderr)


if __name__ == "__main__":
    main()
