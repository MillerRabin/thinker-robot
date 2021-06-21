#include <armParams.h>
#include <arduino.h>
#include <math.h>
#include <Position.h>
#include <arm.h>

Position::Position(const double shoulderAngle, const double elbowAngle, const double wristAngle, const double rotateAngle, const double clawAngle) :
    rotateAngle(rotateAngle),
    shoulderAngle(shoulderAngle),
    elbowAngle(elbowAngle),
    wristAngle(wristAngle),
    clawAngle(clawAngle),
    rotate(rotateAngle),    
    shoulder(rotate.XYRad, shoulderAngle),
    elbow(rotate.XYRad, shoulder.XZRad, elbowAngle),
    wrist(rotate.XYRad, shoulder.XZRad, elbow.XZRad, wristAngle),
    claw(rotate.XYRad, shoulder.XZRad, elbow.XZRad, wrist.XZRad, clawAngle)
{    

}

double Position::getX() {    
    return shoulder.x + elbow.x + wrist.x;
}

double Position::getY() {
    return shoulder.y + elbow.y + wrist.y;
}

double Position::getZ() {            
    return shoulder.z + elbow.z + wrist.z + TOP_OFFSET;
}