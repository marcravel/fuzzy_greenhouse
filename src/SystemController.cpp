#include "SystemController.h"

SystemController::SystemController() : lastLogicUpdate(0) {}

void SystemController::begin() {
    Serial.begin(115200);
    sensors.begin();
    actuator.begin();
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
    currentFuzzyResults = fuzzy.calculateFuzzyResults(currentFuzzyState);
    currentDecisions = fuzzy.defuzzify(currentFuzzyResults);

    // 2. Output Rules (Currently Random/Simulated as per original code)
    generateOutputs(currentDecisions);
    // actuator.update(currentOutputs);
    
    // 3. Update Web Server State
    webServer.updateState(currentReadings, currentFuzzyState, currentFuzzyResults, currentOutputs);
    
    // Optional: Print to Serial for debug
    // fuzzy.printResults(currentReadings, currentFuzzyState);
}

void SystemController::generateOutputs(SystemDecisions decisions) {
    // Map decisions to outputs
    currentOutputs.outputs[0].label = String(decisions.output_label_light);
    currentOutputs.outputs[0].value = decisions.output_value_light;
    
    currentOutputs.outputs[1].label = String(decisions.output_label_water);
    currentOutputs.outputs[1].value = decisions.output_value_water;
    
    currentOutputs.outputs[2].label = String(decisions.output_label_heat);
    currentOutputs.outputs[2].value = decisions.output_value_heat;
    
    currentOutputs.outputs[3].label = String(decisions.output_label_shadow);
    currentOutputs.outputs[3].value = decisions.output_value_shadow;
    
    currentOutputs.outputs[4].label = String(decisions.output_label_cooling);
    currentOutputs.outputs[4].value = decisions.output_value_cooling;
    
}
