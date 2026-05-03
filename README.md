![Arduino App Lab](https://img.shields.io/badge/Arduino%20App%20Lab-0.7.0-blue)
![Platform](https://img.shields.io/badge/macOS-26.3.1-lightgrey)
![Target](https://img.shields.io/badge/Board-UNO%20Q-green)
![Python](https://img.shields.io/badge/Python-3.x-yellow)
![Audio](https://img.shields.io/badge/Audio-ALSA%20%2F%20mpg123-red)

## Introduction

This project is an evolution of the following project:

👉 https://github.com/philippe86220/UNO-Q--WebRadio

The previous version was based on a graphical interface using the WebUI HTML brick in Arduino App Lab.

This new version replaces the graphical interface with:

- physical buttons (Modulino)
- a 20x4 LCD display

However, both projects share the same core architecture:

- Linux audio service:
  `/home/arduino/scripts/radio_service.py`
- systemd service:
  `/etc/systemd/system/radio_service.service`
- shell scripts used to control audio playback

These components are fully described in the previous project and are reused here without modification.

This project focuses on a different interaction model:
a fully hardware-based interface instead of a WebUI.

---

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
