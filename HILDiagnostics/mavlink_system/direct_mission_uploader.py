"""Minimal, standalone mission uploader for c172p SITL validation.

Bypasses listener_pymavlink.py's HITL relay entirely. Uses ONE persistent pymavlink
UDP socket for the whole sequence -- heartbeat -> mission count -> mission items ->
mission ack -> arm -> start -- with no reconnecting in between. This exists because
the relay (which captures MAVSDK's outbound bytes and re-injects them through a
separate socket) breaks PX4's waypoint-manager's per-session partner identity for the
stateful MISSION_COUNT/MISSION_ITEM handshake specifically, even though one-shot
commands (arm, param requests) go through it fine. The relay itself is left
unchanged for actual HITL use -- this script is a SITL-only bypass.

Mission item values mirror controller_mavsdk.py's _build_raw_mission_items() /
mission_c172p.yaml exactly, so the mission actually flown here matches the one the
HITL-relay harness was trying to fly.
"""

from __future__ import annotations

import math
import sys
import time

from pymavlink import mavutil

CONNECTION_ADDRESS = "udpin:0.0.0.0:14555"

TAKEOFF_ALTITUDE_M = 25.0
ACCEPTANCE_RADIUS_M = 80.0
RELATIVE_ALTITUDE_M = 120.0
WAYPOINTS = [
    (17.645045, -63.198281),
    (17.645045, -63.191681),
    (17.636960, -63.191681),
    (17.636960, -63.198281),
]

FRAME_GLOBAL_RELATIVE_ALT_INT = 6
FRAME_MISSION = 2
CMD_NAV_WAYPOINT = 16
CMD_NAV_TAKEOFF = 22
CMD_NAV_RETURN_TO_LAUNCH = 20
MAV_MISSION_TYPE_MISSION = 0
MAV_CMD_COMPONENT_ARM_DISARM = 400
MAV_CMD_MISSION_START = 300
MAV_MISSION_ACCEPTED = 0

UPLOAD_TIMEOUT_S = 30.0
ACK_TIMEOUT_S = 10.0


def build_items(current_lat: float, current_lon: float) -> list[tuple]:
    items = []
    items.append((
        0, FRAME_GLOBAL_RELATIVE_ALT_INT, CMD_NAV_TAKEOFF, 1, 1,
        0.0, 0.0, 0.0, float("nan"),
        round(current_lat * 1e7), round(current_lon * 1e7), TAKEOFF_ALTITUDE_M,
    ))
    seq = 1
    for lat, lon in WAYPOINTS:
        items.append((
            seq, FRAME_GLOBAL_RELATIVE_ALT_INT, CMD_NAV_WAYPOINT, 0, 1,
            0.0, ACCEPTANCE_RADIUS_M, 0.0, float("nan"),
            round(lat * 1e7), round(lon * 1e7), RELATIVE_ALTITUDE_M,
        ))
        seq += 1
    items.append((
        seq, FRAME_MISSION, CMD_NAV_RETURN_TO_LAUNCH, 0, 1,
        0.0, 0.0, 0.0, 0.0, 0, 0, 0.0,
    ))
    return items


def send_item(conn, item: tuple) -> None:
    seq, frame, cmd, current, autocontinue, p1, p2, p3, p4, x, y, z = item
    conn.mav.mission_item_int_send(
        conn.target_system, conn.target_component,
        seq, frame, cmd, current, autocontinue,
        p1, p2, p3, p4, x, y, z, MAV_MISSION_TYPE_MISSION,
    )


def upload_mission(conn, items: list[tuple]) -> bool:
    total = len(items)
    print(f"direct: clearing existing mission (MISSION_CLEAR_ALL)")
    conn.mav.mission_clear_all_send(conn.target_system, conn.target_component, MAV_MISSION_TYPE_MISSION)
    ack = conn.recv_match(type="MISSION_ACK", blocking=True, timeout=ACK_TIMEOUT_S)
    print(f"direct: clear ack: {ack}")

    print(f"direct: sending MISSION_COUNT={total}")
    conn.mav.mission_count_send(conn.target_system, conn.target_component, total, MAV_MISSION_TYPE_MISSION)

    deadline = time.time() + UPLOAD_TIMEOUT_S
    sent = set()
    while time.time() < deadline:
        msg = conn.recv_match(
            type=["MISSION_REQUEST_INT", "MISSION_REQUEST", "MISSION_ACK"],
            blocking=True, timeout=5,
        )
        if msg is None:
            print("direct: TIMEOUT waiting for mission protocol message")
            return False
        mtype = msg.get_type()
        if mtype == "MISSION_ACK":
            print(f"direct: final upload ack: type={msg.type}")
            if msg.type == MAV_MISSION_ACCEPTED:
                print(f"direct: MISSION UPLOAD ACCEPTED ({len(sent)}/{total} items sent)")
                return True
            print(f"direct: MISSION UPLOAD REJECTED (type={msg.type})")
            return False
        req_seq = msg.seq
        if req_seq >= total:
            print(f"direct: PX4 requested out-of-range seq {req_seq}, aborting")
            return False
        send_item(conn, items[req_seq])
        sent.add(req_seq)
        print(f"direct: sent item {req_seq} (requested via {mtype})")

    print("direct: TIMEOUT -- upload did not complete in time")
    return False


