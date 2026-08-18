"""Read-only pymavlink listener for low-level MAVLink debugging."""

from __future__ import annotations

import socket
import threading
import time
from pathlib import Path
from typing import Any, Iterable
from urllib.parse import urlparse

from pymavlink import mavutil

from logger import JsonlLogger
from safety import confirm_pymavlink_commands_allowed


def _parse_udp_forward_target(address: str | None) -> tuple[str, int] | None:
    """Parse a MAVSDK-style 'udp://[host]:port' address into a sendto() target.

    MAVSDK's 'udp://:PORT' form (no host) means "listen locally", which for our purposes
    as a forwarding destination means localhost.
    """
    if not address:
        return None
    parsed = urlparse(address)
    if parsed.scheme not in ("udp", "udpin", "udpout") or parsed.port is None:
        return None
    return (parsed.hostname or "127.0.0.1", parsed.port)


class PymavlinkListener:
    """Listen to selected raw MAVLink messages, write them to JSONL, and (optionally)
    re-broadcast the full raw stream to MAVSDK's UDP port -- and relay MAVSDK's replies
    back to Simulink so commands actually reach the real PX4 over the HITL serial link.

    This forwarding exists because in HITL mode the Simulink PX4 connector relays the
    serial MAVLink stream to a single UDP port (vehicle.pymavlink_address, typically
    14550). MAVSDK listens on a separate port (vehicle.mavsdk_address, typically 14540)
    that nothing else sends to, so without the forward MAVSDK never receives a single
    packet and instructor.connect() hangs forever.

    MAVSDK replies (arm/takeoff/mission commands) get addressed back to whatever
    ephemeral local address _forward_socket's sendto() calls appear to originate from
    (standard MAVLink UDP "reply to last sender" behavior), so that same socket -- not a
    new one -- is what must also recvfrom() them, then relay them on to Simulink's
    command-inbound UDP port (vehicle.simulink_command_address, typically 14541), which
    Simulink mixes into the outbound serial stream to the real board.
    """

    def __init__(self, config: dict[str, Any], base_dir: str | Path | None = None) -> None:
        self.config = config
        self.listener_config = config.get("listener", {})
        self.vehicle_config = config.get("vehicle", {})
        self.address = self.vehicle_config.get("pymavlink_address", "udpin:0.0.0.0:14550")
        self.message_types = set(self.listener_config.get("message_types", []))
        log_file = Path(self.listener_config.get("log_file", "logs/mavlink_raw_log.jsonl"))
        self.log_path = log_file if log_file.is_absolute() else Path(base_dir or ".") / log_file
        self.forward_target: tuple[str, int] | None = None
        if self.listener_config.get("forward_to_mavsdk", True):
            self.forward_target = _parse_udp_forward_target(self.vehicle_config.get("mavsdk_address"))
        self.command_relay_target: tuple[str, int] | None = None
        if self.listener_config.get("relay_commands_to_simulink", True):
            self.command_relay_target = _parse_udp_forward_target(
                self.vehicle_config.get("simulink_command_address")
            )
        self._forward_socket: socket.socket | None = None
        self._command_relay_thread: threading.Thread | None = None
        self._outbound_parser: Any | None = None
        self.master = None
        self.logger: JsonlLogger | None = None
        self._stop = False
        # Buffer for SERIAL_CONTROL replies (PX4 MAVLink shell), captured off this same
        # already-connected socket -- see send_shell_command()/drain_shell_output(). A
        # second independent process binding its own udpin socket on the same
        # pymavlink_address port to read these directly stole/duplicated this listener's
        # own traffic on Windows and broke the whole session's connectivity (confirmed
        # 2026-07-15, session_watch_gate_01 -- see
        # HIL_ZERO_THRUST_AND_PARAM_RELIABILITY_NEXT_STEPS.md section 3.5). Any shell
        # probe must go through this listener's own socket, not a new one.
        self._shell_buffer: list[str] = []
        self._shell_lock = threading.Lock()

        # GPS-innovation abort (added 2026-07-31, see HIL_GPS_FUSION_AND_ACTUATOR_OUTPUT_FIX
        # section 5a / the vibration-model root-cause finding): the wp35->wp36 rejection in
        # session_vibration_full_mission_06 was a real EKF2 rejection of a genuinely bad
        # sensor/trajectory disagreement (vel_ratio=4.19, pos_horiz_ratio=7.93), not something
        # to paper over -- do NOT loosen the EKF's own GPS innovation gates to avoid tripping
        # this. Instead, treat a sustained gate-exceeding innovation as a mission-abort signal:
        # track how long ESTIMATOR_STATUS.vel_ratio/pos_horiz_ratio have continuously exceeded
        # the threshold, and latch self.gps_innovation_abort once that holds for
        # gps_innovation_sustained_s. controller_mavsdk.MavsdkInstructor polls this flag during
        # mission execution and is the one that actually issues hold()/land() -- this class
        # only detects and latches, consistent with "MavsdkInstructor is the only class that
        # sends flight commands."
        safety_config = config.get("safety", {})
        self._gps_innovation_abort_enabled = bool(safety_config.get("gps_innovation_abort_enabled", True))
        self._gps_innovation_threshold = float(safety_config.get("gps_innovation_ratio_threshold", 1.0))
        self._gps_innovation_sustained_s = float(safety_config.get("gps_innovation_sustained_s", 1.0))
        self._gps_innovation_lock = threading.Lock()
        self._gps_innovation_bad_since: float | None = None
        self._gps_innovation_trigger_info: dict[str, Any] | None = None
        self.gps_innovation_abort = threading.Event()

    def connect(self, timeout_s: float = 30.0) -> None:
        print(f"pymavlink: connecting to {self.address}")
        self.master = mavutil.mavlink_connection(self.address)
        heartbeat = self.master.wait_heartbeat(timeout=timeout_s)
        if heartbeat is None:
            raise TimeoutError(f"pymavlink: no HEARTBEAT on {self.address} within {timeout_s}s")
        print(
            "pymavlink: heartbeat from "
            f"system={self.master.target_system} component={self.master.target_component}"
        )
        self.logger = JsonlLogger(self.log_path)
        self.logger.write("LISTENER_CONNECTED", {"address": self.address})
        if self.forward_target is not None:
            self._forward_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            print(
                "pymavlink: forwarding raw MAVLink stream to "
                f"{self.forward_target[0]}:{self.forward_target[1]} for MAVSDK"
            )
            if self.command_relay_target is not None:
                # Timeout lets _relay_commands_to_simulink's recvfrom() notice _stop
                # periodically; harmless to the sendto() calls above sharing this socket,
                # since sendto() on UDP essentially never blocks.
                self._forward_socket.settimeout(1.0)
                # Independent decoder (not self.master's) purely to make MAVSDK's outbound
                # commands visible in console.log/JSONL -- otherwise they vanish into the
                # relay with zero logging, since they never pass through listen_once()'s
                # receive path.
                self._outbound_parser = mavutil.mavlink.MAVLink(None)
                self._outbound_parser.robust_parsing = True
                self._command_relay_thread = threading.Thread(
                    target=self._relay_commands_to_simulink,
                    name="mavsdk-command-relay",
                    daemon=True,
                )
                print(
                    "pymavlink: relaying MAVSDK's outbound commands to "
                    f"{self.command_relay_target[0]}:{self.command_relay_target[1]} for Simulink"
                )
                self._command_relay_thread.start()

    def _relay_commands_to_simulink(self) -> None:
        """Background thread: capture whatever MAVSDK sends back on _forward_socket's
        ephemeral port and relay it to Simulink's UDP Receive block, so arm/takeoff/
        mission commands actually reach the real PX4 over the HITL serial link.
        """
        assert self._forward_socket is not None and self.command_relay_target is not None
        while not self._stop:
            try:
                data, _addr = self._forward_socket.recvfrom(4096)
            except socket.timeout:
                continue
            except ConnectionResetError:
                # Windows-specific: sendto() to MAVSDK's port before mavsdk_server has
                # actually bound it triggers an ICMP port-unreachable, which Windows then
                # delivers as ECONNRESET on the *next* recv on this socket -- even though
                # UDP is connectionless and this is harmless. Without this, the whole
                # relay thread died permanently a few ms into the session (confirmed:
                # first sendto() in listen_once() races mavsdk_server's own startup), so
                # every later MAVSDK command silently went nowhere for the rest of the
                # run. Log once and keep going instead of treating it as fatal.
                print("pymavlink: command relay: ignoring transient ECONNRESET (UDP port-unreachable)")
                continue
            except OSError:
                if self._stop:
                    return
                raise
            self._log_outbound(data)
            try:
                self._forward_socket.sendto(data, self.command_relay_target)
            except OSError as exc:
                print(f"pymavlink: command relay to Simulink failed: {exc}")

    def _log_outbound(self, data: bytes) -> None:
        """Decode and print/log MAVSDK's outbound bytes before relaying them, purely for
        visibility -- a parse failure here must never block the relay itself.
        """
        if self._outbound_parser is None:
            return
        try:
            msgs = self._outbound_parser.parse_buffer(data) or []
        except Exception as exc:  # noqa: BLE001 - diagnostics only, must not break the relay
            print(f"pymavlink: MAVSDK -> PX4: <{len(data)} bytes, failed to decode: {exc}>")
            return
        for msg in msgs:
            msg_type = msg.get_type()
            print(f"{time.strftime('%H:%M:%S')} MAVSDK -> PX4: {msg_type} {msg.to_dict()}")
            if self.logger is not None:
                self.logger.write(f"MAVSDK_OUT_{msg_type}", msg.to_dict())

    def _heal_broken_instance_caches(self) -> int:
        """Repair pymavlink's own add_message() bug (mavutil.py): if a message type's
        instance field is None on its first occurrence, add_message() takes the "simple"
        path and never sets messages[mtype]._instances -- it stays whatever the generated
        message class defaults to (None). If a *later* message of that same type arrives
        with a non-None instance value, add_message() does
        `messages[mtype]._instances[instance_value] = msg`, which raises exactly
        "'NoneType' object does not support item assignment" since _instances is still
        None. Catching that TypeError (see listen_once()) stops the crash but does not fix
        the underlying None, so every future message of that type fails the same way for
        the rest of the session. Scan and reset any broken entries to {} so they recover.
        Returns how many entries were healed (for logging)."""
        if self.master is None:
            return 0
        healed = 0
        for key, value in list(self.master.messages.items()):
            if isinstance(key, str) and "[" in key:
                continue  # per-instance copy (e.g. "GPS_RAW_INT[0]"), not the base entry
            if getattr(value, "_instances", "missing") is None:
                value._instances = {}
                healed += 1
        return healed

    def listen_once(self, timeout_s: float = 1.0) -> bool:
        if self.master is None:
            raise RuntimeError("pymavlink listener is not connected")
        try:
            msg = self.master.recv_match(blocking=True, timeout=timeout_s)
        except TypeError as exc:
            # pymavlink's mavutil.add_message() can corrupt its own internal
            # per-type instance cache when a message's instance field briefly
            # decodes as None (see mavutil.py add_message/_instances). We never
            # read that internal cache ourselves, so just skip this message
            # instead of letting the whole listener thread die.
            healed = self._heal_broken_instance_caches()
            print(
                f"pymavlink: recv_match internal error, skipping message "
                f"(healed {healed} broken instance cache entr{'y' if healed == 1 else 'ies'}): {exc}"
            )
            return False
        except ConnectionResetError:
            # Same Windows-specific quirk already handled in _relay_commands_to_simulink:
            # sendto() to a port nothing is listening on yet triggers an ICMP
            # port-unreachable, which Windows then delivers as ECONNRESET on the *next*
            # recv on this socket, even though UDP is connectionless and this is harmless.
            # Unlike the relay thread, this main receive loop had no protection at all --
            # confirmed 2026-07-31 (session_gps_fix_verify_01): this killed the whole
            # listener (via listen_forever()'s bare `except KeyboardInterrupt`) right after
            # GPS fusion started publishing several new estimator_aid_src_gnss_* topics,
            # which plausibly triggered a fresh burst of port-unreachable replies. Log once
            # and keep going instead of treating it as fatal.
            print("pymavlink: listener: ignoring transient ECONNRESET (UDP port-unreachable)")
            return False
        except OSError as exc:
            if self._stop:
                return False
            print(f"pymavlink: recv_match OS error, skipping message: {exc}")
            return False
        if msg is None:
            return False

        if self._forward_socket is not None and self.forward_target is not None:
            try:
                self._forward_socket.sendto(msg.get_msgbuf(), self.forward_target)
            except OSError as exc:
                print(f"pymavlink: forward to MAVSDK failed: {exc}")

        msg_type = msg.get_type()
        if msg_type == "SERIAL_CONTROL":
            self._capture_serial_control(msg)
        if msg_type == "ESTIMATOR_STATUS":
            self._check_gps_innovation(msg)
        if not self.message_types or msg_type in self.message_types:
            payload = msg.to_dict()
            print(_format_message(msg_type, payload))
            if self.logger is not None:
                self.logger.write(msg_type, payload)
        return True

    def listen_forever(self) -> None:
        print("pymavlink: listener running")
        try:
            while not self._stop:
                self.listen_once(timeout_s=0.5)
        except KeyboardInterrupt:
            print("pymavlink: Ctrl+C received")
        finally:
            self.close()

    def stop(self) -> None:
        self._stop = True

    def close(self) -> None:
        if self.logger is not None:
            self.logger.close()
            self.logger = None
        if self._command_relay_thread is not None:
            self._command_relay_thread.join(timeout=2.0)
            self._command_relay_thread = None
        if self._forward_socket is not None:
            self._forward_socket.close()
            self._forward_socket = None
        if self.master is not None:
            try:
                self.master.close()
            finally:
                self.master = None
        print("pymavlink: listener closed")

    def _check_gps_innovation(self, msg: Any) -> None:
        """Latch self.gps_innovation_abort if ESTIMATOR_STATUS.vel_ratio or .pos_horiz_ratio
        has continuously exceeded the configured threshold (default 1.0, i.e. the innovation
        gate itself) for gps_innovation_sustained_s (default 1.0s). See the __init__ comment
        for why this exists and why it must not be used to justify loosening the EKF's gates.
        """
        if not self._gps_innovation_abort_enabled or self.gps_innovation_abort.is_set():
            return
        vel_ratio = getattr(msg, "vel_ratio", None)
        pos_horiz_ratio = getattr(msg, "pos_horiz_ratio", None)
        if vel_ratio is None or pos_horiz_ratio is None:
            return
        now = time.time()
        bad = vel_ratio > self._gps_innovation_threshold or pos_horiz_ratio > self._gps_innovation_threshold
        with self._gps_innovation_lock:
            if not bad:
                self._gps_innovation_bad_since = None
                return
            if self._gps_innovation_bad_since is None:
                self._gps_innovation_bad_since = now
                return
            if now - self._gps_innovation_bad_since < self._gps_innovation_sustained_s:
                return
            self._gps_innovation_trigger_info = {
                "vel_ratio": vel_ratio,
                "pos_horiz_ratio": pos_horiz_ratio,
                "bad_since": self._gps_innovation_bad_since,
                "triggered_at": now,
            }
            self.gps_innovation_abort.set()
        print(
            f"{time.strftime('%H:%M:%S')} pymavlink: GPS INNOVATION ABORT TRIGGERED -- "
            f"vel_ratio={vel_ratio:.3f} pos_horiz_ratio={pos_horiz_ratio:.3f} sustained "
            f">= {self._gps_innovation_sustained_s:.1f}s (threshold {self._gps_innovation_threshold:.1f})"
        )
        if self.logger is not None:
            self.logger.write("GPS_INNOVATION_ABORT", self._gps_innovation_trigger_info)

    def gps_innovation_status(self) -> dict[str, Any]:
        """Thread-safe snapshot for the async side (MavsdkInstructor) to read."""
        with self._gps_innovation_lock:
            return {
                "enabled": self._gps_innovation_abort_enabled,
                "triggered": self.gps_innovation_abort.is_set(),
                "bad_since": self._gps_innovation_bad_since,
                "trigger_info": dict(self._gps_innovation_trigger_info)
                if self._gps_innovation_trigger_info
                else None,
            }

    def _capture_serial_control(self, msg: Any) -> None:
        try:
            text = bytes(msg.data[: msg.count]).decode("utf-8", errors="replace")
        except Exception:  # noqa: BLE001 - a malformed chunk must not break the listener
            return
        if not text:
            return
        with self._shell_lock:
            self._shell_buffer.append(text)

    def send_shell_command(self, text: str, devnum: int = 10) -> None:
        """Send a PX4 MAVLink shell (SERIAL_CONTROL) command over this listener's own
        already-connected socket. Do not open a second udpin socket on the same address
        to do this -- see the note on _shell_buffer above.
        """
        if not confirm_pymavlink_commands_allowed(self.config):
            raise PermissionError(
                "pymavlink command sending is disabled. Set "
                "safety.allow_pymavlink_commands=true only for explicit experiments."
            )
        if self.master is None:
            raise RuntimeError("pymavlink listener is not connected")
        data = text.encode("utf-8")
        while data:
            chunk = data[:70]
            payload = list(chunk) + [0] * (70 - len(chunk))
            self.master.mav.serial_control_send(
                devnum,
                mavutil.mavlink.SERIAL_CONTROL_FLAG_EXCLUSIVE | mavutil.mavlink.SERIAL_CONTROL_FLAG_RESPOND,
                0,
                0,
                len(chunk),
                payload,
            )
            data = data[70:]

    def drain_shell_output(self) -> str:
        """Return and clear whatever SERIAL_CONTROL text has been captured since the last
        drain (or since connect())."""
        with self._shell_lock:
            text = "".join(self._shell_buffer)
            self._shell_buffer.clear()
        return text

    def send_px4_shell_command(self, command: str) -> None:
        """Experimental placeholder. Disabled unless explicitly allowed in config."""
        if not confirm_pymavlink_commands_allowed(self.config):
            raise PermissionError(
                "pymavlink command sending is disabled. Set "
                "safety.allow_pymavlink_commands=true only for explicit experiments."
            )
        raise NotImplementedError(
            "PX4 shell command transport is not implemented here. Use MAVSDK for flight "
            "commands and a dedicated mavlink_shell.py path for shell experiments."
        )


