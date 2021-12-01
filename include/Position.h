#ifndef position_h
#define position_h

#include <armParams.h>
#include <Position.h>
#include <arm.h>

class Position {
    private:
        ArmOperationResult lastError;
        std::string lastErrorText;
    public:         
        ArmShoulder shoulder;
        ArmElbow elbow;
        ArmWrist wrist;
        ArmClaw claw;        
        Position(
            const double shoulderYAngle = NAN, 
            const double shoulderZAngle = NAN, 
            const double elbowYAngle = NAN, 
            const double wristYAngle = NAN, 
            const double clawXAngle = NAN, 
            const double clawZAngle = NAN, 
            const double clawAngle = NAN
        );
        Position(ArmShoulder shoulder, ArmElbow elbow, ArmWrist wrist, ArmClaw claw);        
        const bool isValid();
        const double getX();
        const double getY();
        const double getZ();
        const double getShoulderZAngle();
        const double getShoulderYAngle();
        const double getElbowYAngle();
        const double getWristYAngle();
        const double getClawXAngle();
        const double getClawZAngle();
        const double getClawAngle();
        ArmOperationResult getLastError() { return lastError; };        
        std::string getLastErrorText() { return lastErrorText; };
        void setLastError(ArmOperationResult error, std::string text) { lastError = error; lastErrorText = text; };
};

#endif