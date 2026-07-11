#!/usr/bin/env python3
"""
Convert a PX4 .ulg flight path into a waypoint mission YAML.

Edit only the CONFIGURATION section below, then run:

    py ulg_to_mission_yaml_hardcoded.py

Install dependencies once:

    py -m pip install pyulog numpy
"""

from __future__ import annotations

import math
import statistics
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Optional, Sequence

import numpy as np

try:
    from pyulog import ULog
except ImportError:
    print(
        "ERROR: pyulog is not installed.\n"
        "Install it with:\n"
        "  py -m pip install pyulog numpy",
        file=sys.stderr,
    )
    raise SystemExit(2)


# ============================================================
# CONFIGURATION
# Change these values before running the script.
# ============================================================

INPUT_ULG_FILE = Path(r"C:\AS\optimAeroPX4SIL\HILDiagnostics\mavlink_system\nav_hil_ground_truth.ulg")

OUTPUT_YAML_FILE = Path(
    r"C:\AS\optimAeroPX4SIL\HILDiagnostics\mavlink_system\generated_mission.yaml"
)

MISSION_NAME = "reconstructed_ulg_mission"

# Path-processing settings
MIN_WAYPOINT_SPACING_M = 5.0
SIMPLIFY_TOLERANCE_M = 2.0
MAX_WAYPOINTS = 30

# Ignore small drift around takeoff and landing.
TAKEOFF_SKIP_DISTANCE_M = 2.0
LANDING_SKIP_DISTANCE_M = 2.0

# Set these to None to estimate them from the .ulg file.
SPEED_M_S = None
RELATIVE_ALTITUDE_M = None

# Mission settings
ACCEPTANCE_RADIUS_M = 2.0
IS_FLY_THROUGH = False
LOITER_TIME_S = 2.0

CLEAR_EXISTING = True
RTL_AFTER_MISSION = False
LAND_AFTER_MISSION = True

TAKEOFF_SETTLE_S = 2.0
MISSION_TIMEOUT_S = 240
LAND_TIMEOUT_S = 90


# ============================================================
# SCRIPT
# Normally, you do not need to edit anything below this line.
# ============================================================

EARTH_RADIUS_M = 6_371_000.0


@dataclass(frozen=True)
class Point:
    timestamp_us: int
    lat_deg: float
    lon_deg: float
    alt_msl_m: float
    vel_n_m_s: float = math.nan
    vel_e_m_s: float = math.nan
    vel_d_m_s: float = math.nan


def get_dataset(ulog: ULog, name: str):
    matches = [dataset for dataset in ulog.data_list if dataset.name == name]
    if not matches:
        return None

    return max(
        matches,
        key=lambda dataset: len(dataset.data.get("timestamp", [])),
    )


def get_field(
    data: dict,
    possible_names: Sequence[str],
    default: Optional[np.ndarray] = None,
):
    for name in possible_names:
        if name in data:
            return np.asarray(data[name])

    return default


def normalize_lat_lon(values: np.ndarray) -> np.ndarray:
    values = np.asarray(values, dtype=float)
    finite = values[np.isfinite(values)]

    # PX4 may store latitude and longitude as degrees or degrees times 1e7.
    if finite.size and np.nanmedian(np.abs(finite)) > 1_000.0:
        values = values / 1e7

    return values


def normalize_altitude(values: np.ndarray) -> np.ndarray:
    values = np.asarray(values, dtype=float)
    finite = values[np.isfinite(values)]

    # PX4 may store altitude in meters or millimeters.
    if finite.size and np.nanmedian(np.abs(finite)) > 20_000.0:
        values = values / 1000.0

    return values


def get_armed_time_range(ulog: ULog) -> Optional[tuple[int, int]]:
    dataset = get_dataset(ulog, "vehicle_status")
    if dataset is None:
        return None

    timestamps = get_field(dataset.data, ["timestamp"])
    arming_state = get_field(dataset.data, ["arming_state"])

    if timestamps is None or arming_state is None or len(timestamps) == 0:
        return None

    # PX4 ARMING_STATE_ARMED is normally 2.
    armed_mask = np.asarray(arming_state) == 2

    if not np.any(armed_mask):
        return None

    armed_timestamps = np.asarray(
        timestamps,
        dtype=np.int64,
    )[armed_mask]

    return int(armed_timestamps[0]), int(armed_timestamps[-1])


