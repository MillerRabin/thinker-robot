#include <arduino.h>
#include <math.h>
#include "armParams.h"
#include "position.h"
#include "arm.h"
#include "armError.h"

Position::Position(const double shoulderYAngle, const double shoulderZAngle, const double elbowYAngle, const double wristYAngle, const double clawXAngle, const double clawZAngle, const double clawAngle) : 
    shoulder(shoulderYAngle, shoulderZAngle),
    elbow(shoulder, elbowYAngle),
    wrist(elbow, wristYAngle),
    claw(wrist, clawXAngle, clawZAngle, clawAngle)    
{
    isValid();
}

Position::Position(ArmShoulder shoulder, ArmElbow elbow, ArmWrist wrist, ArmClaw claw) :    
    shoulder(shoulder),
    elbow(elbow),
    wrist(wrist),
    claw(claw)
{    
    isValid();
}

const double Position::getX() {    
    return shoulder.x + elbow.x + wrist.x + claw.x;
}

const double Position::getY() {
    return shoulder.y + elbow.y + wrist.y + claw.y;
}

const double Position::getZ() {                
    return shoulder.z + elbow.z + wrist.z + claw.z + BASE_HEIGHT;
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

const double Position::getClawZAngle() {
    return claw.getZAngle(wrist);
}

const double Position::getClawAngle() {
    return claw.getAngle();
}

const bool Position::isValid() {        
    const double dzAngle = getShoulderZAngle();
    const double dsAngle = getShoulderYAngle();
    const double deAngle = getElbowYAngle();
    const double dwAngle = getWristYAngle();        
    const double cxAngle = getClawXAngle();        
    const double czAngle = getClawZAngle();        
    const double clawAngle = getClawAngle();    
    if (dzAngle < 0) {
        ArmOperationResult res = (ArmOperationResult)dzAngle;
        setLastError(res, ArmError::getErrorText(res));
        return false;
    }       
    if (dsAngle < 0) {
        ArmOperationResult res = (ArmOperationResult)dsAngle;
        setLastError(res, ArmError::getErrorText(res));
        return false;
    }    
    if (deAngle < 0) {
        ArmOperationResult res = (ArmOperationResult)deAngle;
        setLastError(res, ArmError::getErrorText(res));
        return false;
    }    
    if (dwAngle < 0) {
        ArmOperationResult res = (ArmOperationResult)dwAngle;
        setLastError(res, ArmError::getErrorText(res));
        return false;
    }        
    if (cxAngle < 0) {
        ArmOperationResult res = (ArmOperationResult)cxAngle;
        setLastError(res, ArmError::getErrorText(res));
        return false;
    }
    if (czAngle < 0) {
        ArmOperationResult res = (ArmOperationResult)czAngle;
        setLastError(res, ArmError::getErrorText(res));
        return false;
    }

    if (clawAngle < 0) {
        ArmOperationResult res = (ArmOperationResult)clawAngle;
        setLastError(res, ArmError::getErrorText(res));
        return false;
    }
        
    const double eRad = abs(elbow.getLocalRad(shoulder));
    const double wRad = abs(wrist.getLocalRad(elbow));
    const double sum = (eRad + wRad) / PI * 180;    
    if (sum > MAX_SUM_ANGLE) {                
        setLastError(ERROR_SUM_OF_ANGLES_ABOVE_MAX, ArmError::getErrorText(ERROR_SUM_OF_ANGLES_ABOVE_MAX));
        return false;    
    }
    setLastError(ARM_OPERATION_SUCCESS, "");
    return true;    
}