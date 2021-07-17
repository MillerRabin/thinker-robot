#include "arm.h"
#include "radsList.h"

//----ValidRads-----

void RadsList::addIfValid(ArmShoulder shoulder, const double rad, const double targetLength, const double x, const double y, const double z) {    
    shoulder.setRads(shoulder.ZRad, rad);    
    const double tLength = ArmElbow::getLength(shoulder, x, y, z);
    Serial.printf("Check rad: %f, tLength: %f\n", rad, tLength);
    if (!shoulder.isValid()) return;
    if (!ArmPoint::isEqual(tLength, targetLength)) return;
    rads.push_back(rad);
}


vector<double> RadsList::getRads(const double rad) {        
    sort(rads.begin(), rads.end(), [rad](const double rad1, const double rad2) {
        const double delta1 = abs(rad1 - rad);
        const double delta2 = abs(rad2 - rad);
        return (delta1 < delta2);
    });
    return rads;
}