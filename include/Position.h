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
        ArmRotate rotate;
        ArmShoulder shoulder;
        ArmElbow elbow;
        ArmWrist wrist;        
        Position(const unsigned int sAngle, const unsigned int eAngle, const unsigned int wAngle, const double rAngle);
        double getX();
        double getY();
        double getZ();
        
};

#endif