def extract_global_points(ulog: ULog) -> list[Point]:
    dataset = get_dataset(ulog, "vehicle_global_position")

    if dataset is None:
        raise RuntimeError(
            "The ULog does not contain vehicle_global_position."
        )

    data = dataset.data

    timestamps = get_field(data, ["timestamp"])
    latitudes = get_field(data, ["lat", "latitude_deg"])
    longitudes = get_field(data, ["lon", "longitude_deg"])
    altitudes = get_field(
        data,
        ["alt", "altitude_msl_m", "alt_ellipsoid"],
    )

    if (
        timestamps is None
        or latitudes is None
        or longitudes is None
        or altitudes is None
    ):
        raise RuntimeError(
            "vehicle_global_position is missing timestamp, lat, lon, or alt."
        )

    latitudes = normalize_lat_lon(latitudes)
    longitudes = normalize_lat_lon(longitudes)
    altitudes = normalize_altitude(altitudes)

    count = min(
        len(timestamps),
        len(latitudes),
        len(longitudes),
        len(altitudes),
    )

    timestamps = np.asarray(timestamps[:count], dtype=np.int64)
    latitudes = latitudes[:count]
    longitudes = longitudes[:count]
    altitudes = altitudes[:count]

    velocity_north = get_field(
        data,
        ["vel_n", "vx"],
        np.full(count, np.nan),
    )[:count]

    velocity_east = get_field(
        data,
        ["vel_e", "vy"],
        np.full(count, np.nan),
    )[:count]

    velocity_down = get_field(
        data,
        ["vel_d", "vz"],
        np.full(count, np.nan),
    )[:count]

    valid = (
        np.isfinite(latitudes)
        & np.isfinite(longitudes)
        & np.isfinite(altitudes)
        & (np.abs(latitudes) <= 90.0)
        & (np.abs(longitudes) <= 180.0)
        & ~((latitudes == 0.0) & (longitudes == 0.0))
    )

    armed_range = get_armed_time_range(ulog)

    if armed_range is not None:
        armed_start_us, armed_end_us = armed_range
        valid &= (
            (timestamps >= armed_start_us)
            & (timestamps <= armed_end_us)
        )

    points = [
        Point(
            timestamp_us=int(timestamps[index]),
            lat_deg=float(latitudes[index]),
            lon_deg=float(longitudes[index]),
            alt_msl_m=float(altitudes[index]),
            vel_n_m_s=float(velocity_north[index]),
            vel_e_m_s=float(velocity_east[index]),
            vel_d_m_s=float(velocity_down[index]),
        )
        for index in np.flatnonzero(valid)
    ]

    if len(points) < 2:
        raise RuntimeError(
            "Not enough valid vehicle_global_position samples were found."
        )

    return remove_position_jumps(points)


def haversine_m(point_a: Point, point_b: Point) -> float:
    latitude_a = math.radians(point_a.lat_deg)
    latitude_b = math.radians(point_b.lat_deg)

    delta_latitude = latitude_b - latitude_a
    delta_longitude = math.radians(
        point_b.lon_deg - point_a.lon_deg
    )

    haversine_value = (
        math.sin(delta_latitude / 2.0) ** 2
        + math.cos(latitude_a)
        * math.cos(latitude_b)
        * math.sin(delta_longitude / 2.0) ** 2
    )

    return (
        2.0
        * EARTH_RADIUS_M
        * math.asin(min(1.0, math.sqrt(haversine_value)))
    )


def remove_position_jumps(
    points: Sequence[Point],
    max_speed_m_s: float = 80.0,
) -> list[Point]:
    cleaned = [points[0]]

    for point in points[1:]:
        previous = cleaned[-1]

        delta_time_s = (
            point.timestamp_us - previous.timestamp_us
        ) / 1e6

        if delta_time_s <= 0:
            continue

        distance_m = haversine_m(previous, point)
        calculated_speed_m_s = distance_m / delta_time_s

        if calculated_speed_m_s <= max_speed_m_s:
            cleaned.append(point)

    if len(cleaned) < 2:
        raise RuntimeError(
            "All position samples were rejected as invalid jumps."
        )

    return cleaned


def to_local_xy_m(
    point: Point,
    origin: Point,
) -> tuple[float, float]:
    origin_latitude_rad = math.radians(origin.lat_deg)

    east_m = (
        math.radians(point.lon_deg - origin.lon_deg)
        * EARTH_RADIUS_M
        * math.cos(origin_latitude_rad)
    )

    north_m = (
        math.radians(point.lat_deg - origin.lat_deg)
        * EARTH_RADIUS_M
    )

    return east_m, north_m


def select_spaced_points(
    points: Sequence[Point],
    spacing_m: float,
) -> list[Point]:
    if spacing_m <= 0:
        return list(points)

    selected = [points[0]]
    cumulative_distance_m = 0.0
    previous = points[0]

    for point in points[1:-1]:
        cumulative_distance_m += haversine_m(previous, point)
        previous = point

        if cumulative_distance_m >= spacing_m:
            selected.append(point)
            cumulative_distance_m = 0.0

    if selected[-1] != points[-1]:
        selected.append(points[-1])

    return selected


