#include <Arduino.h>
#include <math.h>
#include "armParams.h"
#include "Position.h"
#include "armError.h"

Position::Position(const double shoulderYAngle, const double shoulderZAngle, const double elbowYAngle, const double wristYAngle, const double clawXAngle, 
                   const double clawYAngle, const double clawZAngle, const double clawAngle) : 
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

const double Position::getX() {    
    return shoulder.x + elbow.x + wrist.x + claw.x;
}

const double Position::getY() {
    return shoulder.y + elbow.y + wrist.y + claw.y;
}

const double Position::getZ() {                
    return shoulder.z + elbow.z + wrist.z + claw.z;
}

const bool Position::isValid() {        
    const double dzAngle = this->shoulder.getZAngle();
    const double dsAngle = this->shoulder.getYAngle();
    const double deAngle = this->elbow.getYAngle();
    const double dwAngle = this->wrist.getYAngle();        
    const double cxAngle = this->claw.getXAngle();        
    const double cyAngle = this->claw.getYAngle();        
    const double czAngle = this->claw.getZAngle();        
    const double clawAngle = this->claw.getAngle();    
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
        
    /*const double eRad = abs(elbow.getLocalRad(shoulder));    
    const double wRad = abs(wrist.getLocalRad(elbow));
    const double sum = (eRad + wRad) / PI * 180;    
    if (sum > MAX_SUM_ANGLE) {                
        setLastError(ERROR_SUM_OF_ANGLES_ABOVE_MAX, ArmError::getSumError(sum));
        return false;    
    }*/
    setLastError(ARM_OPERATION_SUCCESS, "");
    return true;    
}