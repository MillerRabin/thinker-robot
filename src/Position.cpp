#include <armParams.h>
#include <arduino.h>
#include <math.h>
#include <position.h>
#include <arm.h>

Position::Position(const double shoulderYAngle, const double shoulderZAngle, const double elbowYAngle, const double wristYAngle, const double clawXAngle, const double clawAngle) : 
    shoulder(shoulderYAngle, shoulderZAngle),
    elbow(shoulder, elbowYAngle),
    wrist(elbow, wristYAngle),
    claw(wrist, clawXAngle, clawAngle)
{
    Serial.printf("shoulderYAngle: %f, shoulderZAngle: %f, elbowYAngle: %f, wristYAngle: %f, clawXAngle: %f, clawAngle: %f\n", shoulderYAngle, shoulderZAngle, elbowYAngle, wristYAngle, clawXAngle, clawAngle);  
    Serial.printf("shoulder.x: %f, shoulder.y: %f, shoulder.z: %f, YRad: %f, ZRad: %f\n", shoulder.x, shoulder.y, shoulder.z, shoulder.YRad, shoulder.ZRad);  
    Serial.printf("elbow.x: %f, elbow.y: %f, elbow.z: %f, YRad: %f, ZRad: %f\n", elbow.x, elbow.y, elbow.z, elbow.YRad, elbow.ZRad);  
    Serial.printf("wrist.x: %f, wrist.y: %f, wrist.z: %f, YRad: %f, ZRad: %f\n", wrist.x, wrist.y, wrist.z, wrist.YRad, wrist.ZRad);  
}

Position::Position(ArmShoulder shoulder, ArmElbow elbow, ArmWrist wrist, ArmClaw claw) :    
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

const double Position::getShoulderZAngle() {
    return shoulder.getZAngle();
}

const double Position::getShoulderYAngle() {
    return shoulder.getYAngle();
}

const double Position::getElbowYAngle() {
    return elbow.getYAngle(shoulder);
}

const double Position::getWristYAngle() {
    return wrist.getYAngle(elbow);
}

const double Position::getClawXAngle() {
    return claw.getXAngle();
}

const double Position::getClawAngle() {
    return claw.getAngle();
}

const bool Position::isValid() {        
    const double dsAngle = getShoulderYAngle();
    const double deAngle = getElbowYAngle();
    const double dwAngle = getWristYAngle();        
    const double cxAngle = getClawXAngle();        
    const double clawAngle = getClawAngle();        
    if ((dsAngle < 0) || (deAngle < 0) || (dwAngle < 0) || (cxAngle < 0 || (clawAngle < 0))) {        
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