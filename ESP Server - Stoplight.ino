#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>

const char* apSSID = "ESP8266_Stoplight";
const char* apPassword = "12345678";

const int RED_PIN = D1;
const int YELLOW_PIN = D2;
const int GREEN_PIN = D3;

ESP8266WebServer server(80);
Ticker redTicker;          // Used for red LED blinking when commanded
Ticker greenYellowTicker;  // Used for blinking green and yellow together
Ticker autoBlinkTicker;    // Used for auto blinking all LEDs when no clients are connected

bool redState = LOW;       // State used for red blinking
bool autoBlinkState = LOW; // State used for auto blinking (all LEDs)
const unsigned long blinkInterval = 500; // Blink interval in milliseconds

bool autoBlinkingActive = false; // Flag to track auto-blinking mode

// Toggle function for red LED blinking
void toggleRed() {
  redState = !redState;
  digitalWrite(RED_PIN, redState);
}

// Toggle function for blinking green and yellow LEDs simultaneously
void toggleGreenYellow() {
  static bool state = false;
  state = !state;
  digitalWrite(GREEN_PIN, state);
  digitalWrite(YELLOW_PIN, state);
}

// Toggle function for auto blinking all three LEDs
void toggleAll() {
  autoBlinkState = !autoBlinkState;
  digitalWrite(RED_PIN, autoBlinkState);
  digitalWrite(YELLOW_PIN, autoBlinkState);
  digitalWrite(GREEN_PIN, autoBlinkState);
}

void setup() {
  Serial.begin(115200);

  // Initialize LED pins
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);

  // Set up the ESP8266 as an Access Point with hidden SSID
  WiFi.mode(WIFI_AP);
  WiFi.softAP(apSSID, apPassword, 1, true);  // 'true' hides the SSID
  WiFi.setSleep(false);

  // Define server routes that control a single light state at a time

  server.on("/green_on", []() {
    redTicker.detach();
    greenYellowTicker.detach();
    digitalWrite(RED_PIN, LOW);
    digitalWrite(YELLOW_PIN, LOW);
    digitalWrite(GREEN_PIN, HIGH);
    server.send(200, "text/plain", "Green light ON");
  });

  server.on("/yellow_on", []() {
    redTicker.detach();
    greenYellowTicker.detach();
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(YELLOW_PIN, HIGH);
    server.send(200, "text/plain", "Yellow light ON");
  });

  server.on("/red_on", []() {
    redTicker.detach();
    greenYellowTicker.detach();
    digitalWrite(YELLOW_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(RED_PIN, HIGH);
    server.send(200, "text/plain", "Red light ON");
  });

  server.on("/red_blink_start", []() {
    redTicker.detach();
    greenYellowTicker.detach();
    digitalWrite(YELLOW_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    // Start blinking the red LED
    redTicker.attach_ms(blinkInterval, toggleRed);
    server.send(200, "text/plain", "Red light BLINKING");
  });

  server.on("/red_blink_stop", []() {
    redTicker.detach();
    digitalWrite(RED_PIN, LOW);
    server.send(200, "text/plain", "Red blinking STOPPED");
  });

  // New endpoint for blinking green and yellow LEDs together
  server.on("/green_yellow_blink", []() {
    redTicker.detach();
    greenYellowTicker.detach();
    autoBlinkTicker.detach();
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(YELLOW_PIN, LOW);
    // Start blinking green and yellow LEDs simultaneously
    greenYellowTicker.attach_ms(blinkInterval, toggleGreenYellow);
    server.send(200, "text/plain", "Green & Yellow lights BLINKING");
  });

  server.begin();
}

void loop() {
  server.handleClient();

  // Check if any client is connected to the AP.
  if (WiFi.softAPgetStationNum() == 0) {
    // No clients connected: activate auto blinking if not already active.
    if (!autoBlinkingActive) {
      autoBlinkTicker.attach_ms(blinkInterval, toggleAll);
      autoBlinkingActive = true;
    }
  } else {
    // One or more clients connected: disable auto blinking if it is active.
    if (autoBlinkingActive) {
      autoBlinkTicker.detach();
      autoBlinkingActive = false;
      // Optionally, reset the LEDs when auto blinking stops.
      digitalWrite(RED_PIN, LOW);
      digitalWrite(YELLOW_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
    }
  }
}
