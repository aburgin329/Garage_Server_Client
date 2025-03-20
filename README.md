# ESP8266 Stoplight System: Wiring Diagrams and System Explanations

This document provides a comprehensive guide to the ESP8266 Stoplight System. It includes wiring diagrams for both the Distance Sensor Client and the Stoplight Server, along with detailed notes on system behavior, error handling, and additional operational details.

---

## Overview

The system consists of two main components:

1. **Distance Sensor Client:**  
   An ESP8266 module connected to an HC-SR04 ultrasonic sensor. This module continuously measures distance and sends HTTP commands to control the stoplight based on the readings.

2. **Stoplight Server:**  
   An ESP8266 module acting as a WiFi access point that controls three LEDs (red, yellow, and green) based on the HTTP commands received from the client. It also features an auto-blink mode when no WiFi clients are connected.

---

## Wiring Diagrams

### 1. Distance Sensor Client Wiring Diagram

```plaintext
             HC-SR04 Ultrasonic Sensor
          +----------------------------+
          |          VCC               |
          |           |                |  (Connect to 3.3V or 5V as appropriate)
          |           V                |
          |         [Power]            |
          |                            |
          |         TRIG -------------+--------> GPIO D1 (ESP8266)
          |                            |
          |         ECHO -------------+--------> GPIO D2 (ESP8266)
          |                         (Use a voltage divider if needed)
          |                            |
          |         GND  -------------+--------> GND (ESP8266)
          +----------------------------+
```

**Notes:**

- **VCC:** Connect the sensor’s VCC to the appropriate power source. Some HC-SR04 modules require 5V, but if you’re using an ESP8266 (which operates on 3.3V logic), ensure you use the proper voltage or a level shifter if needed.
- **TRIG & ECHO:**
  - **TRIG** is connected to GPIO D1 on the ESP8266.
  - **ECHO** is connected to GPIO D2.
  - If the sensor outputs a 5V signal, use a voltage divider on the ECHO line to protect the ESP8266.
- **GND:** The sensor’s ground must be connected to the ESP8266’s ground.

---

### 2. Stoplight Server Wiring Diagram

```plaintext
             ESP8266 Stoplight Server
           +------------------------------+
           |                              |
           |  GPIO D1 ------------------> [Red LED] ---[Resistor]--- GND
           |                              |
           |  GPIO D2 ------------------> [Yellow LED] ---[Resistor]--- GND
           |                              |
           |  GPIO D3 ------------------> [Green LED] ---[Resistor]--- GND
           |                              |
           +------------------------------+
```

**Notes:**

- **LED Connections:**
  - **Red LED:** Connected to GPIO D1.
  - **Yellow LED:** Connected to GPIO D2.
  - **Green LED:** Connected to GPIO D3.
- **Resistors:** Include appropriate current-limiting resistors in series with each LED to prevent overcurrent damage.
- **GND:** All LED cathodes must share a common ground with the ESP8266.

---

## System Behavior and Additional Notes

### Distance Sensor Client Behavior

- **Distance Measurement:**
  - The HC-SR04 sensor continuously measures the distance to an object. Based on the reading, the client sends HTTP commands to the Stoplight Server:
    - **>15 cm:** Sends `/green_on` (Green LED on).
    - **10–15 cm:** Sends `/yellow_on` (Yellow LED on).
    - **7–10 cm:** Sends `/red_on` (Red LED on).
    - **<7 cm:** Sends `/red_blink_start` (Red LED blinking).
- **0 cm Reading (Error Condition):**
  - A reading of 0 cm indicates a problem. This may be due to:
    - Wiring issues with the sensor.
    - A fault in the sensor itself.
    - A connection problem between the sensor and the ESP8266.
  - When 0 cm is detected, the client sends `/green_yellow_blink` to trigger a blinking pattern for both green and yellow LEDs on the Stoplight Server, serving as an error indicator.

---

### Stoplight Server Behavior

- **Access Point Setup:** The Stoplight Server is configured as a WiFi access point with a hidden SSID (e.g., `ESP8266_Stoplight`) using a preset password.
- **HTTP Endpoints:** The server exposes multiple endpoints to control the LEDs:
  - `/green_on`: Turns on the green LED.
  - `/yellow_on`: Turns on the yellow LED.
  - `/red_on`: Turns on the red LED.
  - `/red_blink_start`: Starts blinking the red LED.
  - `/red_blink_stop`: Stops blinking the red LED.
  - `/green_yellow_blink`: Blinks both the green and yellow LEDs (error indicator).
  - When any endpoint is activated, the server disables conflicting blink modes (by detaching any running tickers) before setting the new state.
- **Auto-Blink Mode:**
  - The server checks the number of connected WiFi clients:
    - **No Clients Connected:** If no device is connected, the server automatically enters an auto-blink mode, where all three LEDs blink. This indicates that the system is in a standby state.
    - **Clients Connected:** Once a client connects, auto-blink is disabled, and the LEDs are controlled via HTTP commands.

---

## Summary of Notes

- **Distance Sensor Client:**
  - Continuously measures distance.
  - Sends commands to the Stoplight Server based on distance thresholds.
  - A 0 cm reading is an error condition that triggers a special blinking mode (`/green_yellow_blink`).
- **Stoplight Server:**
  - Acts as a WiFi Access Point.
  - Controls LEDs via defined HTTP endpoints.
  - Auto-blink mode activates when no clients are connected, ensuring that the system provides a visible indication of its status.
- **Wiring Considerations:**
  - Ensure proper voltage levels for the HC-SR04 sensor when used with a 3.3V ESP8266.
  - Use current-limiting resistors with all LEDs.
  - Maintain a common ground across all devices.

---

This guide should help you set up, wire, and troubleshoot your ESP8266 Stoplight System effectively. Follow the diagrams and notes carefully to ensure proper operation and to diagnose any issues that may arise during deployment.

Happy building!

