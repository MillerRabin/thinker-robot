#include "radsList.h"
#include "ArmPart.h"
#include "ArmShoulder.h"
#include "ArmElbow.h"

//----ValidRads-----

void RadsList::addIfValid(ArmShoulder shoulder, const float rad, const float targetLength, const float x, const float y, const float z) {    
    //shoulder.setRads(rad, shoulder.ZRad);    
    //const float tLength = ArmElbow::getLength(shoulder, x, y, z);    
    if (!shoulder.isValid()) return;    
    //if (!ArmPart::isEqual(tLength, targetLength)) return;
    rads.push_back(rad);
}


std::vector<float> RadsList::getRads(const float rad) {        
    sort(rads.begin(), rads.end(), [rad](const float rad1, const float rad2) {
        const float delta1 = abs(rad1 - rad);
        const float delta2 = abs(rad2 - rad);
        return (delta1 < delta2);
    });
    return rads;
}