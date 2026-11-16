"""Streamlit dashboard: floor plan, current node status and event history.

Run with: streamlit run visualization.py
"""
from datetime import datetime, timezone
import json
import matplotlib.pyplot as plt
from matplotlib.patches import Polygon
import pandas as pd
import streamlit as st

from database import initialize_database, get_latest_rooms, get_nodes, get_history
from floor_plan import ROOM_POLYGONS, ROOM_LABEL_POSITIONS, CORRIDOR_POLYGON, STAIR_POLYGONS

st.set_page_config(page_title="Evacuation Monitor", layout="wide")
initialize_database()

st.title("Evacuation monitoring dashboard")
st.caption("Prototype — simulated values must not be treated as real emergency information.")

# Refresh the page periodically so new messages become visible.
try:
    from streamlit_autorefresh import st_autorefresh
    st_autorefresh(interval=2000, key="dashboard_refresh")
except ImportError:
    st.info("Install streamlit-autorefresh for automatic updates, or refresh the page manually.")

rooms = {row["room_id"]: row for row in get_latest_rooms()}
nodes = get_nodes()
history = get_history(100)

def node_is_online(last_seen: str, timeout_seconds: int = 10) -> bool:
    try:
        seen = datetime.fromisoformat(last_seen)
        if seen.tzinfo is None:
            seen = seen.replace(tzinfo=timezone.utc)
        return (datetime.now(timezone.utc) - seen).total_seconds() <= timeout_seconds
    except Exception:
        return False

online_nodes = sum(node_is_online(n["last_seen"]) for n in nodes)
total_people = sum(int(r["people_count"] or 0) for r in rooms.values())

m1, m2, m3, m4 = st.columns(4)
m1.metric("People reported", total_people)
m2.metric("Nodes online", f"{online_nodes}/{len(nodes)}")
m3.metric("Rooms reporting", len(rooms))
m4.metric("Latest messages", len(history))

st.subheader("Building floor plan")
fig, ax = plt.subplots(figsize=(15, 7))

# Corridor and staircases are schematic, pending exact dimensions.
ax.add_patch(Polygon(CORRIDOR_POLYGON, closed=True, facecolor="#e8e8e8",
                     edgecolor="#555555", linewidth=1.5))
for name, points in STAIR_POLYGONS.items():
    ax.add_patch(Polygon(points, closed=True, facecolor="#d7d7d7",
                         edgecolor="#444444", linewidth=1.5))
    cx = sum(p[0] for p in points) / len(points)
    cy = sum(p[1] for p in points) / len(points)
    ax.text(cx, cy, name, ha="center", va="center", fontsize=8)

for room_id, points in ROOM_POLYGONS.items():
    info = rooms.get(room_id)
    if info is None:
        face = "#eeeeee"
        status = "No data"
        people = "?"
    else:
        blocked = bool(info["room_blocked"])
        face = "#f4a6a6" if blocked else "#a8ddb5"
        status = "BLOCKED" if blocked else "Reporting"
        people = str(info["people_count"])

    ax.add_patch(Polygon(points, closed=True, facecolor=face,
                         edgecolor="#222222", linewidth=2))
    lx, ly = ROOM_LABEL_POSITIONS[room_id]
    ax.text(lx, ly + 0.7, room_id, ha="center", va="center",
            fontsize=12, fontweight="bold")
    ax.text(lx, ly - 0.2, f"People: {people}", ha="center", va="center", fontsize=10)
    ax.text(lx, ly - 1.0, status, ha="center", va="center", fontsize=9)

ax.set_aspect("equal", adjustable="box")
ax.set_xlim(-2, 44)
ax.set_ylim(-10, 15)
ax.set_xlabel("Approximate X position (m)")
ax.set_ylabel("Approximate Y position (m)")
ax.grid(alpha=0.15)
st.pyplot(fig, use_container_width=True)
plt.close(fig)

st.caption("Room shapes and corridor/staircase positions are approximate placeholders based on the supplied sketch.")

left, right = st.columns(2)
with left:
    st.subheader("ESP32 node status")
    if nodes:
        node_rows = []
        for n in nodes:
            node_rows.append({
                "Node": n["node_id"],
                "Room": n["room_id"],
                "People": n["people_count"],
                "Room blocked": bool(n["room_blocked"]),
                "Exit blocked": bool(n["exit_blocked"]),
                "Connection": "Online" if node_is_online(n["last_seen"]) else "Offline",
                "Last seen": n["last_seen"],
            })
        st.dataframe(pd.DataFrame(node_rows), use_container_width=True, hide_index=True)
    else:
        st.info("No ESP32 messages received yet.")

with right:
    st.subheader("Room summary")
    if rooms:
        room_rows = [{
            "Room": room_id,
            "People": info["people_count"],
            "Room blocked": bool(info["room_blocked"]),
            "Exit blocked": bool(info["exit_blocked"]),
            "Reporting nodes": info["node_count"],
        } for room_id, info in sorted(rooms.items())]
        st.dataframe(pd.DataFrame(room_rows), use_container_width=True, hide_index=True)
    else:
        st.info("Waiting for room data.")

st.subheader("Incoming message history")
if history:
    display_rows = []
    for item in history:
        display_rows.append({
            "Received (UTC)": item["received_at"],
            "Node": item["node_id"],
            "Room": item["room_id"],
            "People": item["people_count"],
            "Room blocked": bool(item["room_blocked"]),
            "Exit blocked": bool(item["exit_blocked"]),
        })
    st.dataframe(pd.DataFrame(display_rows), use_container_width=True, hide_index=True)
    with st.expander("Inspect raw JSON messages"):
        for item in history[:20]:
            st.code(item["raw_json"], language="json")
else:
    st.info("No history yet. Start the simulator or send data from an ESP32.")
