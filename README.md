# UNO Q WebRadio (Modulino + LCD + Linux Audio)

This project demonstrates a complete standalone web radio system using the Arduino UNO Q.

It combines:
- physical controls (Modulino buttons)
- LCD display (20x4 I2C)
- Linux audio playback (mpg123 + USB sound card)
- a Python state manager
- a systemd service for persistent audio control

---

## 🚀 Key idea

This project uses a hybrid architecture:

- MCU (Arduino): buttons + LCD
- MPU (Linux): audio, logic, state
- App Lab: used only to start the application

Once started, the system runs entirely on the UNO Q.

---

## ⚙️ Architecture

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
