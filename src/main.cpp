#include <Arduino.h>
#include "SystemController.h"

// Global Controller Instance
SystemController sysController;

void setup() {
  // Initialization delegated to controller
  sysController.begin();
}

void loop() {
  // Main loop logic delegated to controller
  sysController.update();
}