def main() -> int:
    print(f"direct: connecting to {CONNECTION_ADDRESS}")
    conn = mavutil.mavlink_connection(CONNECTION_ADDRESS)

    print("direct: waiting for heartbeat")
    conn.wait_heartbeat(timeout=30)
    print(f"direct: heartbeat from system={conn.target_system} component={conn.target_component}")

    print("direct: waiting for GLOBAL_POSITION_INT (current position for takeoff item)")
    pos = conn.recv_match(type="GLOBAL_POSITION_INT", blocking=True, timeout=15)
    if pos is None:
        print("direct: FAILED -- no GLOBAL_POSITION_INT received")
        return 1
    current_lat = pos.lat / 1e7
    current_lon = pos.lon / 1e7
    print(f"direct: current position lat={current_lat:.7f} lon={current_lon:.7f}")

    items = build_items(current_lat, current_lon)
    print(f"direct: built {len(items)} mission items (takeoff + {len(WAYPOINTS)} waypoints + RTL)")

    if not upload_mission(conn, items):
        print("direct: STOPPING -- upload validation failed, not arming (disarmed-only check requested)")
        return 1

    print("direct: upload validated while disarmed -- continuing on the SAME connection to arm+start")

    print("direct: arming")
    conn.mav.command_long_send(
        conn.target_system, conn.target_component,
        MAV_CMD_COMPONENT_ARM_DISARM, 0,
        1, 0, 0, 0, 0, 0, 0,
    )
    ack = conn.recv_match(type="COMMAND_ACK", blocking=True, timeout=ACK_TIMEOUT_S)
    print(f"direct: arm ack: {ack}")

    print("direct: starting mission (MAV_CMD_MISSION_START)")
    conn.mav.command_long_send(
        conn.target_system, conn.target_component,
        MAV_CMD_MISSION_START, 0,
        0, 0, 0, 0, 0, 0, 0,
    )
    ack = conn.recv_match(type="COMMAND_ACK", blocking=True, timeout=ACK_TIMEOUT_S)
    print(f"direct: mission start ack: {ack}")

    print("direct: monitoring for up to 300s, or until mission advances past the takeoff item")
    t_end = time.time() + 300
    last_seq = None
    last_print = 0.0
    while time.time() < t_end:
        msg = conn.recv_match(
            type=["MISSION_CURRENT", "VFR_HUD", "EXTENDED_SYS_STATE", "ATTITUDE"],
            blocking=True, timeout=2,
        )
        if msg is None:
            continue
        mtype = msg.get_type()
        now = time.time()
        if mtype == "MISSION_CURRENT" and msg.seq != last_seq:
            last_seq = msg.seq
            print(f"direct: MISSION_CURRENT seq={msg.seq}")
            if msg.seq > 0:
                print("direct: mission advanced past the takeoff item -- stopping monitor")
                break
        elif mtype == "VFR_HUD" and now - last_print > 1.0:
            print(
                f"direct: VFR_HUD airspeed={msg.airspeed:.2f} groundspeed={msg.groundspeed:.2f} "
                f"alt={msg.alt:.2f} climb={msg.climb:.2f}"
            )
            last_print = now
        elif mtype == "ATTITUDE" and now - last_print > 1.0:
            print(
                f"direct: ATTITUDE roll={math.degrees(msg.roll):.2f} "
                f"pitch={math.degrees(msg.pitch):.2f} yaw={math.degrees(msg.yaw):.2f}"
            )
        elif mtype == "EXTENDED_SYS_STATE":
            print(f"direct: EXTENDED_SYS_STATE landed_state={msg.landed_state}")

    return 0


if __name__ == "__main__":
    sys.exit(main())
