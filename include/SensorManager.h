#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Wire.h>
#include <BH1750.h>
#include "SystemState.h"

// Define pin constants here or in main
#define DHTPIN 4     
#define DHTTYPE DHT11

#define SDA 21
#define SCL 22

class SensorManager {
public:
    SensorManager();
    void begin();
    
    // Updates sensor readings. 
    // realRead: if true, reads from hardware. if false, might just return cached or simulated.
    SensorReadings readAll(bool simulateUpdates = false);

private:
    DHT dht;
    BH1750 lightSensor;
    
    // Keep track of current state for simulation/smoothing
    float currentTemp;
    float currentHum;
    float currentLight;
    float currentSoil;
    
    unsigned long lastUpdate;
};

#endif // SENSOR_MANAGER_H
