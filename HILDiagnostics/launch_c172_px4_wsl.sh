#!/usr/bin/env bash
set -euo pipefail

session_id="${1:?session id required}"
windows_host="${2:?Windows host address required}"
px4_root="/home/eraltam/SITLV2/PX4-Autopilot-optimAero"
session_dir="/mnt/c/Users/Edison Altamirano/SITLV2/AnelloSummer/optimAeroPX4SIL/HILDiagnostics/logs/session_${session_id}"

mkdir -p "${session_dir}"
cd "${px4_root}"

export PX4_SIM_HOSTNAME="${windows_host}"
exec make px4_sitl_default optimAeroC172pJSBSim \
    >>"${session_dir}/px4_console.log" \
    2>>"${session_dir}/px4_stderr.log"
