#include <armParams.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include <ESP32_Servo.h>
#include "initServer.h"
#include <position.h>
#include <strategy.h>

#define COUNT_LOW 1638
#define COUNT_HIGH 7864
#define COUNT_RANGE COUNT_HIGH - COUNT_LOW
#define TIMER_WIDTH 16


#define GRIPPER_MAX_ANGLE 4000

double gGripper = 0;
double gGripperRotate = 0;
double gRotate = 135;
double gShoulder = 90;
double gElbow = 230;
double gWrist = 235;
ArmError gErrors;

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


void sendSuccess(AsyncWebServerRequest* request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    DynamicJsonBuffer jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["gripper"] = gGripper;
    root["gripper-rotate"] = gGripperRotate;
    root["rotate"] = gRotate;
    root["shoulder"] = gShoulder;
    root["elbow"] = gElbow;
    root["wrist"] = gWrist;
    Position pos(gShoulder, gElbow, gWrist, gRotate, gGripperRotate);        
    root["claw-x"] = pos.getX();
    root["claw-y"] = pos.getY();
    root["claw-z"] = pos.getZ();        
    root["elbow-x"] = pos.shoulder.x + pos.elbow.x;
    root["elbow-y"] = pos.shoulder.y + pos.elbow.y;
    root["elbow-z"] = pos.shoulder.z + pos.elbow.z + BASE_HEIGHT;
    JsonArray &errors = root.createNestedArray("errors");
    gErrors.addToJsonArray(errors);
    root.printTo(*response);
    request->send(response);
    gErrors.clear();
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
        gGripper = tGripper;
        ledcWrite(GRIPPER_ENGINE, tDeg);
    }
}

void setGripperRotate(JsonObject& jsonObj) {
    const int tGripperRotate = getValue(jsonObj, "gripper_rotate", COUNT_LOW, COUNT_HIGH);    
    if (tGripperRotate >= 0 ) {
        const uint tDeg = degToCount(tGripperRotate, 270);
        gGripperRotate = tGripperRotate;
        ledcWrite(GRIPPER_ROTATE_ENGINE, tDeg);
    }
}

void setRotate(JsonObject& jsonObj) {
    const int tRotate = getValue(jsonObj, "rotate", COUNT_LOW, COUNT_HIGH);    
    if (tRotate >= 0 ) {
        const uint tDeg = degToCount(tRotate, ROTATE_Z_MAX);
        gRotate = tRotate;        
        ledcWrite(ROTATE_ENGINE, tDeg);
    }
}

unsigned int setEngine(const unsigned int engine, const unsigned int angle) {
    const uint tDeg = degToCount(angle, 270);        
    ledcWrite(engine, tDeg);        
    return angle;
}


void setShoulder(JsonObject& jsonObj) {
    const int tShoulder = getValue(jsonObj, "shoulder", COUNT_LOW, COUNT_HIGH);    
    if (tShoulder >= 0 )
        gShoulder = setEngine(SHOULDER_ENGINE, tShoulder);
}

void setElbow(JsonObject& jsonObj) {
    const int tElbow = getValue(jsonObj, "elbow", COUNT_LOW, COUNT_HIGH);    
    if (tElbow >= 0 )
        gElbow = setEngine(ELBOW_ENGINE, tElbow);
}

void setWrist(JsonObject& jsonObj) {
    const int tWrist = getValue(jsonObj, "wrist", COUNT_LOW, COUNT_HIGH);    
    if (tWrist >= 0 ) {
        const uint tDeg = degToCount(tWrist, 270);
        gWrist = tWrist;
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
    gErrors = strategy.errors;
    if (gErrors.size() > 0) return;
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
        if (control.engine == ROTATE_ENGINE) {
            gRotate = setEngine(ROTATE_ENGINE, control.angle);
            continue;
        }

    }    
}


AsyncCallbackJsonWebHandler* positionHandler = new AsyncCallbackJsonWebHandler("/position", [](AsyncWebServerRequest *request, JsonVariant &json) {    
    try {
        JsonObject& jsonObj = json.as<JsonObject>();        
        Position pos(gShoulder, gElbow, gWrist, gRotate, gGripperRotate);
        const double tx = getDoubleDef(jsonObj, "claw-x", -360, 360, pos.getX());    
        const double ty = getDoubleDef(jsonObj, "claw-y", -360, 360, pos.getY());    
        const double tz = getDoubleDef(jsonObj, "claw-z", 25, 440, pos.getZ());    
        const double cy = getDoubleDef(jsonObj, "claw-angle-y", WRIST_Y_MIN, WRIST_Y_MAX, NAN);    
        const double cx = getDoubleDef(jsonObj, "claw-angle-x", CLAW_X_MIN, CLAW_X_MAX, NAN);    
        Strategy moveStrategy(pos, tx, ty, tz, cx, cy);
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