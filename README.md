# ESP32 Radar

A web-based proximity radar and distance alarm built with an ESP32 DevKit V1 and HC-SR04 ultrasonic sensor.

The ESP32 measures distance in real time, hosts a local web interface over Wi-Fi, and provides hardware alerts using LEDs and an active buzzer.

> 🟢 Project Status: Working

---

## Features

- 📡 ESP32 Wi-Fi connectivity
- 📏 HC-SR04 ultrasonic distance measurement
- 🌐 Web server hosted directly on the ESP32
- 📊 Live distance monitoring
- 🖥️ Minimal dark radar-style web interface
- 🟢 Green LED for clear distance
- 🔵 Blue LED for warning distance
- 🔴 Red LED for danger/critical distance
- 🔊 Active buzzer distance alerts
- ⚡ Hardware alarm works independently of the web interface
- 💻 PlatformIO + Arduino framework
- 🔓 Open source under the MIT License

---

## How It Works

The HC-SR04 sends an ultrasonic pulse and measures the time required for the echo to return.

The ESP32 then:

1. Sends a trigger pulse to the HC-SR04.
2. Measures the ECHO pulse duration.
3. Calculates the distance.
4. Updates the hardware indicators.
5. Connects to the local Wi-Fi network.
6. Hosts a local HTTP web server.
7. Sends the latest distance to connected browsers.
8. Displays the measurement through the radar-style interface.

The sensor currently measures in one fixed direction.

The web radar is a visual representation of the distance measurement, not a physical 180° scanning radar.

---

## Distance Alerts

| Distance | LED | Buzzer | State |
| --- | --- | --- | --- |
| `> 100 cm` | 🟢 Green | Silent | Clear |
| `50–100 cm` | 🔵 Blue | Slow beep | Warning |
| `20–50 cm` | 🔴 Red | Fast beep | Danger |
| `≤ 20 cm` | 🔴 Red | Continuous | Critical |

The hardware alarm is processed directly by the ESP32 and does not depend on a browser being connected.

---

## Hardware

| Component | Quantity |
| --- | ---: |
| ESP32 DevKit V1 | 1 |
| HC-SR04 ultrasonic sensor | 1 |
| Green LED | 1 |
| Blue LED | 1 |
| Red LED | 1 |
| Active buzzer | 1 |
| 1 kΩ resistor | 5+ |
| Breadboard | 1 |
| Jumper wires | As required |
| Micro-USB data cable | 1 |
| Wi-Fi network | 1 |

---

## GPIO Connections

| Component | ESP32 GPIO |
| --- | ---: |
| HC-SR04 TRIG | GPIO 5 |
| HC-SR04 ECHO | GPIO 18 |
| Green LED | GPIO 25 |
| Blue LED | GPIO 26 |
| Red LED | GPIO 27 |
| Active buzzer | GPIO 23 |

### HC-SR04 Power

| HC-SR04 | ESP32 |
| --- | --- |
| VCC | VIN / 5V |
| GND | GND |
| TRIG | GPIO 5 |
| ECHO | GPIO 18 through voltage divider |

---

## ECHO Voltage Divider

The HC-SR04 can output an ECHO signal around 5V when powered from 5V.

The ESP32 uses 3.3V logic, so the HC-SR04 ECHO signal should **not** be connected directly to an ESP32 GPIO.

The current circuit uses a resistor divider:

```text
HC-SR04 ECHO
     │
    1 kΩ
     │
     ├──────── GPIO 18
     │
    1 kΩ
     │
    GND
````

This reduces the approximately 5V ECHO signal before it reaches GPIO 18.

Always verify the actual voltage and resistor arrangement before changing the circuit.

---

## LED Wiring

Each LED uses a resistor in series.

```text
ESP32 GPIO
    │
   1 kΩ
    │
 LED long leg (+)
 LED short leg (-)
    │
   GND
