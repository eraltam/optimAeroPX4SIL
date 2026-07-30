"""MAVSDK high-level flight instructor for PX4 HIL/SITL sessions."""

from __future__ import annotations

import asyncio
import math
from pathlib import Path
from typing import Any

from mavsdk import System
from mavsdk.action import ActionError
from mavsdk.mission import MissionError, MissionItem, MissionPlan
from mavsdk.mission_raw import MissionItem as RawMissionItem
from mavsdk.mission_raw import MissionRawError
from mavsdk.telemetry import LandedState
import yaml

# MAVLink MAV_FRAME/MAV_CMD/MAV_MISSION_TYPE values used by _build_raw_mission_items -- pulled
# in as plain ints (not a pymavlink import) since this file otherwise has no MAVLink dependency
# of its own; controller_mavsdk.py talks to PX4 exclusively through MAVSDK.
_MAV_FRAME_GLOBAL_RELATIVE_ALT_INT = 6
# Command-only items (no lat/lon/alt of their own) need MAV_FRAME_MISSION, not a positional
# frame -- confirmed via a standalone probe: RETURN_TO_LAUNCH with frame=
# GLOBAL_RELATIVE_ALT_INT (6) got rejected outright (UNSUPPORTED) by upload_mission(), the
# identical item with frame=MISSION (2) uploaded fine.
_MAV_FRAME_MISSION = 2
_MAV_CMD_NAV_WAYPOINT = 16
_MAV_CMD_NAV_LAND = 21
_MAV_CMD_NAV_TAKEOFF = 22
_MAV_CMD_NAV_RETURN_TO_LAUNCH = 20
_MAV_CMD_DO_CHANGE_SPEED = 178
_MAV_MISSION_TYPE_MISSION = 0

from safety import confirm_real_vehicle_allowed, validate_takeoff_altitude


