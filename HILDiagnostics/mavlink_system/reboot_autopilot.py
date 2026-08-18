"""Reboot the real flight controller BEFORE the HITL Simulink session opens the serial port.

Rebooting PX4 through the live MAVSDK/Simulink relay (attempted in session_10 -- see
HITL_PREARM_HEALTH_INVESTIGATION.md section 10) breaks Simulink's already-open serial
handle to the board: MAV_CMD_PREFLIGHT_REBOOT_SHUTDOWN on a USB-native flight controller
(Cube Orange Plus enumerates its own USB CDC-ACM port) physically drops and re-creates
the USB device mid-`sim()`, which the MATLABSystem serial block cannot recover from
("WriteFile returned ERROR", sim aborts) -- and simultaneously corrupts mavsdk_server's
own RPC state ("Unexpected error in RPC handling" on every call after).

The fix has to happen here instead: talk to the board directly over serial, before
Simulink or MAVSDK ever touch the port, so the USB re-enumeration has nothing to
interrupt. Run this once at the very start of every HITL bench session -- before
main.py and before run_hil_automated_session.m (see the updated section 8 reproduction
steps):

    py -3.12 reboot_autopilot.py --baud 921600

By default the port is resolved fresh each run by USB device identity (resolve_cube_port.py),
not assumed to be a fixed "COM4" -- Windows can hand the Cube a different COM number on a later
boot/replug with no error. Pass --port explicitly only if you need to override the resolver.

Why reboot at all: the real board is never power-cycled between HITL sessions (only
MATLAB/main.py get restarted, per section 6.3). If a previous session left PX4 armed or
mid-failsafe (e.g. session_8 ending via "mission did not finish" while still cycling
attitude failsafes, never disarmed/landed), its EKF2 estimator state carries into the
next session and produces spurious prearm failures ("High Accelerometer Bias" / "GPS
Vertical Pos Drift too high") despite a correct, static sensor feed -- session_9's
symptom. Rebooting here gives the board the same clean-slate guarantee the MATLAB
restart already gives the software side.

TERMINATION-STATE VERIFICATION (added 2026-07-31): a plain "did we get a heartbeat back"
check does not prove the reboot actually cleared PX4's internal state. Confirmed directly
(sessions vibration_full_mission_03/04/05, control_150s_test): a prior session that escalated
into NAVIGATION_STATE_TERMINATION (see estimatorCheck.cpp/Commander.cpp) left the board printing
"Preflight Fail: Flight termination active" over STATUSTEXT within the first couple of seconds of
every subsequent *software* reboot -- MAV_CMD_PREFLIGHT_REBOOT_SHUTDOWN alone did not clear it,
even though a full reflash did. This module now actively listens for that specific message after
reconnecting and retries; if a real physical power-cycle is required, it can fall back to that too
(Cube-only relay, COM7/CUBE_POWER -- deliberately does NOT touch GYRO_POWER/COM5, which is for the
real ANELLO device and unrelated/possibly unconnected during pure-HIL testing).
"""

from __future__ import annotations

import argparse
import json
import sys
import time
from pathlib import Path

from pymavlink import mavutil

RELAY_MAP_PATH = Path(r"C:\Users\Edison Altamirano\Documents\USB-Relay\relay_map.json")
RELAY_CMD_ON = bytes([0xA0, 0x01, 0x01, 0xA2])
RELAY_CMD_OFF = bytes([0xA0, 0x01, 0x00, 0xA1])

TERMINATION_MARKERS = ("flight termination", "termination active")


def _connect_and_wait_heartbeat(port: str, baud: int, connect_timeout_s: float):
    """Connect and wait for a HEARTBEAT, working around two confirmed silent-port causes
    (2026-07-28): (1) the board's USB-CDC port can stay silent until DTR/RTS are explicitly
    driven high -- plain pyserial/mavutil defaults leave them at whatever the OS last left them;
    (2) with SYS_AUTOSTART pointed at the real 1003 HIL airframe, the board's USB MAVLink
    instance does not stream anything to a client it hasn't seen a HEARTBEAT from -- a purely
    passive wait_heartbeat() can hang forever on an otherwise perfectly healthy board. Both were
    previously misattributed to "board went silent, needs a relay power-cycle" -- try this first."""
    master = mavutil.mavlink_connection(port, baud=baud)
    master.port.dtr = True
    master.port.rts = True
    end = time.time() + connect_timeout_s
    heartbeat = None
    while heartbeat is None and time.time() < end:
        master.mav.heartbeat_send(mavutil.mavlink.MAV_TYPE_GCS, mavutil.mavlink.MAV_AUTOPILOT_INVALID, 0, 0, 0)
        heartbeat = master.wait_heartbeat(timeout=2)
    return master, heartbeat


