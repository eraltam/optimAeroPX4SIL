#!/usr/bin/env bash
set -euo pipefail

target="${1:-optimAeroEVTOL}"
script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
repo_root="$(cd "${script_dir}/.." && pwd)"

if [[ -z "${PX4_SIM_HOSTNAME:-}" ]]; then
    PX4_SIM_HOSTNAME="$(ip route show default | awk 'NR == 1 { print $3 }')"
fi
export PX4_SIM_HOSTNAME

echo "PX4 target: ${target}"
echo "PX4 simulator host: ${PX4_SIM_HOSTNAME}"
cd "${repo_root}/PX4-Autopilot"
exec make px4_sitl_default "${target}"
