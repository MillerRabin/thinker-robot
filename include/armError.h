#ifndef arm_error_h
#define arm_error_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include "armParams.h"

class ArmError {
    public:    
        static std::string getBaseError(const double x, const double y, const double z);
        static std::string getMaxLengthError(const double length, const double maxLength);
        static std::string getShoulderError();
        static std::string getUnreachableError();
        static std::string getElbowZError(const double z, const double minZ);        
        static std::string getErrorText(ArmOperationResult res);        
};



#endif