def _check_for_termination(master, listen_s: float = 4.0) -> str | None:
    """Drain STATUSTEXT traffic for a few seconds right after a fresh heartbeat and look for
    PX4's own "Preflight Fail: Flight termination active" (or similar) message. Returns the
    matching text, or None if nothing termination-related was seen in the window."""
    end = time.time() + listen_s
    while time.time() < end:
        msg = master.recv_match(type="STATUSTEXT", blocking=True, timeout=0.5)
        if msg is None:
            continue
        text = getattr(msg, "text", "") or ""
        if any(marker in text.lower() for marker in TERMINATION_MARKERS):
            return text
    return None


def reboot_and_wait(port: str, baud: int, connect_timeout_s: float, settle_s: float,
                     termination_listen_s: float = 4.0) -> str | None:
    """Reboot and reconnect once. Returns the termination STATUSTEXT if still latched after
    this reboot, or None if the board came back clean."""
    print(f"reboot_autopilot: connecting directly to {port} @ {baud} baud")
    master, heartbeat = _connect_and_wait_heartbeat(port, baud, connect_timeout_s)
    if heartbeat is None:
        master.close()
        raise TimeoutError(f"reboot_autopilot: no HEARTBEAT on {port} within {connect_timeout_s}s")
    print(
        "reboot_autopilot: heartbeat from "
        f"system={master.target_system} component={master.target_component}"
    )

    print("reboot_autopilot: sending MAV_CMD_PREFLIGHT_REBOOT_SHUTDOWN (param1=1, reboot autopilot)")
    master.mav.command_long_send(
        master.target_system,
        master.target_component,
        mavutil.mavlink.MAV_CMD_PREFLIGHT_REBOOT_SHUTDOWN,
        0,
        1, 0, 0, 0, 0, 0, 0,
    )
    # Don't wait for a COMMAND_ACK here -- the board commonly resets before it can send
    # one, and waiting would just time out.
    master.close()
    print(
        f"reboot_autopilot: closed the port; waiting {settle_s:.0f}s for the board to "
        "reboot and the USB device to re-enumerate"
    )
    time.sleep(settle_s)

    print(f"reboot_autopilot: reconnecting to {port} to confirm a clean boot")
    master, heartbeat = _connect_and_wait_heartbeat(port, baud, connect_timeout_s)
    if heartbeat is None:
        master.close()
        raise TimeoutError(
            f"reboot_autopilot: no HEARTBEAT on {port} within {connect_timeout_s}s after "
            "reboot -- board may still be booting, or the COM port may have been "
            "reassigned; check Device Manager before retrying"
        )

    print(f"reboot_autopilot: listening {termination_listen_s:.0f}s for a termination STATUSTEXT before trusting this boot")
    termination_text = _check_for_termination(master, termination_listen_s)
    master.close()

    if termination_text is not None:
        print(f"reboot_autopilot: WARNING -- still latched: {termination_text!r}")
        return termination_text

    print("reboot_autopilot: confirmed autopilot is back up with no termination latch. Port is now free for Simulink to open.")
    return None


