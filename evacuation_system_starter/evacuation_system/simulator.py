"""Send simulated ESP32 messages to the local API."""
import random
import time
import requests

API_URL = "http://127.0.0.1:8000/api/telemetry"

# Multiple nodes can report on the same room.
NODES = [
    ("ESP32_01", "A330"),
    ("ESP32_02", "A331"),
    ("ESP32_03", "A332"),
    ("ESP32_04", "A333"),
    ("ESP32_05", "A332"),
    ("ESP32_06", "A329"),
]

def make_message(node_id: str, room_id: str) -> dict:
    # Demo-only random values. Not real sensor readings.
    return {
        "node_id": node_id,
        "room_id": room_id,
        "people_count": random.randint(0, 8),
        "room_blocked": random.random() < 0.08,
        "exit_blocked": random.random() < 0.10,
    }


def run_simulator(interval: float = 2.0):
    print(f"Sending simulated data to {API_URL}. Press Ctrl+C to stop.")
    while True:
        node_id, room_id = random.choice(NODES)
        message = make_message(node_id, room_id)
        try:
            response = requests.post(API_URL, json=message, timeout=3)
            response.raise_for_status()
            print("Sent:", message)
        except requests.RequestException as exc:
            print("Could not reach API:", exc)
        time.sleep(interval)


if __name__ == "__main__":
    run_simulator()
