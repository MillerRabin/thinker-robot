#include <armParams.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include <ESP32_Servo.h>
#include "initServer.h"
#include <Position.h>
#include <Strategy.h>

#define COUNT_LOW 1638
#define COUNT_HIGH 7864
#define COUNT_RANGE COUNT_HIGH - COUNT_LOW
#define TIMER_WIDTH 16


#define GRIPPER_MAX_ANGLE 4000

unsigned int gGripper = 0;
unsigned int gGripperRotate = 0;
unsigned int gRotate = 0;
unsigned int gShoulder = 0;
unsigned int gElbow = 0;
unsigned int gWrist = 0;

 
int getValue(JsonObject& jsonObj, const char* key, const unsigned int min, const unsigned int max) {
    if (!jsonObj.containsKey(key)) return -1;
    const char* str = jsonObj[key];
    const int val = atoi(str);
    return val;
}

double getDoubleDef(JsonObject& jsonObj, const char* key, const double min, const double max, const double def) {
    if (!jsonObj.containsKey(key)) return def;    
    const char* str = jsonObj[key];
    const int val = atof(str);
    if (val < min) return def;
    if (val > max) return def;
    return val;
}

int getPosition(JsonObject& jsonObj, const char* key) {
    if (!jsonObj.containsKey(key)) return -1;
    const char* str = jsonObj[key];
    const int val = atoi(str);
    return val;
}

uint countToDeg(const uint value, const uint maxDeg) {
    const float vl = float(value - COUNT_LOW) / (COUNT_RANGE) * maxDeg;    
    return (uint)vl;
}

uint degToCount(const uint value, const uint maxDeg) {        
    const float vl = float(value) / maxDeg * (COUNT_RANGE) + COUNT_LOW;
    return (uint)vl;
}

double getGripperAngleZ(const double grAngle, const double rAngle) {
    return (grAngle + GRIPPER_ROTATE_OFFSET - 90) - (90 - rAngle - ROTATE_OFFSET);
}

double getGripperAngleXY(const double sAngle, const double eAngle, const double wAngle) {    
    const double a1 = (-1 * sAngle + SHOULDER_OFFSET);    
    const double a2 = 180 - (eAngle + ELBOW_OFFSET);    
    const double a3 = wAngle - (WRIST_OFFSET - 95);
    return a1 - a2 + a3;
}

