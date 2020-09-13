#include <AsyncJson.h>
#include <ArduinoJson.h>
#include <ESP32_Servo.h>
#include "initServer.h"
#include <math.h>


#define COUNT_LOW 1638
#define COUNT_HIGH 7864
#define COUNT_RANGE COUNT_HIGH - COUNT_LOW
#define TIMER_WIDTH 16

#define GRIPPER_PIN 25
#define GRIPPER_ROTATE_PIN 26
#define ROTATE_PIN 27
#define SHOULDER_PIN 14
#define WRIST_PIN 12
#define ELBOW_PIN 13

#define GRIPPER_CHANNEL 1
#define GRIPPER_ROTATE_CHANNEL 2
#define ROTATE_CHANNEL 3
#define SHOULDER_CHANNEL 4
#define ELBOW_CHANNEL 5
#define WRIST_CHANNEL 6

#define GRIPPER_MAX_ANGLE 4000

#define SHOULDER_LENGTH 105
#define ELBOW_LENGTH 95
#define WRIST_LENGTH 150

#define SHOULDER_OFFSET 0
#define ELBOW_OFFSET 35
#define WRIST_OFFSET 250
#define ROTATE_OFFSET -45
#define GRIPPER_ROTATE_OFFSET -45


#define TOP_OFFSET 80

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

uint countToDeg(const uint value, const uint maxDeg) {
    const float vl = float(value - COUNT_LOW) / (COUNT_RANGE) * maxDeg;    
    return (uint)vl;
}

uint degToCount(const uint value, const uint maxDeg) {        
    const float vl = float(value) / maxDeg * (COUNT_RANGE) + COUNT_LOW;
    return (uint)vl;
}

double getX(const double sAngle, const double eAngle, const double wAngle, const double rAngle) {
    const double sRad = (sAngle + SHOULDER_OFFSET) * PI / 180.0;
    const double l1 = SHOULDER_LENGTH * cos(sRad);            
    Serial.printf("shoulder %f - %f\n", sAngle, l1);
    const double eRad = (eAngle + sAngle + ELBOW_OFFSET) * PI / 180.0;
    const double l2 = ELBOW_LENGTH * cos(eRad);        
    Serial.printf("elbow %f - %f\n", eAngle, l2);
    const double wRad = (wAngle + sAngle + eAngle + WRIST_OFFSET) * PI / 180.0;
    const double l3 = WRIST_LENGTH * cos(wRad);        
    Serial.printf("wrist %f - %f\n", wAngle, l3);
    const double rRad = (rAngle + ROTATE_OFFSET) * PI / 180.0;    
    return (l1 + l2 + l3) * sin(rRad);
}

double getGripperAngleZ(const double grAngle, const double rAngle) {
    return (grAngle + GRIPPER_ROTATE_OFFSET - 90) - (90 - rAngle - ROTATE_OFFSET);
}

double getGripperAngleXY(const double sAngle, const double eAngle, const double wAngle) {    
    const double a1 = (-1 * sAngle + SHOULDER_OFFSET);
    //Serial.printf("shoulder %f - %f\n", sAngle, a1);
    const double a2 = 180 - (eAngle + ELBOW_OFFSET);
    //Serial.printf("elbow %f - %f\n", eAngle, a2);
    const double a3 = wAngle - (WRIST_OFFSET - 95);
    //Serial.printf("wrist %f - %f\n", wAngle, a3);
    return a1 - a2 + a3;
}


double getY(const double sAngle, const double eAngle, const double wAngle) {
    const double sRad = (sAngle + SHOULDER_OFFSET) * PI / 180.0;
    const double l1 = SHOULDER_LENGTH * sin(sRad);            
    const double eRad = (eAngle + sAngle + ELBOW_OFFSET + 180) * PI / 180.0;
    const double l2 = ELBOW_LENGTH * sin(eRad);    
    const double wRad = (wAngle + sAngle + eAngle + WRIST_OFFSET + 180) * PI / 180.0;
    const double l3 = WRIST_LENGTH * sin(wRad);    
    return l1 + l2 + l3 + TOP_OFFSET;
}

