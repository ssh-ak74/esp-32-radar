#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#ifndef WIFI_SSID
#define WIFI_SSID "ssid"
#endif

#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "password"
#endif

// Wi-Fi
const unsigned long WIFI_CONNECT_TIMEOUT_MS = 20000;

// HC-SR04
const int TRIG_PIN = 5;
const int ECHO_PIN = 18;

// Indicators
const int GREEN_LED = 25;
const int BLUE_LED = 26;
const int RED_LED = 27;
const int BUZZER = 23;

// Timing
const unsigned long SENSOR_INTERVAL_MS = 100;
const unsigned long BUZZER_INTERVAL_MS = 500;

WebServer server(80);

float currentDistance = -1;

unsigned long lastSensorUpdate = 0;
unsigned long lastBuzzerUpdate = 0;

bool buzzerState = false;

// --------------------------------------------------
// Distance measurement
// --------------------------------------------------

float getDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000);

    if (duration == 0) {
        return -1;
    }

    return duration * 0.0343f / 2.0f;
}

// --------------------------------------------------
// Hardware alarm
// --------------------------------------------------

void updateAlarm() {

    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
    digitalWrite(RED_LED, LOW);

    if (currentDistance < 0) {
        digitalWrite(BUZZER, LOW);
        buzzerState = false;
        return;
    }

    if (currentDistance > 100) {

        digitalWrite(GREEN_LED, HIGH);

        digitalWrite(BUZZER, LOW);
        buzzerState = false;

        return;
    }

    if (currentDistance > 50) {

        digitalWrite(BLUE_LED, HIGH);

        if (millis() - lastBuzzerUpdate >= 700) {

            lastBuzzerUpdate = millis();

            buzzerState = !buzzerState;

            digitalWrite(
                BUZZER,
                buzzerState ? HIGH : LOW
            );
        }

        return;
    }

    if (currentDistance > 20) {

        digitalWrite(RED_LED, HIGH);

        if (millis() - lastBuzzerUpdate >= 250) {

            lastBuzzerUpdate = millis();

            buzzerState = !buzzerState;

            digitalWrite(
                BUZZER,
                buzzerState ? HIGH : LOW
            );
        }

        return;
    }

    // Critical distance
    digitalWrite(RED_LED, HIGH);
    digitalWrite(BUZZER, HIGH);

    buzzerState = true;
}

// --------------------------------------------------
// Webpage
// --------------------------------------------------

const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>

<meta name="viewport"
      content="width=device-width, initial-scale=1">

<title>ESP32 Radar by AK74</title>

<style>

:root {
    --bg: #08090a;
    --panel: #0e1012;
    --border: #1b1e21;
    --text: #e7e9eb;
    --muted: #777c82;
    --accent: #8cff9a;
}

* {
    box-sizing: border-box;
}

body {
    margin: 0;
    min-height: 100vh;
    background: var(--bg);
    color: var(--text);
    font-family: Inter, Arial, sans-serif;

    display: flex;
    justify-content: center;

    padding: 24px;
}

.app {
    width: min(900px, 100%);
}

header {
    display: flex;
    justify-content: space-between;
    align-items: center;

    margin-bottom: 18px;
}

.title {
    font-size: 14px;
    font-weight: 600;

    letter-spacing: 0.08em;

    color: var(--text);
}

.status {
    display: flex;
    align-items: center;

    gap: 7px;

    color: var(--muted);

    font-size: 12px;
}

.status-dot {
    width: 7px;
    height: 7px;

    border-radius: 50%;

    background: var(--accent);
}

.radar-panel {
    background: var(--panel);

    border: 1px solid var(--border);

    border-radius: 14px;

    padding: 12px;
}

canvas {
    display: block;

    width: 100%;
    height: auto;

    border-radius: 9px;
}

.info {
    display: flex;

    justify-content: space-between;
    align-items: center;

    margin-top: 12px;

    padding: 0 4px;
}

.label {
    color: var(--muted);

    font-size: 12px;
}

.distance {
    font-size: 22px;

    font-weight: 600;

    letter-spacing: -0.03em;
}

.state {
    margin-top: 8px;

    padding: 0 4px;

    color: var(--muted);

    font-size: 12px;
}

footer {
    text-align: center;

    color: #4f5459;

    font-size: 11px;

    margin-top: 14px;
}

</style>

</head>

<body>

<div class="app">

