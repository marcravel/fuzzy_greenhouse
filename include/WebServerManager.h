#ifndef WEB_SERVER_MANAGER_H
#define WEB_SERVER_MANAGER_H

#include <WebServer.h>
#include "SystemState.h"

class WebServerManager {
public:
    WebServerManager();
    void begin();
    void handleClient();
    
    // Updates the state that will be served to the client
    void updateState(const SensorReadings& sensors, const SystemFuzzyState& fuzzy, const SystemOutputs& outs);
    
    // Updates the inputs from the web UI (manual override)
    // Returns true if new inputs were received
    bool checkManualInputs(SensorReadings& manualInputs);

private:
    WebServer server;
    
    // Cache of current state to serve
    SensorReadings currentSensors;
    SystemFuzzyState currentFuzzy;
    SystemOutputs currentOutputs;
    
    bool newManualInputAvailable;
    SensorReadings lastManualInput;

    // Handlers
    void handleRoot();
    void handleSetInputs();
    void handleData();
};

#endif // WEB_SERVER_MANAGER_H