double getZ(const double sAngle, const double eAngle, const double wAngle, const double rAngle) {
    const double sRad = (sAngle + SHOULDER_OFFSET) * PI / 180.0;
    const double l1 = SHOULDER_LENGTH * cos(sRad);            
    const double eRad = (eAngle + sAngle + ELBOW_OFFSET) * PI / 180.0;
    const double l2 = ELBOW_LENGTH * cos(eRad);    
    const double wRad = (wAngle + sAngle + eAngle + WRIST_OFFSET) * PI / 180.0;
    const double l3 = WRIST_LENGTH * cos(wRad);    
    const double rRad = (rAngle + ROTATE_OFFSET) * PI / 180.0;    
    return (l1 + l2 + l3) * cos(rRad);
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
    root["length-x"] = getX(gShoulder, gElbow, gWrist, gRotate);
    root["length-y"] = getY(gShoulder, gElbow, gWrist);
    root["length-z"] = getZ(gShoulder, gElbow, gWrist, gRotate);
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
        ledcWrite(GRIPPER_CHANNEL, tDeg);
    }
}

void setGripperRotate(JsonObject& jsonObj) {
    const int tGripper = getValue(jsonObj, "gripper_rotate", COUNT_LOW, COUNT_HIGH);    
    if (tGripper >= 0 ) {
        const uint tDeg = degToCount(tGripper, 270);
        gGripperRotate = (unsigned int) tGripper;
        ledcWrite(GRIPPER_ROTATE_CHANNEL, tDeg);
    }
}

void setRotate(JsonObject& jsonObj) {
    const int tGripper = getValue(jsonObj, "rotate", COUNT_LOW, COUNT_HIGH);    
    if (tGripper >= 0 ) {
        const uint tDeg = degToCount(tGripper, 270);
        gRotate = (unsigned int) tGripper;        
        ledcWrite(ROTATE_CHANNEL, tDeg);
    }
}

void setShoulder(JsonObject& jsonObj) {
    const int tGripper = getValue(jsonObj, "shoulder", COUNT_LOW, COUNT_HIGH);    
    if (tGripper >= 0 ) {
        const uint tDeg = degToCount(tGripper, 270);
        gShoulder = (unsigned int) tGripper;
        ledcWrite(SHOULDER_CHANNEL, tDeg);
    }
}

void setElbow(JsonObject& jsonObj) {
    const int tGripper = getValue(jsonObj, "elbow", COUNT_LOW, COUNT_HIGH);    
    if (tGripper >= 0 ) {
        const uint tDeg = degToCount(tGripper, 270);
        gElbow = (unsigned int) tGripper;
        ledcWrite(ELBOW_CHANNEL, tDeg);
    }
}

void setWrist(JsonObject& jsonObj) {
    const int tGripper = getValue(jsonObj, "wrist", COUNT_LOW, COUNT_HIGH);    
    if (tGripper >= 0 ) {
        const uint tDeg = degToCount(tGripper, 270);
        gWrist = (unsigned int) tGripper;
        ledcWrite(WRIST_CHANNEL, tDeg);
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

void enableEngines() {    
    ledcSetup(GRIPPER_CHANNEL, 50, TIMER_WIDTH);
    ledcSetup(GRIPPER_ROTATE_CHANNEL, 50, TIMER_WIDTH);
    ledcSetup(ROTATE_CHANNEL, 50, TIMER_WIDTH);
    ledcSetup(SHOULDER_CHANNEL, 50, TIMER_WIDTH);
    ledcSetup(ELBOW_CHANNEL, 50, TIMER_WIDTH);
    ledcSetup(WRIST_CHANNEL, 50, TIMER_WIDTH);
    ledcAttachPin(GRIPPER_PIN, GRIPPER_CHANNEL);
    ledcAttachPin(GRIPPER_ROTATE_PIN, GRIPPER_ROTATE_CHANNEL);
    ledcAttachPin(ROTATE_PIN, ROTATE_CHANNEL);
    ledcAttachPin(SHOULDER_PIN, SHOULDER_CHANNEL);
    ledcAttachPin(ELBOW_PIN, ELBOW_CHANNEL);
    ledcAttachPin(WRIST_PIN, WRIST_CHANNEL);
    Server.addHandler(engineHandler);    
}