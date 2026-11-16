"""SQLite storage for raw telemetry and latest node state."""
import json
import sqlite3
from pathlib import Path
from datetime import datetime, timezone

DB_PATH = Path(__file__).parent / "data" / "evacuation.db"


def _connect():
    DB_PATH.parent.mkdir(parents=True, exist_ok=True)
    connection = sqlite3.connect(DB_PATH, timeout=10)
    connection.row_factory = sqlite3.Row
    return connection


def initialize_database():
    with _connect() as conn:
        conn.execute("""
            CREATE TABLE IF NOT EXISTS telemetry_log (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                received_at TEXT NOT NULL,
                node_id TEXT NOT NULL,
                room_id TEXT NOT NULL,
                people_count INTEGER NOT NULL,
                room_blocked INTEGER NOT NULL,
                exit_blocked INTEGER NOT NULL,
                raw_json TEXT NOT NULL
            )
        """)
        conn.execute("""
            CREATE TABLE IF NOT EXISTS node_status (
                node_id TEXT PRIMARY KEY,
                room_id TEXT NOT NULL,
                last_seen TEXT NOT NULL,
                people_count INTEGER NOT NULL,
                room_blocked INTEGER NOT NULL,
                exit_blocked INTEGER NOT NULL
            )
        """)


def save_message(message: dict) -> int:
    received_at = datetime.now(timezone.utc).isoformat()
    with _connect() as conn:
        cursor = conn.execute("""
            INSERT INTO telemetry_log
            (received_at, node_id, room_id, people_count, room_blocked,
             exit_blocked, raw_json)
            VALUES (?, ?, ?, ?, ?, ?, ?)
        """, (
            received_at, message["node_id"], message["room_id"],
            message["people_count"], int(message.get("room_blocked", False)),
            int(message.get("exit_blocked", False)),
            json.dumps(message, ensure_ascii=False)
        ))
        conn.execute("""
            INSERT INTO node_status
            (node_id, room_id, last_seen, people_count, room_blocked, exit_blocked)
            VALUES (?, ?, ?, ?, ?, ?)
            ON CONFLICT(node_id) DO UPDATE SET
                room_id=excluded.room_id,
                last_seen=excluded.last_seen,
                people_count=excluded.people_count,
                room_blocked=excluded.room_blocked,
                exit_blocked=excluded.exit_blocked
        """, (
            message["node_id"], message["room_id"], received_at,
            message["people_count"], int(message.get("room_blocked", False)),
            int(message.get("exit_blocked", False))
        ))
        return cursor.lastrowid


def get_latest_rooms():
    with _connect() as conn:
        rows = conn.execute("""
            SELECT room_id,
                   SUM(people_count) AS people_count,
                   MAX(room_blocked) AS room_blocked,
                   MAX(exit_blocked) AS exit_blocked,
                   COUNT(*) AS node_count,
                   MAX(last_seen) AS last_seen
            FROM node_status
            GROUP BY room_id
            ORDER BY room_id
        """).fetchall()
        return [dict(row) for row in rows]


def get_nodes():
    with _connect() as conn:
        rows = conn.execute("""
            SELECT node_id, room_id, last_seen, people_count,
                   room_blocked, exit_blocked
            FROM node_status ORDER BY node_id
        """).fetchall()
        return [dict(row) for row in rows]


def get_history(limit: int = 100):
    with _connect() as conn:
        rows = conn.execute("""
            SELECT id, received_at, node_id, room_id, people_count,
                   room_blocked, exit_blocked, raw_json
            FROM telemetry_log ORDER BY id DESC LIMIT ?
        """, (limit,)).fetchall()
        return [dict(row) for row in rows]
