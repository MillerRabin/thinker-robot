#ifndef position_h
#define position_h

#include <armParams.h>
#include <position.h>
#include <arm.h>

class Line {
    public:
        const double a;
        const double b;
        const double c;
        Line(const double x1, const double z1, const double x2, const double z2);
        const double getZByX(const double x);
};

class Position {
    public: 
        ArmRotate rotate;
        ArmShoulder shoulder;
        ArmElbow elbow;
        ArmWrist wrist;
        ArmClaw claw;        
        Position(const double shoulderAngle = NAN, const double elbowAngle = NAN, const double wristAngle = NAN, const double rotateAngle = NAN, const double clawAngle = NAN);
        Position(ArmRotate rotate, ArmShoulder shoulder, ArmElbow elbow, ArmWrist wrist, ArmClaw claw);        
        const bool isValid();
        const double getX();
        const double getY();
        const double getZ();
        const double getRotateAngle();
        const double getShoulderAngle();
        const double getElbowAngle();
        const double getWristAngle();
        const double getClawXAngle();
};

#endif