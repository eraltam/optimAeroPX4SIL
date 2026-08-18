"""ULog lifecycle control for the static SiPhOG bench campaign (plan Section 5).

PX4 defaults to SDLOG_MODE=0 (log-on-arm only). These static bench sessions never arm, so the
onboard logger must be started/stopped deliberately, and the resulting log must be identified by
an exact before/after log-ID diff -- never by "latest", since a board with no RTC/GPS lock (as
seen here: all existing log entries report `date=2000-01-01`) cannot be trusted to sort logs by
timestamp.

IMPORTANT, found empirically 2026-07-28 -- do not "fix" this back without re-reading the
`logger help` output: `MAV_CMD_LOGGING_START`/`STOP` do NOT control the onboard SD-card
recording session. That MAVLink command pair controls real-time ULog *streaming* over MAVLink
(LOGGING_DATA messages, for a companion computer), a separate PX4 feature. Sending it here got
ACKed MAV_RESULT_ACCEPTED every time but silently created no file at all. The actual recording
session is controlled by the NSH `logger on` / `logger off` verbs (the daemon task itself, run
by `logger start`/`logger stop`, must already be running -- calling the bare `stop` variant
kills the whole daemon, not just the active recording, and must be restarted with `logger
start` before `on`/`off` will work again). Reached over the same non-interactive
SERIAL_CONTROL/NSH-shell mechanism as `anello_siphog_probe.py`/`mavlink_shell_probe.py`.

Uses that one NSH-shell connection for both logger on/off control (by parsing its text output,
which conveniently prints the exact log file path on `on` and bytes-written on `off`) and for
LOG_REQUEST_LIST/LOG_ENTRY (structured MAVLink, sent on the same underlying connection object),
then hands off -- sequentially, not concurrently -- to MAVSDK's `log_files` plugin for the actual
chunked download, since MAVSDK already implements LOG_REQUEST_DATA/LOG_DATA reassembly correctly.

Usage as a library:
    from ulog_session import ULogRecorder

Usage as a script (runs one complete deliberate capture):
    py -3.12 ulog_session.py --duration-s 300 --output-dir <session_dir> --session-id qual_gate
"""
from __future__ import annotations

import argparse
import asyncio
import re
import sys
import time
from pathlib import Path

from pymavlink import mavutil

sys.path.insert(0, str(Path(__file__).resolve().parent))
from resolve_cube_port import resolve_cube_port  # noqa: E402


