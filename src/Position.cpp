#include <Arduino.h>
#include <math.h>
#include "armParams.h"
#include "Position.h"
#include "armError.h"

Position::Position(const float shoulderYAngle, const float shoulderZAngle, const float elbowYAngle, const float wristYAngle, const float clawXAngle, 
                   const float clawYAngle, const float clawZAngle, const float clawAngle) : 
    shoulder(shoulderYAngle, shoulderZAngle),
    elbow(shoulder, elbowYAngle),
    wrist(elbow, wristYAngle),
    claw(wrist, clawXAngle, clawYAngle, clawZAngle, clawAngle)    
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

const float Position::getX() {    
    return shoulder.x + elbow.x + wrist.x + claw.x;
}

const float Position::getY() {
    return shoulder.y + elbow.y + wrist.y + claw.y;
}

const float Position::getZ() {                
    return shoulder.z + elbow.z + wrist.z + claw.z + BASE_HEIGHT;
}

const bool Position::isValid() {        
    const float dzAngle = this->shoulder.getZAngle();
    const float dsAngle = this->shoulder.getYAngle();
    const float deAngle = this->elbow.getYAngle();
    const float dwAngle = this->wrist.getYAngle();        
    const float cxAngle = this->claw.getXAngle();        
    const float cyAngle = this->claw.getYAngle();        
    const float czAngle = this->claw.getZAngle();        
    const float clawAngle = this->claw.getAngle();    
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
        
    /*const float eRad = abs(elbow.getLocalRad(shoulder));    
    const float wRad = abs(wrist.getLocalRad(elbow));
    const float sum = (eRad + wRad) / PI * 180;    
    if (sum > MAX_SUM_ANGLE) {                
        setLastError(ERROR_SUM_OF_ANGLES_ABOVE_MAX, ArmError::getSumError(sum));
        return false;    
    }*/
    setLastError(ARM_OPERATION_SUCCESS, "");
    return true;    
}