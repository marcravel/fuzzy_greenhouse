#include "SensorManager.h"

SensorManager::SensorManager() 
    : dht(DHTPIN, DHTTYPE), currentTemp(25.0), currentHum(60.0), currentLight(5000.0), currentSoil(45.0), lastUpdate(0) {}

void SensorManager::begin() {
    dht.begin();
    // Initial read to populate values
    readAll(false);
}

SensorReadings SensorManager::readAll(bool simulateUpdates) {
    SensorReadings readings;
    
    // 1. Read Physical Sensors (DHT)
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (!isnan(t)) currentTemp = t;
    if (!isnan(h)) currentHum = h;

    // 2. Simulate other sensors (Light & Soil) if requested
    // Logic taken from original server_module.cpp loop
    if (simulateUpdates) {
        currentLight += random(-100, 101); // Fluctuate light
        currentSoil += random(-5, 6) / 10.0f; // Fluctuate soil
        
        // Bounds checking (optional but good practice)
        if (currentLight < 0) currentLight = 0;
        if (currentSoil < 0) currentSoil = 0;
        if (currentSoil > 100) currentSoil = 100;
    }

    readings.temperature = currentTemp;
    readings.humidity = currentHum;
    readings.lightLevel = currentLight;
    readings.soilMoisture = currentSoil;

    return readings;
}
