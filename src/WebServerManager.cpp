#include "WebServerManager.h"
#include <WiFi.h>
#include "web_index.h" // Preserving existing HTML header file

const char* ssid = "ESP32_Sera";
const char* password = "12345678";

// Term label array for JSON generation
const String terms[] = {"Cok Dusuk", "Dusuk", "Orta", "Yuksek", "Cok Yuksek"};

WebServerManager::WebServerManager() : server(80), newManualInputAvailable(false) {}

void WebServerManager::begin() {
    WiFi.softAP(ssid, password);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    // Bind handlers using lambda or specific static glue if needed, 
    // but cleaner to use std::bind or lambda in Arduino if supported,
    // or just simple instance method wrappers if we had a singleton.
    // For simplicity with this library style:
    server.on("/", [this]() { handleRoot(); });
    server.on("/set", [this]() { handleSetInputs(); });
    server.on("/data", [this]() { handleData(); });

    server.begin();
}

void WebServerManager::handleClient() {
    server.handleClient();
}

void WebServerManager::updateState(const SensorReadings& sensors, const SystemFuzzyState& fuzzy, const FuzzyResults& fuzzyRes, const SystemOutputs& outs) {
    currentSensors = sensors;
    currentFuzzy = fuzzy;
    currentFuzzyRes = fuzzyRes;
    currentOutputs = outs;
}

bool WebServerManager::checkManualInputs(SensorReadings& manualInputs) {
    if (newManualInputAvailable) {
        manualInputs = lastManualInput;
        newManualInputAvailable = false;
        return true;
    }
    return false;
}

void WebServerManager::handleRoot() {
    server.send(200, "text/html", index_html);
}

void WebServerManager::handleSetInputs() {
    // Check which args are present and update the manual input cache
    // We start with current sensors so we only update changed fields
    lastManualInput = currentSensors; 

    if (server.hasArg("temp")) lastManualInput.temperature = server.arg("temp").toFloat();
    if (server.hasArg("hum")) lastManualInput.humidity = server.arg("hum").toFloat();
    if (server.hasArg("light")) lastManualInput.lightLevel = server.arg("light").toFloat();
    if (server.hasArg("soil")) lastManualInput.soilMoisture = server.arg("soil").toFloat();

    newManualInputAvailable = true;
    server.send(200, "text/plain", "Degerler Guncellendi!");
}

void WebServerManager::handleData() {
    // Build JSON exactly as the frontend expects
    String json = "{";
    
    // "raw_inputs"
    json += "\"raw_inputs\":[";
    json += String(currentSensors.temperature) + ",";
    json += String(currentSensors.humidity) + ",";
    json += String(currentSensors.lightLevel) + ",";
    json += String(currentSensors.soilMoisture);
    json += "],";

    // "inputs_m" (Matrix of memberships)
    // Order: Temp, Hum, Light, Soil
    json += "\"inputs_m\":[";
    
    // Helper lambda to append array
    auto appendMu = [&](const FuzzyMembership& mu, bool last) {
        json += "[";
        json += String(mu.cok_dusuk) + ",";
        json += String(mu.dusuk) + ",";
        json += String(mu.orta) + ",";
        json += String(mu.yuksek) + ",";
        json += String(mu.cok_yuksek);
        json += "]";
        if (!last) json += ",";
    };

    appendMu(currentFuzzy.temp_mu, false);
    appendMu(currentFuzzy.hum_mu, false);
    appendMu(currentFuzzy.light_mu, false);
    appendMu(currentFuzzy.soil_mu, true);

    json += "],";

    // "inputs_active" (Best label for each input)
    json += "\"inputs_active\":[";
    
    auto appendActive = [&](float val, const FuzzyMembership& mu, char* label, bool lastItem) {
        json += "{";
        json += "\"val\":" + String(val) + ",";
        json += "\"label\":\"" + String(label) + "\"";
        json += "}";
        if(!lastItem) json += ",";
    };

    appendActive(currentSensors.temperature, currentFuzzy.temp_mu, currentFuzzyRes.temp_result.primary_label, false);
    appendActive(currentSensors.humidity, currentFuzzy.hum_mu, currentFuzzyRes.hum_result.primary_label, false);
    appendActive(currentSensors.lightLevel, currentFuzzy.light_mu, currentFuzzyRes.light_result.primary_label, false);
    appendActive(currentSensors.soilMoisture, currentFuzzy.soil_mu, currentFuzzyRes.soil_result.primary_label, true);

    json += "],";

    // "outputs"
    json += "\"outputs\":[";
    for(int i=0; i<5; i++){
        json += "{";
        json += "\"value\":" + String(currentOutputs.outputs[i].value) + ",";
        json += "\"label\":\"" + currentOutputs.outputs[i].label + "\"";
        json += "}";
        if(i < 4) json += ",";
    }
    json += "],";

    json += "\"uptime\":" + String(millis()/1000);
    json += "}";

    server.send(200, "application/json", json);
}
