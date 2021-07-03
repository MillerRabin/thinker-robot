#ifndef position_h
#define position_h

#include <armParams.h>
#include <position.h>
#include <arm.h>

class Position {
    public: 
        ArmRotate rotate;
        ArmShoulder shoulder;
        ArmElbow elbow;
        ArmWrist wrist;
        ArmClaw claw;        
        Position(const double shoulderAngle = NAN, const double elbowAngle = NAN, const double wristAngle = NAN, const double rotateAngle = NAN, const double clawAngle = NAN);
        Position(ArmRotate rotate, ArmShoulder shoulder, ArmElbow elbow, ArmWrist wrist, ArmClaw claw);
        bool isValid();
        const double getX();
        const double getY();
        const double getZ();
        const double getRotateAngle();
        const double getShoulderAngle();
        const double getElbowAngle();
        const double getWristAngle();
        const double getClawAngle();
};

#endif