# UNO Q WebRadio (Modulino BUTTONS + LCD + Linux Audio)

This project demonstrates a complete standalone web radio system using the Arduino UNO Q.

It combines:
- physical controls (Modulino buttons)
- LCD display (20x4 I2C)
- Linux audio playback (mpg123 + USB sound card)
- a Python state manager
- a systemd service for persistent audio control

---

## Key idea

This project uses a hybrid architecture:

- MCU (Arduino): buttons + LCD
- MPU (Linux): audio, logic, state
- App Lab: used only to start the application

Once started, the system runs entirely on the UNO Q.

---

## Architecture

```
Button press
↓
MCU (Arduino sketch)
↓
Bridge.call()
↓
main.py (App Lab / Python)
↓
radio_service.py (Linux service)
↓
mpg123 + amixer
↓
Audio output (USB sound card)
```


---

## System components

### 1. Arduino sketch (MCU)

- Reads Modulino buttons
- Sends commands via Bridge
- Displays:
  - station name
  - volume
  - status (ON/OFF)

---

### 2. `main.py` (Python / App Lab)

- Maintains a simplified system state:

```python
STATE = {
    "station_index": 0,
    "volume": 50,
    "running": False
}
```

- Handles:
  - next station
  - next volume
  - stop  
- Returns formatted strings for LCD:
```
RTL|50|ON
```
### 3. radio_service.py (Linux)
Runs as a systemd service.

Location:

```
/home/arduino/scripts/radio_service.py
```

Systemd unit:

```
/etc/systemd/system/radio_service.service
```
Responsibilities:  
- receives HTTP requests
- launches shell scripts
- controls:
  - mpg123 (audio)
  - amixer (volume)

---

## API

```
/status
/stop
/info
/rtl
/inter
/musique
/nostalgie
/mradio
/volume?value=50
```
---

## Standalone after launch

**Behavior**

1. Power the UNO Q
2. Start the app from App Lab (Wi-Fi)
3. The radio starts
4. You can:
  - close App Lab
  - shut down your computer
    
👉 The radio continues running

---

**Explanation**

- `radio_service.py` is started automatically at boot via systemd
- `main.py` is started via App Lab
- once started, everything runs locally on the UNO Q
  
👉 App Lab is only a launcher

---

## Observed behavior

- The system keeps running after App Lab is closed
- The system keeps running after the computer is powered off
- The system remains controllable via physical buttons

---

## Important note
This is not a fully automatic boot system.  
The application must be started once via App Lab.  
After that, it becomes fully autonomous.  

---

## Hardware used
- Arduino UNO Q
- Modulino buttons
- LCD 20x4 I2C 3,3V (address 0x27) 
- USB sound card
- speaker

---

👤 Author
Project developed as an exploration of the UNO Q hybrid architecture (MCU + Linux).