class MavlinkShell:
    """Non-interactive MAVLink SERIAL_CONTROL NSH shell (see anello_siphog_probe.py)."""

    def __init__(self, port: str, baud: int, devnum: int = 10, timeout_s: float = 20.0):
        device = f"{port},{baud}"
        self.mav = mavutil.mavlink_connection(device, autoreconnect=True)
        # Confirmed 2026-07-28 (PLAN_STATIC_A1_REPEAT_SIPHOG_LONGDURATION.md Section 0.5): with
        # SYS_AUTOSTART=1003 active, the board stays silent (DTR/RTS notwithstanding) until it
        # sees a HEARTBEAT from us -- force DTR/RTS and keep re-announcing until one lands, rather
        # than a single heartbeat_send() + one passive wait (which can hang forever on an
        # otherwise perfectly healthy board).
        self.mav.port.dtr = True
        self.mav.port.rts = True
        hb = None
        end = time.time() + timeout_s
        while hb is None and time.time() < end:
            self.mav.mav.heartbeat_send(mavutil.mavlink.MAV_TYPE_GENERIC, mavutil.mavlink.MAV_AUTOPILOT_INVALID, 0, 0, 0)
            hb = self.mav.wait_heartbeat(timeout=2)
        if hb is None:
            raise TimeoutError(f"ulog_session: no heartbeat on {port}")
        self.port_num = devnum
        self.port = port
        self.baud = baud

    def write(self, s: str) -> None:
        b = s.encode("utf-8")
        while len(b) > 0:
            n = min(len(b), 70)
            chunk = list(b[:n]) + [0] * (70 - n)
            self.mav.mav.serial_control_send(
                self.port_num,
                mavutil.mavlink.SERIAL_CONTROL_FLAG_EXCLUSIVE | mavutil.mavlink.SERIAL_CONTROL_FLAG_RESPOND,
                0, 0, n, chunk,
            )
            b = b[n:]

    def read_for(self, duration_s: float) -> str:
        deadline = time.time() + duration_s
        out = []
        while time.time() < deadline:
            m = self.mav.recv_match(type="SERIAL_CONTROL", blocking=True, timeout=0.2)
            if m is not None and m.count:
                out.append(bytes(m.data[: m.count]).decode("utf-8", errors="replace"))
        return "".join(out)

    def run(self, cmd: str, read_s: float = 2.0) -> str:
        self.write(cmd + "\n")
        return self.read_for(read_s)

    def list_log_ids(self, timeout_s: float = 15.0) -> dict[int, dict]:
        """Return {log_id: {"size": int, "time_utc": int}} via LOG_REQUEST_LIST/LOG_ENTRY."""
        self.mav.mav.log_request_list_send(self.mav.target_system, self.mav.target_component, 0, 0xFFFF)
        entries: dict[int, dict] = {}
        expected_num_logs = None
        deadline = time.time() + timeout_s
        while time.time() < deadline:
            msg = self.mav.recv_match(type="LOG_ENTRY", blocking=True, timeout=1.0)
            if msg is None:
                if expected_num_logs is not None and len(entries) >= expected_num_logs:
                    break
                continue
            expected_num_logs = msg.num_logs
            entries[msg.id] = {"size": msg.size, "time_utc": msg.time_utc}
            if len(entries) >= expected_num_logs:
                break
        if expected_num_logs is None:
            raise TimeoutError("ulog_session: no LOG_ENTRY response to LOG_REQUEST_LIST")
        if len(entries) != expected_num_logs:
            raise RuntimeError(
                f"ulog_session: incomplete log list -- board reports {expected_num_logs} logs, "
                f"received {len(entries)} distinct entries within {timeout_s}s"
            )
        return entries

    def close(self) -> None:
        self.mav.mav.serial_control_send(self.port_num, 0, 0, 0, 0, [0] * 70)
        self.mav.close()


def ensure_logger_daemon_running(shell: MavlinkShell) -> None:
    out = shell.run("logger status", 3.0)
    if "not running" in out:
        out = shell.run("logger start", 3.0)
        if "logger started" not in out and "already running" not in out:
            raise RuntimeError(f"ulog_session: could not start logger daemon, output: {out!r}")


def start_recording(shell: MavlinkShell, poll_timeout_s: float = 15.0) -> str | None:
    """Send `logger on`, then POLL `logger status` (idempotent, reliable) until it confirms
    recording -- the one-shot echo from `logger on` itself is unreliable: SERIAL_CONTROL
    response timing/chunking can miss it entirely even though the command took effect (found
    empirically 2026-07-28 -- a "no confirmation" exception here previously left the board
    recording anyway, undetected, for several minutes). Never trust the transient echo alone.
    """
    status = shell.run("logger status", 3.0)
    if "Full File Logging Running" in status:
        raise RuntimeError(f"ulog_session: logger already recording before start_recording was called: {status!r}")

    shell.run("logger on", 2.0)
    deadline = time.time() + poll_timeout_s
    while time.time() < deadline:
        status = shell.run("logger status", 2.0)
        if "Full File Logging Running" in status:
            m = re.search(r"Log file:\s*(\S+)", status)
            return m.group(1) if m else None
        time.sleep(1.0)
    raise RuntimeError(f"ulog_session: logger did not confirm recording within {poll_timeout_s}s, last status: {status!r}")


def stop_recording(shell: MavlinkShell, poll_timeout_s: float = 15.0) -> None:
    """Send `logger off`, then poll `logger status` until it confirms recording has stopped."""
    shell.run("logger off", 2.0)
    deadline = time.time() + poll_timeout_s
    while time.time() < deadline:
        status = shell.run("logger status", 2.0)
        if "Not logging" in status:
            return
        time.sleep(1.0)
    raise RuntimeError(f"ulog_session: logger did not confirm stop within {poll_timeout_s}s, last status: {status!r}")


