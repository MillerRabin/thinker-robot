#include <ArduinoJson.h>
#include "armParams.h"
#include "armEngines.h"
#include "Strategy.h"
#include "armQueue.h"

//Start engines parameter

unsigned int ArmEngines::instancesCount;
volatile double ArmEngines::shoulderZAngle = SHOULDER_Z_PHYSICAL_ANGLE;
volatile double ArmEngines::shoulderYAngle = SHOULDER_Y_PHYSICAL_ANGLE;
volatile double ArmEngines::elbowYAngle = ELBOW_Y_PHYSICAL_ANGLE;
volatile double ArmEngines::wristYAngle = WRIST_Y_PHYSICAL_ANGLE;         
volatile double ArmEngines::clawXAngle = CLAW_X_PHYSICAL_ANGLE;
volatile double ArmEngines::clawYAngle = CLAW_Y_PHYSICAL_ANGLE;
volatile double ArmEngines::clawZAngle = CLAW_Z_PHYSICAL_ANGLE;
volatile double ArmEngines::clawAngle = CLAW_PHYSICAL_ANGLE;
Position ArmEngines::lastPosition = Position();
TaskHandle_t ArmEngines::loopTask = NULL;
ArmQueue ArmEngines::queue = ArmQueue();

//-------ArmEngines---------

