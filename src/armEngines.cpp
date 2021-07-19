#include <ArduinoJson.h>
#include "armParams.h"
#include "armEngines.h"
#include "strategy.h"

//Start engines parameter

double ArmEngines::shoulderZAngle = SHOULDER_Z_PHYSICAL_ANGLE;
double ArmEngines::shoulderYAngle = SHOULDER_Y_PHYSICAL_ANGLE;
double ArmEngines::elbowYAngle = ELBOW_Y_PHYSICAL_ANGLE;
double ArmEngines::wristYAngle = WRIST_Y_PHYSICAL_ANGLE;         
double ArmEngines::clawXAngle = CLAW_X_PHYSICAL_ANGLE;
double ArmEngines::clawAngle = CLAW_PHYSICAL_ANGLE;

ArmEngines::ArmEngines() {
    ledcSetup(CLAW_ENGINE, 50, TIMER_WIDTH);
    ledcSetup(CLAW_X_ENGINE, 50, TIMER_WIDTH);
    ledcSetup(SHOULDER_Z_ENGINE, 50, TIMER_WIDTH);
    ledcSetup(SHOULDER_Y_ENGINE, 50, TIMER_WIDTH);
    ledcSetup(ELBOW_Y_ENGINE, 50, TIMER_WIDTH);
    ledcSetup(WRIST_Y_ENGINE, 50, TIMER_WIDTH);
    ledcAttachPin(CLAW_PIN, CLAW_ENGINE);
    ledcAttachPin(CLAW_X_PIN, CLAW_X_ENGINE);
    ledcAttachPin(SHOULDER_Z_PIN, SHOULDER_Z_ENGINE);
    ledcAttachPin(SHOULDER_Y_PIN, SHOULDER_Y_ENGINE);
    ledcAttachPin(ELBOW_Y_PIN, ELBOW_Y_ENGINE);
    ledcAttachPin(WRIST_Y_PIN, WRIST_Y_ENGINE); 
}

void ArmEngines::set(const double shoulderYAngle, const double shoulderZAngle, const double elbowYAngle, const double wristYAngle, const double clawXAngle, const double clawAngle) {
    errors.clear();
    Position pos = Position(shoulderYAngle, shoulderZAngle, elbowYAngle, wristYAngle, clawXAngle, clawAngle);
    if (!pos.isValid()) return;        
    ArmEngines::shoulderYAngle = shoulderYAngle;
    const uint shoulderY = degToCount(shoulderYAngle, 270);
    ArmEngines::shoulderZAngle = shoulderZAngle;
    const uint shoulderZ = degToCount(shoulderZAngle, 270);
    ArmEngines::elbowYAngle = elbowYAngle;
    const uint elbowY = degToCount(elbowYAngle, 270);
    ArmEngines::wristYAngle =  wristYAngle;
    const uint wristY = degToCount(wristYAngle, 270);
    ArmEngines::clawXAngle =  clawXAngle;
    const uint clawX = degToCount(clawXAngle, 270);
    ArmEngines::clawAngle =  clawAngle;
    const uint claw = degToCount(clawAngle, 180);
    ledcWrite(SHOULDER_Z_ENGINE, shoulderZ);
    ledcWrite(SHOULDER_Y_ENGINE, shoulderY);
    ledcWrite(ELBOW_Y_ENGINE, elbowY);
    ledcWrite(WRIST_Y_ENGINE,wristY);
    ledcWrite(CLAW_X_ENGINE, clawX);
    ledcWrite(CLAW_ENGINE, claw);
}

double ArmEngines::getDouble(JsonObject& jsonObj, const char* key) {
    if (!jsonObj.containsKey(key)) return NAN;
    const char* str = jsonObj[key];
    return atof(str);    
}

 const double ArmEngines::setEngine(const uint engine, const double angle) {
    const uint tDeg = degToCount(angle, 270);        
    ledcWrite(engine, tDeg);        
    return angle;
}

const uint ArmEngines::degToCount(const double value, const uint maxDeg) {        
    const double vl = value / maxDeg * (COUNT_RANGE) + COUNT_LOW;
    return (uint)round(vl);
}

void ArmEngines::applyStrategy(Strategy strategy) {
    errors = strategy.errors;
    if (errors.size() > 0) return;
    for(EngineControl control : strategy.sequence) {    
        if (control.engine == SHOULDER_Y_ENGINE) {            
            ArmEngines::shoulderYAngle = setEngine(SHOULDER_Y_ENGINE, control.angle);
            continue;
        }
        if (control.engine == ELBOW_Y_ENGINE) {
            ArmEngines::elbowYAngle = setEngine(ELBOW_Y_ENGINE, control.angle);
            continue;
        }
        if (control.engine == WRIST_Y_ENGINE) {
            ArmEngines::wristYAngle = setEngine(WRIST_Y_ENGINE, control.angle);    
            continue;
        }
        if (control.engine == SHOULDER_Z_ENGINE) {
            ArmEngines::shoulderZAngle = setEngine(SHOULDER_Z_ENGINE, control.angle);
            continue;
        }
        if (control.engine == CLAW_X_ENGINE) {
            ArmEngines::clawXAngle = setEngine(CLAW_X_ENGINE, control.angle);
            continue;
        }
        if (control.engine == CLAW_ENGINE) {
            ArmEngines::clawAngle = setEngine(CLAW_ENGINE, control.angle);
            continue;
        }
    }
}

Position ArmEngines::getPosition() {
    return Position(
        ArmEngines::shoulderYAngle, 
        ArmEngines::shoulderZAngle, 
        ArmEngines::elbowYAngle, 
        ArmEngines::wristYAngle, 
        ArmEngines::clawXAngle, 
        ArmEngines::clawAngle
    );
}

void ArmEngines::set(JsonObject& jsonObj) {
    set(
        getDouble(jsonObj, "shoulder-z"),
        getDouble(jsonObj, "shoulder-y"),
        getDouble(jsonObj, "elbow-y"),
        getDouble(jsonObj, "wrist-y"),
        getDouble(jsonObj, "claw-x"),
        getDouble(jsonObj, "claw")    
    );
}