def _format_message(msg_type: str, payload: dict[str, Any]) -> str:
    compact_keys: dict[str, Iterable[str]] = {
        "HEARTBEAT": ("type", "autopilot", "base_mode", "custom_mode", "system_status"),
        "SYS_STATUS": ("voltage_battery", "current_battery", "battery_remaining"),
        "BATTERY_STATUS": ("voltages", "current_battery", "battery_remaining"),
        "ATTITUDE": ("roll", "pitch", "yaw", "rollspeed", "pitchspeed", "yawspeed"),
        "LOCAL_POSITION_NED": ("x", "y", "z", "vx", "vy", "vz"),
        "GLOBAL_POSITION_INT": ("lat", "lon", "alt", "relative_alt", "vx", "vy", "vz"),
        "GPS_RAW_INT": ("fix_type", "lat", "lon", "alt", "eph", "epv", "vel", "satellites_visible"),
        "VFR_HUD": ("airspeed", "groundspeed", "heading", "throttle", "alt", "climb"),
        "ACTUATOR_OUTPUT_STATUS": ("active", "actuator"),
        "SERVO_OUTPUT_RAW": ("servo1_raw", "servo2_raw", "servo3_raw", "servo4_raw"),
        "HOME_POSITION": ("latitude", "longitude", "altitude", "x", "y", "z"),
        "COMMAND_ACK": ("command", "result"),
        "MISSION_ACK": ("type",),
        "MISSION_CURRENT": ("seq",),
        "MISSION_ITEM_REACHED": ("seq",),
        "EXTENDED_SYS_STATE": ("vtol_state", "landed_state"),
        "STATUSTEXT": ("severity", "text"),
    }
    keys = compact_keys.get(msg_type)
    if not keys:
        return f"{time.strftime('%H:%M:%S')} {msg_type}: {payload}"
    fields = ", ".join(f"{key}={payload.get(key)}" for key in keys if key in payload)
    return f"{time.strftime('%H:%M:%S')} {msg_type}: {fields}"