def perpendicular_distance_m(
    point: Point,
    line_start: Point,
    line_end: Point,
    origin: Point,
) -> float:
    point_x, point_y = to_local_xy_m(point, origin)
    start_x, start_y = to_local_xy_m(line_start, origin)
    end_x, end_y = to_local_xy_m(line_end, origin)

    delta_x = end_x - start_x
    delta_y = end_y - start_y

    if delta_x == 0.0 and delta_y == 0.0:
        return math.hypot(
            point_x - start_x,
            point_y - start_y,
        )

    interpolation = (
        (point_x - start_x) * delta_x
        + (point_y - start_y) * delta_y
    ) / (
        delta_x * delta_x
        + delta_y * delta_y
    )

    interpolation = max(0.0, min(1.0, interpolation))

    closest_x = start_x + interpolation * delta_x
    closest_y = start_y + interpolation * delta_y

    return math.hypot(
        point_x - closest_x,
        point_y - closest_y,
    )


def simplify_path(
    points: Sequence[Point],
    tolerance_m: float,
    origin: Point,
) -> list[Point]:
    if len(points) <= 2:
        return list(points)

    line_start = points[0]
    line_end = points[-1]

    maximum_distance_m = -1.0
    maximum_index = -1

    for index in range(1, len(points) - 1):
        distance_m = perpendicular_distance_m(
            points[index],
            line_start,
            line_end,
            origin,
        )

        if distance_m > maximum_distance_m:
            maximum_distance_m = distance_m
            maximum_index = index

    if maximum_distance_m > tolerance_m:
        left = simplify_path(
            points[: maximum_index + 1],
            tolerance_m,
            origin,
        )

        right = simplify_path(
            points[maximum_index:],
            tolerance_m,
            origin,
        )

        return left[:-1] + right

    return [line_start, line_end]


def limit_waypoint_count(
    points: Sequence[Point],
    maximum_waypoints: int,
) -> list[Point]:
    if maximum_waypoints < 1:
        raise ValueError("MAX_WAYPOINTS must be at least 1.")

    if len(points) <= maximum_waypoints:
        return list(points)

    indices = np.linspace(
        0,
        len(points) - 1,
        maximum_waypoints,
    )

    rounded_indices = sorted(
        set(int(round(index)) for index in indices)
    )

    return [points[index] for index in rounded_indices]


def trim_takeoff_and_landing(
    points: Sequence[Point],
    takeoff_skip_m: float,
    landing_skip_m: float,
) -> list[Point]:
    if len(points) <= 2:
        return list(points)

    takeoff_point = points[0]
    landing_point = points[-1]

    first_index = 0

    for index, point in enumerate(points):
        if haversine_m(takeoff_point, point) >= takeoff_skip_m:
            first_index = index
            break

    last_index = len(points) - 1

    for index in range(len(points) - 1, -1, -1):
        if haversine_m(landing_point, points[index]) >= landing_skip_m:
            last_index = index
            break

    if first_index <= last_index:
        trimmed = list(points[first_index : last_index + 1])

        if trimmed:
            return trimmed

    return list(points)


def estimate_speed_m_s(points: Sequence[Point]) -> float:
    velocity_speeds = [
        math.hypot(
            point.vel_n_m_s,
            point.vel_e_m_s,
        )
        for point in points
        if (
            math.isfinite(point.vel_n_m_s)
            and math.isfinite(point.vel_e_m_s)
        )
    ]

    velocity_speeds = [
        speed
        for speed in velocity_speeds
        if 0.5 <= speed <= 50.0
    ]

    if velocity_speeds:
        return float(statistics.median(velocity_speeds))

    calculated_speeds = []

    for point_a, point_b in zip(points, points[1:]):
        delta_time_s = (
            point_b.timestamp_us - point_a.timestamp_us
        ) / 1e6

        if delta_time_s <= 0:
            continue

        speed_m_s = (
            haversine_m(point_a, point_b)
            / delta_time_s
        )

        if 0.5 <= speed_m_s <= 50.0:
            calculated_speeds.append(speed_m_s)

    if calculated_speeds:
        return float(statistics.median(calculated_speeds))

    return 3.0


