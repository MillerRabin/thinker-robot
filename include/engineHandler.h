#ifndef engine_handler_h
#define engine_handler_h

#include <AsyncJson.h>
#include <ArduinoJson.h>
#include "armEngines.h"
#include "armDetectors.h"
#include "Target.h"
#include "armParams.h"
#include "initServer.h"
#include "Position.h"
#include "Strategy.h"
#include "armEngines.h"
#include "angles.h"

class EngineHandler {
    private:
        void sendSuccess(AsyncWebServerRequest* request, Position pos, ArmDetectorsData data, std::string strategyType);
        void sendError(AsyncWebServerRequest* request, const char* message);    
        AsyncCallbackJsonWebHandler* positionHandler;
        AsyncCallbackJsonWebHandler* engineHandler;
    public: 
        ArmEngines armEngines;
        ArmDetectors armDetectors;
        EngineHandler();
        ~EngineHandler();
};

#endif