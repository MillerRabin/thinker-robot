#include <armParams.h>
#include <arduino.h>
#include <math.h>
#include <Position.h>
#include <arm.h>

Position::Position(const unsigned int sAngle, const unsigned int eAngle, const unsigned int wAngle, const double rAngle) :
    rotateAngle(rAngle),
    shoulderAngle(sAngle),
    elbowAngle(eAngle),
    wristAngle(wAngle),
    rotate(rAngle),
    shoulder(rotate.XYRad, shoulderAngle),
    elbow(rotate.XYRad, shoulder.XZRad, elbowAngle),
    wrist(rotate.XYRad, shoulder.XZRad, elbow.XZRad, wristAngle)
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