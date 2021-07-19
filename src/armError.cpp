#include "armError.h"
#include "string.h"
#include <ArduinoJson.h>
#include "position.h"

void ArmError::addBaseError(const double x, const double y, const double z) {
    char sx[25]; 
    dtostrf(x, 1, 3, sx);
    char sy[25]; 
    dtostrf(y, 1, 3, sy);
    char sz[25]; 
    dtostrf(z, 1, 3, sz);
    std::string rString = std::string("EOUTOFRANGE: The Arm can`t move through it`s base. x: ") + sx + std::string(", y: ") + sy + std::string(", z: ") + sz;        
    errors.push_back(rString);
}

void ArmError::addMaxLengthError(const double length, const double maxLength) {
    char ss[25]; 
    dtostrf(length, 1, 3, ss);
    char ms[25]; 
    dtostrf(maxLength, 1, 3, ms);
    std::string rString = std::string("EOUTOFRANGE: length: ") + ss + std::string(" is out of range. Max length is: ") + ms;        
    errors.push_back(rString);
}

void ArmError::addUnreachableError() {    
    std::string rString = "EUNREACHABLE: The point is unreachable"; 
    errors.push_back(rString);
}

void ArmError::addShoulderError() {    
    std::string rString = "EUNREACHABLE: The shoulder angle is not found to achieve specific point"; 
    errors.push_back(rString);
}

void ArmError::addElbowZError(const double z, const double minZ) {    
    char mz[25]; 
    dtostrf(minZ, 1, 3, mz);    
    char sz[25]; 
    dtostrf(z, 1, 3, sz);
    std::string rString = std::string("EOUTOFRANGE: The elbow z: ") + sz + std::string(" is below minimum: ") + mz;        
    errors.push_back(rString);
}

void ArmError::flushToJsonArray(JsonArray& arr) {    
    for(std::string error : errors) {  
        const char* elem = error.c_str();         
        arr.add((char *)elem);
    }    
    errors.clear();
}

size_t ArmError::size() {
    return errors.size();
}


void ArmError::clear() {
    errors.clear();
}