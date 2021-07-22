#include <AsyncJson.h>
#include <ArduinoJson.h>
#include "armParams.h"
#include "initServer.h"
#include "position.h"
#include "strategy.h"
#include "armEngines.h"
#include "engineHandler.h"

void EngineHandler::sendSuccess(AsyncWebServerRequest* request, Position pos) {
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    JsonObject& pAngles = root.createNestedObject("target-physical-angles");
    pAngles["claw-angle"] = pos.getClawAngle();
    pAngles["claw-angle-x"] = pos.getClawXAngle();    
    pAngles["shoulder-angle-y"] = pos.getShoulderYAngle();
    pAngles["shoulder-angle-z"] = pos.getShoulderZAngle();
    pAngles["elbow-angle-y"] = pos.getElbowYAngle();
    pAngles["wrist-angle-y"] = pos.getWristYAngle(); 
    JsonObject& lAngles = root.createNestedObject("target-logical-angles");
    lAngles["claw-angle"] = pos.getClawAngle();
    lAngles["claw-angle-x"] = double(pos.claw.XRad / PI) * 180;
    lAngles["shoulder-angle-y"] = double(pos.shoulder.YRad / PI) * 180;
    lAngles["shoulder-angle-z"] = double(pos.shoulder.ZRad / PI) * 180;
    lAngles["elbow-angle-y"] = double(pos.elbow.YRad / PI) * 180;
    lAngles["wrist-angle-y"] = double(pos.wrist.YRad / PI) * 180;
    JsonObject& positions = root.createNestedObject("target-position");
    positions["claw-x"] = pos.getX();
    positions["claw-y"] = pos.getY();
    positions["claw-z"] = pos.getZ();        
    positions["elbow-x"] = pos.shoulder.x + pos.elbow.x;
    positions["elbow-y"] = pos.shoulder.y + pos.elbow.y;
    positions["elbow-z"] = pos.shoulder.z + pos.elbow.z + BASE_HEIGHT;
    ArmOperationResult res = pos.getLastError();
    JsonObject& stats = root.createNestedObject("stats");
    stats["arm-operation-queued"] = ArmEngines::queue.size();
    
    if (res != ARM_OPERATION_SUCCESS) {
        JsonObject& error = root.createNestedObject("error");
        error["code"] = (int)res;
        error["text"] = pos.getLastErrorText().c_str();
    }
        
    root.printTo(*response);
    request->send(response);    
}

void EngineHandler::sendError(AsyncWebServerRequest* request, const char* message) {
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonBuffer jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["error"] = message;
    root.printTo(*response);
    request->send(response);
}

EngineHandler::EngineHandler() {      
    engineHandler = new AsyncCallbackJsonWebHandler("/move", [this](AsyncWebServerRequest* request, JsonVariant& json) mutable {    
        try {
            JsonObject& jsonObj = json.as<JsonObject>();
            Position pos = armEngines.set(jsonObj);
            sendSuccess(request, pos);
        } catch (const std::exception& e) {
            sendError(request, e.what());
        } catch (...) {
            sendError(request, "Unknown Error");
        }
    });

    positionHandler = new AsyncCallbackJsonWebHandler("/position", [this](AsyncWebServerRequest* request, JsonVariant& json) mutable {    
        try {
            JsonObject& jsonObj = json.as<JsonObject>();        
            Position pos = armEngines.getPosition();        
            Strategy moveStrategy(
                pos, 
                ArmEngines::getDouble(jsonObj, "claw-x"),
                ArmEngines::getDouble(jsonObj, "claw-y"),
                ArmEngines::getDouble(jsonObj, "claw-z"),
                ArmEngines::getDouble(jsonObj, "claw-angle-x"),
                ArmEngines::getDouble(jsonObj, "claw-angle-y"),                
                ArmEngines::getDouble(jsonObj, "claw-angle")            
            );
            Position rPos = armEngines.applyStrategy(moveStrategy);            
            sendSuccess(request, rPos);
        } catch (const std::exception& e) {
            sendError(request, e.what());
        } catch (...) {
            sendError(request, "Unknown Error");
        }
    });

    Server.addHandler(engineHandler);    
    Server.addHandler(positionHandler);    
}

EngineHandler::~EngineHandler() {      
    delete engineHandler;
    delete positionHandler; 
}