async def _download_async(port: str, baud: int, log_id: int, output_path: Path, timeout_s: float) -> Path:
    from mavsdk import System

    drone = System()
    await drone.connect(system_address=f"serial://{port}:{baud}")
    async with asyncio.timeout(timeout_s):
        async for state in drone.core.connection_state():
            if state.is_connected:
                break

    entries = await drone.log_files.get_entries()
    matches = [e for e in entries if e.id == log_id]
    if not matches:
        raise RuntimeError(f"ulog_session: log id {log_id} not found in board's entry list ({len(entries)} entries)")
    entry = matches[0]

    tmp_path = output_path.with_suffix(output_path.suffix + ".part")
    output_path.parent.mkdir(parents=True, exist_ok=True)
    async for progress in drone.log_files.download_log_file(entry, str(tmp_path)):
        pct = progress.progress * 100.0
        print(f"\r  download log {log_id}: {pct:5.1f}%", end="", flush=True)
    print()

    if not tmp_path.exists() or tmp_path.stat().st_size == 0:
        raise RuntimeError(f"ulog_session: download produced no data for log id {log_id}")
    if entry.size_bytes and tmp_path.stat().st_size != entry.size_bytes:
        raise RuntimeError(
            f"ulog_session: downloaded size {tmp_path.stat().st_size} != reported size "
            f"{entry.size_bytes} for log id {log_id} -- not renaming to final name"
        )
    tmp_path.rename(output_path)
    return output_path


def download_log_id(port: str, baud: int, log_id: int, output_path: Path, timeout_s: float = 120.0) -> Path:
    return asyncio.run(_download_async(port, baud, log_id, output_path, timeout_s))


def run_capture(port: str | None, baud: int, duration_s: float, output_dir: Path, session_id: str) -> dict:
    port = port or resolve_cube_port()
    shell = MavlinkShell(port, baud)
    print(f"ulog_session: connected NSH shell on {port}")

    ensure_logger_daemon_running(shell)
    before_ids = set(shell.list_log_ids().keys())
    print(f"ulog_session: before capture, {len(before_ids)} logs on board")

    file_path = start_recording(shell)
    t_start = time.time()
    print(f"ulog_session: recording started ({file_path}); capturing for {duration_s:.0f}s")

    time.sleep(duration_s)

    stop_recording(shell)
    t_end = time.time()
    print("ulog_session: recording stopped (confirmed via logger status)")

    time.sleep(2.0)  # let the FS flush/close settle before re-listing
    after = shell.list_log_ids()
    after_ids = set(after.keys())
    new_ids = after_ids - before_ids
    shell.close()

    if len(new_ids) != 1:
        raise RuntimeError(
            f"ulog_session: expected exactly 1 new log id, found {len(new_ids)}: {sorted(new_ids)} "
            f"(before={sorted(before_ids)}, after={sorted(after_ids)})"
        )
    new_id = next(iter(new_ids))
    reported_size = after[new_id]["size"]
    print(f"ulog_session: new log id = {new_id}, board-reported size = {reported_size} bytes, path={file_path}")

    output_path = output_dir / f"{session_id}_log{new_id}.ulg"
    download_log_id(port, baud, new_id, output_path)
    print(f"ulog_session: downloaded to {output_path}")

    return {
        "log_id": new_id,
        "file_path_on_board": file_path,
        "reported_size_from_log_entry": reported_size,
        "output_path": str(output_path),
        "requested_duration_s": duration_s,
        "actual_duration_s": t_end - t_start,
        "port": port,
        "baud": baud,
    }


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--port", default=None)
    parser.add_argument("--baud", type=int, default=57600)
    parser.add_argument("--duration-s", type=float, required=True)
    parser.add_argument("--output-dir", type=Path, required=True)
    parser.add_argument("--session-id", default=None)
    args = parser.parse_args()

    session_id = args.session_id or f"static_qual_{int(args.duration_s)}s"
    result = run_capture(args.port, args.baud, args.duration_s, args.output_dir, session_id)
    print(result)


if __name__ == "__main__":
    main()
