# ESP32 Radar

A simple web-based distance radar built with an **ESP32 DevKit V1** and an **HC-SR04 ultrasonic sensor**.

The ESP32 measures the distance to nearby objects and hosts a local web interface that displays the sensor readings in real time over Wi-Fi.

> 🟢 **Project status: Working**

## Features

* 📡 ESP32 Wi-Fi connectivity
* 📏 HC-SR04 ultrasonic distance measurement
* 🌐 Local web server hosted directly on the ESP32
* ⚡ Real-time distance display
* 🖥️ Minimal dark-themed web interface
* 🔌 Simple breadboard-based hardware
* 💻 Arduino framework + PlatformIO

## Hardware

| Component                    |    Quantity |
| ---------------------------- | ----------: |
| ESP32 DevKit V1              |           1 |
| HC-SR04 ultrasonic sensor    |           1 |
| 830-point breadboard         |           1 |
| 1 kΩ resistor                |           2 |
| 4.7 kΩ resistor              |           1 |
| Jumper wires                 | As required |
| Micro-USB data cable         |           1 |
| Wi-Fi router/network         |           1 |
| PC with VS Code + PlatformIO |           1 |

## Development Environment

* **VS Code**
* **PlatformIO**
* **Arduino framework**
* **C++**

## Circuit

### HC-SR04 → ESP32

| HC-SR04 Pin | Connection                      |
| ----------- | ------------------------------- |
| VCC         | ESP32 VIN / 5 V                 |
| TRIG        | GPIO 25                         |
| ECHO        | GPIO 26 through voltage divider |
| GND         | ESP32 GND                       |

### ECHO voltage divider

The HC-SR04 operates with a 5 V supply, while ESP32 GPIOs use 3.3 V logic. The ECHO signal should therefore be reduced before connecting it to the ESP32.

The intended circuit is:

```text
HC-SR04 ECHO
     │
   R1
     │
     ├──────── GPIO 26
     │
   R2
     │
    GND
```

Use resistor values that keep the voltage at GPIO 26 within the ESP32's acceptable input range.

> **Important:** Do not connect a 5 V HC-SR04 ECHO signal directly to an ESP32 GPIO.

## How to Build

### 1. Prepare the components

Gather the ESP32, HC-SR04, breadboard, resistors, jumper wires, and a Micro-USB **data** cable.

### 2. Connect the HC-SR04

Connect the sensor according to the circuit table above.

Make sure:

* VCC is connected to 5 V/VIN.
* GND is connected to GND.
* TRIG is connected to GPIO 25.
* ECHO passes through the voltage divider before reaching GPIO 26.

### 3. Connect the ESP32

Connect the ESP32 to your computer using a Micro-USB data cable.

Open the project in VS Code with PlatformIO and check that the ESP32's serial/COM port is detected.

### 4. Configure Wi-Fi

Add your Wi-Fi network credentials to the project source code before uploading.

For example:

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
```

Replace the placeholder values with your network credentials.

> Keep your Wi-Fi credentials private. Do not commit real passwords to a public GitHub repository.

### 5. Build and upload

Open the project in VS Code with PlatformIO and upload the firmware to the ESP32.

You can also run PlatformIO from CMD:

```cmd
C:\Users\%USERNAME%\.platformio\penv\Scripts\platformio.exe run --target upload
```

### 6. Open the Serial Monitor

Start the serial monitor at **115200 baud**.

```cmd
C:\Users\%USERNAME%\.platformio\penv\Scripts\platformio.exe device monitor --port COM3 --baud 115200
```

Replace `COM3` with the actual COM port assigned to your ESP32.

If everything is working correctly, you should see output similar to:

```text
Wi-Fi connected!
ESP32 IP address: 192.168.x.x
Web server started!
```

### 7. Open the radar interface

Find the IP address printed by the ESP32.

Open it in a browser on a device connected to the **same Wi-Fi network**:

```text
http://192.168.x.x
```

The web interface should load and display the current distance measured by the HC-SR04.

## Project Structure

A basic project layout looks like:

```text
esp-32-radar/
├── src/
│   └── main.cpp
├── platformio.ini
└── README.md
```

## ESP32 Code

The main firmware is available here:

**[`main.cpp`](https://github.com/ssh-ak74/esp-32-radar/blob/main/main.cpp)**

The firmware is responsible for:

1. Connecting the ESP32 to Wi-Fi.
2. Triggering the HC-SR04.
3. Measuring the ECHO pulse.
4. Calculating the distance.
5. Running the local web server.
6. Sending the current sensor data to the browser.

## Current Limitations

This version is **not yet a physical scanning radar**.

The HC-SR04 currently measures distance in a single fixed direction. The radar-style interface is a visualization of that distance measurement.

A servo-controlled sensor will be added in a future version to enable actual angular scanning.

## Future Features

Planned improvements include:

* [ ] 180° servo scanning
* [ ] Real-time radar sweep animation
* [ ] OLED display
* [ ] Object tracking
* [ ] Adjustable detection range
* [ ] Buzzer alerts
* [ ] Mobile-friendly interface
* [ ] Multiple detection zones
* [ ] Improved sensor filtering
* [ ] Configurable scan speed

## Safety & Hardware Notes

* Do **not** connect the HC-SR04 ECHO output directly to an ESP32 GPIO if the module outputs a 5 V signal.
* Verify the voltage-divider circuit before powering the system.
* Use a proper USB **data** cable for programming the ESP32.
* Keep Wi-Fi passwords and other credentials out of public source code.
* Disconnect power before changing the breadboard wiring.

## License

This project is open source. You are free to study, modify, reuse, and redistribute the project according to the terms of the repository's license.

## Author

**AK74**

GitHub: [@ssh-ak74](https://github.com/ssh-ak74)

---

⭐ If this project helped you learn something about ESP32, ultrasonic sensors, or embedded web servers, consider starring the repository.
