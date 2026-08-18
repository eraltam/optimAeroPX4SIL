#!/bin/bash
set -e

TARGET=/home/edison/PX4-Autopilot
STAMP=$(date +%Y%m%d_%H%M%S)

if [ -d "$TARGET" ]; then
  echo "Moving aside existing (broken-gitfile) directory to ${TARGET}_broken_${STAMP}"
  mv "$TARGET" "${TARGET}_broken_${STAMP}"
fi

echo "Cloning https://github.com/eraltam/PX4-Autopilot-optimAero.git into $TARGET"
git clone https://github.com/eraltam/PX4-Autopilot-optimAero.git "$TARGET"

cd "$TARGET"
echo "Checking out commit febaee4bb06bdefd4e3ea6c0d93e18bd822e55e2 (matches Windows-side HEAD)"
git checkout febaee4bb06bdefd4e3ea6c0d93e18bd822e55e2

echo "Initializing submodules recursively (this is the long part)"
git submodule update --init --recursive

echo "--- final git status ---"
git status --short | head -20
echo "--- HEAD ---"
git rev-parse HEAD
echo "--- SYS_HITL check ---"
grep -n SYS_HITL ROMFS/px4fmu_common/init.d/airframes/1003_optim_aero_hex.hil || echo "NOT FOUND"
echo "--- clone+submodules done ---"
