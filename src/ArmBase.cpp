#include <ArmBase.h>

//------Coords------

Coords::Coords(const double length, const double YRad, const double ZRad) : 
    length(length),
    x(length * cos(YRad) * cos(ZRad)),
    y(length * cos(YRad) * sin(ZRad)),
    z(length * sin(YRad))    
{}

const bool Coords::isEqual(const double x, const double y, const double z, const double tolerance) {    
    const double deltaX = this->x - x;
    const double deltaY = this->y - y;
    const double deltaZ = this->z - z;    
    return (deltaX < tolerance) && (deltaX > -tolerance) &&
           (deltaY < tolerance) && (deltaY > -tolerance) &&
           (deltaZ < tolerance) && (deltaZ > -tolerance);
}

//------ArmBase------

void ArmBase::updateCoords() {    
    const double xLength = this->getXLength();
    const double yLength = this->getYLength();    
    const double zLength = this->getZLength();
        
    const double zRadTotal = this->ZRad + this->ZRadLocal;
    const double yRadTotal = this->YRad + this->YRadLocal;

    const double zxLocal = zLength * cos(zRadTotal) * cos(this->YRad);
    const double zyLocal = zLength * sin(zRadTotal) * cos(this->YRad);;
    const double zzLocal = zLength * sin(this->YRad);
    const double yxLocal = yLength * cos(yRadTotal) * cos(zRadTotal);
    const double yyLocal = yLength * cos(yRadTotal) * sin(zRadTotal);
    const double yzLocal = yLength * sin(yRadTotal);
    const double xxLocal = xLength * cos(yRadTotal) * cos(zRadTotal);
    const double xyLocal = xLength * cos(yRadTotal) * sin(zRadTotal);
    const double xzLocal = xLength * sin(yRadTotal);
    
    this->x = zxLocal + yxLocal + xxLocal;
    this->y = zyLocal + yyLocal + xyLocal;
    this->z = zzLocal + yzLocal + xzLocal;    
}

void ArmBase::setRads(const double xRad, const double yRad, const double zRad) {    
    this->YRad = yRad;
    this->ZRad = zRad;
    this->XRad = xRad;
}

void ArmBase::setRadsLocal(const double xRad, const double yRad, const double zRad) {    
    this->XRadLocal = xRad;
    this->YRadLocal = yRad;
    this->ZRadLocal = zRad;
}

const double ArmBase::getXRad(const double angle) {    
    return (this->getXScale() * angle + this->getXBase()) * PI / 180.0;    
}

const double ArmBase::getYRad(const double angle) {    
    return (this->getYScale() * angle + this->getYBase()) * PI / 180.0;    
}

const double ArmBase::getZRad(const double angle) {    
    return (this->getZScale() * angle + this->getZBase()) * PI / 180.0;    
}

const double ArmBase::getXRad() {    
    return this->XRad + this->XRadLocal;
}

const double ArmBase::getYRad() {    
    return this->YRad + this->YRadLocal;
}

const double ArmBase::getZRad() {    
    return this->ZRad + this->ZRadLocal;
}

const double ArmBase::getXAngleFromRad(const double rad) {
    if (rad == NAN) return NAN;
    const double sdeg = rad / PI * 180.0;
    double sAngle = (sdeg - this->getXBase()) / this->getXScale();        
    if (sAngle < 0)
        sAngle = 360 + sAngle;
    const double tAngle = sAngle - (trunc(sAngle / 360) * 360);
    return tAngle;
}

const double ArmBase::getYAngleFromRad(const double rad) {
    if (rad == NAN) return NAN;
    const double sdeg = rad / PI * 180.0;
    double sAngle = (sdeg - this->getYBase()) / this->getYScale();        
    if (sAngle < 0)
        sAngle = 360 + sAngle;
    const double tAngle = sAngle - (trunc(sAngle / 360) * 360);
    return tAngle;
}

const double ArmBase::getZAngleFromRad(const double rad) {
    if (rad == NAN) return NAN;
    const double sdeg = rad / PI * 180.0;
    double sAngle = (sdeg - this->getZBase()) / this->getZScale();        
    if (sAngle < 0)
        sAngle = 360 + sAngle;
    const double tAngle = sAngle - (trunc(sAngle / 360) * 360);
    return tAngle;
}

const double ArmBase::getRadFromXY(const double x, const double y) {
    const double lengthV = sqrt(x*x + y*y);
    const double sr = (lengthV != 0) ? y / lengthV : 0;
    const double siny = asin(sr);    
    const double res = (x < 0) ? 
                       (y < 0) ? -PI - siny : PI - siny
                       : siny;
    return res;
}


