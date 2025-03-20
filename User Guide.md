# ESP8266 Stoplight System User Guide

This guide provides simple instructions for setting up and using the ESP8266 Stoplight System. The system consists of two main components that work together:

1. **Distance Sensor Client:**  
   Uses an HC-SR04 ultrasonic sensor to measure distance and send HTTP commands.
   
2. **Stoplight Server:**  
   Acts as a WiFi access point that controls three LEDs (red, yellow, and green) based on the commands received.

---

## 1. System Setup

### Distance Sensor Client

- **Hardware Connections:**
  - **VCC:** Connect to the appropriate power source (3.3V or 5V as required by your sensor).
  - **TRIG:** Connect to GPIO D1 on the ESP8266.
  - **ECHO:** Connect to GPIO D2 on the ESP8266 (use a voltage divider if the sensor outputs 5V).
  - **GND:** Connect to the ESP8266 ground.
  
- **Software:**
  - Upload the provided Distance Sensor Client code to your ESP8266.
  - This code continuously measures distance and sends HTTP commands based on the measured thresholds.

### Stoplight Server

- **Hardware Connections:**
  - **Red LED:** Connect to GPIO D1 (include a current-limiting resistor in series).
  - **Yellow LED:** Connect to GPIO D2 (with a resistor).
  - **Green LED:** Connect to GPIO D3 (with a resistor).
  - All LED cathodes should share a common ground with the ESP8266.
  
- **Software:**
  - Upload the Stoplight Server code to your ESP8266.
  - The server is set up as a WiFi access point (e.g., with SSID `ESP8266_Stoplight`) and listens for HTTP commands to control the LED states.

---

## 2. How to Use the System

1. **Power On the Devices:**
   - Ensure both the Distance Sensor Client and the Stoplight Server are powered on.
   - The Stoplight Server creates a WiFi access point.

2. **Connect to the WiFi Network:**
   - On your smartphone, laptop, or any WiFi-enabled device, connect to the access point (e.g., `ESP8266_Stoplight`).

3. **Operation:**
   - The Distance Sensor Client continuously measures the distance and sends HTTP commands based on the reading:
     - **Greater than 15 cm:** Sends `/green_on` (Green LED turns on).
     - **Between 10 cm and 15 cm:** Sends `/yellow_on` (Yellow LED turns on).
     - **Between 7 cm and 10 cm:** Sends `/red_on` (Red LED turns on).
     - **Less than 7 cm:** Sends `/red_blink_start` (Red LED blinks).
     - **0 cm (Error Condition):** Sends `/green_yellow_blink` (Both green and yellow LEDs blink to indicate a sensor or wiring issue).
     
   - The Stoplight Server updates the LEDs based on the commands it receives.
   - If no device is connected to the Stoplight Server's access point, the server enters an auto-blink mode where all LEDs blink, indicating standby.

---

## 3. Troubleshooting

- **No LED Response / Incorrect LED Behavior:**
  - Verify all LED wiring and resistor values.
  - Confirm that the ESP8266 is powered properly.
  - Test endpoints manually via a web browser or HTTP tool.

- **Distance Sensor Issues:**
  - Check that the HC-SR04 sensor is wired correctly.
  - Use the serial monitor to ensure the sensor is providing proper distance readings.
  - A 0 cm reading typically indicates a wiring issue, sensor fault, or connection problem.

- **Auto-Blink Mode Activation:**
  - Ensure a device is connected to the ESP8266 access point.
  - Double-check the SSID and password used in the Stoplight Server code.

---

## 4. Final Notes

- **Review Wiring Diagrams:**  
  Make sure your hardware setup matches the wiring diagrams provided in the project documentation.
  
- **Serial Monitoring:**  
  Use serial output for debugging sensor readings and verifying the HTTP commands.

By following this guide, you should be able to set up, operate, and troubleshoot your ESP8266 Stoplight System successfully.

Happy building!
