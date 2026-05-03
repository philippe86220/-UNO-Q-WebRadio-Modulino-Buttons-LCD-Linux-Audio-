from urllib.request import urlopen
import json

from arduino.app_utils import App, Bridge

HOST_SERVICE = "http://172.17.0.1:9000"

RADIOS = [
    {"key": "info", "name": "France Info"},
    {"key": "rtl", "name": "RTL"},
    {"key": "inter", "name": "France Inter"},
    {"key": "musique", "name": "France Musique"},
    {"key": "nostalgie", "name": "Nostalgie"},
    {"key": "mradio", "name": "M Radio Top 50"},
]

STATE = {
    "station_index": 0,
    "volume": 50,
    "running": False
}

def proxy_get(path):
    try:
        with urlopen(HOST_SERVICE + path, timeout=5) as r:
            return json.loads(r.read().decode("utf-8"))
    except Exception as e:
        return {"ok": False, "error": str(e)}

def lcd_state():
    radio = RADIOS[STATE["station_index"]]
    status = "ON" if STATE["running"] else "OFF"
    return f'{radio["name"]}|{STATE["volume"]}|{status}'

def play_current_station():
    radio = RADIOS[STATE["station_index"]]
    result = proxy_get("/" + radio["key"])

    if result.get("ok"):
        STATE["running"] = True

    return lcd_state()

def next_station():
    if STATE["running"]:
        STATE["station_index"] += 1

    if STATE["station_index"] >= len(RADIOS):
        STATE["station_index"] = 0

    return play_current_station()

def next_volume():
    STATE["volume"] += 10

    if STATE["volume"] > 100:
        STATE["volume"] = 0

    proxy_get(f'/volume?value={STATE["volume"]}')

    return lcd_state()

def stop_radio():
    proxy_get("/stop")
    STATE["running"] = False
    return lcd_state()

def startup_radio():
    proxy_get("/stop")
    STATE["volume"] = 50
    proxy_get("/volume?value=50")
    play_current_station()

Bridge.provide("next_station", next_station)
Bridge.provide("next_volume", next_volume)
Bridge.provide("stop_radio", stop_radio)
Bridge.provide("lcd_state", lcd_state)

startup_radio()

App.run()
