"""Session directory helpers shared by run and analysis scripts."""

from __future__ import annotations

import re
from pathlib import Path


def default_logs_root(script_dir: Path) -> Path:
    return script_dir.resolve().parent / "logs"


def session_name_from_id(session_id: str) -> str:
    cleaned = re.sub(r"[^A-Za-z0-9_.-]+", "_", str(session_id).strip())
    if not cleaned:
        raise ValueError("session id cannot be empty")
    return f"session_{cleaned}"


def resolve_session_dir(
    script_dir: Path,
    session_dir: str | None = None,
    session_id: str | None = None,
    fallback_dir: Path | None = None,
) -> Path:
    if session_dir and session_id:
        raise ValueError("use either --session-dir or --session-id, not both")

    logs_root = default_logs_root(script_dir)
    if session_id:
        return logs_root / session_name_from_id(session_id)

    if session_dir:
        if session_dir.strip().lower() in {"auto", "latest"}:
            latest = latest_session_dir(logs_root)
            if latest is None:
                raise FileNotFoundError(f"no latest MATLAB session found under {logs_root}")
            return latest
        path = Path(session_dir)
        return path.resolve()

    latest = latest_session_dir(logs_root)
    if latest is not None:
        return latest
    if fallback_dir is not None:
        return fallback_dir.resolve()
    raise FileNotFoundError(f"no latest MATLAB session found under {logs_root}")


def latest_session_dir(logs_root: Path) -> Path | None:
    pointer = logs_root / "latest_session.txt"
    if pointer.exists():
        raw = pointer.read_text(encoding="utf-8").strip()
        if raw:
            path = Path(raw)
            if not path.is_absolute():
                path = logs_root / path
            if path.exists() and path.is_dir():
                return path.resolve()

    candidates = [path for path in logs_root.glob("session_*") if path.is_dir()]
    if not candidates:
        return None
    return max(candidates, key=lambda path: path.stat().st_mtime).resolve()
