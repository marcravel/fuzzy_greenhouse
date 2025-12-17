#ifndef SYSTEM_CONTROLLER_H
#define SYSTEM_CONTROLLER_H

#include "SensorManager.h"
#include "FuzzyLogic.h"
#include "WebServerManager.h"

class SystemController {
public:
    SystemController();
    void begin();
    void update();

private:
    SensorManager sensors;
    FuzzyLogic fuzzy;
    WebServerManager webServer;

    // Simulation/Cache state
    SensorReadings currentReadings;
    SystemFuzzyState currentFuzzyState;
    SystemDecisions currentDecisions;
    SystemOutputs currentOutputs;
    
    unsigned long lastLogicUpdate;
    
    // Logic helpers
    void processLogic();
    void generateOutputs(); // Currently simulation logic
};

#endif // SYSTEM_CONTROLLER_H
