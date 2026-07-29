"""Relay PX4's WSL-local MAVLink endpoint to a Windows host bidirectionally."""

from __future__ import annotations

import argparse
import socket


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--windows-host", required=True)
    parser.add_argument("--listen-port", type=int, default=14550)
    parser.add_argument("--px4-host", default="127.0.0.1")
    parser.add_argument("--px4-port", type=int, default=18570)
    args = parser.parse_args()

    endpoint_windows = (args.windows_host, args.listen_port)
    endpoint_px4 = (args.px4_host, args.px4_port)

    relay = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    relay.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    relay.bind(("0.0.0.0", args.listen_port))

    print(
        f"Relaying UDP :{args.listen_port} between "
        f"{endpoint_px4[0]}:{endpoint_px4[1]} and "
        f"{endpoint_windows[0]}:{endpoint_windows[1]}",
        flush=True,
    )

    packet_count = 0
    while True:
        packet, source = relay.recvfrom(65535)
        destination = endpoint_windows if source[1] == args.px4_port else endpoint_px4
        relay.sendto(packet, destination)
        packet_count += 1
        if packet_count <= 10:
            print(f"{packet_count}: {source} -> {destination} ({len(packet)} bytes)", flush=True)


if __name__ == "__main__":
    main()
