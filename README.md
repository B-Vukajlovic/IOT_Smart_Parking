# Smart Parking System

This repository demonstrates a **smart parking** solution integrating:
- **Arduino sensors** that measure distances to determine spot occupancy.
- **Flask web server** (`app.py`) to receive and display sensor data.
- **PyQt** desktop interface (`test.py` + `parking_ui.py`) for local monitoring.

## Main Components
1. **Arduino Sketches** (`ino_a1.ino`, `ino_a2.ino`, `sensor_test.ino`):
   - Read distance from ultrasonic sensors.
   - Control LEDs to indicate free/occupied spots.
   - Send updates to the Flask server (via HTTP, serial, or Wi-Fi).
2. **Flask App** (`app.py`):
   - Endpoints `/update` (sensor data) and `/error`.
   - Stores data in memory, provides a live dashboard (`index.html`).
   - Pushes updates via Socket.IO to connected browsers.
3. **Web Interface**:
   - Displays free spots, sensor distances, and LED statuses.
   - Shows errors reported by the Arduino.
4. **PyQt Desktop GUI** (`test.py`):
   - Loads `parking.ui` to visualize occupancy.
   - Lets you manually enter spot states and updates the UI.

## How to Run
1. **Flask Server**  
   - python app.py
   - Access at http://localhost:5000.

2. **Arduino**  
   - Open and upload ino_a1.ino and ino_a2.ino in the Arduino IDE on two seperate Arduino's.
   - Adjust threshold distances, pin connections as needed.

## Notes
For networking, Arduino can communicate via Wi-Fi/Ethernet shield or serial.
