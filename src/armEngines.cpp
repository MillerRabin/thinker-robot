#include <ArduinoJson.h>
#include "armParams.h"
#include "armEngines.h"
#include "strategy.h"
#include "armQueue.h"

//Start engines parameter

unsigned int ArmEngines::instancesCount;
volatile double ArmEngines::shoulderZAngle = SHOULDER_Z_PHYSICAL_ANGLE;
volatile double ArmEngines::shoulderYAngle = SHOULDER_Y_PHYSICAL_ANGLE;
volatile double ArmEngines::elbowYAngle = ELBOW_Y_PHYSICAL_ANGLE;
volatile double ArmEngines::wristYAngle = WRIST_Y_PHYSICAL_ANGLE;         
volatile double ArmEngines::clawXAngle = CLAW_X_PHYSICAL_ANGLE;
volatile double ArmEngines::clawAngle = CLAW_PHYSICAL_ANGLE;
TaskHandle_t ArmEngines::loopTask = NULL;
ArmQueue ArmEngines::queue = ArmQueue();

//-------ArmEngines---------

ArmEngines::ArmEngines() {
    ArmEngines::instancesCount++;
    if (ArmEngines::instancesCount > 1) 
        return;
    
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
    xTaskCreate(
        ArmEngines::loop,
        "ArmEnginesLoop",
        4096,
        NULL,
        tskIDLE_PRIORITY,
        &ArmEngines::loopTask);    
}

ArmEngines::~ArmEngines() {    
    ArmEngines::instancesCount--;    
    if (ArmEngines::instancesCount > 0)
        return;    
    vTaskDelete(ArmEngines::loopTask);
}

void ArmEngines::loop( void* param ) {    
    while (true) {
        ArmQueueItem* aq = ArmEngines::queue.dequeue();
        
        if (aq == NULL) {
            vTaskDelay(1000 / portTICK_RATE_MS);            
            continue;
        }

        const unsigned int iterations = aq->iterations;
        const unsigned int iterationDelay = aq->iterationDelay;
        const unsigned int postDelay = aq->postDelay;

        const double targetShoulderYAngle = aq->shoulderYAngle;    
        const double targetShoulderZAngle = aq->shoulderZAngle;
        const double targetElbowYAngle = aq->elbowYAngle;    
        const double targetWristYAngle = aq->wristYAngle;    
        const double targetClawXAngle = aq->clawXAngle;    
        const double targetClawAngle = aq->clawAngle;
        
        delete aq;

        const double sourceShoulderYAngle = ArmEngines::shoulderYAngle;
        const double sourceShoulderZAngle = ArmEngines::shoulderZAngle;
        const double sourceElbowYAngle = ArmEngines::elbowYAngle;
        const double sourceWristYAngle = ArmEngines::wristYAngle;;    
        const double sourceClawXAngle = ArmEngines::clawXAngle;;    
        const double sourceClawAngle = ArmEngines::clawAngle;;
                
        const double shoulderYAngleRange = targetShoulderYAngle - sourceShoulderYAngle;
        const double shoulderZAngleRange = targetShoulderZAngle - sourceShoulderZAngle;
        const double elbowYAngleRange = targetElbowYAngle - sourceElbowYAngle;
        const double wristYAngleRange = targetWristYAngle - sourceWristYAngle;
        const double clawXAngleRange = targetClawXAngle - sourceClawXAngle;
        const double clawAngleRange = targetClawAngle - sourceClawAngle;

        for (unsigned int counter = 1; counter <= iterations; counter++) {
            const double shoulderYAngleInc = shoulderYAngleRange / iterations * counter;
            const double shoulderZAngleInc = shoulderZAngleRange / iterations * counter;
            const double elbowYAngleInc = elbowYAngleRange / iterations * counter;
            const double wristYAngleInc = wristYAngleRange / iterations * counter;
            const double clawXAngleInc = clawXAngleRange / iterations * counter;
            const double clawAngleInc = clawAngleRange / iterations * counter;

            ArmEngines::shoulderYAngle = (counter <= iterations) ? sourceShoulderYAngle + shoulderYAngleInc : targetShoulderYAngle;    
            ArmEngines::shoulderZAngle = (counter <= iterations) ? sourceShoulderZAngle + shoulderZAngleInc : targetShoulderZAngle;
            ArmEngines::elbowYAngle = (counter <= iterations) ? sourceElbowYAngle + elbowYAngleInc : targetElbowYAngle;    
            ArmEngines::wristYAngle = (counter <= iterations) ? sourceWristYAngle + wristYAngleInc : targetWristYAngle;    
            ArmEngines::clawXAngle = (counter <= iterations) ? sourceClawXAngle + clawXAngleInc : targetClawXAngle;    
            ArmEngines::clawAngle = (counter <= iterations) ? sourceClawAngle + clawAngleInc : targetClawAngle;
                
            const unsigned int shoulderY = degToCount(ArmEngines::shoulderYAngle, 270);
            const unsigned int shoulderZ = degToCount(ArmEngines::shoulderZAngle, 270);
            const unsigned int elbowY = degToCount(ArmEngines::elbowYAngle, 270);
            const unsigned int wristY = degToCount(ArmEngines::wristYAngle, 270);
            const unsigned int clawX = degToCount(ArmEngines::clawXAngle, 270);
            const unsigned int claw = degToCount(ArmEngines::clawAngle, 180);
        
            ledcWrite(SHOULDER_Z_ENGINE, shoulderZ);
            ledcWrite(SHOULDER_Y_ENGINE, shoulderY);
            ledcWrite(ELBOW_Y_ENGINE, elbowY);
            ledcWrite(WRIST_Y_ENGINE,wristY);
            ledcWrite(CLAW_X_ENGINE, clawX);
            ledcWrite(CLAW_ENGINE, claw);
            vTaskDelay(iterationDelay / portTICK_RATE_MS);
        }                        
        vTaskDelay(postDelay / portTICK_RATE_MS);        
    }
}