def power_cycle_cube_relay(settle_s: float = 2.0) -> None:
    """Power-cycle ONLY the Cube relay (CUBE_POWER, COM7 per relay_map.json) -- deliberately
    does not touch GYRO_POWER (COM5), which powers the real ANELLO device and is unrelated to
    (and may not even be connected during) pure-HIL testing. Requires pyserial; imported lazily
    so a missing relay_map.json or pyserial only breaks this fallback path, not the normal
    software-reboot path above."""
    import serial

    if not RELAY_MAP_PATH.exists():
        raise RuntimeError(f"relay map not found at {RELAY_MAP_PATH}; cannot power-cycle the Cube")
    relays = json.loads(RELAY_MAP_PATH.read_text(encoding="utf-8"))
    if "CUBE_POWER" not in relays:
        raise RuntimeError(f"CUBE_POWER not found in {RELAY_MAP_PATH}; found: {list(relays)}")
    cube_port = relays["CUBE_POWER"]["port"]

    print(f"reboot_autopilot: power-cycling Cube relay only (CUBE_POWER, {cube_port})")
    with serial.Serial(cube_port, 9600, timeout=1) as ser:
        ser.write(RELAY_CMD_OFF)
    print(f"reboot_autopilot: Cube relay OFF, waiting {settle_s:.0f}s")
    time.sleep(settle_s)
    with serial.Serial(cube_port, 9600, timeout=1) as ser:
        ser.write(RELAY_CMD_ON)
    print("reboot_autopilot: Cube relay ON, waiting for the board to boot")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument(
        "--port", default=None,
        help="Serial port the flight controller enumerates as. Default: resolve fresh by USB "
        "device identity (resolve_cube_port.py) rather than assuming a fixed port -- Windows can "
        "reassign COM numbers across boots/replugs. Pass explicitly only to override.",
    )
    parser.add_argument("--baud", type=int, default=921600)
    parser.add_argument("--connect-timeout-s", type=float, default=30.0)
    parser.add_argument(
        "--settle-s", type=float, default=15.0, help="Wait after sending reboot before reconnecting"
    )
    parser.add_argument(
        "--max-soft-reboot-attempts", type=int, default=2,
        help="How many plain MAV_CMD_PREFLIGHT_REBOOT_SHUTDOWN attempts to try before falling "
        "back to a physical Cube power-cycle if termination is still latched.",
    )
    parser.add_argument(
        "--no-power-cycle-fallback", action="store_true",
        help="Do not attempt a physical relay power-cycle if soft reboots can't clear a "
        "termination latch -- just fail. Use this if the Cube relay isn't wired up.",
    )
    args = parser.parse_args()

    port = args.port
    if port is None:
        from resolve_cube_port import resolve_cube_port
        try:
            port = resolve_cube_port()
        except RuntimeError as exc:
            print(f"reboot_autopilot: FAILED: {exc}", file=sys.stderr)
            sys.exit(1)
        print(f"reboot_autopilot: resolved Cube port = {port} (by USB device identity)")

    termination_text = None
    try:
        for attempt in range(1, args.max_soft_reboot_attempts + 1):
            print(f"reboot_autopilot: soft-reboot attempt {attempt}/{args.max_soft_reboot_attempts}")
            termination_text = reboot_and_wait(port, args.baud, args.connect_timeout_s, args.settle_s)
            if termination_text is None:
                return  # clean boot confirmed
            print("reboot_autopilot: termination still latched after this soft reboot")

        if args.no_power_cycle_fallback:
            print(
                f"reboot_autopilot: FAILED: termination still latched after "
                f"{args.max_soft_reboot_attempts} soft reboots ({termination_text!r}), "
                "and --no-power-cycle-fallback was set",
                file=sys.stderr,
            )
            sys.exit(1)

        print(
            f"reboot_autopilot: termination still latched after {args.max_soft_reboot_attempts} "
            "soft reboots -- falling back to a physical Cube power-cycle"
        )
        power_cycle_cube_relay()

        # A true power-cycle (unlike a soft reboot) can make Windows re-enumerate the Cube on a
        # different COM port -- re-resolve by USB device identity rather than assuming the same
        # port number is still valid. resolve_cube_port() is single-shot (raises if not yet
        # enumerated), so poll it rather than assuming the board is already up.
        from resolve_cube_port import resolve_cube_port
        print("reboot_autopilot: re-resolving Cube COM port after power-cycle (may have changed)")
        resolve_deadline = time.time() + max(args.connect_timeout_s, 30.0)
        port = None
        while time.time() < resolve_deadline:
            try:
                port = resolve_cube_port(log=False)
                break
            except RuntimeError:
                time.sleep(1.0)
        if port is None:
            raise TimeoutError("reboot_autopilot: Cube did not re-enumerate after power-cycle")
        print(f"reboot_autopilot: resolved Cube port = {port} after power-cycle")

        print(f"reboot_autopilot: reconnecting to {port} after power-cycle")
        master, heartbeat = _connect_and_wait_heartbeat(port, args.baud, max(args.connect_timeout_s, 30.0))
        if heartbeat is None:
            master.close()
            raise TimeoutError(f"reboot_autopilot: no HEARTBEAT on {port} after power-cycle")
        print(f"reboot_autopilot: listening for termination STATUSTEXT after power-cycle")
        termination_text = _check_for_termination(master, listen_s=4.0)
        master.close()

        if termination_text is not None:
            print(
                f"reboot_autopilot: FAILED: termination still latched even after a physical "
                f"power-cycle ({termination_text!r}) -- this needs manual investigation, not "
                "another automated retry",
                file=sys.stderr,
            )
            sys.exit(1)

        print("reboot_autopilot: confirmed clean boot after physical power-cycle. Port is now free for Simulink to open.")

    except TimeoutError as exc:
        print(f"reboot_autopilot: FAILED: {exc}", file=sys.stderr)
        sys.exit(1)
    except RuntimeError as exc:
        print(f"reboot_autopilot: FAILED (power-cycle fallback): {exc}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
