#include "armError.h"
#include "string.h"
#include <ArduinoJson.h>
#include "Position.h"
#include "armParams.h"

std::string ArmError::getBaseError(const double x, const double y, const double z) {
    char sx[25]; 
    dtostrf(x, 1, 3, sx);
    char sy[25]; 
    dtostrf(y, 1, 3, sy);
    char sz[25]; 
    dtostrf(z, 1, 3, sz);
    return std::string("The Arm can`t move through it`s base. x: ") + sx + std::string(", y: ") + sy + std::string(", z: ") + sz;            
}

std::string ArmError::getMaxLengthError(const double length, const double maxLength) {
    char ss[25]; 
    dtostrf(length, 1, 3, ss);
    char ms[25]; 
    dtostrf(maxLength, 1, 3, ms);
    return std::string("length: ") + ss + std::string(" is out of range. Max length is: ") + ms;        
    
}

std::string ArmError::getUnreachableError() {    
    return "EUNREACHABLE: The point is unreachable"; 
    
}

std::string ArmError::getShoulderError() {    
    return "EUNREACHABLE: The shoulder angle is not found to achieve specific point";     
}

std::string ArmError::getElbowZError(const double z, const double minZ) {    
    char mz[25]; 
    dtostrf(minZ, 1, 3, mz);    
    char sz[25]; 
    dtostrf(z, 1, 3, sz);
    return std::string("The elbow z: ") + sz + std::string(" is below minimum: ") + mz;            
}

std::string ArmError::getSumError(const double sum) {    
    char mz[25]; 
    dtostrf(sum, 1, 3, mz);    
    char sz[25]; 
    dtostrf(MAX_SUM_ANGLE, 1, 3, sz);
    return std::string("the sum of angles: ") + mz + std::string(" is above maximum: ") + sz;            
}

std::string ArmError::getErrorText(ArmOperationResult res) {
    switch(res) {
        case ARM_OPERATION_SUCCESS:
            return "Success";
        case ERROR_SHOULDER_Y_ANGLE_IS_NAN:
            return "Shoulder Y angle is NAN";
        case ERROR_SHOULDER_Y_ANGLE_LESS_MIN:
            return "Shoulder Y angle less minimum";
        case ERROR_SHOULDER_Y_ANGLE_ABOVE_MAX:
            return "Shoulder Y angle above maximum";
        case ERROR_SHOULDER_Z_ANGLE_IS_NAN:
            return "Shoulder Z angle is NAN";
        case ERROR_SHOULDER_Z_ANGLE_LESS_MIN:
            return "Shoulder Z angle less minimum";
        case ERROR_SHOULDER_Z_ANGLE_ABOVE_MAX:
            return "Shoulder Z angle above maximum";
        case ERROR_ELBOW_Y_ANGLE_IS_NAN:
            return "Elbow Y angle is NAN";
        case ERROR_ELBOW_Y_ANGLE_LESS_MIN:
            return "Elbow Y angle less minimum";
        case ERROR_ELBOW_Y_ANGLE_ABOVE_MAX:
            return "Elbow Y angle above maximum";
        case ERROR_WRIST_Y_ANGLE_IS_NAN:
            return "Wrist Y angle is NAN";
        case ERROR_WRIST_Y_ANGLE_LESS_MIN:
            return "Wrist Y angle less minimum";
        case ERROR_WRIST_Y_ANGLE_ABOVE_MAX:
            return "Wrist Y angle above maximum";
        case ERROR_CLAW_X_ANGLE_IS_NAN:
            return "Claw X angle is NAN";
        case ERROR_CLAW_X_ANGLE_LESS_MIN:
            return "Claw X angle less minimum";
        case ERROR_CLAW_X_ANGLE_ABOVE_MAX:
            return "Claw X angle above maximum";
        case ERROR_CLAW_ANGLE_IS_NAN:
            return "Claw angle is NAN";
        case ERROR_CLAW_ANGLE_LESS_MIN:
            return "Claw angle less minimum";
        case ERROR_CLAW_ANGLE_ABOVE_MAX:
            return "Claw angle above maximum";
        case ERROR_OUT_OF_RANGE:
            return "The parameters is out of range";
        case ERROR_POINT_UNREACHABLE:
            return "The point is unreachable";
        case ERROR_SUM_OF_ANGLES_ABOVE_MAX:
            return "the sum of angles is above maximum";
        case ERROR_COMMAND_QUEUE_IS_FULL:
            return "command queue is full";
        case ERROR_COMMAND_QUEUE_ITEM_IS_INVALID:
            return "Command in queue is invalid";
        default:
            char sz[25]; 
            itoa(res, sz, 10);
            return std::string("Unknown error: ") + sz;
    }
    
}