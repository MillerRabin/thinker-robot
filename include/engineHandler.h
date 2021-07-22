#ifndef engine_handler_h
#define engine_handler_h

#include "ArmEngines.h"
#include <AsyncJson.h>

class EngineHandler {
    private:
        void sendSuccess(AsyncWebServerRequest* request, Position pos);
        void sendError(AsyncWebServerRequest* request, const char* message);    
        AsyncCallbackJsonWebHandler* positionHandler;
        AsyncCallbackJsonWebHandler* engineHandler;
    public: 
        ArmEngines armEngines;
        EngineHandler();
        ~EngineHandler();
};

#endif