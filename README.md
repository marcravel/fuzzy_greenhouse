# Fuzzy Greenhouse Project

This project implements a Fuzzy Logic Controller for a Smart Greenhouse using an ESP32. It monitors Temperature, Humidity, Light, and Soil Moisture to make control decisions.

## Project Structure (Refactored)

The project has been refactored into a modular, object-oriented architecture:

### Modules
- **SystemController**: The main brain of the system. Initialized in `main.cpp`, it coordinates sensors, logic, and the web server.
- **SensorManager**: Handles reading from physical sensors (DHT) and simulation for others.
- **FuzzyLogic**: Encapsulates the fuzzy logic mathematics and rules. Keeps the original Turkish terminology for domain logic variables to match the fuzzy design requirements.
- **WebServerManager**: Manages the WiFi Access Point and serves the Web Interface (Dashboard).

### Key Files
- `src/main.cpp`: Entry point, minimal setup.
- `src/SystemController.cpp`: Logic loop and orchestration.
- `src/FuzzyLogic.cpp`: Membership functions and fuzzification.
- `src/WebServerManager.cpp`: HTTP server and API logic.
- `include/SystemState.h`: Shared data structures defining the system state.

## How to Run
1. Open in PlatformIO.
2. Build and Upload to ESP32.
3. Connect to WiFi `ESP32_Sera` (Password: `12345678`).
4. Navigate to `192.168.4.1` in your browser.

## Features
- real-time monitoring of sensors.
- Fuzzy logic based classification (Cok Dusuk -> Cok Yuksek).
- Manual override capability via Web UI.
- JSON API at `/data`.