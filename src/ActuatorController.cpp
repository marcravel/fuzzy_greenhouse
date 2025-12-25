#include "ActuatorController.h"

// Calibration Constants (These would need tuning)
#define MAX_HEATING_KW 2.0f      // Max heater power
#define MAX_COOLING_MICRON 50.0f // Max misting/cooling setting
#define MAX_LIGHTING_LUX 20000.0f
#define FLOW_RATE_L_PER_SEC 0.05f 
#define STEPS_PER_METER 1000     // Calibration for shadow system

ActuatorController::ActuatorController() 
    : lastUpdate(0), isWatering(false), wateringStartTime(0), wateringDuration(0),
      currentShadowSteps(0), targetShadowSteps(0) {}

void ActuatorController::begin() {
    // PWM Setup for Heating, Cooling, Lighting
    ledcSetup(PWM_CH_HEATING, PWM_FREQ, PWM_RES);
    ledcAttachPin(PIN_HEATING_PWM, PWM_CH_HEATING);

    ledcSetup(PWM_CH_COOLING, PWM_FREQ, PWM_RES);
    ledcAttachPin(PIN_COOLING_PWM, PWM_CH_COOLING);

    ledcSetup(PWM_CH_LIGHTING, PWM_FREQ, PWM_RES);
    ledcAttachPin(PIN_LIGHTING_PWM, PWM_CH_LIGHTING);

    // Digital & Stepper Setup
    pinMode(PIN_WATERING_DIGITAL, OUTPUT);
    digitalWrite(PIN_WATERING_DIGITAL, LOW);

    pinMode(PIN_SHADOW_STEP, OUTPUT);
    pinMode(PIN_SHADOW_DIR, OUTPUT);
    
    Serial.println("Actuator Controller Initialized");
}

void ActuatorController::update(const SystemOutputs& outputs) {
    // Map fuzzy outputs to actuators
    // Index mapping assumed from SystemController.cpp:
    // 0: Heating, 1: Cooling, 2: Shadow, 3: Water, 4: Lighting
    
    // setHeating(outputs.outputs[0].value);
    // setCooling(outputs.outputs[1].value);
    // setShadowing(outputs.outputs[2].value);
    // setWatering(outputs.outputs[3].value);
    setLighting(outputs.outputs[4].value); // For testing, set constant lighting

    // Handle Stepper Movement (Non-blocking simple implementation)
    // In a real loop, you'd pulse every X millis. Here we burst a few steps per update or assume external driver.
    // For simulation/blocking-proof logic, we'll just log or do a small move.
    if (currentShadowSteps != targetShadowSteps) {
        // Determine direction
        bool dir = (targetShadowSteps > currentShadowSteps);
        digitalWrite(PIN_SHADOW_DIR, dir ? HIGH : LOW);
        
        // Single step per update loop to be non-blocking? 
        // Or small burst. Let's do small burst.
        digitalWrite(PIN_SHADOW_STEP, HIGH);
        delayMicroseconds(10); // Very short delay
        digitalWrite(PIN_SHADOW_STEP, LOW);
        
        if (dir) currentShadowSteps++; else currentShadowSteps--;
    }

    // Handle Watering Timer
    if (isWatering) {
        if (millis() - wateringStartTime > wateringDuration) {
            digitalWrite(PIN_WATERING_DIGITAL, LOW);
            isWatering = false;
            Serial.println("Watering Finished");
        }
    }
}

void ActuatorController::setHeating(float kw) {
    // Map kw to PWM duty (0-255)
    int duty = map((long)(kw * 100), 0, (long)(MAX_HEATING_KW * 100), 0, 255);
    duty = constrain(duty, 0, 255);
    ledcWrite(PWM_CH_HEATING, duty);
}

void ActuatorController::setCooling(float micron) {
    // Map micron to PWM
    int duty = map((long)(micron * 100), 0, (long)(MAX_COOLING_MICRON * 100), 0, 255);
    duty = constrain(duty, 0, 255);
    ledcWrite(PWM_CH_COOLING, duty);
}

void ActuatorController::setLighting(float lux) {
    // Map lux to PWM
    int duty = map((long)lux, 0, (long)MAX_LIGHTING_LUX, 0, 255);
    duty = constrain(duty, 0, 255);
    ledcWrite(PWM_CH_LIGHTING, duty);
    Serial.println("Lighting set to duty: " + String(duty));
}

void ActuatorController::setWatering(float liters) {
    // If we receive a new non-zero water request and we aren't already watering (or maybe we add to it?)
    // Simpler logic: if liters > 0.1 and not watering, start.
    if (liters > 0.1 && !isWatering) {
        float durationSec = liters / FLOW_RATE_L_PER_SEC;
        wateringDuration = durationSec * 1000;
        wateringStartTime = millis();
        isWatering = true;
        digitalWrite(PIN_WATERING_DIGITAL, HIGH);
        Serial.printf("Watering Started: %.2f L for %lu ms\n", liters, wateringDuration);
    }
}

void ActuatorController::setShadowing(float meters) {
    targetShadowSteps = (int)(meters * STEPS_PER_METER);
}
