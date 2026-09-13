# ESP32 Radar

A simple web-based radar project built with an **ESP32 DevKit V1** and an **HC-SR04 ultrasonic distance sensor**.

The ESP32 measures the distance to nearby objects and hosts a local web interface that displays the sensor data in real time over Wi-Fi.

> 🟢 **Project Status: Working**

---

## Features

* 📡 ESP32 Wi-Fi connectivity
* 📏 HC-SR04 ultrasonic distance measurement
* 🌐 Local web server hosted directly on the ESP32
* ⚡ Live distance display
* 🖥️ Minimal dark radar-style interface
* 🔌 Breadboard-based hardware
* 💻 Arduino framework
* 🛠️ PlatformIO development environment

---

## Hardware Requirements

| Component                 |    Quantity |
| ------------------------- | ----------: |
| ESP32 DevKit V1           |           1 |
| HC-SR04 ultrasonic sensor |           1 |
| 830-point breadboard      |           1 |
| 1 kΩ resistor             |           2 |
| 4.7 kΩ resistor           |           1 |
| Jumper wires              | As required |
| Micro-USB data cable      |           1 |
| Wi-Fi router/network      |           1 |
| PC                        |           1 |

---

## Development Environment

* **VS Code**
* **PlatformIO**
* **Arduino framework**
* **C++**

---

# How It Works

The HC-SR04 sends an ultrasonic pulse and measures the time required for the echo to return.

The ESP32:

1. Sends a trigger pulse to the HC-SR04.
2. Measures the ECHO pulse duration.
3. Calculates the distance.
4. Connects to the local Wi-Fi network.
5. Starts a web server.
6. Sends the measured distance to the browser.
7. Updates the radar interface in real time.

The current version measures distance in a **single fixed direction**.

A servo-controlled scanning mechanism is planned for a future version.

---

# Breadboard Wiring

The complete breadboard layout is documented in:

