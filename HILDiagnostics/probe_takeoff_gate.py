"""Targeted probe for the mc_pos_control takeoff-ramp gate, per
HIL_ZERO_THRUST_AND_PARAM_RELIABILITY_NEXT_STEPS.md section 3.2/3.3 (2026-07-14, third pass).

Source-code trace (src/modules/mc_pos_control/Takeoff/Takeoff.cpp): the takeoff state machine only
advances ready_for_takeoff -> rampup (required before any nonzero thrust setpoint exists) when
`vehicle_constraints.want_takeoff == true`. While state < rampup, MulticopterPositionControl.cpp
explicitly overwrites the setpoint with acceleration[2] = +100 ("High downwards acceleration to make
sure there's no thrust"). want_takeoff is computed in flight_mode_manager's FlightTaskAuto from the
mission's own trajectory_setpoint -- if flight_mode_manager isn't alive or trajectory_setpoint is
stale, want_takeoff never goes true and thrust stays zero forever, matching every observed symptom
(armed, healthy EKF2, right control-mode flags, PX4Controls exactly zero) without needing the
actuator_armed.lockdown hypothesis (already de-prioritized -- pwm_out_sim's `-m hil` mode ignores
lockdown by design, see the same doc section).

Run this DURING an active Simulink HIL session, connecting over the QGC UDP mirror (14550) --
matches mavlink_shell_probe.py's approach. Per the doc's own lesson from session_lockdown_probe_test/
_test2 (the relay is too saturated for short read windows + tight polling), this script is meant to
be run as a small number of single-shot passes at key moments (e.g. right after arm, and again a few
seconds after the takeoff command is accepted) -- not as a tight continuous poll.
"""
import sys
import time
from mavlink_shell_probe import MavlinkShell

TOPICS = [
    "vehicle_constraints",
    "trajectory_setpoint",
    "vehicle_local_position",
]


def main(address):
    print(f"Connecting takeoff-gate probe to {address} ...")
    shell = MavlinkShell(address)
    print("Connected. Priming shell...")
    shell.write("\n")
    time.sleep(1.0)
    shell.read_for(1.0)  # discard the prompt/banner

    print("\n=== flight_mode_manager status (is it even running?) ===")
    shell.write("flight_mode_manager status\n")
    print(shell.read_for(5.0))

    for topic in TOPICS:
        print(f"\n=== listener {topic} -n 1 ===")
        shell.write(f"listener {topic} -n 1\n")
        output = shell.read_for(5.0)
        print(output if output.strip() else "(no output)")

    shell.close()


if __name__ == "__main__":
    address = sys.argv[1] if len(sys.argv) > 1 else "udpin:0.0.0.0:14550"
    main(address)
