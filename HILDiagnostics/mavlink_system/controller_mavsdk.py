"""MAVSDK high-level flight instructor for PX4 HIL/SITL sessions."""

from __future__ import annotations

import asyncio
import math
from pathlib import Path
from typing import Any

from mavsdk import System
from mavsdk.action import ActionError
from mavsdk.mission import MissionItem, MissionPlan
from mavsdk.telemetry import LandedState
import yaml

from safety import confirm_real_vehicle_allowed, validate_takeoff_altitude


class GpsInnovationAbort(Exception):
    """Raised by _wait_mission_complete once the automatic GPS-innovation abort has already
    commanded hold()+land() (see _monitor_gps_innovation_abort). Callers should treat this as
    a handled, deliberate abort -- not an error to retry or to work around by loosening the
    EKF's own innovation gates."""


class MavsdkInstructor:
    """High-level controller. This is the only class that sends flight commands."""

    def __init__(
        self,
        config: dict[str, Any],
        config_dir: str | Path | None = None,
        base_dir: str | Path | None = None,
        listener: Any | None = None,
    ) -> None:
        self.config = config
        self.vehicle_config = config.get("vehicle", {})
        self.safety_config = config.get("safety", {})
        self.address = self.vehicle_config.get("mavsdk_address", "udp://:14540")
        self.config_dir = Path(config_dir) if config_dir is not None else Path.cwd()
        self.base_dir = Path(base_dir) if base_dir is not None else None
        # PymavlinkListener instance (or None), reused to fire the takeoff-gate probe
        # through its own already-connected socket -- see _run_takeoff_gate_probe().
        self.listener = listener
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

    async def _poll_health_until_ready(self) -> None:
        async for health in self.drone.telemetry.health():
            if health.is_global_position_ok and health.is_home_position_ok and health.is_armable:
                print("MAVSDK: position health is OK and vehicle is armable")
                return
            print(
                "MAVSDK: health pending "
                f"global={health.is_global_position_ok} home={health.is_home_position_ok} "
                f"armable={health.is_armable}"
            )
            await asyncio.sleep(1.0)

    async def arm(self) -> None:
        if not confirm_real_vehicle_allowed(self.config):
            print("MAVSDK DRY-RUN: arm()")
            return
        await self._run_action("arm", self.drone.action.arm)

    async def takeoff(self) -> None:
        altitude = float(self.vehicle_config.get("takeoff_altitude_m", 3.0))
        max_altitude = float(self.safety_config.get("max_takeoff_altitude_m", 10.0))
        validate_takeoff_altitude(altitude, max_altitude)

        if not confirm_real_vehicle_allowed(self.config):
            print(f"MAVSDK DRY-RUN: set_takeoff_altitude({altitude})")
            print("MAVSDK DRY-RUN: takeoff()")
            return

        try:
            await self.drone.action.set_takeoff_altitude(altitude)
        except ActionError as exc:
            print(f"MAVSDK: set_takeoff_altitude failed: {exc}")
            raise

        # PX4's own COM_DISARM_PRFLT (default 10.0s) auto-disarms if it never takes off,
        # so the live window to inspect vehicle_constraints/trajectory_setpoint/
        # flight_mode_manager is only ~8s after this call -- see
        # HIL_ZERO_THRUST_AND_PARAM_RELIABILITY_NEXT_STEPS.md section 3.3/3.4. Fire the
        # probe concurrently with the takeoff() RPC itself (not after it returns) to use
        # as much of that budget as possible.
        probe_task = (
            asyncio.create_task(self._run_takeoff_gate_probe()) if self.listener is not None else None
        )
        await self._run_action("takeoff", self.drone.action.takeoff)
        if probe_task is not None:
            await probe_task

    async def land(self) -> None:
        if not confirm_real_vehicle_allowed(self.config):
            print("MAVSDK DRY-RUN: land()")
            return
        await self._run_action("land", self.drone.action.land)

    async def hold(self) -> None:
        if not confirm_real_vehicle_allowed(self.config):
            print("MAVSDK DRY-RUN: hold()")
            return
        await self._run_action("hold", self.drone.action.hold)

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
            await asyncio.wait_for(asyncio.gather(*tasks), timeout=timeout_s)
        except TimeoutError:
            print(f"MAVSDK: telemetry sampling timed out after {timeout_s}s")
            for task in tasks:
                task.cancel()
            await asyncio.gather(*tasks, return_exceptions=True)

    async def run_demo_sequence(self) -> None:
        await self.print_basic_telemetry(samples=3)

        mission_path = self._mission_path()
        mission_requested = bool(self.vehicle_config.get("auto_mission", False) and mission_path)

        if (
            self.vehicle_config.get("auto_arm", False)
            or self.vehicle_config.get("auto_takeoff", False)
            or mission_requested
        ):
            await self.wait_until_ready()

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

        if bool(mission_config.get("clear_existing", True)):
            print("MAVSDK: clearing existing onboard mission")
            await self.drone.mission.clear_mission()

        rtl_after_mission = bool(mission_config.get("rtl_after_mission", False))
        print(f"MAVSDK: RTL after mission set to {rtl_after_mission}")
        await self.drone.mission.set_return_to_launch_after_mission(rtl_after_mission)

        print("MAVSDK: uploading mission")
        await self.drone.mission.upload_mission(plan)
        await self.drone.mission.set_current_mission_item(0)
        print("MAVSDK: mission upload accepted")

        if self.vehicle_config.get("auto_arm", False):
            await self.arm()
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
        await self.drone.mission.start_mission()
        try:
            await self._wait_mission_complete(
                expected_total=len(mission_items),
                timeout_s=float(mission_config.get("mission_timeout_s", 180.0)),
            )
        except GpsInnovationAbort as exc:
            # _monitor_gps_innovation_abort already issued hold()+land() -- do not send a
            # second, redundant land() here, and do not report this as MISSION SUCCESS.
            print(f"MAVSDK: MISSION ABORTED (GPS innovation): {exc}")
            await self._wait_landed(timeout_s=float(mission_config.get("land_timeout_s", 90.0)))
            return

        if bool(mission_config.get("land_after_mission", True)):
            await self.land()
            await self._wait_landed(timeout_s=float(mission_config.get("land_timeout_s", 90.0)))

        print("MAVSDK: MISSION SUCCESS: takeoff, waypoint mission, and landing completed")

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

    async def _run_takeoff_gate_probe(self) -> None:
        """Probe vehicle_constraints/trajectory_setpoint/flight_mode_manager during the
        live armed window, through the listener's own already-connected socket (not a
        second independent one -- see PymavlinkListener._shell_buffer's docstring for why
        that broke the whole session on 2026-07-15). Source-code trace for what these mean:
        HIL_ZERO_THRUST_AND_PARAM_RELIABILITY_NEXT_STEPS.md section 3.2/3.3.
        """
        if self.listener is None:
            return
        started = asyncio.get_running_loop().time()
        lines = ["trigger: MAVSDK sending takeoff() (probe started at t=0.00s)"]

        async def probe(label: str, command: str, read_window_s: float) -> None:
            elapsed = asyncio.get_running_loop().time() - started
            try:
                self.listener.send_shell_command(command)
            except Exception as exc:  # noqa: BLE001 - diagnostic probe must not break takeoff()
                lines.append(f"[t+{elapsed:5.2f}s] {label}: send failed: {exc}")
                return
            await asyncio.sleep(read_window_s)
            output = self.listener.drain_shell_output()
            lines.append(f"[t+{elapsed:5.2f}s] === {label} ===")
            lines.append(output.strip() or "(no output)")

        try:
            await probe("flight_mode_manager status", "flight_mode_manager status\n", 1.5)
            await probe("listener vehicle_constraints -n 1", "listener vehicle_constraints -n 1\n", 1.5)
            await probe("listener trajectory_setpoint -n 1", "listener trajectory_setpoint -n 1\n", 1.5)
        except Exception as exc:  # noqa: BLE001 - diagnostic probe must not break takeoff()
            lines.append(f"probe error: {exc}")

        if self.base_dir is not None:
            out_path = self.base_dir / "takeoff_gate_probe_result.txt"
            out_path.write_text("\n".join(lines) + "\n", encoding="utf-8")
            print(f"MAVSDK: takeoff-gate probe results written to {out_path}")
        else:
            print("MAVSDK: takeoff-gate probe results:\n" + "\n".join(lines))

    async def _run_action(self, name: str, action) -> None:
        try:
            print(f"MAVSDK: sending {name}()")
            await action()
            print(f"MAVSDK: {name} accepted")
        except ActionError as exc:
            print(f"MAVSDK: {name} failed: {exc}")
            raise

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

    async def _wait_mission_complete(self, expected_total: int, timeout_s: float) -> None:
        print("MAVSDK: monitoring mission progress")
        done_event = asyncio.Event()
        progress_task = asyncio.create_task(
            self._monitor_mission_progress(expected_total, done_event)
        )
        # GPS-innovation abort: only wired up when a PymavlinkListener is present (it's the
        # one that actually watches ESTIMATOR_STATUS -- see listener_pymavlink.py). See
        # HIL_GPS_FUSION_AND_ACTUATOR_OUTPUT_FIX section 5a for why this exists (the
        # wp35->wp36 vibration-model-induced rejection) -- this is a mission-abort safety
        # net, not a substitute for the EKF's own gates.
        abort_task = (
            asyncio.create_task(self._monitor_gps_innovation_abort())
            if self.listener is not None
            else None
        )
        deadline = asyncio.get_running_loop().time() + timeout_s
        try:
            while True:
                if abort_task is not None and abort_task.done():
                    raise GpsInnovationAbort(abort_task.result())
                if done_event.is_set() or await self.drone.mission.is_mission_finished():
                    print("MAVSDK: all mission waypoints reached")
                    return
                if asyncio.get_running_loop().time() >= deadline:
                    raise TimeoutError(f"MAVSDK: mission did not finish within {timeout_s:.1f}s")
                await asyncio.sleep(1.0)
        finally:
            progress_task.cancel()
            await asyncio.gather(progress_task, return_exceptions=True)
            if abort_task is not None and not abort_task.done():
                abort_task.cancel()
                await asyncio.gather(abort_task, return_exceptions=True)

    async def _monitor_gps_innovation_abort(self) -> str:
        """Poll PymavlinkListener.gps_innovation_abort during mission execution. On trigger,
        immediately hold() then land() -- deliberately NOT return_to_launch(): RTL depends on
        the same horizontal position estimate that just failed its own innovation check, so
        commanding a long-range return under that condition is exactly the wrong move.
        """
        assert self.listener is not None
        while True:
            if self.listener.gps_innovation_abort.is_set():
                status = self.listener.gps_innovation_status()
                info = status.get("trigger_info") or {}
                message = (
                    "GPS innovation abort: "
                    f"vel_ratio={info.get('vel_ratio')} pos_horiz_ratio={info.get('pos_horiz_ratio')}"
                )
                print(f"MAVSDK: {message} -- holding then landing immediately")
                try:
                    await self.hold()
                except ActionError as exc:
                    print(f"MAVSDK: hold() during GPS-innovation abort failed, landing anyway: {exc}")
                await asyncio.sleep(1.0)
                await self.land()
                return message
            await asyncio.sleep(0.2)

    async def _monitor_mission_progress(self, expected_total: int, done_event: asyncio.Event) -> None:
        last_seen: tuple[int, int] | None = None
        async for progress in self.drone.mission.mission_progress():
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