**[`breadboard_830_pin_datasheet.md`](https://github.com/ssh-ak74/esp-32-radar/blob/main/breadboard_830_pin_datasheet.md)**

## ESP32

The ESP32 DevKit V1 is positioned across the center gap of the breadboard.

```text
ESP32 DevKit V1
B–J, rows 1–15
```

## HC-SR04

| HC-SR04 Pin | Breadboard Position |
| ----------- | ------------------- |
| VCC         | A30                 |
| TRIG        | A31                 |
| ECHO        | A32                 |
| GND         | A33                 |

## Jumper Wires

| From | To            | Purpose        |
| ---- | ------------- | -------------- |
| A2   | B33           | GND            |
| I1   | B30           | VCC / VIN      |
| I8   | B31           | TRIG / GPIO 25 |
| I7   | E38           | ECHO / GPIO 26 |
| E41  | Blue (-) rail | GND            |

## Resistors

| Value  | From | To  |
| ------ | ---- | --- |
| 1 kΩ   | B32  | B35 |
| 1 kΩ   | C35  | C38 |
| 4.7 kΩ | D38  | D41 |

### ECHO Voltage Divider

The HC-SR04 is powered from 5 V and its ECHO output can be approximately 5 V.

The ESP32 uses 3.3 V logic, so the ECHO signal should **not be connected directly** to GPIO 26.

The intended electrical arrangement is:

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

> ⚠️ **Important:** The exact resistor values must be selected so that the voltage reaching GPIO 26 remains within the ESP32's specified input limits. The currently documented `1 kΩ + 1 kΩ + 4.7 kΩ` three-resistor arrangement should **not** be described as a guaranteed safe 3.3 V divider.

If changing the hardware, update `breadboard_830_pin_datasheet.md` so the documented positions match the actual circuit.

---

# GPIO Connections

| Function     | ESP32 GPIO |
| ------------ | ---------: |
| HC-SR04 TRIG |    GPIO 25 |
| HC-SR04 ECHO |    GPIO 26 |
| HC-SR04 VCC  |  VIN / 5 V |
| HC-SR04 GND  |        GND |

---

# Build Instructions

## 1. Prepare the Components

Gather:

* ESP32 DevKit V1
* HC-SR04
* Breadboard
* Resistors
* Jumper wires
* Micro-USB data cable

Make sure the USB cable supports **data transfer**, not only charging.

---

## 2. Assemble the ESP32

Insert the ESP32 DevKit V1 across the center gap of the breadboard.

The board should sit securely without shorting pins together.

---

## 3. Connect the HC-SR04

Connect the sensor according to the wiring table:

```text
HC-SR04       ESP32
-----------------------
VCC      →    VIN / 5 V
TRIG     →    GPIO 25
ECHO     →    GPIO 26
GND      →    GND
```

The ECHO connection must pass through the appropriate voltage-divider circuit.

---

## 4. Connect the Voltage Divider

Place the resistors according to the verified circuit design.

The basic topology is:

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

Check the connections carefully before powering the ESP32.

---

## 5. Connect the ESP32 to Your PC

Connect the ESP32 to your PC using a **Micro-USB data cable**.

After connecting it, check Windows Device Manager or PlatformIO to determine which COM port was assigned to the board.

For example:

```text
COM3
```

Your COM port may be different.

---

# Software Setup

## 1. Install VS Code

Install Visual Studio Code if it isn't already installed.

## 2. Install PlatformIO

Install the **PlatformIO IDE** extension in VS Code.

## 3. Open the Project

Clone or download this repository and open the project folder in VS Code.

The project should contain:

```text
esp-32-radar/
├── src/
│   └── main.cpp
├── platformio.ini
├── breadboard_830_pin_datasheet.md
└── README.md
```

---

# Wi-Fi Configuration

Before uploading the firmware, configure your Wi-Fi credentials in the source code.

Example:

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
```

Replace the placeholder values with your own network credentials.

> 🔐 **Security:** Never commit your real Wi-Fi password to a public GitHub repository.

For a public project, consider using a separate configuration file or another method for keeping credentials out of version control.

---

# Upload the Firmware

Open the project in VS Code with PlatformIO.

Then upload the firmware to the ESP32.

You can also run PlatformIO from CMD:

```cmd
C:\Users\%USERNAME%\.platformio\penv\Scripts\platformio.exe run --target upload
```

If PlatformIO cannot find the board, check the USB connection and COM port.

---

# Serial Monitor

After uploading the firmware, open the Serial Monitor at **115200 baud**.

Example:

```cmd
C:\Users\%USERNAME%\.platformio\penv\Scripts\platformio.exe device monitor --port COM3 --baud 115200
```

Replace `COM3` with the actual port assigned to your ESP32.

If the connection is successful, you should see output similar to:

```text
Wi-Fi connected!
ESP32 IP address: 192.168.x.x
Web server started!
```

The exact IP address will depend on your local network.

---

# Open the Radar

Once the ESP32 connects to Wi-Fi, copy the IP address shown in the Serial Monitor.

For example:

```text
ESP32 IP address: 192.168.1.42
```

Open the address in a web browser:

```text
http://192.168.1.42
```

The computer or phone accessing the page must be connected to the **same local network** as the ESP32.

The radar interface will display the current distance measured by the HC-SR04.

---

# ESP32 Code

The main firmware is located in:

**[`src/main.cpp`](https://github.com/ssh-ak74/esp-32-radar/blob/main/src/main.cpp)**

The firmware handles:

* Wi-Fi connection
* HC-SR04 triggering
* ECHO pulse measurement
* Distance calculation
* HTTP web server
* Live sensor data
* Radar interface

---

# Project Structure

```text
esp-32-radar/
│
├── src/
│   └── main.cpp
│
├── platformio.ini
│
├── breadboard_830_pin_datasheet.md
│
└── README.md
```

---

# Current Interface

The current web interface provides a minimal dark radar-style display with live distance information.

The interface is intentionally lightweight so that it can be served directly from the ESP32 without requiring an external server.

---

# Current Limitations

The current version is **not yet a true scanning radar**.

The HC-SR04 is currently mounted in a fixed position and measures distance in one direction.

The radar-style interface is therefore a visualization of the current distance measurement rather than a 180° physical scan.

A servo-controlled sensor will be added in a future version.

---

# Future Features

Planned improvements:

* [ ] 180° servo scanning
* [ ] Real-time radar sweep
* [ ] OLED display
* [ ] Object tracking
* [ ] Adjustable detection range
* [ ] Buzzer alerts
* [ ] Mobile-friendly interface
* [ ] Configurable scan speed
* [ ] Multiple detection zones
* [ ] Improved distance filtering
* [ ] Object history
* [ ] Better radar visualization

---

# Troubleshooting

## ESP32 does not appear in PlatformIO

Check:

* USB cable supports data
* USB connection is secure
* ESP32 is powered
* Correct USB-to-serial drivers are installed
* The correct COM port is selected

## Upload fails

Try:

1. Disconnecting and reconnecting the ESP32.
2. Selecting the correct COM port.
3. Pressing the **BOOT** button during upload if required.
4. Checking the PlatformIO environment configuration.

## Serial Monitor shows nothing

Verify:

```text
Baud rate: 115200
```

Also make sure the correct COM port is selected.

## ESP32 connects to Wi-Fi but the page does not open

Check that:

* The IP address is correct.
* Your PC/phone is on the same Wi-Fi network.
* The ESP32 is still connected.
* The web server started successfully.

## Distance readings are incorrect

Check:

* HC-SR04 VCC
* HC-SR04 GND
* TRIG → GPIO 25
* ECHO → GPIO 26
* Voltage-divider wiring
* Breadboard connections
* Sensor orientation

---

# Hardware Safety Notes

> ⚠️ **Do not connect a 5 V HC-SR04 ECHO signal directly to an ESP32 GPIO.**

The HC-SR04 can output a signal around its supply voltage, while the ESP32 operates with 3.3 V logic.

Always verify the voltage-divider design and measure the resulting GPIO voltage if you are unsure.

Also:

* Disconnect power before changing wiring.
* Avoid short circuits between 5 V and GND.
* Double-check breadboard rows before powering the circuit.
* Do not rely solely on wire colors to identify connections.

---

# Project Status

🟢 **Working**

### Currently implemented

* [x] HC-SR04 distance measurement
* [x] ESP32 Wi-Fi connection
* [x] Local web server
* [x] Live distance display
* [x] Minimal dark radar interface

### Planned

* [ ] 180° servo scanning
* [ ] Radar sweep
* [ ] OLED output
* [ ] Object tracking
* [ ] Buzzer alerts
* [ ] Mobile interface

---

# License

This project is open source.

You are free to study, modify, reuse, and redistribute the project according to the terms of the repository's license.

If you use or modify this project, consider keeping attribution to the original repository.

---

# Author

**AK74**

GitHub: **[@ssh-ak74](https://github.com/ssh-ak74)**

---

⭐ If this project helped you learn something about ESP32, ultrasonic sensors, embedded systems, or web servers, consider starring the repository.
