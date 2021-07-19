#include <AsyncJson.h>
#include <ArduinoJson.h>
#include "armParams.h"
#include "initServer.h"
#include "position.h"
#include "strategy.h"
#include "armEngines.h"

ArmEngines armEngines = ArmEngines();
        
void sendSuccess(AsyncWebServerRequest* request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonBuffer jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    JsonObject &pAngles = root.createNestedObject("physicalAngles");
    pAngles["claw"] = armEngines.getClawAngle();
    pAngles["claw-x"] = armEngines.getClawXAngle();    
    pAngles["shoulder-y"] = armEngines.getShoulderYAngle();
    pAngles["shoulder-z"] = armEngines.getShoulderZAngle();
    pAngles["elbow-y"] = armEngines.getElbowYAngle();
    pAngles["wrist-y"] = armEngines.getWristYAngle();
    Position pos = armEngines.getPosition();
    JsonObject &positions = root.createNestedObject("positions");
    positions["claw-x"] = pos.getX();
    positions["claw-y"] = pos.getY();
    positions["claw-z"] = pos.getZ();        
    positions["elbow-x"] = pos.shoulder.x + pos.elbow.x;
    positions["elbow-y"] = pos.shoulder.y + pos.elbow.y;
    positions["elbow-z"] = pos.shoulder.z + pos.elbow.z + BASE_HEIGHT;
    JsonArray &errors = root.createNestedArray("errors");
    armEngines.errors.flushToJsonArray(errors);
    root.printTo(*response);
    request->send(response);    
}

void sendError(AsyncWebServerRequest *request, const char* message) {
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonBuffer jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["error"] = message;
    root.printTo(*response);
    request->send(response);
}

AsyncCallbackJsonWebHandler* engineHandler = new AsyncCallbackJsonWebHandler("/move", [](AsyncWebServerRequest *request, JsonVariant &json) {    
    try {
        JsonObject& jsonObj = json.as<JsonObject>();
        armEngines.set(jsonObj);
        sendSuccess(request);
    } catch (const std::exception& e) {
        sendError(request, e.what());
    } catch (...) {
        sendError(request, "Unknown Error");
    }
});

AsyncCallbackJsonWebHandler* positionHandler = new AsyncCallbackJsonWebHandler("/position", [](AsyncWebServerRequest *request, JsonVariant &json) {    
    try {
        JsonObject& jsonObj = json.as<JsonObject>();        
        Position pos = armEngines.getPosition();        
        Strategy moveStrategy(
            pos, 
            ArmEngines::getDouble(jsonObj, "claw-x"),
            ArmEngines::getDouble(jsonObj, "claw-y"),
            ArmEngines::getDouble(jsonObj, "claw-z"),
            ArmEngines::getDouble(jsonObj, "claw-angle-y"),
            ArmEngines::getDouble(jsonObj, "claw-angle-x"),
            ArmEngines::getDouble(jsonObj, "claw-angle")            
        );
        armEngines.applyStrategy(moveStrategy);
        sendSuccess(request);        
    } catch (const std::exception& e) {
        sendError(request, e.what());
    } catch (...) {
        sendError(request, "Unknown Error");
    }
});

void enableEngines() {       
    Server.addHandler(engineHandler);    
    Server.addHandler(positionHandler);    
}