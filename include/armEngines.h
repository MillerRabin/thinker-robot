#ifndef arm_engines_h
#define arm_engines_h

#define COUNT_LOW 1638
#define COUNT_HIGH 7864
#define COUNT_RANGE (COUNT_HIGH - COUNT_LOW)
#define TIMER_WIDTH 16

#include "armError.h"
#include "armParams.h"
#include "strategy.h"
#include "position.h"
#include "strategy.h"
#include "armQueue.h"

//------ArmEngines------

class ArmEngines {
    private:              
        static unsigned int instancesCount;
        static volatile double shoulderZAngle;
        static volatile double shoulderYAngle;
        static volatile double elbowYAngle;
        static volatile double wristYAngle;         
        static volatile double clawXAngle;
        static volatile double clawAngle;  
        static const double setEngine(const uint engine, const double angle);        
        static const uint degToCount(const double value, const uint maxDeg);
        static TaskHandle_t loopTask;
        static void loop( void* param );     
    public:    
        ArmEngines();        
        ~ArmEngines();
        Position set(const double shoulderYAngle, const double shoulderZAngle, const double elbowYAngle, const double wristYAngle, const double clawXAngle, const double clawAngle);
        Position set(JsonObject& jsonObj);
        Position applyStrategy(Strategy strategy);
        static double getDouble(JsonObject& jsonObj, const char* key);
        static double getDoubleDef(JsonObject& jsonObj, const char* key, const double def);        
        const double getShoulderZAngle() { return ArmEngines::shoulderZAngle; };
        const double getShoulderYAngle() { return ArmEngines::shoulderYAngle; };
        const double getElbowYAngle() { return ArmEngines::elbowYAngle; };
        const double getWristYAngle() { return ArmEngines::wristYAngle; };
        const double getClawXAngle() { return ArmEngines::clawXAngle; };
        const double getClawAngle() { return ArmEngines::clawAngle; };
        Position getPosition();
        static ArmQueue queue;        
};

#endif