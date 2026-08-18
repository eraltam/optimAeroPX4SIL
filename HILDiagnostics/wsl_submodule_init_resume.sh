#!/bin/bash
set -e
cd /home/edison/PX4-Autopilot

echo "Tree has a stale gitlink at src/drivers/uavcan/libuavcan with no matching .gitmodules"
echo "entry (this airframe uses UAVCAN_ENABLE=0, so it's not needed) -- initializing only the"
echo "submodule paths that .gitmodules actually defines, explicitly, to skip that one path."

mapfile -t PATHS < <(git config -f .gitmodules --get-regexp path | awk '{print $2}')
echo "Found ${#PATHS[@]} defined submodule paths"

git submodule update --init --recursive -- "${PATHS[@]}"

echo "--- final git status (top-level, short) ---"
git status --short | head -20
echo "--- submodule status summary ---"
git submodule status | head -50
echo "--- SYS_HITL check ---"
grep -n SYS_HITL ROMFS/px4fmu_common/init.d/airframes/1003_optim_aero_hex.hil || echo "NOT FOUND"
echo "--- done ---"
