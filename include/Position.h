#ifndef position_h
#define position_h

#include <armParams.h>
#include <Position.h>
#include <arm.h>


class Position {
    public: 
        const unsigned int rotateAngle;
        const unsigned int shoulderAngle;
        const unsigned int elbowAngle;
        const unsigned int wristAngle;
        const unsigned int clawAngle;
        ArmRotate rotate;
        ArmShoulder shoulder;
        ArmElbow elbow;
        ArmWrist wrist;
        ArmClaw claw;        
        Position(const double shoulderAngle, const double elbowAngle, const double wristAngle, const double rotateAngle, const double clawAngle);
        double getX();
        double getY();
        double getZ();
        
};

#endif