# ESP8266 Stoplight System Troubleshooting Guide

This guide provides a straightforward set of steps to diagnose and resolve common issues with your ESP8266 Stoplight System. It covers problems with both the Distance Sensor Client and the Stoplight Server.

---

## Introduction

If your ESP8266 Stoplight System is not behaving as expected, use this guide to identify potential problems and follow the recommended troubleshooting steps.

---

## Common Issues and Solutions

### 1. No LED Response or Incorrect LED Behavior
- **Problem:** LEDs are not lighting up, blinking incorrectly, or not responding to commands.
- **Troubleshooting Steps:**
  - **Verify Wiring:**  
    Check that all LED connections match the wiring diagram. Ensure each LED has a current-limiting resistor and a proper connection to the designated GPIO pin.
  - **Power Check:**  
    Confirm that the ESP8266 is powered properly and that there are no loose connections.
  - **Endpoint Testing:**  
    Use a web browser or HTTP tool to manually trigger the endpoints (e.g., `/green_on`, `/red_on`) and observe if the corresponding LED responds.
  - **Conflict Resolution:**  
    Ensure that no multiple blink modes are active simultaneously. Verify that tickers are detached before new commands are sent.

---

### 2. Distance Sensor Client Not Sending Correct Commands
- **Problem:** The sensor is not detecting distance accurately, leading to improper LED commands.
- **Troubleshooting Steps:**
  - **Sensor Wiring:**  
    Ensure the HC-SR04 sensor is connected correctly:
    - **VCC:** Connected to the appropriate power source (3.3V or 5V, with level shifting if required).
    - **TRIG:** Connected to GPIO D1.
    - **ECHO:** Connected to GPIO D2 (use a voltage divider if using a 5V sensor).
    - **GND:** Connected to ESP8266 ground.
  - **Test the Sensor:**  
    Run sample code to print sensor readings to the serial monitor. Check for stable and logical distance measurements.
  - **Code Review:**  
    Ensure the code correctly calculates distance and sends HTTP commands based on the defined thresholds.

---

### 3. 0 cm Distance Reading (Error Condition)
- **Problem:** The sensor returns a reading of 0 cm.
- **Troubleshooting Steps:**
  - **Inspect Wiring:**  
    Verify that the sensor's wiring is secure and that there are no loose or disconnected wires.
  - **Sensor Health:**  
    Check if the sensor is physically damaged or faulty.
  - **Voltage Levels:**  
    Confirm that the sensor is receiving the correct voltage and that level shifting is in place if needed.
  - **Error Indicator:**  
    Note that a 0 cm reading triggers the `/green_yellow_blink` endpoint as an error indicator. This helps to signal that something is wrong with the sensor or its connections.

---

### 4. Auto-Blink Mode Activation / No WiFi Client Connection
- **Problem:** The Stoplight Server enters auto-blink mode (all LEDs blinking), indicating no WiFi client is connected.
- **Troubleshooting Steps:**
  - **Client Connection:**  
    Ensure that a device is connecting to the ESP8266 access point (`ESP8266_Stoplight`).
  - **SSID and Password:**  
    Double-check that the SSID and password configured in the Stoplight Server code match what you are trying to connect to.
  - **WiFi Status:**  
    Verify that your client device's WiFi is enabled and not interfering with the connection.

---

## Additional Tips

- **Serial Monitoring:**  
  Use `Serial.print()` statements in your code to output sensor readings and debug messages to the serial monitor.
- **Double-Check Diagrams:**  
  Refer back to the wiring diagrams and notes to ensure your hardware setup is correct.
- **Power Supply:**  
  Ensure that all ESP8266 modules and sensors receive a stable power supply.
- **Firmware and Libraries:**  
  Make sure your code is up-to-date and that you are using the latest ESP8266 libraries in the Arduino IDE.

---

## Conclusion

Following the steps in this troubleshooting guide should help you resolve most common issues with the ESP8266 Stoplight System. If problems persist, review your wiring and code configuration carefully and consider reaching out to relevant support communities for additional help.

Happy troubleshooting!
