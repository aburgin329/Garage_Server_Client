Below is a README in Markdown format that explains how the two pieces of code work together, including details on the auto-blink mode when no WiFi clients are connected and what a 0 cm distance reading signifies.

⸻

ESP8266 Stoplight System

Overview

This project implements a wireless stoplight system using ESP8266 modules that communicate over WiFi. The system consists of two main components:
	1.	Distance Sensor Client:
An ESP8266 module connected to an HC-SR04 ultrasonic sensor. It measures the distance to an object and sends HTTP commands to the stoplight server based on the measured distance.
	2.	Stoplight Server:
An ESP8266 module acting as an access point that controls three LEDs (red, yellow, and green). It listens for HTTP requests to change the LED states and has an auto-blink mode when no clients are connected.

Features
	•	Distance-Based Control:
The client code evaluates the distance measured by the HC-SR04 sensor and sends a corresponding command:
	•	Green Light On: Distance > 15 cm.
	•	Yellow Light On: Distance between 10 cm and 15 cm.
	•	Red Light On: Distance between 7 cm and 10 cm.
	•	Red Blink (Backup): Distance less than 7 cm.
	•	Green & Yellow Blink: Distance equals 0 cm, indicating a possible wiring issue, sensor malfunction, or a problem with the sensor connection on the ESP8266.
	•	Auto-Blink Mode:
The stoplight server is set up as an access point. When no client is connected to the network, all three LEDs automatically blink. This indicates that the system is not being controlled remotely (i.e., WiFi is not connected to a client).
	•	HTTP Endpoints:
The stoplight server exposes several HTTP endpoints to control the lights:
	•	/green_on – Turns on the green LED.
	•	/yellow_on – Turns on the yellow LED.
	•	/red_on – Turns on the red LED.
	•	/red_blink_start – Starts blinking the red LED.
	•	/red_blink_stop – Stops blinking the red LED.
	•	/green_yellow_blink – Starts blinking both the green and yellow LEDs (triggered by a 0 cm sensor reading).

Hardware Requirements
	•	ESP8266 Modules: Two units (one for the sensor client and one for the stoplight server).
	•	HC-SR04 Ultrasonic Sensor: For distance measurement.
	•	LEDs: Three LEDs (red, yellow, and green) with appropriate current-limiting resistors.
	•	Miscellaneous: Breadboard, jumper wires, and a power supply.

Software Requirements
	•	Arduino IDE with ESP8266 board support.
	•	Libraries:
	•	ESP8266WiFi
	•	ESP8266HTTPClient
	•	ESP8266WebServer
	•	Ticker

Wiring

Distance Sensor Client
	•	HC-SR04 Sensor:
	•	Trigger Pin: Connected to a digital output (e.g., D1).
	•	Echo Pin: Connected to a digital input (e.g., D2).

Stoplight Server
	•	LED Connections:
	•	Red LED: Connected to pin D1.
	•	Yellow LED: Connected to pin D2.
	•	Green LED: Connected to pin D3.

Code Details

Distance Sensor Client Code
	•	Functionality:
	•	Measures distance using the HC-SR04 sensor.
	•	Sends HTTP GET requests to the stoplight server based on the distance:
	•	15 cm: /green_on
	•	10–15 cm: /yellow_on
	•	7–10 cm: /red_on
	•	<7 cm: /red_blink_start
	•	0 cm: Triggers /green_yellow_blink, indicating a possible wiring issue or sensor malfunction.
	•	Error Handling:
A distance reading of 0 cm typically suggests that something is wrong with the wiring on the sensor, the ESP8266 connection to the sensor, or the sensor itself.

Stoplight Server Code
	•	Access Point Mode:
Configured as an access point with the SSID ESP8266_Stoplight and password 12345678. The SSID is hidden.
	•	HTTP Endpoints:
	•	Commands such as /green_on, /yellow_on, /red_on, /red_blink_start, /red_blink_stop, and the new /green_yellow_blink endpoint control the LEDs.
	•	Auto-Blink Mode:
When no client is connected to the ESP8266 access point (i.e., WiFi is not connected), an auto-blink mode is activated. In this mode, a ticker toggles all three LEDs to indicate that the system is awaiting a connection.

Troubleshooting
	•	0 cm Distance Reading:
If the sensor measures 0 cm, it likely indicates a wiring issue, a faulty sensor, or a problem with the ESP8266’s connection to the sensor. Verify all sensor connections and check that the sensor is functioning correctly.
	•	No WiFi Connection:
When the stoplight server has no connected clients, all three LEDs will automatically blink. This auto-blink mode is a built-in indicator that no device is currently connected to the network.

Usage Instructions
	1.	Upload the Code:
	•	Flash the distance sensor client code onto one ESP8266 module.
	•	Flash the stoplight server code onto a second ESP8266 module.
	2.	Wiring:
	•	Connect the HC-SR04 sensor to the client ESP8266 as described.
	•	Wire the LEDs to the stoplight server ESP8266 according to the pin definitions.
	3.	Operation:
	•	The client ESP8266 will continuously measure distance and send HTTP commands to update the stoplight state.
	•	Connect your controlling device to the stoplight’s WiFi access point (ESP8266_Stoplight) to send commands or monitor the LED behavior.
	•	Observe the LED responses based on the sensor’s readings:
	•	Normal operation changes the LED state.
	•	A 0 cm reading triggers a green and yellow blink, indicating a potential sensor issue.
	•	No WiFi clients connected will result in auto-blinking of all three LEDs.

Conclusion

This ESP8266-based stoplight system demonstrates how sensor data can be used to control remote devices via WiFi. It incorporates built-in error indicators for sensor issues (0 cm reading) and connectivity problems (auto-blink mode when no client is connected). Follow the setup instructions carefully, and feel free to customize the behavior to suit your project requirements.

Happy building!

⸻

