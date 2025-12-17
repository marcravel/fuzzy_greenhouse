#include "SystemController.h"

SystemController::SystemController() : lastLogicUpdate(0) {}

void SystemController::begin() {
    Serial.begin(115200);
    sensors.begin();
    webServer.begin(); // Starts Access Point
    
    // Initial logic run
    processLogic();
}

void SystemController::update() {
    unsigned long now = millis();

    // Check for web overrides (manual set from UI)
    SensorReadings manualInputs;
    if (webServer.checkManualInputs(manualInputs)) {
        // Use manual inputs for one cycle or state override
        currentReadings = manualInputs;
        processLogic(); // Re-calc immediately
    }

    // Periodic Update (e.g., every 1 second)
    if (now - lastLogicUpdate > 1000) {
        lastLogicUpdate = now;
        
        // Read actual sensors (with simulation for missing ones)
        // Note: In a real system, you might want to separate "reading" from "logic" 
        // to avoid overwriting manual inputs instantly if that was the intent.
        // For this refactor, we stick to the original behavior of continuous update.
        currentReadings = sensors.readAll(true); 
        
        processLogic();
    }

    // Always handle web clients
    webServer.handleClient();
}

void SystemController::processLogic() {
    // 1. Fuzzify
    currentFuzzyState = fuzzy.calculateMemberships(currentReadings);

    // 2. Output Rules (Currently Random/Simulated as per original code)
    currentDecisions = fuzzy.evaluateRules(currentFuzzyState);
    generateOutputs();
    
    // 3. Update Web Server State
    webServer.updateState(currentReadings, currentFuzzyState, currentOutputs);
    
    // Optional: Print to Serial for debug
    // fuzzy.printResults(currentReadings, currentFuzzyState);
}

void SystemController::generateOutputs() {
    for (int i = 0; i < 5; i++) {
        currentOutputs.outputs[i].membership = 0.0;
        currentOutputs.outputs[i].value = 0.0;
    }

    currentOutputs.outputs[0].label = currentDecisions.heating;
    currentOutputs.outputs[1].label = currentDecisions.cooling;
    currentOutputs.outputs[2].label = currentDecisions.shadow;
    currentOutputs.outputs[3].label = currentDecisions.water;
    currentOutputs.outputs[4].label = currentDecisions.lighting;
}