def estimate_home_altitude_m(
    points: Sequence[Point],
) -> float:
    sample_count = max(
        1,
        min(
            len(points),
            max(10, len(points) // 20),
        ),
    )

    return float(
        statistics.median(
            point.alt_msl_m
            for point in points[:sample_count]
        )
    )


def estimate_relative_altitude_m(
    points: Sequence[Point],
    home_altitude_m: float,
) -> float:
    relative_altitudes = [
        point.alt_msl_m - home_altitude_m
        for point in points
    ]

    airborne_altitudes = [
        altitude_m
        for altitude_m in relative_altitudes
        if altitude_m > 1.0
    ]

    if not airborne_altitudes:
        return 5.0

    return max(
        1.0,
        float(statistics.median(airborne_altitudes)),
    )


def yaml_bool(value: bool) -> str:
    return "true" if value else "false"


def generate_yaml(
    waypoints: Sequence[Point],
    speed_m_s: float,
    relative_altitude_m: float,
) -> str:
    lines = [
        "mission:",
        f"  enabled: {yaml_bool(True)}",
        f'  name: "{MISSION_NAME}"',
        "",
        "  # Values reconstructed or estimated from the PX4 ULog.",
        f"  speed_m_s: {speed_m_s:.2f}",
        f"  relative_altitude_m: {relative_altitude_m:.2f}",
        f"  acceptance_radius_m: {ACCEPTANCE_RADIUS_M:.2f}",
        f"  is_fly_through: {yaml_bool(IS_FLY_THROUGH)}",
        f"  loiter_time_s: {LOITER_TIME_S:.2f}",
        "",
        f"  clear_existing: {yaml_bool(CLEAR_EXISTING)}",
        f"  rtl_after_mission: {yaml_bool(RTL_AFTER_MISSION)}",
        f"  land_after_mission: {yaml_bool(LAND_AFTER_MISSION)}",
        f"  takeoff_settle_s: {TAKEOFF_SETTLE_S:.2f}",
        f"  mission_timeout_s: {MISSION_TIMEOUT_S}",
        f"  land_timeout_s: {LAND_TIMEOUT_S}",
        "",
        "  waypoints:",
    ]

    for index, point in enumerate(waypoints, start=1):
        lines.extend(
            [
                f'    - name: "wp{index:02d}"',
                f"      latitude_deg: {point.lat_deg:.7f}",
                f"      longitude_deg: {point.lon_deg:.7f}",
            ]
        )

    return "\n".join(lines) + "\n"


def main() -> int:
    if not INPUT_ULG_FILE.is_file():
        print(
            f"ERROR: Input file not found:\n{INPUT_ULG_FILE}",
            file=sys.stderr,
        )
        return 2

    if MIN_WAYPOINT_SPACING_M < 0:
        print(
            "ERROR: MIN_WAYPOINT_SPACING_M cannot be negative.",
            file=sys.stderr,
        )
        return 2

    if SIMPLIFY_TOLERANCE_M < 0:
        print(
            "ERROR: SIMPLIFY_TOLERANCE_M cannot be negative.",
            file=sys.stderr,
        )
        return 2

    try:
        ulog = ULog(str(INPUT_ULG_FILE))
        full_path = extract_global_points(ulog)

        home_altitude_m = estimate_home_altitude_m(full_path)

        mission_speed_m_s = (
            float(SPEED_M_S)
            if SPEED_M_S is not None
            else estimate_speed_m_s(full_path)
        )

        mission_relative_altitude_m = (
            float(RELATIVE_ALTITUDE_M)
            if RELATIVE_ALTITUDE_M is not None
            else estimate_relative_altitude_m(
                full_path,
                home_altitude_m,
            )
        )

        waypoint_candidates = select_spaced_points(
            full_path,
            MIN_WAYPOINT_SPACING_M,
        )

        waypoint_candidates = trim_takeoff_and_landing(
            waypoint_candidates,
            TAKEOFF_SKIP_DISTANCE_M,
            LANDING_SKIP_DISTANCE_M,
        )

        if len(waypoint_candidates) > 2:
            waypoint_candidates = simplify_path(
                waypoint_candidates,
                SIMPLIFY_TOLERANCE_M,
                full_path[0],
            )

        waypoints = limit_waypoint_count(
            waypoint_candidates,
            MAX_WAYPOINTS,
        )

        if not waypoints:
            raise RuntimeError(
                "No waypoints remained after path filtering."
            )

        yaml_text = generate_yaml(
            waypoints,
            mission_speed_m_s,
            mission_relative_altitude_m,
        )

        OUTPUT_YAML_FILE.parent.mkdir(
            parents=True,
            exist_ok=True,
        )

        OUTPUT_YAML_FILE.write_text(
            yaml_text,
            encoding="utf-8",
        )

    except Exception as error:
        print(
            f"ERROR: {error}",
            file=sys.stderr,
        )
        return 1

    print("")
    print("Mission conversion completed.")
    print(f"Input file:          {INPUT_ULG_FILE}")
    print(f"Valid path samples:  {len(full_path)}")
    print(f"Generated waypoints: {len(waypoints)}")
    print(f"Mission speed:       {mission_speed_m_s:.2f} m/s")
    print(
        "Relative altitude:   "
        f"{mission_relative_altitude_m:.2f} m"
    )
    print(f"Output file:         {OUTPUT_YAML_FILE}")
    print("")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
