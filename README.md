# ESP32 Radar

Build a simple web-based radar using an **ESP32 DevKit V1** and an **HC-SR04 ultrasonic sensor**.

The ESP32 measures distance with the HC-SR04 and hosts a local web interface that displays the sensor data in real time.

## Requirements

* ESP32 DevKit V1
* HC-SR04 ultrasonic sensor
* 830-point breadboard
* 1 kΩ resistors ×2
* 4.7 kΩ resistor ×1
* Jumper wires
* Micro-USB data cable
* Wi-Fi router/network
* PC with VS Code + PlatformIO

## Development Environment

* VS Code
* PlatformIO
* Arduino framework

## How to Build

### 1. Prepare the components

Gather the ESP32, HC-SR04, resistors, breadboard, jumper wires, and USB data cable.

### 2. Connect the HC-SR04

The HC-SR04 has four pins:

| HC-SR04 | ESP32 / Circuit                 |
| ------- | ------------------------------- |
| VCC     | VIN / 5 V                       |
| TRIG    | GPIO 25                         |
| ECHO    | GPIO 26 through voltage divider |
| GND     | GND                             |

### 3. Build the ECHO voltage divider

The HC-SR04 operates at 5 V, while the ESP32 GPIO is designed for 3.3 V logic.

Use:

```text
HC-SR04 ECHO
     │
    1 kΩ
     │
    1 kΩ
     │────── GPIO 26
    4.7 kΩ
     │
    GND
```

This reduces the ECHO voltage to a safer level for the ESP32.

### 4. Connect the ESP32

Connect the ESP32 to the PC using a **Micro-USB data cable**.

Make sure the correct COM port appears in PlatformIO.

### 5. Upload the firmware

Open the project in VS Code with PlatformIO and upload the firmware.

Alternatively, from CMD:

```bat
C:\Users\AK74\.platformio\penv\Scripts\platformio.exe run --target upload
```

### 6. Open the Serial Monitor

```bat
C:\Users\AK74\.platformio\penv\Scripts\platformio.exe device monitor --port COM3 --baud 115200
```

The ESP32 should connect to Wi-Fi and print its local IP address:

```text
Wi-Fi connected!
ESP32 IP address: 192.168.x.x
Web server started!
```

### 7. Open the radar

Enter the ESP32's IP address in a browser on the same network:

```text
http://192.168.x.x
```

The radar interface will display the measured distance in real time.

## ESP32 Code

```cpp
// Will be added when the project firmware is finalized.
```

## Future Features

* [ ] 180° servo scanning
* [ ] Real-time radar sweep
* [ ] OLED display
* [ ] Object tracking
* [ ] Adjustable detection range
* [ ] Buzzer alerts
* [ ] Better mobile interface

## Project Status

🟢 **Working**

Current version supports:

* HC-SR04 distance measurement
* ESP32 Wi-Fi connection
* Local web server
* Live distance display
* Minimal dark radar interface
