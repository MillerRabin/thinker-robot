
#include <ArmShoulder.h>
#include "radsList.h"

vector<double> ArmShoulder::getAvailableRads(const double maxLength, const double x, const double y, const double z) {    
    const double a = ArmShoulder::getLengthFromPoint(x, y, z);    
    const double c = getLength();    
    const double acSum = a + c;

    const double b = (maxLength > acSum) ? acSum : maxLength;

    const double p = (a + b + c) / 2;
    const double pa = p - a;
    const double pb = p - b;
    const double pc = p - c;
    
    const double h = 2 * sqrt(p * pa * pb * pc) / a;    
    
    const double sinh = h / c;
    const double trad = asin(sinh);    
    RadsList radsList;
    if (isnan(trad)) {        
        return radsList.getRads(NAN);
    }

    const double wz = z;
    const double lsin = wz / a;
    const double lrad = asin(lsin);
            
    const double cRad = this->YRad;    
    
    radsList.addIfValid(*this, trad + lrad, b, x, y, z);            
    radsList.addIfValid(*this, trad - lrad, b, x, y, z);            
    radsList.addIfValid(*this, (PI - trad) + lrad, b, x, y, z);        
    radsList.addIfValid(*this, (PI - trad) - lrad, b, x, y, z);    
    radsList.addIfValid(*this, (PI + trad) + lrad, b, x, y, z);        
    radsList.addIfValid(*this, (PI + trad) - lrad, b, x, y, z);    

    return radsList.getRads(cRad);
}