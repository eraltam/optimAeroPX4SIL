#!/usr/bin/env python3
"""Download the most recent PX4 onboard log (.ulg) via MAVSDK, after a HIL session.

Uses the QGC UDP relay (port 14550) that PX4HITLConnector.slx already mirrors MAVLink
onto concurrently with the primary serial link (see optimAeroPX4SIL/CLAUDE.md section
18.2) -- NOT the HITL serial port itself, which HILDiagnostics/connection.py's own
docstring warns is "owned exclusively by Simulink for the duration of the HIL run."
Run this AFTER the MATLAB sim() call has returned, so the flight has actually ended and
the log is closed and flushed on the board.
"""
import asyncio
import sys
from pathlib import Path

from mavsdk import System


async def download_latest_log(output_path: Path, address: str = "udpin://0.0.0.0:14550", timeout_s: float = 30.0) -> Path:
    drone = System()
    await drone.connect(system_address=address)

    print(f"Waiting for connection on {address}...")
    async with asyncio.timeout(timeout_s):
        async for state in drone.core.connection_state():
            if state.is_connected:
                print("Connected.")
                break

    entries = await drone.log_files.get_entries()
    if not entries:
        raise RuntimeError("No log entries reported by the board.")
    latest = max(entries, key=lambda e: e.id)
    print(f"Downloading log id={latest.id}, size={latest.size_bytes} bytes, date={latest.date}")

    output_path.parent.mkdir(parents=True, exist_ok=True)
    async for progress in drone.log_files.download_log_file(latest, str(output_path)):
        pct = progress.progress * 100.0
        print(f"\r  {pct:5.1f}%", end="", flush=True)
    print(f"\nSaved: {output_path}")
    return output_path


if __name__ == "__main__":
    out = Path(sys.argv[1]) if len(sys.argv) > 1 else Path("downloaded_log.ulg")
    addr = sys.argv[2] if len(sys.argv) > 2 else "udpin://0.0.0.0:14550"
    asyncio.run(download_latest_log(out, addr))
