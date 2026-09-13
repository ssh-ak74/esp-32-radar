## Breadboard Wiring

The complete breadboard layout used by this project is documented below.

### ESP32

- ESP32 DevKit V1 → B–J, rows 1–15

### HC-SR04

| Pin | Breadboard Position |
|---|---|
| VCC | A30 |
| TRIG | A31 |
| ECHO | A32 |
| GND | A33 |

### Jumper Wires

| From | To | Purpose |
|---|---|---|
| A2 | B33 | GND |
| I1 | B30 | VCC / VIN |
| I8 | B31 | TRIG / GPIO25 |
| I7 | E38 | ECHO / GPIO26 |
| E41 | Blue (-) rail | GND |

### Resistors

| Value | From | To |
|---|---|---|
| 1 kΩ | B32 | B35 |
| 1 kΩ | C35 | C38 |
| 4.7 kΩ | D38 | D41 |

> **Note:** The HC-SR04 ECHO signal is routed to GPIO26 through the resistor network shown above. The network reduces the ECHO voltage before it reaches the ESP32. Verify the resistor configuration and resulting GPIO voltage before reproducing the circuit.
