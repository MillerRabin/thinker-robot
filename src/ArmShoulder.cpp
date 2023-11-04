
#include <ArmShoulder.h>
#include "radsList.h"

ArmShoulder::ArmShoulder(const float yAngle, const float zAngle) {
    ArmBase base = ArmBase{ 0.0, PI / 2, 0.0 };
    build(base, 0, yAngle, zAngle);
}

std::vector<float> ArmShoulder::getAvailableRads(const float maxLength, const float x, const float y, const float z) {    
    const float a = ArmShoulder::getLengthFromPoint(x, y, z);    
    const float c = getLength();    
    const float acSum = a + c;

    const float b = (maxLength > acSum) ? acSum : maxLength;

    const float p = (a + b + c) / 2;
    const float pa = p - a;
    const float pb = p - b;
    const float pc = p - c;
    
    const float h = 2 * sqrt(p * pa * pb * pc) / a;    
    
    const float sinh = h / c;
    const float trad = asin(sinh);    
    RadsList radsList;
    if (isnan(trad)) {        
        return radsList.getRads(NAN);
    }

    const float wz = z;
    const float lsin = wz / a;
    const float lrad = asin(lsin);
            
    const float cRad = this->YRad;    
    
    radsList.addIfValid(*this, trad + lrad, b, x, y, z);            
    radsList.addIfValid(*this, trad - lrad, b, x, y, z);            
    radsList.addIfValid(*this, (PI - trad) + lrad, b, x, y, z);        
    radsList.addIfValid(*this, (PI - trad) - lrad, b, x, y, z);    
    radsList.addIfValid(*this, (PI + trad) + lrad, b, x, y, z);        
    radsList.addIfValid(*this, (PI + trad) - lrad, b, x, y, z);    

    return radsList.getRads(cRad);
}

const float ArmShoulder::getZRadFromXY(const float x, const float y) {
    const float rRad = ArmBase::getRadFromXY(x, y);
    const float minRad = (SHOULDER_Z_MIN + SHOULDER_Z_BASE) / 180.0 * PI;
    const float maxRad = (SHOULDER_Z_MAX + SHOULDER_Z_BASE) / 180.0 * PI;    
    if (rRad > maxRad) 
        return (rRad - PI);        
    if (rRad < minRad) 
        return (rRad + PI);
    return rRad;
}
