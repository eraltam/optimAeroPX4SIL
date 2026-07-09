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

    py -3.12 reboot_autopilot.py --port COM4 --baud 921600

Why reboot at all: the real board is never power-cycled between HITL sessions (only
MATLAB/main.py get restarted, per section 6.3). If a previous session left PX4 armed or
mid-failsafe (e.g. session_8 ending via "mission did not finish" while still cycling
attitude failsafes, never disarmed/landed), its EKF2 estimator state carries into the
next session and produces spurious prearm failures ("High Accelerometer Bias" / "GPS
Vertical Pos Drift too high") despite a correct, static sensor feed -- session_9's
symptom. Rebooting here gives the board the same clean-slate guarantee the MATLAB
restart already gives the software side.
"""

from __future__ import annotations

import argparse
import sys
import time

from pymavlink import mavutil


def reboot_and_wait(port: str, baud: int, connect_timeout_s: float, settle_s: float) -> None:
    print(f"reboot_autopilot: connecting directly to {port} @ {baud} baud")
    master = mavutil.mavlink_connection(port, baud=baud)
    heartbeat = master.wait_heartbeat(timeout=connect_timeout_s)
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
    master = mavutil.mavlink_connection(port, baud=baud)
    heartbeat = master.wait_heartbeat(timeout=connect_timeout_s)
    master.close()
    if heartbeat is None:
        raise TimeoutError(
            f"reboot_autopilot: no HEARTBEAT on {port} within {connect_timeout_s}s after "
            "reboot -- board may still be booting, or the COM port may have been "
            "reassigned; check Device Manager before retrying"
        )
    print("reboot_autopilot: confirmed autopilot is back up. Port is now free for Simulink to open.")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--port", default="COM4", help="Serial port the flight controller enumerates as")
    parser.add_argument("--baud", type=int, default=921600)
    parser.add_argument("--connect-timeout-s", type=float, default=30.0)
    parser.add_argument(
        "--settle-s", type=float, default=15.0, help="Wait after sending reboot before reconnecting"
    )
    args = parser.parse_args()

    try:
        reboot_and_wait(args.port, args.baud, args.connect_timeout_s, args.settle_s)
    except TimeoutError as exc:
        print(f"reboot_autopilot: FAILED: {exc}", file=sys.stderr)
        sys.exit(1)


if __name__ == "__main__":
    main()