const double ArmBase::getYRadFromPos(const double localX, const double localY, const double localZ, const double length) {        
    const double tLength = isnan(length) ? getLength() : length;        
    
    const double sinz = (tLength > 0) ? localZ / tLength : 0;    
    const double sina = asin(sinz);  
    const double asina = abs(sina);
        
    Coords coord = Coords(tLength, sina, ZRad); 
    if (coord.isEqual(localX, localY, localZ)) return sina;
    const double a1 =  PI + asina;
    Coords coord2 = Coords(tLength, a1, ZRad);    
    if (coord2.isEqual(localX, localY, localZ)) return a1;
    const double a2 =  PI - asina;
    Coords coord3 = Coords(tLength, a2, ZRad);        
    if (coord3.isEqual(localX, localY, localZ)) return a2;
    return NAN;    
}

const bool ArmBase::isEqual(const double op1, const double op2, const double tolerance) {
    const double delta = op1 - op2;
    return (delta < tolerance) && (delta > -tolerance);
}

const double ArmBase::getLengthFromPoint(const double x, const double y, const double z) {    
    const double dz = z;        
    const double qz = dz * dz;
    const double qx = x * x;
    const double qy = y * y;
    return sqrt(qx + qy + qz);
}

const double ArmBase::validateXAngle(const double angle) {    
    if (isnan(angle)) return ERROR_X_ANGLE_IS_NAN;    
    if (angle < getXMinAngle()) return ERROR_X_ANGLE_LESS_MIN;            
    if (angle > getXMaxAngle()) return ERROR_X_ANGLE_ABOVE_MAX;            
    return angle;
}


const double ArmBase::validateYAngle(const double angle) {    
    if (isnan(angle)) return ERROR_Y_ANGLE_IS_NAN;    
    if (angle < getYMinAngle()) return ERROR_Y_ANGLE_LESS_MIN;            
    if (angle > getYMaxAngle()) return ERROR_Y_ANGLE_ABOVE_MAX;            
    return angle;
}


const double ArmBase::validateZAngle(const double angle) {    
    if (isnan(angle)) return ERROR_Z_ANGLE_IS_NAN;    
    if (angle < getZMinAngle()) return ERROR_Z_ANGLE_LESS_MIN;            
    if (angle > getZMaxAngle()) return ERROR_Z_ANGLE_ABOVE_MAX;            
    return angle;
}

const double ArmBase::getZRadFromXY(const double x, const double y) {
    const double rRad = ArmBase::getRadFromXY(x, y);
    const double minRad = (SHOULDER_Z_MIN + SHOULDER_Z_BASE) / 180.0 * PI;
    const double maxRad = (SHOULDER_Z_MAX + SHOULDER_Z_BASE) / 180.0 * PI;    
    if (rRad > maxRad) 
        return (rRad - PI);        
    if (rRad < minRad) 
        return (rRad + PI);
    return rRad;
}

const double ArmBase::getXAngle(const bool validate) {        
    const double angle = getXAngleFromRad(XRadLocal);
    if (!validate)
        return angle;
    return ArmBase::validateXAngle(angle);
}


const double ArmBase::getYAngle(const bool validate) {    
    const double angle = getYAngleFromRad(YRadLocal);
    if (!validate)
        return angle;
    return ArmBase::validateYAngle(angle);
}

const double ArmBase::getZAngle(const bool validate) {    
    const double angle = getZAngleFromRad(ZRadLocal);
    if (!validate)
        return angle;
    return ArmBase::validateZAngle(angle);
}

const bool ArmBase::isValid() {
    if (isnan(XRad) || isnan(YRad) || isnan(ZRad)) return false;
    const double sMinXRad = getXRad(getXMinAngle());
    const double sMaxXRad = getXRad(getXMaxAngle());    
    const double minXRad = (sMinXRad > sMaxXRad) ? sMaxXRad : sMinXRad;
    const double maxXRad = (sMinXRad > sMaxXRad) ? sMinXRad : sMaxXRad;    
    if ((XRad < minXRad) || (XRad > maxXRad)) return false;            
    const double sMinYRad = getYRad(getYMinAngle());
    const double sMaxYRad = getYRad(getYMaxAngle());    
    const double minYRad = (sMinYRad > sMaxYRad) ? sMaxYRad : sMinYRad;
    const double maxYRad = (sMinYRad > sMaxYRad) ? sMinYRad : sMaxYRad;    
    if ((YRad < minYRad) || (YRad > maxYRad)) return false;            
    const double sMinZRad = getZRad(getZMinAngle());
    const double sMaxZRad = getZRad(getZMaxAngle());    
    const double minZRad = (sMinZRad > sMaxZRad) ? sMaxZRad : sMinZRad;
    const double maxZRad = (sMinZRad > sMaxZRad) ? sMinZRad : sMaxZRad;    
    if ((ZRad < minZRad) || (ZRad > maxZRad)) return false;                        
    return true;
}
