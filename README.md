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
