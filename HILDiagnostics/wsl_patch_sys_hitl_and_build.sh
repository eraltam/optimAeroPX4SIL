#!/bin/bash
set -e
cd /home/edison/PX4-Autopilot
FILE=ROMFS/px4fmu_common/init.d/airframes/1003_optim_aero_hex.hil

echo "--- before patch ---"
grep -n SYS_HITL "$FILE"

sed -i 's/^param set SYS_HITL 1$/# TEMPORARILY DISABLED for static ANELLO IMU bench test (block A1) -- restore before HIL use\n# param set SYS_HITL 1/' "$FILE"

echo "--- after patch ---"
grep -n -B1 SYS_HITL "$FILE"

echo "--- git diff for this file ---"
git diff -- "$FILE"

echo "--- starting build: make cubepilot_cubeorangeplus_default ---"
echo "start time: $(date)"
make cubepilot_cubeorangeplus_default 2>&1
echo "end time: $(date)"

echo "--- build output artifacts ---"
find build -maxdepth 3 -iname "*.px4" 2>/dev/null

echo "--- done ---"
