"""MAVSDK high-level flight instructor for PX4 HIL/SITL sessions."""

from __future__ import annotations

import asyncio
from pathlib import Path
from typing import Any

from mavsdk import System
from mavsdk.action import ActionError

from safety import confirm_real_vehicle_allowed, validate_takeoff_altitude


class MavsdkInstructor:
    """High-level controller. This is the only class that sends flight commands."""

    def __init__(self, config: dict[str, Any]) -> None:
        self.config = config
        self.vehicle_config = config.get("vehicle", {})
        self.safety_config = config.get("safety", {})
        self.address = self.vehicle_config.get("mavsdk_address", "udp://:14540")
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

        print("MAVSDK: waiting for global position and home position health")
        timeout_s = float(self.vehicle_config.get("health_timeout_s", 60.0))
        start = asyncio.get_running_loop().time()
        async for health in self.drone.telemetry.health():
            if health.is_global_position_ok and health.is_home_position_ok:
                print("MAVSDK: position health is OK")
                return
            if asyncio.get_running_loop().time() - start > timeout_s:
                raise TimeoutError(
                    "MAVSDK: timed out waiting for global/home position health "
                    f"after {timeout_s}s"
                )
            print(
                "MAVSDK: health pending "
                f"global={health.is_global_position_ok} home={health.is_home_position_ok}"
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
            await asyncio.wait_for(asyncio.gather(*tasks), timeout=timeout_s)
        except TimeoutError:
            print(f"MAVSDK: telemetry sampling timed out after {timeout_s}s")
            for task in tasks:
                task.cancel()
            await asyncio.gather(*tasks, return_exceptions=True)

    async def run_demo_sequence(self) -> None:
        await self.print_basic_telemetry(samples=3)

        if self.vehicle_config.get("auto_arm", False) or self.vehicle_config.get("auto_takeoff", False):
            await self.wait_until_ready()

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

    async def dump_params(self, path: str | Path) -> None:
        """Write all current PX4 parameters to a text file (name=value per line).

        Used to capture px4_params_before.txt / px4_params_after.txt so a debug
        session can diff parameter state across a run without QGroundControl.
        """
        print(f"MAVSDK: dumping PX4 parameters to {path}")
        all_params = await self.drone.param.get_all_params()
        lines: list[str] = []
        for attr_name in ("int_params", "float_params", "custom_params"):
            for param in getattr(all_params, attr_name, None) or []:
                lines.append(f"{param.name}={param.value}")
        lines.sort()
        out_path = Path(path)
        out_path.parent.mkdir(parents=True, exist_ok=True)
        out_path.write_text("\n".join(lines) + ("\n" if lines else ""), encoding="utf-8")
        print(f"MAVSDK: wrote {len(lines)} parameters to {out_path}")

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
