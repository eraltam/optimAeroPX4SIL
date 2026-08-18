#!/bin/bash
set -e
SRC=/mnt/c/AS/optimAeroPX4SIL/PX4-Autopilot/
DST=/home/edison/PX4-Autopilot/

echo "Starting rsync from $SRC to $DST (excluding build/) at $(date)"
rsync -a --delete \
  --exclude 'build/' \
  --info=progress2 \
  "$SRC" "$DST"
echo "rsync finished at $(date)"

cd "$DST"
echo "--- git status after sync ---"
git status --short | head -50
echo "--- done ---"
