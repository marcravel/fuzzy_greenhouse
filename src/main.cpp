#include <Arduino.h>

// Test ESP32-WROOM-32 pins
int ledPins[] = {2, 5, 16, 0};  // Common onboard LED pins
int numPins = sizeof(ledPins) / sizeof(ledPins[0]);

void setup() {

  for (int i = 0; i < numPins; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  for (int i = 0; i < numPins; i++) {
    digitalWrite(ledPins[i], HIGH);
    delay(500);
    digitalWrite(ledPins[i], LOW);
    delay(500);
  }
}
