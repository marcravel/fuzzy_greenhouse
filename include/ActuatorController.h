#ifndef ACTUATOR_CONTROLLER_H
#define ACTUATOR_CONTROLLER_H

#include <Arduino.h>
#include "SystemState.h"

// Pin Definitions (ESP32)
#define PIN_COOLING_PWM 16
#define PIN_HEATING_PWM 17
#define PIN_WATERING_DIGITAL 18
#define PIN_LIGHTING_PWM 19
#define PIN_SHADOW_STEP 23
#define PIN_SHADOW_DIR 25

// PWM Channels (ESP32 LEDC)
#define PWM_CH_HEATING 0
#define PWM_CH_COOLING 1
#define PWM_CH_LIGHTING 2
#define PWM_FREQ 5000
#define PWM_RES 8

class ActuatorController {
public:
    ActuatorController();
    void begin();
    
    // Updates actuators based on fuzzy outputs
    void update(const SystemOutputs& outputs);

private:
    // Helper to map physical units to actuator signals
    void setHeating(float kw);
    void setCooling(float micron);
    void setLighting(float lux);
    void setWatering(float liters);
    void setShadowing(float meters);

    // Internal state for non-blocking operations could go here
    unsigned long lastUpdate;
    bool isWatering;
    unsigned long wateringStartTime;
    unsigned long wateringDuration;
    
    int currentShadowSteps;
    int targetShadowSteps;
};

#endif // ACTUATOR_CONTROLLER_H
