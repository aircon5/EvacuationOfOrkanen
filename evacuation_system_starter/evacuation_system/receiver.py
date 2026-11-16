"""Shared receiver-side validation helpers.

The HTTP endpoint itself lives in main.py. This module documents the
expected message shape and can be imported by other Python components.
"""
VALID_ROOM_IDS = {"A329", "A330", "A331", "A332", "A333"}

def normalize_message(payload: dict) -> dict:
    required = {"node_id", "room_id", "people_count"}
    missing = required - payload.keys()
    if missing:
        raise ValueError(f"Missing fields: {', '.join(sorted(missing))}")

    room_id = str(payload["room_id"]).upper()
    if room_id not in VALID_ROOM_IDS:
        raise ValueError(f"Unknown room_id: {room_id}")

    people_count = int(payload["people_count"])
    if people_count < 0:
        raise ValueError("people_count cannot be negative")

    return {
        "node_id": str(payload["node_id"]),
        "room_id": room_id,
        "people_count": people_count,
        "room_blocked": bool(payload.get("room_blocked", False)),
        "exit_blocked": bool(payload.get("exit_blocked", False)),
        "timestamp": payload.get("timestamp"),
    }