```

Connections:

```text
GPIO 25 → 1 kΩ → Green LED → GND
GPIO 26 → 1 kΩ → Blue LED  → GND
GPIO 27 → 1 kΩ → Red LED   → GND
```

---

## Buzzer Wiring

```text
GPIO 23 → Buzzer +
GND     → Buzzer -
```

The project uses an active buzzer, so the ESP32 controls it with a simple HIGH/LOW signal.

---

## Software

### Development Environment

* VS Code
* PlatformIO
* Arduino framework
* C++
* ESP32 Arduino core

### Main Libraries

```cpp
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
```

No external web server is required.

The webpage is embedded directly into the ESP32 firmware.

---

## Project Structure

```text
esp-32-radar/
│
├── src/
│   └── main.cpp
│
├── platformio.ini
├── webpage.html
├── README.md
└── LICENSE
```

---

## Wi-Fi Configuration

Configure your Wi-Fi credentials through PlatformIO build flags.

Example:

```ini
build_flags =
    -DWIFI_SSID=\"YOUR_WIFI_NAME\"
    -DWIFI_PASSWORD=\"YOUR_WIFI_PASSWORD\"
```

Do not commit real Wi-Fi passwords to a public repository.

---

## Build & Upload

Open the project in VS Code with PlatformIO installed.

Build:

```bash
platformio run
```

Upload:

```bash
platformio run --target upload
```

Or on Windows:

```text
C:\Users\%USERNAME%\.platformio\penv\Scripts\platformio.exe run --target upload
```

---

## Serial Monitor

The firmware uses a baud rate of `115200`.

```bash
platformio device monitor --baud 115200
```

Example output:

```text
Wi-Fi connected!
ESP32 IP address: 192.168.x.x
Web server started!

Distance: 96.42 cm
Distance: 94.87 cm
Distance: 91.53 cm
Distance: 53.21 cm
Distance: 38.74 cm
```

Failed ultrasonic measurements are ignored by the serial output to keep the monitor clean.

---

## Open the Radar

After the ESP32 connects to Wi-Fi, the serial monitor will display its local IP address:

```text
ESP32 IP address: 192.168.1.42
```

Open that address in a browser:

```text
http://192.168.1.42
```

Your computer or phone must be connected to the same local network as the ESP32.

The web interface will display:

* Current distance
* Radar visualization
* Connection status
* Current proximity state

---

## Web Interface

The interface is designed to be lightweight enough to run directly from the ESP32.

It includes:

* Dark radar-style visualization
* Animated sweep
* Distance display
* Proximity state
* Live connection status
* Distance-based target colors

The interface does not require a separate frontend server.

---

## Current Limitations

This is **not yet a true physical radar scanner**.

The HC-SR04 is currently fixed in one direction, so the ESP32 only knows the distance directly in front of the sensor.

The animated sweep on the webpage is a visual effect and does not represent a physically rotating sensor.

A future version can add a servo motor to rotate the HC-SR04 and collect distance measurements across multiple angles.

---

## Safety Notes

⚠️ **Do not connect a 5V HC-SR04 ECHO signal directly to an ESP32 GPIO.**

Use an appropriate voltage-divider or level-shifting circuit.

Also:

* Disconnect power before changing wiring.
* Avoid short circuits between 5V and GND.
* Double-check breadboard connections.
* Verify resistor values before powering the circuit.
* Do not rely solely on wire colors when troubleshooting.

---

## License

This project is licensed under the MIT License.

See [`LICENSE`](LICENSE) for the full license text.

---

## Author

**AK74**

GitHub: [@ssh-ak74](https://github.com/ssh-ak74)

---

⭐ If this project helped you learn something about ESP32, ultrasonic sensors, embedded systems, or web servers, consider starring the repository.

[1]: https://github.com/ssh-ak74/esp-32-radar "GitHub - ssh-ak74/esp-32-radar: Radar using HC-SR04 controlled by ESP32 · GitHub"
