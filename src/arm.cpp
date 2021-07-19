#include <arm.h>
#include <math.h>
#include <arduino.h>
#include "armParams.h"
#include "radsList.h"
#include <string>

using namespace std;

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

//------ArmPoint-------

const double ArmPoint::getXRad(const double angle) {    
    return (this->getXScale() * angle + this->getXBase()) * PI / 180.0;    
}

const double ArmPoint::getYRad(const double angle) {    
    return (this->getYScale() * angle + this->getYBase()) * PI / 180.0;    
}

const double ArmPoint::getZRad(const double angle) {    
    return (this->getZScale() * angle + this->getZBase()) * PI / 180.0;    
}

void ArmPoint::setCoords() {    
    const double length = this->getLength();
    this->x = length * cos(this->YRad) * cos(this->ZRad);
    this->y = length * cos(this->YRad) * sin(this->ZRad);
    this->z = length * sin(this->YRad);
}

const double ArmPoint::getXAngleFromRad(const double rad) {
    if (rad == NAN) return NAN;
    const double sdeg = rad / PI * 180.0;
    double sAngle = (sdeg - this->getXBase()) / this->getXScale();        
    if (sAngle < 0)
        sAngle = 360 + sAngle;
    const double tAngle = sAngle - (trunc(sAngle / 360) * 360);
    return tAngle;
}


const double ArmPoint::getYAngleFromRad(const double rad) {
    if (rad == NAN) return NAN;
    const double sdeg = rad / PI * 180.0;
    double sAngle = (sdeg - this->getYBase()) / this->getYScale();        
    if (sAngle < 0)
        sAngle = 360 + sAngle;
    const double tAngle = sAngle - (trunc(sAngle / 360) * 360);
    return tAngle;
}

const double ArmPoint::getZAngleFromRad(const double rad) {
    if (rad == NAN) return NAN;
    const double sdeg = rad / PI * 180.0;
    double sAngle = (sdeg - this->getZBase()) / this->getZScale();        
    if (sAngle < 0)
        sAngle = 360 + sAngle;
    const double tAngle = sAngle - (trunc(sAngle / 360) * 360);
    return tAngle;
}

const double ArmPoint::getRadFromXY(const double x, const double y) {
    const double lengthV = sqrt(x*x + y*y);
    const double sr = (lengthV != 0) ? y / lengthV : 0;
    const double siny = asin(sr);    
    const double res = (x < 0) ? 
                       (y < 0) ? -PI - siny : PI - siny
                       : siny;
    return res;
}


const double ArmPoint::getYRadFromPos(const double localX, const double localY, const double localZ) {                    
    const double length = this->getLength();    
    const double sinz = (length > 0) ? localZ / length : 0;
    const double sina = asin(sinz);  
    const double asina = abs(sina);
    
    Coords coord = Coords(length, sina, ZRad);    
    if (coord.isEqual(localX, localY, localZ)) return sina;
    const double a1 =  PI + asina;
    Coords coord2 = Coords(length, a1, ZRad);    
    if (coord2.isEqual(localX, localY, localZ)) return a1;
    const double a2 =  PI - asina;
    Coords coord3 = Coords(length, a2, ZRad);    
    if (coord3.isEqual(localX, localY, localZ)) return a2;
    return NAN;    
}

const bool ArmPoint::isEqual(const double op1, const double op2, const double tolerance) {
    const double delta = op1 - op2;
    return (delta < tolerance) && (delta > -tolerance);
}

//----ArmShoulder-----

const double ArmShoulder::getLength(const double x, const double y, const double z) {    
    const double dz = z - BASE_HEIGHT;        
    const double qz = dz * dz;
    const double qx = x * x;
    const double qy = y * y;
    return sqrt(qx + qy + qz);
}

ArmShoulder::ArmShoulder(const double yAngle, const double zAngle) {            
    const double yRad = getYRad(yAngle);    
    const double zRad = getZRad(zAngle);
    setRads(yRad, zRad);
}

const double ArmShoulder::getZRadFromXY(const double x, const double y) {
    const double rRad = ArmPoint::getRadFromXY(x, y);
    const double minRad = (SHOULDER_Z_MIN + SHOULDER_Z_BASE) / 180.0 * PI;
    const double maxRad = (SHOULDER_Z_MAX + SHOULDER_Z_BASE) / 180.0 * PI;
    Serial.printf("rRad: %f, minRad: %f, maxRad: %f\n", rRad, minRad, maxRad);
    if (rRad > maxRad) 
        return (rRad - PI);        
    if (rRad < minRad) 
        return (rRad + PI);
    return rRad;
}

const double ArmShoulder::validateYAngle(const double angle) {    
    if (isnan(angle)) return ERROR_SHOULDER_Y_ANGLE_IS_NAN;    
    if (angle < SHOULDER_Y_MIN) return ERROR_SHOULDER_Y_ANGLE_LESS_MIN;            
    if (angle > SHOULDER_Y_MAX) return ERROR_SHOULDER_Y_ANGLE_ABOVE_MAX;            
    return angle;
}

