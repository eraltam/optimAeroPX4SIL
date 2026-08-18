#!/bin/bash
set -e
cd /home/edison/PX4-Autopilot
STAMP=$(date +%Y%m%d_%H%M%S)
BACKUP_DIR=/home/edison/px4_autopilot_wsl_backup_${STAMP}
mkdir -p "$BACKUP_DIR"

git diff > "$BACKUP_DIR/tracked_modifications.patch"

git status --porcelain | while IFS= read -r line; do
  prefix="${line:0:2}"
  if [ "$prefix" = "??" ]; then
    echo "${line:3}"
  fi
done > "$BACKUP_DIR/untracked_file_list.txt"

tar -cf "$BACKUP_DIR/untracked_files.tar" -T "$BACKUP_DIR/untracked_file_list.txt" 2> "$BACKUP_DIR/tar_errors.log" || true

echo "BACKUP_DIR=$BACKUP_DIR"
ls -la "$BACKUP_DIR"