<header>

    <div class="title">
        RADAR V2
    </div>

    <div class="status">

        <span class="status-dot"></span>

        <span id="status">
            ONLINE
        </span>

    </div>

</header>

<div class="radar-panel">

    <canvas
        id="radar"
        width="900"
        height="520">
    </canvas>

    <div class="info">

        <span class="label">
            DISTANCE
        </span>

        <span
            class="distance"
            id="distance">
            -- cm
        </span>

    </div>

    <div class="state">

        STATUS:
        <span id="state">
            WAITING
        </span>

    </div>

</div>

<footer>
    ESP32 · HC-SR04 · By AK74
</footer>

</div>

<script>

const canvas =
    document.getElementById("radar");

const ctx =
    canvas.getContext("2d");

let distance = -1;

let sweep = 0;

// --------------------------------------------------
// State
// --------------------------------------------------

function getState() {

    if (distance < 0) {
        return "NO ECHO";
    }

    if (distance > 100) {
        return "CLEAR";
    }

    if (distance > 50) {
        return "WARNING";
    }

    if (distance > 20) {
        return "DANGER";
    }

    return "CRITICAL";
}

// --------------------------------------------------
// Radar
// --------------------------------------------------

function drawRadar() {

    const w = canvas.width;
    const h = canvas.height;

    ctx.clearRect(
        0,
        0,
        w,
        h
    );

    ctx.fillStyle = "#090b0c";

    ctx.fillRect(
        0,
        0,
        w,
        h
    );

    const cx = w / 2;

    const cy = h - 25;

    const radius = Math.min(
        w * 0.43,
        h - 45
    );

    // Range rings

    ctx.strokeStyle = "#1b2420";

    ctx.lineWidth = 1;

    for (let i = 1; i <= 4; i++) {

        const r =
            radius * i / 4;

        ctx.beginPath();

        ctx.arc(
            cx,
            cy,
            r,
            Math.PI,
            Math.PI * 2
        );

        ctx.stroke();
    }

    // Center line

    ctx.strokeStyle =
        "#26302b";

    ctx.beginPath();

    ctx.moveTo(
        cx,
        cy
    );

    ctx.lineTo(
        cx,
        cy - radius
    );

    ctx.stroke();

    // Left line

    ctx.beginPath();

    ctx.moveTo(
        cx,
        cy
    );

    ctx.lineTo(
        cx - radius,
        cy
    );

    ctx.stroke();

    // Right line

    ctx.beginPath();

    ctx.moveTo(
        cx,
        cy
    );

    ctx.lineTo(
        cx + radius,
        cy
    );

    ctx.stroke();

    // Range labels

    ctx.fillStyle =
        "#4e5752";

    ctx.font =
        "11px Arial";

    for (let i = 1; i <= 4; i++) {

        const r =
            radius * i / 4;

        ctx.fillText(
            (i * 50) + " cm",
            cx + 7,
            cy - r + 4
        );
    }

    // Sweep animation

    sweep += 0.018;

    if (sweep > Math.PI) {
        sweep = 0;
    }

    const angle =
        Math.PI + sweep;

    const sx =
        cx +
        Math.cos(angle) *
        radius;

    const sy =
        cy +
        Math.sin(angle) *
        radius;

    ctx.strokeStyle =
        "rgba(140,255,154,0.65)";

    ctx.lineWidth = 2;

    ctx.beginPath();

    ctx.moveTo(
        cx,
        cy
    );

    ctx.lineTo(
        sx,
        sy
    );

    ctx.stroke();

    // Target

    if (
        distance > 0 &&
        distance <= 200
    ) {

        const r =
            (distance / 200) *
            radius;

        const targetAngle =
            -Math.PI / 2;

        const x =
            cx +
            Math.cos(targetAngle) *
            r;

        const y =
            cy +
            Math.sin(targetAngle) *
            r;

        let targetColor =
            "#8cff9a";

        if (distance <= 50) {

            targetColor =
                "#ff5555";

        } else if (distance <= 100) {

            targetColor =
                "#5599ff";
        }

        // Target dot

        ctx.beginPath();

        ctx.arc(
            x,
            y,
            5,
            0,
            Math.PI * 2
        );

        ctx.fillStyle =
            targetColor;

        ctx.fill();

        // Target ring

        ctx.beginPath();

        ctx.arc(
            x,
            y,
            11,
            0,
            Math.PI * 2
        );

        ctx.strokeStyle =
            "rgba(140,255,154,0.25)";

        ctx.stroke();
    }

    // Center

    ctx.beginPath();

    ctx.arc(
        cx,
        cy,
        4,
        0,
        Math.PI * 2
    );

    ctx.fillStyle =
        "#8cff9a";

    ctx.fill();

    requestAnimationFrame(
        drawRadar
    );
}

