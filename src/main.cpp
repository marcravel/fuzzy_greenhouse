#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// Import the separate HTML file
#include "web_index.h"
#include "fuzzy_functions.h"

const char* ssid = "ESP32_Sera";
const char* password = "12345678";

WebServer server(80);

// --- Data Models ---
float rawInputs[4] = {25.0, 60.0, 5000.0, 45.0}; // 4 Inputs: Temp, Hum, Light, Soil
// 5 Linguistic terms per input
float inputMemberships[4][5]; 
// Labels: CD (Cok Dusuk), D (Dusuk), O (Orta), Y (Yuksek), CY (Cok Yuksek)
String terms[] = {"Cok Dusuk", "Dusuk", "Orta", "Yuksek", "Cok Yuksek"};

// Output Structure
struct OutputData {
  float membership; // 0.0 - 1.0 degree
  float value;      // Calculated crisp value (e.g., kW, Lux)
  String label;     // Fuzzy label
};
OutputData outputs[5]; // Heating, Cooling, Shading, Irrigation, Lighting

// --- Simulation Logic ---

// Determines fuzzy membership (Simulated Triangle MF)
void calculateFuzzyMemberships() {
  // Use real fuzzification functions from fuzzy_functions.* to populate
  // `inputMemberships` for each of the 4 inputs (Temp, Hum, Light, Soil).
  FuzzyInput t = fuzzify_temperature_final(rawInputs[0]);
  inputMemberships[0][0] = (float)t.cok_dusuk;
  inputMemberships[0][1] = (float)t.dusuk;
  inputMemberships[0][2] = (float)t.orta;
  inputMemberships[0][3] = (float)t.yuksek;
  inputMemberships[0][4] = (float)t.cok_yuksek;

  FuzzyInput h = fuzzify_huminidty_final(rawInputs[1]);
  inputMemberships[1][0] = (float)h.cok_dusuk;
  inputMemberships[1][1] = (float)h.dusuk;
  inputMemberships[1][2] = (float)h.orta;
  inputMemberships[1][3] = (float)h.yuksek;
  inputMemberships[1][4] = (float)h.cok_yuksek;

  FuzzyInput l = fuzzify_isik_final(rawInputs[2]);
  inputMemberships[2][0] = (float)l.cok_dusuk;
  inputMemberships[2][1] = (float)l.dusuk;
  inputMemberships[2][2] = (float)l.orta;
  inputMemberships[2][3] = (float)l.yuksek;
  inputMemberships[2][4] = (float)l.cok_yuksek;

  FuzzyInput s = fuzzify_topraknemi_final(rawInputs[3]);
  inputMemberships[3][0] = (float)s.cok_dusuk;
  inputMemberships[3][1] = (float)s.dusuk;
  inputMemberships[3][2] = (float)s.orta;
  inputMemberships[3][3] = (float)s.yuksek;
  inputMemberships[3][4] = (float)s.cok_yuksek;
}

void calculateOutputs() {
  // Simulate Output calculations
  for (int i = 0; i < 5; i++) {
    outputs[i].membership = random(10, 100) / 100.0;
    outputs[i].label = terms[random(0, 5)];
    
    // Assign varying ranges for crisp values
    if (i == 0) outputs[i].value = random(0, 1000) / 100.0; // Heating kW
    else if (i == 1) outputs[i].value = random(0, 500) / 100.0; // Cooling Micron
    else if (i == 2) outputs[i].value = random(0, 100); // Shading cm
    else if (i == 3) outputs[i].value = random(0, 100); // Irrigation Lt
    else if (i == 4) outputs[i].value = random(1000, 20000); // Lighting Lux
  }
}

// --- Web Server Handlers ---

void handleRoot() {
  server.send(200, "text/html", index_html);
}

// API to receive inputs from the dashboard
void handleSetInputs() {
  if (server.hasArg("temp")) rawInputs[0] = server.arg("temp").toFloat();
  if (server.hasArg("hum")) rawInputs[1] = server.arg("hum").toFloat();
  if (server.hasArg("light")) rawInputs[2] = server.arg("light").toFloat();
  if (server.hasArg("soil")) rawInputs[3] = server.arg("soil").toFloat();

  // Recalculate based on new inputs
  calculateFuzzyMemberships();
  calculateOutputs();

  // server.send(200, "text/plain", "Degerler Guncellendi!");
}

// API to send JSON data to dashboard (AJAX)
void handleData() {
  // Construct JSON manually to avoid dependencies, though ArduinoJson is recommended.
  String json = "{";
  
  // 1. Input Memberships (Matrix)
  json += "\"inputs_m\":[";
  for(int i=0; i<4; i++) {
    json += "[";
    for(int j=0; j<5; j++) {
      json += String(inputMemberships[i][j]);
      if(j<4) json += ",";
    }
    json += "]";
    if(i<3) json += ",";
  }
  json += "],";

  // 2. Active Input States
  json += "\"inputs_active\":[";
  for(int i=0; i<4; i++) {
    // Find highest membership term for display
    int bestTerm = 0; 
    float maxVal = -1;
    for(int k=0; k<5; k++) { 
      if(inputMemberships[i][k] > maxVal) { 
        maxVal = inputMemberships[i][k]; bestTerm = k; 
      } 
    }
    
    json += "{";
    json += "\"val\":" + String(rawInputs[i]) + ",";
    json += "\"label\":\"" + terms[bestTerm] + "\"";
    json += "}";
    if(i<3) json += ",";
  }
  json += "],";

  // 3. Outputs
  json += "\"outputs\":[";
  for(int i=0; i<5; i++) {
    json += "{";
    json += "\"membership\":" + String(outputs[i].membership) + ",";
    json += "\"value\":" + String(outputs[i].value) + ",";
    json += "\"label\":\"" + outputs[i].label + "\"";
    json += "}";
    if(i<4) json += ",";
  }
  json += "],";

  json += "\"uptime\":" + String(millis()/1000);
  json += "}";

  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);

  // Initial Calculation
  calculateFuzzyMemberships();
  calculateOutputs();

  // Wifi Setup
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // Server Routes
  server.on("/", handleRoot);
  server.on("/set", handleSetInputs); // For button "Tüm Değerleri Uygula"
  server.on("/data", handleData);     // For background AJAX updates

  server.begin();
}

void loop() {
  server.handleClient();
  // In a real application, you would run the fuzzy logic processing here periodically
}