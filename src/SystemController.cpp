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
    if (now - lastLogicUpdate > 10000) {
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
    Serial.println("temp: " + currentFuzzyState.temp_mu.sozel_ifade); // Example usage
    Serial.println("hum: " + currentFuzzyState.hum_mu.sozel_ifade);
    Serial.println("light: " + currentFuzzyState.light_mu.sozel_ifade);
    Serial.println("soil: " + currentFuzzyState.soil_mu.sozel_ifade);

    // 2. Output Rules (Currently Random/Simulated as per original code)
    generateOutputs();
    
    // 3. Update Web Server State
    webServer.updateState(currentReadings, currentFuzzyState, currentOutputs);
    
    // Optional: Print to Serial for debug
    // fuzzy.printResults(currentReadings, currentFuzzyState);
}

void SystemController::generateOutputs() {
    // Ported from original calculateOutputs logic
    // This seems to be a simulation of decision making
    String terms[] = {"Cok Dusuk", "Dusuk", "Orta", "Yuksek", "Cok Yuksek"};
    
    for (int i = 0; i < 5; i++) {
        currentOutputs.outputs[i].membership = random(10, 100) / 100.0f;
        currentOutputs.outputs[i].label = terms[random(0, 5)];
        
        if (i == 0) currentOutputs.outputs[i].value = random(0, 1000) / 100.0; 
        else if (i == 1) currentOutputs.outputs[i].value = random(0, 500) / 100.0;
        else if (i == 2) currentOutputs.outputs[i].value = random(0, 100);
        else if (i == 3) currentOutputs.outputs[i].value = random(0, 100);
        else if (i == 4) currentOutputs.outputs[i].value = random(1000, 20000);
    }
}
