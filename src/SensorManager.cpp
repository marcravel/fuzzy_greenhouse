#include "SensorManager.h"

SensorManager::SensorManager() 
    : dht(DHTPIN, DHTTYPE), currentTemp(10.0), currentHum(35.0), currentLight(0.0), currentSoil(45.0), lastUpdate(0) {}

void SensorManager::begin() {
    Wire.begin(SDA, SCL);
    delay(200);
    lightSensor.begin();
    dht.begin();

    Serial.println("Scanning...");
    for (int addr = 1; addr < 127; addr++)
    {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0)
        {
            Serial.print("Found at 0x");
            Serial.println(addr, HEX);
        }
    }
    // Initial read to populate values
    readAll(true);
}

SensorReadings SensorManager::readAll(bool simulateUpdates) {
    SensorReadings readings;
    
    // 1. Read Physical Sensors (DHT)
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    float lux = lightSensor.readLightLevel();

    if (!isnan(t)) currentTemp = t;
    if (!isnan(h)) currentHum = h;
    if (!isnan(lux)) currentLight = lux;

    // 2. Simulate other sensors (Light & Soil) if requested
    if (simulateUpdates) {
        // currentTemp += (3 / 10.0f);
        // if (currentTemp >= 45.0f) currentTemp = 0.0f;
        // currentHum += 3;
        // if (currentHum >= 100) currentHum = 20;
        // currentLight += 300;
        // if (currentLight >= 20000) currentLight = 0; 
        // currentSoil += 3;
        if (currentSoil >= 100) currentSoil = 20;
        
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