void sendSuccess(AsyncWebServerRequest* request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonBuffer jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["gripper"] = gGripper;
    root["gripper-rotate"] = gGripperRotate;
    root["gripper-rotate-angle-z"] = getGripperAngleZ(gGripperRotate, gRotate);
    root["gripper-rotate-angle-xy"] = getGripperAngleXY(gShoulder, gElbow, gWrist);
    root["rotate"] = gRotate;
    root["shoulder"] = gShoulder;
    root["elbow"] = gElbow;
    root["wrist"] = gWrist;
    const Position pos(gShoulder, gElbow, gWrist, gRotate);    
    root["length-x"] = pos.x.length;
    root["length-y"] = pos.y.length;
    root["length-z"] = pos.z.length;
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


void setGripper(JsonObject& jsonObj) {
    const int tGripper = getValue(jsonObj, "gripper", COUNT_LOW, GRIPPER_MAX_ANGLE);    
    if (tGripper >= 0 ) {
        const uint tDeg = degToCount(tGripper, 270);
        gGripper = (unsigned int) tGripper;
        ledcWrite(GRIPPER_ENGINE, tDeg);
    }
}

void setGripperRotate(JsonObject& jsonObj) {
    const int tGripper = getValue(jsonObj, "gripper_rotate", COUNT_LOW, COUNT_HIGH);    
    if (tGripper >= 0 ) {
        const uint tDeg = degToCount(tGripper, 270);
        gGripperRotate = (unsigned int) tGripper;
        ledcWrite(GRIPPER_ROTATE_ENGINE, tDeg);
    }
}

void setRotate(JsonObject& jsonObj) {
    const int tGripper = getValue(jsonObj, "rotate", COUNT_LOW, COUNT_HIGH);    
    if (tGripper >= 0 ) {
        const uint tDeg = degToCount(tGripper, 270);
        gRotate = (unsigned int) tGripper;        
        ledcWrite(ROTATE_ENGINE, tDeg);
    }
}

unsigned int setEngine(const unsigned int engine, const unsigned int angle) {
        const uint tDeg = degToCount(angle, 270);        
        ledcWrite(engine, tDeg);        
        return angle;
}


void setShoulder(JsonObject& jsonObj) {
    const int tGripper = getValue(jsonObj, "shoulder", COUNT_LOW, COUNT_HIGH);    
    if (tGripper >= 0 )
        gShoulder = setEngine(SHOULDER_ENGINE, tGripper);
}

void setElbow(JsonObject& jsonObj) {
    const int tGripper = getValue(jsonObj, "elbow", COUNT_LOW, COUNT_HIGH);    
    if (tGripper >= 0 )
        gElbow = setEngine(ELBOW_ENGINE, tGripper);
}

void setWrist(JsonObject& jsonObj) {
    const int tGripper = getValue(jsonObj, "wrist", COUNT_LOW, COUNT_HIGH);    
    if (tGripper >= 0 ) {
        const uint tDeg = degToCount(tGripper, 270);
        gWrist = (unsigned int) tGripper;
        ledcWrite(WRIST_ENGINE, tDeg);
    }
}

AsyncCallbackJsonWebHandler* engineHandler = new AsyncCallbackJsonWebHandler("/move", [](AsyncWebServerRequest *request, JsonVariant &json) {    
    try {
        JsonObject& jsonObj = json.as<JsonObject>();
        setGripper(jsonObj);
        setGripperRotate(jsonObj);
        setRotate(jsonObj);
        setShoulder(jsonObj);
        setElbow(jsonObj);
        setWrist(jsonObj);
        sendSuccess(request);
    } catch (const std::exception& e) {
        sendError(request, e.what());
    } catch (...) {
        sendError(request, "Unknown Error");
    }
});

void apply(Strategy strategy) {
    for(EngineControl control : strategy.sequence) {    
        if (control.engine == SHOULDER_ENGINE) {
            gShoulder = setEngine(SHOULDER_ENGINE, control.angle);
            continue;
        }
        if (control.engine == ELBOW_ENGINE) {
            gElbow = setEngine(ELBOW_ENGINE, control.angle);
            continue;
        }
        if (control.engine == WRIST_ENGINE) {
            gWrist = setEngine(WRIST_ENGINE, control.angle);    
            continue;
        }
    }    
}


AsyncCallbackJsonWebHandler* positionHandler = new AsyncCallbackJsonWebHandler("/position", [](AsyncWebServerRequest *request, JsonVariant &json) {    
    try {
        JsonObject& jsonObj = json.as<JsonObject>();        
        const Position pos(gShoulder, gElbow, gWrist, gRotate);        
        const double tx = getDoubleDef(jsonObj, "x", 95, 200, pos.x.length);    
        const double ty = getDoubleDef(jsonObj, "y", -50, 200, pos.y.length);    
        const double tz = getDoubleDef(jsonObj, "z", -50, 200, pos.z.length);    
        Strategy moveStrategy(pos, tx, ty, tz);
        apply(moveStrategy);
        sendSuccess(request);        
    } catch (const std::exception& e) {
        sendError(request, e.what());
    } catch (...) {
        sendError(request, "Unknown Error");
    }
});

void enableEngines() {    
    ledcSetup(GRIPPER_ENGINE, 50, TIMER_WIDTH);
    ledcSetup(GRIPPER_ROTATE_ENGINE, 50, TIMER_WIDTH);
    ledcSetup(ROTATE_ENGINE, 50, TIMER_WIDTH);
    ledcSetup(SHOULDER_ENGINE, 50, TIMER_WIDTH);
    ledcSetup(ELBOW_ENGINE, 50, TIMER_WIDTH);
    ledcSetup(WRIST_ENGINE, 50, TIMER_WIDTH);
    ledcAttachPin(GRIPPER_PIN, GRIPPER_ENGINE);
    ledcAttachPin(GRIPPER_ROTATE_PIN, GRIPPER_ROTATE_ENGINE);
    ledcAttachPin(ROTATE_PIN, ROTATE_ENGINE);
    ledcAttachPin(SHOULDER_PIN, SHOULDER_ENGINE);
    ledcAttachPin(ELBOW_PIN, ELBOW_ENGINE);
    ledcAttachPin(WRIST_PIN, WRIST_ENGINE);    
    Server.addHandler(engineHandler);    
    Server.addHandler(positionHandler);    
}