const double ArmShoulder::validateZAngle(const double angle) {    
    if (isnan(angle)) return ERROR_SHOULDER_Z_ANGLE_IS_NAN;    
    if (angle < SHOULDER_Z_MIN) return ERROR_SHOULDER_Z_ANGLE_LESS_MIN;            
    if (angle > SHOULDER_Z_MAX) return ERROR_SHOULDER_Z_ANGLE_ABOVE_MAX;            
    return angle;
}

const double ArmShoulder::getYAngle(const bool validate) {    
    const double angle = getYAngleFromRad(YRad);
    if (!validate)
        return angle;
    return ArmShoulder::validateYAngle(angle);
}

const double ArmShoulder::getZAngle(const bool validate) {    
    const double angle = getZAngleFromRad(ZRad);
    if (!validate)
        return angle;
    return ArmShoulder::validateZAngle(angle);
}

vector<double> ArmShoulder::getAvailableRads(const double maxLength, const double x, const double y, const double z) {    
    const double a = ArmShoulder::getLength(x, y, z);
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

    const double wz = z - BASE_HEIGHT;
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

void ArmShoulder::setRads(const double yRad, const double zRad) {
    YRad = yRad;
    ZRad = zRad;    
    setCoords();   
}

const bool ArmShoulder::isValid() {
    if (isnan(YRad)) return false;    
    const double minRad = getYRad(getYMinAngle());
    const double maxRad = getYRad(getYMaxAngle());    
    if (YRad < minRad) return false;            
    if (YRad > maxRad) return false;            
    return true;
}

//------ArmElbow------


ArmElbow::ArmElbow(ArmShoulder shoulder, const double yAngle) {    
    const double eRad = getYRad(yAngle) + shoulder.YRad;
    setRads(eRad, shoulder.ZRad);
}

const double ArmElbow::getYAngle(ArmShoulder shoulder, const bool validate) {    
    const double angle = getYAngleFromRad(YRad - shoulder.YRad);
    if (!validate)
        return angle;
    return ArmElbow::validateYAngle(angle);    
}

const double ArmElbow::validateYAngle(const double angle) {    
    if (isnan(angle)) return ERROR_ELBOW_Y_ANGLE_IS_NAN;    
    if (angle < ELBOW_Y_MIN) return ERROR_ELBOW_Y_ANGLE_LESS_MIN;            
    if (angle > ELBOW_Y_MAX) return ERROR_ELBOW_Y_ANGLE_ABOVE_MAX;            
    return angle;      
}

void ArmElbow::setRads(const double yRad, const double zRad) {
    YRad = yRad;
    ZRad = zRad;    
    setCoords();
}

const double ArmElbow::getLength(ArmShoulder shoulder, const double x, const double y, const double z) {    
    const double sx = x - shoulder.x ;
    const double sy = y - shoulder.y;
    const double sz = z - (shoulder.z + BASE_HEIGHT);    
    const double qx = sx * sx;
    const double qy = sy * sy;
    const double qz = sz * sz;        
    return sqrt(qx + qy + qz);
}

const double ArmElbow::getYAngleFromPos(ArmShoulder shoulder, const double x, const double y, const double z) {        
    const double erad = getYRadFromPos(x, y, z);    
    return getYAngleFromRad(erad - shoulder.YRad);    
}

void ArmElbow::setPosLocal(const double localX, const double localY, const double localZ) {        
    const double erad = getYRadFromPos(localX, localY, localZ);    
    setRads(erad, ZRad);    
}

void ArmElbow::setPos(ArmShoulder shoulder, const double x, const double y, const double z) {        
    const double localX = x - shoulder.x;    
    const double localY = y - shoulder.y;
    const double localZ = z - shoulder.z - BASE_HEIGHT;
    const double erad = getYRadFromPos(localX, localY, localZ);    
    setRads(erad, shoulder.ZRad);    
}

const double ArmElbow::getLocalRad(ArmShoulder shoulder) {            
    return YRad - shoulder.YRad;
}

const bool ArmElbow::isValid(ArmShoulder shoulder) {    
    const double angle = getYAngle(shoulder);
    if (angle < 0) return false;
    const double tz = z + shoulder.z + BASE_HEIGHT;
    if (tz < 0) return false;
    return true;
}


//------ArmWrist------


ArmWrist::ArmWrist(ArmElbow elbow, const double yAngle) {    
    const double wRad = this->getYRad(yAngle);        
    setRads(wRad + elbow.YRad, elbow.ZRad);
}

const double ArmWrist::validateYAngle(const double angle) {    
    if (isnan(angle)) return ERROR_WRIST_Y_ANGLE_IS_NAN;        
    if (angle < WRIST_Y_MIN) return ERROR_WRIST_Y_ANGLE_LESS_MIN;     
    if (angle > WRIST_Y_MAX) return ERROR_WRIST_Y_ANGLE_ABOVE_MAX;            
    return angle;      
}

const double ArmWrist::getYAngle(ArmElbow elbow, const bool validate) {
    const double angle = getYAngleFromRad(this->YRad - elbow.YRad);
    if (!validate)
        return angle;
    return ArmWrist::validateYAngle(angle);    
}

void ArmWrist::setRads(const double yRad, const double zRad) {
    this->YRad = yRad;
    this->ZRad = zRad;
    this->setCoords();
}

const double ArmWrist::getLength(ArmShoulder shoulder, ArmElbow elbow, const double x, const double y, const double z) {        
    const double sx = x - (shoulder.x + elbow.x);
    const double sy = y - (shoulder.y + elbow.y);
    const double sz = z - (shoulder.z + elbow.z + BASE_HEIGHT);    
    const double qx = sx * sx;
    const double qy = sy * sy;
    const double qz = sz * sz;    
    return sqrt(qx + qy + qz);
}

const double ArmWrist::getLocalRad(ArmElbow elbow) {            
    return YRad - elbow.YRad;
}


void ArmWrist::setPos(ArmShoulder shoulder, ArmElbow elbow, const double x, const double y, const double z) {        
    const double wx = x - (shoulder.x + elbow.x);
    const double wy = y - (shoulder.y + elbow.y);
    const double wz = z - (shoulder.z + elbow.z + BASE_HEIGHT);
    double wrad = getYRadFromPos(wx, wy, wz);
    setRads(wrad, elbow.ZRad);    
}

const bool ArmWrist::isValid(ArmElbow elbow) {    
    const double angle = getYAngle(elbow);
    if (angle < 0) return false;
    return true;
}

//-------ArmClaw-------

ArmClaw::ArmClaw(ArmWrist wrist, const double clawXAngle, const double clawAngle) {
    const double cxRad = getXRad(clawXAngle);
    const double clawRad = getClawRad(clawAngle);
    setRads(wrist.ZRad, wrist.YRad, cxRad, clawRad);
}

const double ArmClaw::getClawRad(const double angle) {    
    return (CLAW_SCALE * angle + CLAW_BASE) * PI / 180.0;
}

ArmClaw::ArmClaw(const double xRad, const double yRad, const double zRad, const double clawRad) {    
    setRads(xRad, yRad, zRad, clawRad);
}

void ArmClaw::setRads(const double xRad, const double yRad, const double zRad, const double clawRad) {
    this->YRad = yRad;
    this->ZRad = zRad;
    if (!isnan(xRad))
        this->XRad = xRad;
    if (!isnan(clawRad))
        this->clawRad = clawRad;
    setCoords();    
}

const double ArmClaw::getAngle(const bool validate) {     
     const double angle = getClawAngleFromRad(clawRad);
    if (!validate)
        return angle;
    return ArmClaw::validateClawAngle(angle);    
}

const double ArmClaw::getClawAngleFromRad(const double rad) {
    if (rad == NAN) return NAN;
    const double sdeg = rad / PI * 180.0;
    double sAngle = (sdeg - CLAW_BASE) / CLAW_SCALE;        
    if (sAngle < 0)
        sAngle = 360 + sAngle;
    const double tAngle = sAngle - (trunc(sAngle / 360) * 360);
    return tAngle;
}


const double ArmClaw::getXAngle(const bool validate) {     
     const double angle = getXAngleFromRad(XRad);
    if (!validate)
        return angle;
    return ArmClaw::validateXAngle(angle);    
}

const double ArmClaw::getYAngle(ArmWrist wrist, const bool validate) {
    const double angle = getYAngleFromRad(wrist.YRad);
    if (!validate)
        return angle;
    return ArmClaw::validateYAngle(angle);    
}

const double ArmClaw::validateYAngle(const double angle) {    
    if (isnan(angle)) return ERROR_WRIST_Y_ANGLE_IS_NAN;        
    if (angle < WRIST_Y_MIN) return ERROR_WRIST_Y_ANGLE_LESS_MIN;     
    if (angle > WRIST_Y_MAX) return ERROR_WRIST_Y_ANGLE_ABOVE_MAX;            
    return angle;      
}

const double ArmClaw::validateXAngle(const double angle) {        
    if (isnan(angle)) return ERROR_CLAW_X_ANGLE_IS_NAN;            
    if (angle < CLAW_X_MIN) return ERROR_CLAW_X_ANGLE_LESS_MIN;     
    if (angle > CLAW_X_MAX) return ERROR_CLAW_X_ANGLE_ABOVE_MAX;            
    return angle;      
}

const double ArmClaw::validateClawAngle(const double angle) {        
    if (isnan(angle)) return ERROR_CLAW_ANGLE_IS_NAN;            
    if (angle < CLAW_MIN) return ERROR_CLAW_ANGLE_LESS_MIN;     
    if (angle > CLAW_MAX) return ERROR_CLAW_ANGLE_ABOVE_MAX;            
    return angle;      
}
