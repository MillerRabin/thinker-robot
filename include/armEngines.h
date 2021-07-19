#ifndef arm_engines_h
#define arm_engines_h

#define COUNT_LOW 1638
#define COUNT_HIGH 7864
#define COUNT_RANGE COUNT_HIGH - COUNT_LOW
#define TIMER_WIDTH 16

#include "armError.h"
#include "armParams.h"
#include "strategy.h"
#include "position.h"
#include "strategy.h"

//------ArmEngines------

class ArmEngines {
    private:              
        static double shoulderZAngle;
        static double shoulderYAngle;
        static double elbowYAngle;
        static double wristYAngle;         
        static double clawXAngle;
        static double clawAngle;  
        static const double setEngine(const uint engine, const double angle);        
        static const uint degToCount(const double value, const uint maxDeg);        
    public:    
        ArmEngines();        
        void set(const double shoulderYAngle, const double shoulderZAngle, const double elbowYAngle, const double wristYAngle, const double clawXAngle, const double clawAngle);
        void set(JsonObject& jsonObj);
        void applyStrategy(Strategy strategy);
        static double getDouble(JsonObject& jsonObj, const char* key);
        ArmError errors;
        const double getShoulderZAngle() { return ArmEngines::shoulderZAngle; };
        const double getShoulderYAngle() { return ArmEngines::shoulderYAngle; };
        const double getElbowYAngle() { return ArmEngines::elbowYAngle; };
        const double getWristYAngle() { return ArmEngines::wristYAngle; };
        const double getClawXAngle() { return ArmEngines::clawXAngle; };
        const double getClawAngle() { return ArmEngines::clawAngle; };
        Position getPosition();
};

#endif