Position ArmEngines::set(
        const double shoulderYAngle, 
        const double shoulderZAngle, 
        const double elbowYAngle, 
        const double wristYAngle, 
        const double clawXAngle, 
        const double clawAngle,
        const unsigned int iterations,
        const unsigned int postDelay,
        const unsigned int iterationDelay
    ) {    

    Position pos = Position(shoulderYAngle, shoulderZAngle, elbowYAngle, wristYAngle, clawXAngle, clawAngle);
    if (!pos.isValid()) return pos;
        
    ArmOperationResult res = ArmEngines::queue.enqueue(
            shoulderYAngle, 
            shoulderZAngle, 
            elbowYAngle, 
            wristYAngle, 
            clawXAngle, 
            clawAngle, 
            iterations,
            postDelay,
            iterationDelay
    );
    pos.setLastError(res, ArmError::getErrorText(res));
    return pos;
}

Position ArmEngines::set(JsonObject& jsonObj) {
    return set(        
        getDoubleDef(jsonObj, "shoulder-y", ArmEngines::shoulderYAngle),
        getDoubleDef(jsonObj, "shoulder-z", ArmEngines::shoulderZAngle),
        getDoubleDef(jsonObj, "elbow-y", ArmEngines::elbowYAngle),
        getDoubleDef(jsonObj, "wrist-y", ArmEngines::wristYAngle),
        getDoubleDef(jsonObj, "claw-x", ArmEngines::clawXAngle),
        getDoubleDef(jsonObj, "claw", ArmEngines::clawAngle),
        getDoubleDef(jsonObj, "iterations", 100),
        getDoubleDef(jsonObj, "post-delay", 1000),
        getDoubleDef(jsonObj, "iteration-delay", 50)    
    );
}

double ArmEngines::getDouble(JsonObject& jsonObj, const char* key) {
    if (!jsonObj.containsKey(key)) return NAN;
    const char* str = jsonObj[key];
    return atof(str);    
}

unsigned int ArmEngines::getUintDef(JsonObject& jsonObj, const char* key, unsigned int def) {
    if (!jsonObj.containsKey(key)) return def;
    const char* str = jsonObj[key];
    return atoi(str);    
}

double ArmEngines::getDoubleDef(JsonObject& jsonObj, const char* key, const double def) {
    if (!jsonObj.containsKey(key)) return def;
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

Position ArmEngines::applyStrategy(Strategy strategy) {    
    ArmOperationResult res = strategy.position.getLastError();    
    if (res != ARM_OPERATION_SUCCESS) return strategy.position;

    double shoulderYAngle = NAN;
    double shoulderZAngle = NAN;    
    double elbowYAngle = NAN;
    double wristYAngle = NAN;
    double clawXAngle = NAN;
    double clawAngle = NAN;

    for(EngineControl control : strategy.sequence) {    
        if (control.engine == SHOULDER_Y_ENGINE) {            
            shoulderYAngle = control.angle;
            continue;
        }
        if (control.engine == ELBOW_Y_ENGINE) {
            elbowYAngle = control.angle;
            continue;
        }
        if (control.engine == WRIST_Y_ENGINE) {
            wristYAngle = control.angle;
            continue;
        }
        if (control.engine == SHOULDER_Z_ENGINE) {
            shoulderZAngle = control.angle;
            continue;
        }
        if (control.engine == CLAW_X_ENGINE) {
            clawXAngle = control.angle;
            continue;
        }
        if (control.engine == CLAW_ENGINE) {
            clawAngle = control.angle;
            continue;
        }
    }
    return set(shoulderYAngle, shoulderZAngle, elbowYAngle, wristYAngle, clawXAngle, clawAngle, strategy.iterations, strategy.postDelay, strategy.iterationDelay);
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