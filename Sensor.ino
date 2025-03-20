#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// HC-SR04 sensor pins (adjust as needed)
#define TRIG_PIN D1
#define ECHO_PIN D2

// WiFi credentials to connect to the stoplight device’s AP
const char* ssid = "ESP8266_Stoplight";      
const char* password = "12345678";

// IP address of the stoplight device (default for ESP8266 AP)
const char* stoplightServer = "192.168.4.1";

// Define stoplight states
enum StoplightState {
  GREEN,
  YELLOW,
  RED,
  BACKUP,  // Flashing red (backup)
  GREEN_YELLOW_BLINK  // New state for blinking green & yellow
};

StoplightState currentState = GREEN;

void setup() {
  Serial.begin(115200);
  
  // Setup sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Connect to the stoplight's WiFi network (AP mode)
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
}

long measureDistance() {
  // Clear the trigger pin
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // Trigger the sensor: send a 10-microsecond HIGH pulse
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Read the echo pulse duration in microseconds
  long duration = pulseIn(ECHO_PIN, HIGH);
  
  // Calculate distance in centimeters.
  long distance = (duration * 0.0343) / 2;
  return distance;
}

void sendStateCommand(StoplightState state) {
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://";
    url += stoplightServer;
    
    // Map each state to the corresponding endpoint
    switch(state) {
      case GREEN:
        url += "/green_on";
        break;
      case YELLOW:
        url += "/yellow_on";
        break;
      case RED:
        url += "/red_on";
        break;
      case BACKUP:
        url += "/red_blink_start";
        break;
      case GREEN_YELLOW_BLINK:
        url += "/green_yellow_blink"; // New endpoint for blinking green & yellow
        break;
    }
    
    Serial.print("Sending request: ");
    Serial.println(url);
    
    // Create a WiFiClient and use the new API for HTTPClient::begin
    WiFiClient client;
    http.begin(client, url);
    
    int httpCode = http.GET();
    if(httpCode > 0) {
      Serial.printf("HTTP Response code: %d\n", httpCode);
    } else {
      Serial.println("HTTP GET failed");
    }
    http.end();
  }
}

void loop() {
  // Measure the distance using the HC-SR04
  long distance = measureDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Define distance thresholds (in centimeters)
  StoplightState newState;

  if(distance == 0) {                   // If no object detected (error case)
    newState = GREEN_YELLOW_BLINK;
  } else if(distance > 15) {             // Far away
    newState = GREEN;
  } else if(distance > 10 && distance <= 15) { // Getting close
    newState = YELLOW;
  } else if(distance > 7 && distance <= 10) {  // Perfect distance (park)
    newState = RED;
  } else {                          // Too close: not enough room, backup state
    newState = BACKUP;
  }
  
  // If the state has changed, send the appropriate command
  if(newState != currentState) {
    currentState = newState;
    sendStateCommand(newState);
  }
  
  // Update every 500ms
  delay(500);
}