class MavsdkInstructor:
    """High-level controller. This is the only class that sends flight commands."""

    def __init__(
        self,
        config: dict[str, Any],
        config_dir: str | Path | None = None,
        base_dir: str | Path | None = None,
    ) -> None:
        self.config = config
        self.vehicle_config = config.get("vehicle", {})
        self.safety_config = config.get("safety", {})
        self.address = self.vehicle_config.get("mavsdk_address", "udp://:14540")
        self.config_dir = Path(config_dir) if config_dir is not None else Path.cwd()
        self.base_dir = Path(base_dir) if base_dir is not None else None
        self.drone = System()

    async def connect(self) -> None:
        print(f"MAVSDK: connecting to {self.address}")
        await self.drone.connect(system_address=self.address)
        timeout_s = float(self.vehicle_config.get("connect_timeout_s", 60.0))
        await asyncio.wait_for(self._wait_connected(), timeout=timeout_s)

    async def _wait_connected(self) -> None:
        async for state in self.drone.core.connection_state():
            if state.is_connected:
                print("MAVSDK: vehicle connected")
                return
        raise RuntimeError("MAVSDK: connection stream ended before vehicle connected")

    async def wait_until_ready(self) -> None:
        if not self.safety_config.get("require_position_ok_for_takeoff", True):
            print("MAVSDK: position health gate disabled by config")
            return

        # is_global_position_ok/is_home_position_ok alone are NOT sufficient: PX4 can report
        # both true while still failing its own broader prearm checks (e.g. EKF2's "vertical
        # velocity unstable" -- see HITL_PREARM_HEALTH_INVESTIGATION.md section 5.4/5.5), and
        # arm() would then get rejected with COMMAND_DENIED anyway. is_armable is MAVSDK's
        # aggregate "PX4 actually allows arming right now" flag -- wait on that too, since it's
        # the one that reflects PX4's real prearm check result, not just position health.
        print("MAVSDK: waiting for global/home position health and armable status")
        timeout_s = float(self.vehicle_config.get("health_timeout_s", 60.0))
        # Wrapped in asyncio.wait_for rather than checking elapsed time inside the loop body:
        # the old in-loop check only fired when a *new* health message arrived, so a connection
        # that goes silent (e.g. the underlying HITL link dying when Simulink's sim() ends,
        # session_14) hung this forever instead of timing out -- same failure class dump_params
        # had (10.5).
        try:
            await asyncio.wait_for(self._poll_health_until_ready(), timeout=timeout_s)
        except asyncio.TimeoutError as exc:
            raise TimeoutError(
                "MAVSDK: timed out waiting for global/home position health and armable "
                f"status after {timeout_s}s"
            ) from exc

    async def _poll_health_until_ready(self, attempts: int = 4) -> None:
        # mavsdk_server's gRPC layer can throw an "Unexpected error in RPC handling" on the
        # very first item of a freshly-opened stream if it's still settling right after
        # connect() (confirmed live 2026-07-24, immediately following a large 922-parameter
        # dump) -- same transient-RPC-hiccup class _run_action/_run_mission_call already retry
        # around, just surfacing here as a stream error instead of an action timeout. Re-opening
        # the subscription a few times is enough for it to clear; PX4's own health state is
        # unaffected by this, only the gRPC bridge.
        last_exc: Exception | None = None
        for attempt in range(1, attempts + 1):
            try:
                async for health in self.drone.telemetry.health():
                    if (
                        health.is_global_position_ok
                        and health.is_home_position_ok
                        and health.is_armable
                    ):
                        print("MAVSDK: position health is OK and vehicle is armable")
                        return
                    print(
                        "MAVSDK: health pending "
                        f"global={health.is_global_position_ok} "
                        f"home={health.is_home_position_ok} armable={health.is_armable}"
                    )
                    await asyncio.sleep(1.0)
                return
            except Exception as exc:  # noqa: BLE001 -- mavsdk raises grpc.aio.AioRpcError here
                last_exc = exc
                print(
                    f"MAVSDK: health stream failed on attempt {attempt}/{attempts}: {exc!r}"
                )
                if attempt < attempts:
                    await asyncio.sleep(2.0)
        assert last_exc is not None
        raise last_exc

    async def arm(self) -> None:
        if not confirm_real_vehicle_allowed(self.config):
            print("MAVSDK DRY-RUN: arm()")
            return
        attempts = int(self.vehicle_config.get("arm_attempts", 4))
        await self._run_action("arm", self.drone.action.arm, attempts=attempts)

    async def takeoff(self) -> None:
        altitude = float(self.vehicle_config.get("takeoff_altitude_m", 3.0))
        max_altitude = float(self.safety_config.get("max_takeoff_altitude_m", 10.0))
        validate_takeoff_altitude(altitude, max_altitude)

        if not confirm_real_vehicle_allowed(self.config):
            print(f"MAVSDK DRY-RUN: set_takeoff_altitude({altitude})")
            print("MAVSDK DRY-RUN: takeoff()")
            return

        try:
            await self._run_action(
                "set_takeoff_altitude", lambda: self.drone.action.set_takeoff_altitude(altitude)
            )
        except (ActionError, asyncio.TimeoutError) as exc:
            # Non-fatal, unlike every other action call here: confirmed live that this
            # specific call (a PARAM_SET/PARAM_VALUE round trip, unlike arm/mission's
            # COMMAND_LONG/MAVLink-mission-protocol messages) is consistently broken on this
            # relay path -- all 4 retries timed out every time, not a one-off transient loss.
            # PX4 already has a reasonable MIS_TAKEOFF_ALT default from the airframe file, so
            # skip the override and fly with that instead of blocking the whole sequence over
            # a "nice to have" altitude tweak.
            print(
                f"MAVSDK: set_takeoff_altitude failed after retries ({exc!r}), continuing with "
                "PX4's existing MIS_TAKEOFF_ALT instead"
            )
        await self._run_action("takeoff", self.drone.action.takeoff)

    async def land(self) -> None:
        if not confirm_real_vehicle_allowed(self.config):
            print("MAVSDK DRY-RUN: land()")
            return
        await self._run_action("land", self.drone.action.land)

    async def return_to_launch(self) -> None:
        if not confirm_real_vehicle_allowed(self.config):
            print("MAVSDK DRY-RUN: return_to_launch()")
            return
        await self._run_action("return_to_launch", self.drone.action.return_to_launch)

    async def disarm(self) -> None:
        if not confirm_real_vehicle_allowed(self.config):
            print("MAVSDK DRY-RUN: disarm()")
            return
        await self._run_action("disarm", self.drone.action.disarm)

    async def print_basic_telemetry(self, samples: int = 5) -> None:
        print("MAVSDK: basic telemetry")
        timeout_s = float(self.vehicle_config.get("telemetry_timeout_s", 15.0))
        tasks = [
            asyncio.create_task(self._print_position(samples)),
            asyncio.create_task(self._print_battery(samples)),
            asyncio.create_task(self._print_flight_mode(samples)),
        ]
        try:
            results = await asyncio.wait_for(
                asyncio.gather(*tasks, return_exceptions=True), timeout=timeout_s
            )
        except TimeoutError:
            print(f"MAVSDK: telemetry sampling timed out after {timeout_s}s")
            for task in tasks:
                task.cancel()
            results = await asyncio.gather(*tasks, return_exceptions=True)
        # return_exceptions=True so one stream erroring (e.g. a transient mavsdk_server gRPC
        # hiccup on telemetry.battery(), confirmed live 2026-07-24 -- PX4 itself was still
        # publishing a healthy battery_status at the time) doesn't abort this purely
        # informational probe and take the whole mission run down with it.
        for task, result in zip(tasks, results):
            if isinstance(result, Exception):
                print(f"MAVSDK: telemetry probe failed (non-fatal): {result!r}")

    async def run_demo_sequence(self) -> None:
        await self.print_basic_telemetry(samples=3)

        mission_path = self._mission_path()
        mission_requested = bool(self.vehicle_config.get("auto_mission", False) and mission_path)

        if (
            self.vehicle_config.get("auto_arm", False)
            or self.vehicle_config.get("auto_takeoff", False)
            or mission_requested
        ):
            await self._apply_pre_arm_params()
            try:
                await self.wait_until_ready()
            except TimeoutError:
                if not self.vehicle_config.get("continue_on_health_timeout", False):
                    raise
                print(
                    "MAVSDK: stationary pre-arm health gate timed out; continuing to "
                    "mission upload before the separately ordered SITL force-arm"
                )

        if mission_requested:
            await self.run_waypoint_mission(mission_path)
            return

        if self.vehicle_config.get("auto_arm", False):
            await self.arm()

        if self.vehicle_config.get("auto_takeoff", False):
            await self.takeoff()
            if confirm_real_vehicle_allowed(self.config):
                print("MAVSDK: hover wait 5 s")
                await asyncio.sleep(5.0)
            await self.land()
        else:
            print("MAVSDK: auto_takeoff=false; no takeoff command requested")

    async def run_waypoint_mission(self, mission_path: Path) -> None:
        mission_config = self._load_mission_config(mission_path)
        mission_name = str(mission_config.get("name", mission_path.stem))
        plan = self._build_mission_plan(mission_config)
        mission_items = plan.mission_items

        print(f"MAVSDK: mission '{mission_name}' loaded from {mission_path}")
        print(f"MAVSDK: mission contains {len(mission_items)} waypoint(s)")
        for idx, item in enumerate(mission_items, start=1):
            print(
                "  waypoint "
                f"{idx}: lat={item.latitude_deg:.7f} lon={item.longitude_deg:.7f} "
                f"rel_alt={item.relative_altitude_m:.1f}m speed={item.speed_m_s:.1f}m/s"
            )

        if not confirm_real_vehicle_allowed(self.config):
            print("MAVSDK DRY-RUN: clear/upload/start waypoint mission")
            if self.vehicle_config.get("auto_arm", False):
                print("MAVSDK DRY-RUN: arm()")
            if self.vehicle_config.get("auto_takeoff", False):
                print("MAVSDK DRY-RUN: takeoff before mission")
            print("MAVSDK DRY-RUN: land after mission")
            return

        rtl_after_mission = bool(mission_config.get("rtl_after_mission", False))
        # include_takeoff_item: fixed-wing missions need PX4 to own the whole ground-to-cruise
        # transition as ONE continuous mission (MAV_CMD_NAV_TAKEOFF as item 0), not a separate
        # action.takeoff() followed by a mission upload -- confirmed live on c172p that arming
        # straight into AUTO_MISSION with no takeoff item at all causes a full loss of control
        # (attitude/altitude diverged, essentially a crash), while a standalone action.takeoff()
        # first gets it safely airborne but then PX4 hands off to AUTO_LOITER instead of
        # AUTO_MISSION and it never navigates. Neither of those is a real mission-authoring
        # pattern; a NAV_TAKEOFF item is. Left opt-in (default False) so hexarotor's
        # already-working path (config.yaml/mission.yaml, tested end-to-end) is untouched.
        use_takeoff_item = bool(mission_config.get("include_takeoff_item", False))

        if use_takeoff_item:
            await self._run_takeoff_item_mission(mission_config, mission_items, rtl_after_mission)
            return

        if bool(mission_config.get("clear_existing", True)):
            print("MAVSDK: clearing existing onboard mission")
            await self._run_mission_call("clear_mission", self.drone.mission.clear_mission)

        print(f"MAVSDK: RTL after mission set to {rtl_after_mission}")
        await self._run_mission_call(
            "set_return_to_launch_after_mission",
            lambda: self.drone.mission.set_return_to_launch_after_mission(rtl_after_mission),
        )

        print("MAVSDK: uploading mission")
        await self._run_mission_call("upload_mission", lambda: self.drone.mission.upload_mission(plan))
        await self._run_mission_call(
            "set_current_mission_item", lambda: self.drone.mission.set_current_mission_item(0)
        )
        print("MAVSDK: mission upload accepted")

        if self.vehicle_config.get("auto_arm", False):
            await self.arm()
            await self._apply_post_arm_params()
        else:
            print("MAVSDK: auto_arm=false; mission start may be rejected by PX4")

        if self.vehicle_config.get("auto_takeoff", False):
            await self.takeoff()
            takeoff_altitude = float(self.vehicle_config.get("takeoff_altitude_m", 3.0))
            await self._wait_relative_altitude(takeoff_altitude)
            settle_s = float(mission_config.get("takeoff_settle_s", 2.0))
            if settle_s > 0:
                print(f"MAVSDK: takeoff settle wait {settle_s:.1f} s")
                await asyncio.sleep(settle_s)
        else:
            print("MAVSDK: auto_takeoff=false; starting uploaded mission without explicit takeoff")

        print("MAVSDK: starting waypoint mission")
        await self._run_mission_call("start_mission", self.drone.mission.start_mission)
        await self._wait_mission_complete(
            expected_total=len(mission_items),
            timeout_s=float(mission_config.get("mission_timeout_s", 180.0)),
            plugin=self.drone.mission,
        )

        if bool(mission_config.get("land_after_mission", True)):
            await self.land()
            await self._wait_landed(timeout_s=float(mission_config.get("land_timeout_s", 90.0)))

        print("MAVSDK: MISSION SUCCESS: takeoff, waypoint mission, and landing completed")

    async def _run_takeoff_item_mission(
        self,
        mission_config: dict[str, Any],
        mission_items: list[MissionItem],
        rtl_after_mission: bool,
    ) -> None:
        # A NAV_TAKEOFF item's x/y=0,0 is NOT treated as "use wherever we are" by PX4's mission
        # feasibility checker, despite that being the usual real-GCS convention -- confirmed
        # live: it reads 0,0 as literal lat=0/lon=0 (off the coast of west Africa) and rejects
        # the mission as "First waypoint too far away: 7178487m, 5000 max". Fetch the vehicle's
        # actual current position and use that instead.
        try:
            current_lat, current_lon = await asyncio.wait_for(
                self._get_current_position(),
                timeout=float(self.vehicle_config.get("initial_position_timeout_s", 15.0)),
            )
        except asyncio.TimeoutError:
            try:
                current_lat = float(self.vehicle_config["initial_latitude_deg"])
                current_lon = float(self.vehicle_config["initial_longitude_deg"])
            except (KeyError, TypeError, ValueError) as exc:
                raise TimeoutError(
                    "MAVSDK: no current position fix and no valid configured initial coordinates"
                ) from exc
            print(
                "MAVSDK: current position unavailable before yaw alignment; "
                f"using configured SITL reset position {current_lat:.7f}, {current_lon:.7f}"
            )
        raw_items = self._build_raw_mission_items(
            mission_config, mission_items, rtl_after_mission, current_lat, current_lon
        )
        print(
            f"MAVSDK: mission_raw plan has {len(raw_items)} item(s) "
            "(takeoff + waypoints + explicit fixed-wing termination)"
        )

        print("MAVSDK: clearing existing onboard mission (mission_raw)")
        await self._run_mission_call(
            "clear_mission (raw)", self.drone.mission_raw.clear_mission, exc_types=MissionRawError
        )

        print("MAVSDK: uploading mission (mission_raw)")
        await self._run_mission_call(
            "upload_mission (raw)",
            lambda: self.drone.mission_raw.upload_mission(raw_items),
            exc_types=MissionRawError,
        )
        await self._run_mission_call(
            "set_current_mission_item (raw)",
            lambda: self.drone.mission_raw.set_current_mission_item(0),
            exc_types=MissionRawError,
        )
        print("MAVSDK: mission upload accepted")

        if self.vehicle_config.get("auto_arm", False):
            await self.arm()
            await self._apply_post_arm_params()
        else:
            print("MAVSDK: auto_arm=false; mission start may be rejected by PX4")

        # No standalone action.takeoff() here on purpose -- item 0 of raw_items IS the takeoff,
        # PX4 flies it as part of AUTO_MISSION once started, same as arming under a
        # QGroundControl-uploaded fixed-wing plan.
        print("MAVSDK: starting waypoint mission (raw, PX4 owns the takeoff item)")
        await self._run_mission_call(
            "start_mission (raw)",
            self.drone.mission_raw.start_mission,
            attempts=int(self.vehicle_config.get("start_mission_attempts", 4)),
            exc_types=MissionRawError,
        )
        delayed_params_task = asyncio.create_task(self._apply_delayed_post_start_params())
        await self._wait_mission_complete(
            expected_total=len(raw_items),
            timeout_s=float(mission_config.get("mission_timeout_s", 180.0)),
            plugin=self.drone.mission_raw,
        )
        await delayed_params_task
        explicit_land = bool(mission_config.get("land_after_mission", False))
        print("MAVSDK: mission_raw items exhausted")

        if explicit_land:
            # The raw plan already contains an approach waypoint and NAV_LAND.
            # Sending action.land() only after an RTL item never works for fixed
            # wing: RTL remains active while orbiting home and therefore never
            # becomes a completed mission item.
            await self._wait_landed(
                timeout_s=float(mission_config.get("land_timeout_s", 900.0))
            )
            print(
                "MAVSDK: MISSION SUCCESS: takeoff item, waypoint mission, explicit "
                "approach, and landing completed"
            )
        else:
            print(
                "MAVSDK: MISSION SUCCESS: takeoff item, waypoint mission, and RTL "
                "handoff completed (landing not requested)"
            )

    async def _apply_post_arm_params(self) -> None:
        """Apply optional staged EKF settings after initialization but before takeoff."""
        params = self.vehicle_config.get("post_arm_params", {})
        for name, value in params.items():
            print(f"MAVSDK: setting post-arm parameter {name}={value}")
            if isinstance(value, int):
                call = self.drone.param.set_param_int(name, value)
            else:
                call = self.drone.param.set_param_float(name, float(value))
            await asyncio.wait_for(call, timeout=15.0)
            print(f"MAVSDK: post-arm parameter {name} accepted")

    async def _apply_delayed_post_start_params(self) -> None:
        """Apply optional EKF settings after takeoff has begun and yaw is observable."""
        params = self.vehicle_config.get("delayed_post_start_params", {})
        if not params:
            return
        delay_s = float(self.vehicle_config.get("delayed_post_start_params_delay_s", 0.0))
        print(f"MAVSDK: waiting {delay_s:.1f}s before delayed post-start parameters")
        await asyncio.sleep(delay_s)
        for name, value in params.items():
            print(f"MAVSDK: setting delayed post-start parameter {name}={value}")
            if isinstance(value, int):
                call = self.drone.param.set_param_int(name, value)
            else:
                call = self.drone.param.set_param_float(name, float(value))
            await asyncio.wait_for(call, timeout=15.0)
            print(f"MAVSDK: delayed post-start parameter {name} accepted")

    async def _apply_pre_arm_params(self) -> None:
        """Apply optional pre-arm thresholds before waiting on PX4's armable flag."""
        params = self.vehicle_config.get("pre_arm_params", {})
        for name, value in params.items():
            print(f"MAVSDK: setting pre-arm parameter {name}={value}")
            if isinstance(value, int):
                call = self.drone.param.set_param_int(name, value)
            else:
                call = self.drone.param.set_param_float(name, float(value))
            await asyncio.wait_for(call, timeout=15.0)
            print(f"MAVSDK: pre-arm parameter {name} accepted")

    async def _get_current_position(self) -> tuple[float, float]:
        async for position in self.drone.telemetry.position():
            return position.latitude_deg, position.longitude_deg
        raise RuntimeError("MAVSDK: telemetry.position() stream ended before yielding a fix")

    def _build_raw_mission_items(
        self,
        mission_config: dict[str, Any],
        mission_items: list[MissionItem],
        rtl_after_mission: bool,
        current_lat: float,
        current_lon: float,
    ) -> list[RawMissionItem]:
        takeoff_altitude = float(self.vehicle_config.get("takeoff_altitude_m", 25.0))
        takeoff_heading_deg = float(self.vehicle_config.get("takeoff_heading_deg", 105.0))
        takeoff_runway_m = float(self.vehicle_config.get("takeoff_runway_m", 700.0))

        # A fixed-wing NAV_TAKEOFF item at the exact current position has no
        # usable runway bearing. PX4 then steers toward an undefined/behind
        # track while still on its wheels instead of accelerating straight to
        # rotation speed. Place the takeoff target ahead on the configured
        # runway heading; altitude remains relative to home.
        earth_radius_m = 6371000.0
        heading_rad = math.radians(takeoff_heading_deg)
        lat_rad = math.radians(current_lat)
        takeoff_lat = current_lat + math.degrees(
            takeoff_runway_m * math.cos(heading_rad) / earth_radius_m
        )
        takeoff_lon = current_lon + math.degrees(
            takeoff_runway_m * math.sin(heading_rad)
            / (earth_radius_m * math.cos(lat_rad))
        )

        items: list[RawMissionItem] = []
        seq = 0

        # x/y = the vehicle's actual current position, NOT 0/0 -- see the comment in
        # _run_takeoff_item_mission on why 0/0 (the usual "wherever we are" convention) doesn't
        # work here: PX4's feasibility checker reads it as literal lat=0/lon=0 and rejects the
        # mission as being ~7178 km from home.
        items.append(
            RawMissionItem(
                seq, _MAV_FRAME_GLOBAL_RELATIVE_ALT_INT, _MAV_CMD_NAV_TAKEOFF,
                1, 1,
                0.0, 0.0, 0.0, float("nan"),
                round(takeoff_lat * 1e7), round(takeoff_lon * 1e7), takeoff_altitude,
                _MAV_MISSION_TYPE_MISSION,
            )
        )
        seq += 1

        # NOT inserting a MAV_CMD_DO_CHANGE_SPEED item here (tried it, upload_mission() rejected
        # the whole plan with UNSUPPORTED -- mission_raw's upload validation apparently doesn't
        # accept a bare DO_* command mixed in with NAV_* items, at least not shaped this way).
        # Cruise speed falls back to PX4's own FW_AIRSPD_TRIM default instead of
        # mission_c172p.yaml's speed_m_s -- a real gap, but a separate problem from the takeoff
        # item this method exists to fix. Revisit if per-mission airspeed control matters later.
        for item in mission_items:
            hold_time = 0.0 if math.isnan(item.loiter_time_s) else item.loiter_time_s
            items.append(
                RawMissionItem(
                    seq, _MAV_FRAME_GLOBAL_RELATIVE_ALT_INT, _MAV_CMD_NAV_WAYPOINT,
                    0, 1,
                    hold_time, item.acceptance_radius_m, 0.0, float("nan"),
                    round(item.latitude_deg * 1e7), round(item.longitude_deg * 1e7),
                    item.relative_altitude_m,
                    _MAV_MISSION_TYPE_MISSION,
                )
            )
            seq += 1

        if bool(mission_config.get("land_after_mission", False)):
            # A fixed-wing NAV_RETURN_TO_LAUNCH item is intentionally not used
            # here. PX4 circles home at RTL altitude indefinitely, so the item
            # never completes and the old post-mission action.land() call is
            # unreachable. Author a straight-in approach followed by NAV_LAND.
            landing_heading_deg = float(
                mission_config.get("landing_heading_deg", takeoff_heading_deg)
            )
            landing_approach_m = float(
                mission_config.get("landing_approach_m", 900.0)
            )
            landing_approach_alt_m = float(
                mission_config.get("landing_approach_altitude_m", 60.0)
            )
            landing_heading_rad = math.radians(landing_heading_deg)
            approach_lat = current_lat - math.degrees(
                landing_approach_m * math.cos(landing_heading_rad) / earth_radius_m
            )
            approach_lon = current_lon - math.degrees(
                landing_approach_m * math.sin(landing_heading_rad)
                / (earth_radius_m * math.cos(lat_rad))
            )
            items.append(
                RawMissionItem(
                    seq, _MAV_FRAME_GLOBAL_RELATIVE_ALT_INT, _MAV_CMD_NAV_WAYPOINT,
                    0, 1,
                    0.0, 120.0, 0.0, float("nan"),
                    round(approach_lat * 1e7), round(approach_lon * 1e7),
                    landing_approach_alt_m,
                    _MAV_MISSION_TYPE_MISSION,
                )
            )
            seq += 1
            items.append(
                RawMissionItem(
                    seq, _MAV_FRAME_GLOBAL_RELATIVE_ALT_INT, _MAV_CMD_NAV_LAND,
                    0, 1,
                    0.0, 0.0, 0.0, landing_heading_deg,
                    round(current_lat * 1e7), round(current_lon * 1e7), 0.0,
                    _MAV_MISSION_TYPE_MISSION,
                )
            )
            seq += 1
        elif rtl_after_mission:
            items.append(
                RawMissionItem(
                    seq, _MAV_FRAME_MISSION, _MAV_CMD_NAV_RETURN_TO_LAUNCH,
                    0, 1,
                    0.0, 0.0, 0.0, 0.0,
                    0, 0, 0.0,
                    _MAV_MISSION_TYPE_MISSION,
                )
            )
            seq += 1

        return items

    async def dump_params(self, path: str | Path) -> None:
        """Write all current PX4 parameters to a text file (name=value per line).

        Used to capture px4_params_before.txt / px4_params_after.txt so a debug
        session can diff parameter state across a run without QGroundControl.

        Wrapped in a timeout: get_all_params() has no internal timeout of its own, and a
        heavily loaded MAVLink link (e.g. session_11 -- recurring prearm STATUSTEXT
        traffic saturating the serial link right after an autopilot reboot) can leave
        the ~900-parameter PARAM_REQUEST_LIST exchange incomplete forever, which would
        otherwise hang the entire session before arm()/wait_until_ready() ever run.
        """
        print(f"MAVSDK: dumping PX4 parameters to {path}")
        timeout_s = float(self.vehicle_config.get("param_dump_timeout_s", 30.0))
        all_params = await asyncio.wait_for(self.drone.param.get_all_params(), timeout=timeout_s)
        lines: list[str] = []
        for attr_name in ("int_params", "float_params", "custom_params"):
            for param in getattr(all_params, attr_name, None) or []:
                lines.append(f"{param.name}={param.value}")
        lines.sort()
        out_path = Path(path)
        out_path.parent.mkdir(parents=True, exist_ok=True)
        out_path.write_text("\n".join(lines) + ("\n" if lines else ""), encoding="utf-8")
        print(f"MAVSDK: wrote {len(lines)} parameters to {out_path}")

    async def _run_action(
        self, name: str, action, attempts: int = 4, timeout_s: float = 15.0
    ) -> None:
        # Same single-shot-RPC-with-no-retry problem as mission calls (see
        # _run_mission_call below) -- confirmed live: a hexarotor's set_takeoff_altitude()/
        # takeoff() hung after arm() succeeded, and PX4's own preflight-disarm timer kicked in
        # before the (never-arriving) response would have. arm()/land()/return_to_launch()/
        # disarm() all go through here too and are equally exposed.
        #
        # Retrying alone isn't enough: confirmed live that a lost packet doesn't always surface
        # as ActionError -- set_takeoff_altitude() just hung with no response for 4.5+ minutes,
        # never raising anything for the retry loop to catch. MAVSDK's action calls apparently
        # have no client-side timeout of their own, unlike mission calls (which do eventually
        # raise MissionError: TIMEOUT on their own). Wrapping each attempt in asyncio.wait_for
        # turns that hang into somethig retryable too.
        last_exc: ActionError | asyncio.TimeoutError | None = None
        for attempt in range(1, attempts + 1):
            try:
                print(f"MAVSDK: sending {name}() (attempt {attempt}/{attempts})")
                await asyncio.wait_for(action(), timeout=timeout_s)
                print(f"MAVSDK: {name} accepted")
                return
            except (ActionError, asyncio.TimeoutError) as exc:
                last_exc = exc
                print(f"MAVSDK: {name} failed on attempt {attempt}/{attempts}: {exc!r}")
                if attempt < attempts:
                    await asyncio.sleep(2.0)
        assert last_exc is not None
        raise last_exc

    async def _run_mission_call(
        self,
        name: str,
        action,
        attempts: int = 4,
        timeout_s: float = 15.0,
        exc_types: type[Exception] | tuple[type[Exception], ...] = MissionError,
    ) -> None:
        # The mission microservice's MISSION_CLEAR_ALL/MISSION_ACK-style RPCs are a single
        # request/response pair with no built-in retry (unlike telemetry streams, which just
        # keep publishing) -- a single UDP packet lost on this bridge (WSL2 NAT hop +
        # pymavlink forward relay, see listener_pymavlink.py) times out the whole call with no
        # recovery. Retrying the same idempotent call a few times is cheap and matches how a
        # human retrying the same QGroundControl button click would recover from the same
        # transient loss. Also wrapped in wait_for -- see _run_action's comment on why relying
        # on the call to raise its own timeout isn't reliable enough on its own.
        # exc_types defaults to MissionError (the `mission` plugin); pass MissionRawError for
        # the `mission_raw` plugin used by _run_takeoff_item_mission -- same RPC, different
        # exception type.
        last_exc: Exception | None = None
        for attempt in range(1, attempts + 1):
            try:
                print(f"MAVSDK: sending {name}() (attempt {attempt}/{attempts})")
                await asyncio.wait_for(action(), timeout=timeout_s)
                print(f"MAVSDK: {name} accepted")
                return
            except (exc_types, asyncio.TimeoutError) as exc:
                last_exc = exc
                print(f"MAVSDK: {name} failed on attempt {attempt}/{attempts}: {exc!r}")
                if attempt < attempts:
                    await asyncio.sleep(2.0)
        assert last_exc is not None
        raise last_exc

    async def _print_position(self, samples: int) -> None:
        count = 0
        async for position in self.drone.telemetry.position():
            print(
                "  position: "
                f"lat={position.latitude_deg:.7f} lon={position.longitude_deg:.7f} "
                f"abs_alt={position.absolute_altitude_m:.2f} rel_alt={position.relative_altitude_m:.2f}"
            )
            count += 1
            if count >= samples:
                return

    async def _print_battery(self, samples: int) -> None:
        count = 0
        async for battery in self.drone.telemetry.battery():
            print(
                "  battery: "
                f"voltage={battery.voltage_v:.2f} remaining={battery.remaining_percent:.2f}"
            )
            count += 1
            if count >= samples:
                return

    async def _print_flight_mode(self, samples: int) -> None:
        count = 0
        async for flight_mode in self.drone.telemetry.flight_mode():
            print(f"  flight_mode: {flight_mode}")
            count += 1
            if count >= samples:
                return

    def _mission_path(self) -> Path | None:
        raw_path = self.vehicle_config.get("mission_file")
        if not raw_path:
            return None
        path = Path(str(raw_path))
        if not path.is_absolute():
            path = self.config_dir / path
        return path

    def _load_mission_config(self, mission_path: Path) -> dict[str, Any]:
        if not mission_path.exists():
            raise FileNotFoundError(f"MAVSDK: mission file not found: {mission_path}")
        with mission_path.open("r", encoding="utf-8") as stream:
            document = yaml.safe_load(stream) or {}
        mission_config = document.get("mission", document)
        if not isinstance(mission_config, dict):
            raise ValueError(f"MAVSDK: mission file must contain a mapping: {mission_path}")
        if mission_config.get("enabled", True) is False:
            raise ValueError(f"MAVSDK: mission is disabled in {mission_path}")
        return mission_config

    def _build_mission_plan(self, mission_config: dict[str, Any]) -> MissionPlan:
        waypoints = mission_config.get("waypoints", [])
        if not isinstance(waypoints, list) or not waypoints:
            raise ValueError("MAVSDK: mission.yaml must contain at least one waypoint")

        max_mission_altitude = float(
            self.safety_config.get(
                "max_mission_altitude_m",
                self.safety_config.get("max_takeoff_altitude_m", 10.0),
            )
        )
        mission_items = [
            self._mission_item_from_config(idx, waypoint, mission_config, max_mission_altitude)
            for idx, waypoint in enumerate(waypoints, start=1)
        ]
        return MissionPlan(mission_items)

    def _mission_item_from_config(
        self,
        idx: int,
        waypoint: Any,
        mission_config: dict[str, Any],
        max_mission_altitude: float,
    ) -> MissionItem:
        if not isinstance(waypoint, dict):
            raise ValueError(f"MAVSDK: waypoint {idx} must be a mapping")

        latitude = _required_float(waypoint, "latitude_deg", idx)
        longitude = _required_float(waypoint, "longitude_deg", idx)
        altitude = _float_value(
            waypoint.get(
                "relative_altitude_m",
                mission_config.get(
                    "relative_altitude_m",
                    self.vehicle_config.get("takeoff_altitude_m", 3.0),
                ),
            ),
            "relative_altitude_m",
            idx,
        )

        if not -90.0 <= latitude <= 90.0:
            raise ValueError(f"MAVSDK: waypoint {idx} latitude out of range: {latitude}")
        if not -180.0 <= longitude <= 180.0:
            raise ValueError(f"MAVSDK: waypoint {idx} longitude out of range: {longitude}")
        if altitude <= 0.0:
            raise ValueError(f"MAVSDK: waypoint {idx} altitude must be positive: {altitude}")
        if altitude > max_mission_altitude:
            raise ValueError(
                f"MAVSDK: waypoint {idx} altitude {altitude} m exceeds "
                f"safety.max_mission_altitude_m={max_mission_altitude} m"
            )

        speed = _float_value(
            waypoint.get("speed_m_s", mission_config.get("speed_m_s", 3.0)),
            "speed_m_s",
            idx,
        )
        acceptance_radius = _float_value(
            waypoint.get("acceptance_radius_m", mission_config.get("acceptance_radius_m", 2.0)),
            "acceptance_radius_m",
            idx,
        )

        return MissionItem(
            latitude,
            longitude,
            altitude,
            speed,
            bool(waypoint.get("is_fly_through", mission_config.get("is_fly_through", False))),
            _optional_float(waypoint.get("gimbal_pitch_deg", mission_config.get("gimbal_pitch_deg"))),
            _optional_float(waypoint.get("gimbal_yaw_deg", mission_config.get("gimbal_yaw_deg"))),
            _enum_value(
                MissionItem.CameraAction,
                waypoint.get("camera_action", mission_config.get("camera_action", "NONE")),
                "camera_action",
                idx,
            ),
            _optional_float(waypoint.get("loiter_time_s", mission_config.get("loiter_time_s", math.nan))),
            _float_value(
                waypoint.get("camera_photo_interval_s", mission_config.get("camera_photo_interval_s", 0.0)),
                "camera_photo_interval_s",
                idx,
            ),
            acceptance_radius,
            _optional_float(waypoint.get("yaw_deg", mission_config.get("yaw_deg"))),
            _float_value(
                waypoint.get("camera_photo_distance_m", mission_config.get("camera_photo_distance_m", 0.0)),
                "camera_photo_distance_m",
                idx,
            ),
            _enum_value(
                MissionItem.VehicleAction,
                waypoint.get("vehicle_action", mission_config.get("vehicle_action", "NONE")),
                "vehicle_action",
                idx,
            ),
        )

    async def _wait_relative_altitude(self, target_altitude_m: float) -> None:
        timeout_s = float(self.vehicle_config.get("takeoff_timeout_s", 60.0))
        threshold = max(0.5, 0.8 * target_altitude_m)
        print(f"MAVSDK: waiting for takeoff altitude >= {threshold:.1f} m")
        start = asyncio.get_running_loop().time()
        last_print = 0.0

        async for position in self.drone.telemetry.position():
            rel_altitude = position.relative_altitude_m
            now = asyncio.get_running_loop().time()
            if rel_altitude >= threshold:
                print(f"MAVSDK: takeoff altitude reached rel_alt={rel_altitude:.2f} m")
                return
            if now - start > timeout_s:
                raise TimeoutError(
                    "MAVSDK: timed out waiting for takeoff altitude "
                    f">= {threshold:.1f} m after {timeout_s:.1f}s"
                )
            if now - last_print >= 2.0:
                print(f"MAVSDK: takeoff climb pending rel_alt={rel_altitude:.2f} m")
                last_print = now

    async def _wait_mission_complete(self, expected_total: int, timeout_s: float, plugin=None) -> None:
        # plugin: self.drone.mission (default) or self.drone.mission_raw -- both expose
        # is_mission_finished()/mission_progress() with the same shape, so the raw-mission path
        # (_run_takeoff_item_mission) can reuse this unchanged instead of duplicating it.
        if plugin is None:
            plugin = self.drone.mission
        print("MAVSDK: monitoring mission progress")
        done_event = asyncio.Event()
        progress_task = asyncio.create_task(
            self._monitor_mission_progress(expected_total, done_event, plugin)
        )
        deadline = asyncio.get_running_loop().time() + timeout_s
        try:
            while True:
                if done_event.is_set() or await plugin.is_mission_finished():
                    print("MAVSDK: all mission waypoints reached")
                    return
                if asyncio.get_running_loop().time() >= deadline:
                    raise TimeoutError(f"MAVSDK: mission did not finish within {timeout_s:.1f}s")
                await asyncio.sleep(1.0)
        finally:
            progress_task.cancel()
            await asyncio.gather(progress_task, return_exceptions=True)

    async def _monitor_mission_progress(
        self, expected_total: int, done_event: asyncio.Event, plugin=None
    ) -> None:
        if plugin is None:
            plugin = self.drone.mission
        last_seen: tuple[int, int] | None = None
        async for progress in plugin.mission_progress():
            current = int(progress.current)
            total = int(progress.total)
            if (current, total) != last_seen:
                print(f"MAVSDK: mission progress {current}/{total}")
                last_seen = (current, total)
            if total > 0 and total != expected_total:
                print(f"MAVSDK: mission progress total from PX4 is {total}, expected {expected_total}")
            if total > 0 and current >= total:
                done_event.set()
                return

    async def _wait_landed(self, timeout_s: float) -> None:
        print("MAVSDK: waiting for landing confirmation")
        start = asyncio.get_running_loop().time()
        last_print = 0.0

        async for landed_state in self.drone.telemetry.landed_state():
            now = asyncio.get_running_loop().time()
            if landed_state == LandedState.ON_GROUND:
                print("MAVSDK: landing confirmed ON_GROUND")
                return
            if now - start > timeout_s:
                raise TimeoutError(f"MAVSDK: landing not confirmed within {timeout_s:.1f}s")
            if now - last_print >= 2.0:
                print(f"MAVSDK: landing pending state={landed_state}")
                last_print = now


def _required_float(mapping: dict[str, Any], key: str, waypoint_idx: int) -> float:
    if key not in mapping:
        raise ValueError(f"MAVSDK: waypoint {waypoint_idx} missing required field '{key}'")
    return _float_value(mapping[key], key, waypoint_idx)


def _float_value(value: Any, key: str, waypoint_idx: int) -> float:
    try:
        return float(value)
    except (TypeError, ValueError) as exc:
        raise ValueError(
            f"MAVSDK: waypoint {waypoint_idx} field '{key}' must be numeric, got {value!r}"
        ) from exc


def _optional_float(value: Any) -> float:
    if value is None:
        return math.nan
    if isinstance(value, str) and value.strip().lower() in {"", "nan", "none", "null"}:
        return math.nan
    return float(value)


def _enum_value(enum_type, value: Any, key: str, waypoint_idx: int):
    enum_name = str(value).strip().upper().replace("-", "_").replace(" ", "_")
    if enum_name in enum_type.__members__:
        return enum_type[enum_name]
    valid = ", ".join(enum_type.__members__)
    raise ValueError(
        f"MAVSDK: waypoint {waypoint_idx} field '{key}' must be one of {valid}, got {value!r}"
    )
