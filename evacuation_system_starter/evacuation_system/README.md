# Evacuation monitoring prototype

## 1. Create and activate a virtual environment (Windows PowerShell)

```powershell
python -m venv .venv
.venv\Scripts\Activate.ps1
pip install -r requirements.txt
```

## 2. Start the API receiver

Open a terminal in this folder:

```powershell
uvicorn main:app --host 0.0.0.0 --port 8000 --reload
```

API documentation: http://127.0.0.1:8000/docs

## 3. Start the dashboard

Open a second terminal in the same folder:

```powershell
streamlit run visualization.py
```

The dashboard normally opens at http://localhost:8501.

## 4. Start simulated ESP32 nodes

Open a third terminal:

```powershell
python simulator.py
```

The simulator sends fake messages to the API. The dashboard refreshes every two seconds.

## 5. Message format for a real ESP32

Send an HTTP POST request to:

`http://YOUR_COMPUTER_IP:8000/api/telemetry`

JSON body:

```json
{
  "node_id": "ESP32_01",
  "room_id": "A330",
  "people_count": 3,
  "room_blocked": false,
  "exit_blocked": false
}
```

Use the computer's LAN IP address instead of 127.0.0.1 when the ESP32 is on the network. Allow port 8000 through the computer firewall if needed.

## Important design notes

- Multiple nodes may report for the same room.
- This prototype currently sums the latest people_count from each node assigned to a room. Only use that aggregation if each node counts a distinct group/area; otherwise it could double-count people.
- The latest room state uses the most recent report per node. A room is marked blocked if any reporting node says it is blocked.
- A node is shown online if it has sent a message within the last 10 seconds.
- The supplied floor plan geometry is approximate. Confirm exact wall, corridor, doorway and stair measurements before relying on it.
- Simulated data is for software testing only, not for real evacuation decisions.