ArmEngines::ArmEngines() {
    ArmEngines::instancesCount++;
    if (ArmEngines::instancesCount > 1) 
        return;
    
    ledcSetup(CLAW_ENGINE, TIMER_FREQ, TIMER_WIDTH);
    ledcSetup(CLAW_X_ENGINE, TIMER_FREQ, TIMER_WIDTH);
    ledcSetup(CLAW_Y_ENGINE, TIMER_FREQ, TIMER_WIDTH);
    ledcSetup(CLAW_Z_ENGINE, TIMER_FREQ, TIMER_WIDTH);
    ledcSetup(SHOULDER_Z_ENGINE, TIMER_FREQ, TIMER_WIDTH);
    ledcSetup(SHOULDER_Y_ENGINE, TIMER_FREQ, TIMER_WIDTH);
    ledcSetup(ELBOW_Y_ENGINE, TIMER_FREQ, TIMER_WIDTH);
    ledcSetup(WRIST_Y_ENGINE, TIMER_FREQ, TIMER_WIDTH);
    ledcAttachPin(CLAW_PIN, CLAW_ENGINE);
    ledcAttachPin(CLAW_X_PIN, CLAW_X_ENGINE);
    ledcAttachPin(CLAW_Y_PIN, CLAW_Y_ENGINE);
    ledcAttachPin(CLAW_Z_PIN, CLAW_Z_ENGINE);
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

const unsigned int ArmEngines::getIterations(const double shoulderYAngleRange, const double shoulderZAngleRange, const double elbowYAngleRange, const double wristYAngleRange, const double clawXAngleRange, 
                                            const double clawYAngleRange, const double clawZAngleRange, const double clawAngleRange) {
    const double maxRange = getMaxRange(shoulderYAngleRange, shoulderZAngleRange, elbowYAngleRange, wristYAngleRange, clawXAngleRange, clawYAngleRange, clawZAngleRange, clawAngleRange);
    const unsigned int mi =  int(maxRange);
    return (mi == 0) ? 1 : mi;
}

const double ArmEngines::getMaxRange(
    const double shoulderYAngleRange, 
    const double shoulderZAngleRange, 
    const double elbowYAngleRange, 
    const double wristYAngleRange, 
    const double clawXAngleRange, 
    const double clawYAngleRange, 
    const double clawZAngleRange, 
    const double clawAngleRange
) {
    const double aShoulderYAngleRange = abs(shoulderYAngleRange);
    const double aShoulderZAngleRange = abs(shoulderZAngleRange);
    const double aElbowYAngleRange = abs(elbowYAngleRange);
    const double aWristYAngleRange = abs(wristYAngleRange);
    const double aClawXAngleRange = abs(clawXAngleRange);
    const double aClawYAngleRange = abs(clawYAngleRange);
    const double aClawZAngleRange = abs(clawZAngleRange);
    const double aClawAngleRange = abs(clawAngleRange);

    double max = aShoulderYAngleRange;
    max = (aShoulderZAngleRange > max) ? aShoulderZAngleRange : max;
    max = (aElbowYAngleRange > max) ? aElbowYAngleRange : max;
    max = (aWristYAngleRange > max) ? aWristYAngleRange : max;
    max = (aClawXAngleRange > max) ? aClawXAngleRange : max;
    max = (aClawYAngleRange > max) ? aClawYAngleRange : max;
    max = (aClawZAngleRange > max) ? aClawZAngleRange : max;
    max = (aClawAngleRange > max) ? aClawAngleRange : max;
    return max;
}


void ArmEngines::loop( void* param ) {    
    while (true) {
        ArmQueueItem* aq = ArmEngines::queue.dequeue();
        
        if (aq == NULL) {
            vTaskDelay(1000 / portTICK_RATE_MS);            
            continue;
        }

        const unsigned int speed = aq->speed;
        const unsigned int postDelay = aq->postDelay;

        const double targetShoulderYAngle = aq->shoulderYAngle;    
        const double targetShoulderZAngle = aq->shoulderZAngle;
        const double targetElbowYAngle = aq->elbowYAngle;    
        const double targetWristYAngle = aq->wristYAngle;    
        const double targetClawXAngle = aq->clawXAngle;    
        const double targetClawYAngle = aq->clawYAngle;    
        const double targetClawZAngle = aq->clawZAngle;    
        const double targetClawAngle = aq->clawAngle;
        
        delete aq;

        const double sourceShoulderYAngle = ArmEngines::shoulderYAngle;
        const double sourceShoulderZAngle = ArmEngines::shoulderZAngle;
        const double sourceElbowYAngle = ArmEngines::elbowYAngle;
        const double sourceWristYAngle = ArmEngines::wristYAngle;;    
        const double sourceClawXAngle = ArmEngines::clawXAngle;;    
        const double sourceClawYAngle = ArmEngines::clawYAngle;;    
        const double sourceClawZAngle = ArmEngines::clawZAngle;;    
        const double sourceClawAngle = ArmEngines::clawAngle;;
                
        const double shoulderYAngleRange = targetShoulderYAngle - sourceShoulderYAngle;
        const double shoulderZAngleRange = targetShoulderZAngle - sourceShoulderZAngle;
        const double elbowYAngleRange = targetElbowYAngle - sourceElbowYAngle;
        const double wristYAngleRange = targetWristYAngle - sourceWristYAngle;
        const double clawXAngleRange = targetClawXAngle - sourceClawXAngle;
        const double clawYAngleRange = targetClawYAngle - sourceClawYAngle;
        const double clawZAngleRange = targetClawZAngle - sourceClawZAngle;
        const double clawAngleRange = targetClawAngle - sourceClawAngle;

        const unsigned int iterations = getIterations(shoulderYAngleRange, shoulderZAngleRange, elbowYAngleRange, wristYAngleRange, clawXAngleRange, clawYAngleRange, clawZAngleRange, clawAngleRange) / speed;
        const unsigned int iterationDelay = DEFAULT_ITERATION_DELAY;
        
        for (unsigned int counter = 1; counter <= iterations; counter++) {
            const double shoulderYAngleInc = shoulderYAngleRange / iterations * counter;
            const double shoulderZAngleInc = shoulderZAngleRange / iterations * counter;
            const double elbowYAngleInc = elbowYAngleRange / iterations * counter;
            const double wristYAngleInc = wristYAngleRange / iterations * counter;
            const double clawXAngleInc = clawXAngleRange / iterations * counter;
            const double clawYAngleInc = clawYAngleRange / iterations * counter;
            const double clawZAngleInc = clawZAngleRange / iterations * counter;
            const double clawAngleInc = clawAngleRange / iterations * counter;

            ArmEngines::shoulderYAngle = (counter <= iterations) ? sourceShoulderYAngle + shoulderYAngleInc : targetShoulderYAngle;    
            ArmEngines::shoulderZAngle = (counter <= iterations) ? sourceShoulderZAngle + shoulderZAngleInc : targetShoulderZAngle;
            ArmEngines::elbowYAngle = (counter <= iterations) ? sourceElbowYAngle + elbowYAngleInc : targetElbowYAngle;    
            ArmEngines::wristYAngle = (counter <= iterations) ? sourceWristYAngle + wristYAngleInc : targetWristYAngle;    
            ArmEngines::clawXAngle = (counter <= iterations) ? sourceClawXAngle + clawXAngleInc : targetClawXAngle;    
            ArmEngines::clawYAngle = (counter <= iterations) ? sourceClawYAngle + clawYAngleInc : targetClawYAngle;    
            ArmEngines::clawZAngle = (counter <= iterations) ? sourceClawZAngle + clawZAngleInc : targetClawZAngle;    
            ArmEngines::clawAngle = (counter <= iterations) ? sourceClawAngle + clawAngleInc : targetClawAngle;
                
            const unsigned int shoulderY = degToCount(ArmEngines::shoulderYAngle, SHOULDER_Y_PHYSICAL_MAX, SHOULDER_Y_COUNT_LOW, SHOULDER_Y_COUNT_HIGH);
            const unsigned int shoulderZ = degToCount(ArmEngines::shoulderZAngle, 270);
            const unsigned int elbowY = degToCount(ArmEngines::elbowYAngle, 270, ELBOW_Y_COUNT_LOW, ELBOW_Y_COUNT_HIGH);
            const unsigned int wristY = degToCount(ArmEngines::wristYAngle, 270);
            const unsigned int clawX = degToCount(ArmEngines::clawXAngle, 270);
            const unsigned int clawY = degToCount(ArmEngines::clawYAngle, CLAW_Y_PHYSICAL_MAX, CLAW_Y_COUNT_LOW, CLAW_Y_COUNT_HIGH);
            const unsigned int clawZ = degToCount(ArmEngines::clawZAngle, 270);
            const unsigned int claw = degToCount(ArmEngines::clawAngle, 180);
        
            ledcWrite(SHOULDER_Z_ENGINE, shoulderZ);
            ledcWrite(SHOULDER_Y_ENGINE, shoulderY);
            ledcWrite(ELBOW_Y_ENGINE, elbowY);
            ledcWrite(WRIST_Y_ENGINE,wristY);
            ledcWrite(CLAW_X_ENGINE, clawX);
            ledcWrite(CLAW_Y_ENGINE, clawY);
            ledcWrite(CLAW_Z_ENGINE, clawZ);
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
        const double clawYAngle, 
        const double clawZAngle, 
        const double clawAngle,
        const double speed,
        const unsigned int postDelay        
    ) {    
    Position pos = Position(shoulderYAngle, shoulderZAngle, elbowYAngle, wristYAngle, clawXAngle, clawYAngle, clawZAngle, clawAngle);
    if (!pos.isValid()) return pos;
    ArmOperationResult res = ArmEngines::queue.enqueue(
            shoulderYAngle, 
            shoulderZAngle, 
            elbowYAngle, 
            wristYAngle, 
            clawXAngle, 
            clawYAngle, 
            clawZAngle, 
            clawAngle, 
            speed,
            postDelay            
    );
    pos.setLastError(res, ArmError::getErrorText(res));
    if (res == ARM_OPERATION_SUCCESS)
        ArmEngines::lastPosition = pos;
    return pos;
}

Position ArmEngines::set(
        Position pos,
        const double speed,
        const unsigned int postDelay        
    ) {    
    
    if (!pos.isValid()) return pos;    
    ArmOperationResult res = ArmEngines::queue.enqueue(
            pos.getShoulderYAngle(),
            pos.getShoulderZAngle(),        
            pos.getElbowYAngle(), 
            pos.getWristYAngle(), 
            pos.getClawXAngle(), 
            pos.getClawYAngle(), 
            pos.getClawZAngle(), 
            pos.getClawAngle(), 
            speed,
            postDelay
    );
    pos.setLastError(res, ArmError::getErrorText(res));
    if (res == ARM_OPERATION_SUCCESS)
        ArmEngines::lastPosition = pos;
    return pos;
}

Position ArmEngines::set(JsonObject& jsonObj) {
    Position pos = ArmEngines::getPosition();
    return set(
        getDoubleDef(jsonObj, "shoulder-y", pos.getShoulderYAngle()),
        getDoubleDef(jsonObj, "shoulder-z", pos.getShoulderZAngle()),
        getDoubleDef(jsonObj, "elbow-y", pos.getElbowYAngle()),
        getDoubleDef(jsonObj, "wrist-y", pos.getWristYAngle()),
        getDoubleDef(jsonObj, "claw-x", pos.getClawXAngle()),        
        getDoubleDef(jsonObj, "claw-y", pos.getClawYAngle()),        
        getDoubleDef(jsonObj, "claw-z", pos.getClawZAngle()),
        getDoubleDef(jsonObj, "claw", pos.getClawAngle()),
        getDoubleDef(jsonObj, "speed", DEFAULT_SPEED),
        getDoubleDef(jsonObj, "post-delay", DEFAULT_POST_DELAY)        
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

const uint ArmEngines::degToCount(const double value, const uint maxDeg, const uint impulseMin, const uint impulseMax) {
    const uint range = impulseMax - impulseMin;
    const double impulseLength = value / maxDeg * (range) + impulseMin;    
    const double maxImpulseLength = (1.0 / TIMER_FREQ) * 1000000;
    const double diff = impulseLength / maxImpulseLength;
    const unsigned count = 65535 * diff;
    return count;
}

Position ArmEngines::applyStrategy(Strategy strategy) {    
    ArmOperationResult res = strategy.endPosition.getLastError();    
    if (res != ARM_OPERATION_SUCCESS) return strategy.endPosition;
    return set(
        strategy.endPosition,
        strategy.speed, 
        strategy.postDelay        
    );
}


Position ArmEngines::getPosition() {
    if (ArmEngines::lastPosition.isValid())
        return ArmEngines::lastPosition;

    ArmEngines::lastPosition = Position(
        ArmEngines::shoulderYAngle, 
        ArmEngines::shoulderZAngle, 
        ArmEngines::elbowYAngle, 
        ArmEngines::wristYAngle, 
        ArmEngines::clawXAngle, 
        ArmEngines::clawZAngle, 
        ArmEngines::clawAngle
    );
    
    return ArmEngines::lastPosition;
}