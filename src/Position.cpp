#include <armParams.h>
#include <arduino.h>
#include <math.h>
#include <position.h>
#include <arm.h>

Position::Position(const double shoulderAngle, const double elbowAngle, const double wristAngle, const double rotateAngle, const double clawAngle) :
    rotate(rotateAngle),    
    shoulder(rotate, shoulderAngle),
    elbow(shoulder, elbowAngle),
    wrist(elbow, wristAngle),
    claw(wrist, clawAngle)
{    

}

Position::Position(ArmRotate rotate, ArmShoulder shoulder, ArmElbow elbow, ArmWrist wrist, ArmClaw claw) :
    rotate(rotate),
    shoulder(shoulder),
    elbow(elbow),
    wrist(wrist),
    claw(claw)
{    

}

const double Position::getX() {    
    return shoulder.x + elbow.x + wrist.x;
}

const double Position::getY() {
    return shoulder.y + elbow.y + wrist.y;
}

const double Position::getZ() {                
    return shoulder.z + elbow.z + wrist.z + BASE_HEIGHT;
}

const double Position::getRotateAngle() {
    return rotate.getAngle();
}

const double Position::getShoulderAngle() {
    return shoulder.getAngle();
}

const double Position::getElbowAngle() {
    return elbow.getAngle(shoulder);
}

const double Position::getWristAngle() {
    return wrist.getAngle(elbow);
}

const double Position::getClawAngle() {
    return claw.getAngle();
}

const bool Position::isValid() {        
    const double dsAngle = getShoulderAngle();
    const double deAngle = getElbowAngle();
    const double dwAngle = getWristAngle();        
    if ((dsAngle < 0) || (deAngle < 0) || (dwAngle < 0)) {        
        return false;
    }    
    const double eRad = abs(elbow.getLocalRad(shoulder));
    const double wRad = abs(wrist.getLocalRad(elbow));
    const double sum = (eRad + wRad) / PI * 180;    
    if (sum > MAX_SUM_ANGLE) {                
        return false;    
    }
    return true;    
}