// --------------------------------------------------
// Distance API
// --------------------------------------------------

async function updateDistance() {

    try {

        const response =
            await fetch(
                "/distance",
                {
                    cache: "no-store"
                }
            );

        distance =
            parseFloat(
                await response.text()
            );

        if (distance < 0) {

            document.getElementById(
                "distance"
            ).innerText =
                "-- cm";

        } else {

            document.getElementById(
                "distance"
            ).innerText =
                distance.toFixed(1)
                + " cm";
        }

        document.getElementById(
            "state"
        ).innerText =
            getState();

        document.getElementById(
            "status"
        ).innerText =
            "ONLINE";

    } catch (error) {

        distance = -1;

        document.getElementById(
            "distance"
        ).innerText =
            "-- cm";

        document.getElementById(
            "state"
        ).innerText =
            "OFFLINE";

        document.getElementById(
            "status"
        ).innerText =
            "OFFLINE";
    }
}

setInterval(
    updateDistance,
    200
);

updateDistance();

drawRadar();

</script>

</body>
</html>
)rawliteral";

// --------------------------------------------------
// HTTP handlers
// --------------------------------------------------

void handleRoot() {

    server.send(
        200,
        "text/html",
        webpage
    );
}

void handleDistance() {

    if (currentDistance < 0) {

        server.send(
            200,
            "text/plain",
            "-1"
        );

    } else {

        server.send(
            200,
            "text/plain",
            String(
                currentDistance,
                1
            )
        );
    }
}

// --------------------------------------------------
// Setup
// --------------------------------------------------

void setup() {

    Serial.begin(115200);

    // HC-SR04

    pinMode(
        TRIG_PIN,
        OUTPUT
    );

    pinMode(
        ECHO_PIN,
        INPUT
    );

    digitalWrite(
        TRIG_PIN,
        LOW
    );

    // Indicators

    pinMode(
        GREEN_LED,
        OUTPUT
    );

    pinMode(
        BLUE_LED,
        OUTPUT
    );

    pinMode(
        RED_LED,
        OUTPUT
    );

    pinMode(
        BUZZER,
        OUTPUT
    );

    digitalWrite(
        GREEN_LED,
        LOW
    );

    digitalWrite(
        BLUE_LED,
        LOW
    );

    digitalWrite(
        RED_LED,
        LOW
    );

    digitalWrite(
        BUZZER,
        LOW
    );

    // Wi-Fi

    WiFi.mode(
        WIFI_STA
    );

    WiFi.begin(
        WIFI_SSID,
        WIFI_PASSWORD
    );

    Serial.println();

    Serial.print(
        "Connecting to Wi-Fi"
    );

    const unsigned long
        wifiConnectStart =
            millis();

    while (
        WiFi.status() != WL_CONNECTED &&
        (
            millis() -
            wifiConnectStart
        ) <
        WIFI_CONNECT_TIMEOUT_MS
    ) {

        delay(500);

        Serial.print(".");
    }

    Serial.println();

    if (
        WiFi.status() ==
        WL_CONNECTED
    ) {

        Serial.println(
            "Wi-Fi connected!"
        );

        Serial.print(
            "ESP32 IP address: "
        );

        Serial.println(
            WiFi.localIP()
        );

    } else {

        Serial.println(
            "Wi-Fi connection timeout. "
            "Continuing without network."
        );
    }

    // Web server

    server.on(
        "/",
        handleRoot
    );

    server.on(
        "/distance",
        handleDistance
    );

    server.begin();

    Serial.println(
        "Web server started!"
    );
}

// --------------------------------------------------
// Main loop
// --------------------------------------------------

void loop() {

    server.handleClient();

    // Measure distance independently
    // of the webpage.

    if (
        millis() -
        lastSensorUpdate >=
        SENSOR_INTERVAL_MS
    ) {

        lastSensorUpdate =
            millis();

        currentDistance =
            getDistance();

        updateAlarm();

        if (currentDistance < 0) {

            Serial.println(
                "No echo"
            );

        } else {

            Serial.print(
                "Distance: "
            );

            Serial.print(
                currentDistance
            );

            Serial.println(
                " cm"
            );
        }
    }
}
