"""FastAPI receiver for ESP32 telemetry.

Run with: uvicorn main:app --host 0.0.0.0 --port 8000 --reload
"""
from datetime import datetime, timezone
from typing import Optional
from fastapi import FastAPI, HTTPException
from pydantic import BaseModel, Field
from database import initialize_database, save_message, get_latest_rooms, get_nodes

app = FastAPI(title="Evacuation Monitoring API", version="0.1.0")


class Telemetry(BaseModel):
    node_id: str = Field(min_length=1, max_length=64)
    room_id: str = Field(min_length=1, max_length=16)
    people_count: int = Field(ge=0, le=1000)
    room_blocked: bool = False
    exit_blocked: bool = False
    timestamp: Optional[str] = None


@app.on_event("startup")
def startup():
    initialize_database()


@app.get("/")
def root():
    return {"message": "Evacuation Monitoring API is running", "docs": "/docs"}


@app.get("/health")
def health():
    return {"status": "ok", "server_time": datetime.now(timezone.utc).isoformat()}


@app.post("/api/telemetry")
def receive_telemetry(message: Telemetry):
    try:
        saved = save_message(message.dict())
        return {"accepted": True, "message_id": saved}
    except Exception as exc:
        raise HTTPException(status_code=500, detail=f"Could not save telemetry: {exc}")


@app.get("/api/rooms")
def rooms():
    return get_latest_rooms()


@app.get("/api/nodes")
def nodes():
    return get_nodes()
