#include <arm.h>
#include <math.h>
#include <arduino.h>
#include <armParams.h>
#include <string>

using namespace std;

//------Coords------

Coords::Coords(const double length, const double XZRad, const double XYRad) : 
    length(length),
    x(length * cos(XZRad) * cos(XYRad)),
    y(length * cos(XZRad) * sin(XYRad)),
    z(length * sin(XZRad))    
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

const double ArmPoint::getRad(const double angle) {    
    return (this->getScale() * angle + this->getBase()) * PI / 180.0;    
}

void ArmPoint::setCoords() {    
    const double length = this->getLength();
    this->x = length * cos(this->XZRad) * cos(this->XYRad);
    this->y = length * cos(this->XZRad) * sin(this->XYRad);
    this->z = length * sin(this->XZRad);
}

const double ArmPoint::getAngleFromRad(const double rad) {
    if (rad == NAN) return NAN;
    const double sdeg = rad / PI * 180.0;
    double sAngle = (sdeg - this->getBase()) / this->getScale();        
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


const double ArmPoint::getRadFromPos(const double localX, const double localY, const double localZ) {                    
    const double length = this->getLength();    
    const double sinz = (length > 0) ? localZ / length : 0;
    const double sina = asin(sinz);  
    const double asina = abs(sina);
    
    Coords coord = Coords(length, sina, XYRad);    
    if (coord.isEqual(localX, localY, localZ)) return sina;
    const double a1 =  PI + asina;
    Coords coord2 = Coords(length, a1, XYRad);    
    if (coord2.isEqual(localX, localY, localZ)) return a1;
    const double a2 =  PI - asina;
    Coords coord3 = Coords(length, a2, XYRad);    
    if (coord3.isEqual(localX, localY, localZ)) return a2;
    return NAN;    
}

const bool ArmPoint::isValid() {
    if (isnan(XZRad)) return false;    
    const double minRad = getRad(getMinAngle());
    const double maxRad = getRad(getMaxAngle());
    Serial.printf("XZRad: %f, minRad: %f, maxRad: %f\n", XZRad, minRad, maxRad);
    if (XZRad < minRad) return false;            
    if (XZRad > maxRad) return false;            
    return true;
}

const bool ArmPoint::isEqual(const double op1, const double op2, const double tolerance) {
    const double delta = op1 - op2;
    return (delta < tolerance) && (delta > -tolerance);
}

//----ArmRotate-----

ArmRotate::ArmRotate(const double rotateAngle) {            
    this->XYRad = this->getRad(rotateAngle);
}

const double ArmRotate::getAngle() {
    const double rad = this->XYRad;
    return getAngleFromRad(rad);
}

void ArmRotate::setPoints(const double rotateRad) {
    XZRad = 0;
    XYRad = rotateRad;    
    setCoords();   
}

void ArmRotate::setRadFromPos(const double x, const double y) {
    const double rRad = ArmPoint::getRadFromXY(x, y);
    const double minRad = (ROTATE_MIN + ROTATE_BASE) / 180.0 * PI;
    const double maxRad = (ROTATE_MAX + ROTATE_BASE) / 180.0 * PI;
    
    if (rRad > maxRad) 
        return setPoints(rRad - PI);        
    if (rRad < minRad) 
        return setPoints(rRad + PI);
    return setPoints(rRad);        
}

//----ArmShoulder-----

const double ArmShoulder::getLength(const double x, const double y, const double z) {    
    const double dz = z - BASE_HEIGHT;        
    const double qz = dz * dz;
    const double qx = x * x;
    const double qy = y * y;
    return sqrt(qx + qy + qz);
}

ArmShoulder::ArmShoulder(ArmRotate rotate, const double shoulderAngle) {            
    const double sRad = this->getRad(shoulderAngle);    
    this->setPoints(rotate.XYRad, sRad);
}

const double ArmShoulder::validateAngle(const double angle) {    
    if (isnan(angle)) return ERROR_SHOULDER_ANGLE_IS_NAN;    
    if (angle < SHOULDER_MIN) return ERROR_SHOULDER_ANGLE_LESS_MIN;            
    if (angle > SHOULDER_MAX) return ERROR_SHOULDER_ANGLE_ABOVE_MAX;            
    return angle;
}

const double ArmShoulder::getAngle(const bool validate) {
    const double rad = this->XZRad;
    const double angle = getAngleFromRad(rad);
    if (!validate)
        return angle;
    return ArmShoulder::validateAngle(angle);
}

void ArmShoulder::setAvailableLength(const double maxLength, const double x, const double y, const double z) {    
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
    
    const double wz = z - BASE_HEIGHT;
    const double lsin = wz / a;
    const double lrad = asin(lsin);
        
    const double srad = trad + lrad;

    setPoints(this->XYRad, srad);

    const double tLength = ArmElbow::getLength(*this, x, y, z);

    if (ArmPoint::isEqual(tLength, b)) return;
    const double drad = (PI - trad) + lrad;
    setPoints(this->XYRad, drad);
    const double dLength = ArmElbow::getLength(*this, x, y, z);

    if (ArmPoint::isEqual(dLength, b)) return;
    const double mrad = (PI - trad) - lrad;
    setPoints(this->XYRad, mrad);
    const double mLength = ArmElbow::getLength(*this, x, y, z);

    if (ArmPoint::isEqual(mLength, b)) return;
    setPoints(this->XYRad, NAN);
}

void ArmShoulder::setPoints(const double rotateRad, const double shoulderRad) {
    XZRad = shoulderRad;
    XYRad = rotateRad;    
    setCoords();   
}

void ArmShoulder::setRotate(ArmRotate rotate) {
    XYRad = rotate.XYRad;
    setCoords();
}

void ArmShoulder::setXZRad(const double rad) {
    XZRad = rad;
    setCoords();
}


//------ArmElbow------


ArmElbow::ArmElbow(ArmShoulder shoulder, const double elbowAngle) {    
    const double eRad = this->getRad(elbowAngle) + shoulder.XZRad;
    this->setPoints(shoulder.XYRad, eRad);
}

const double ArmElbow::getAngle(ArmShoulder shoulder, const bool validate) {    
    const double angle = getAngleFromRad(this->XZRad - shoulder.XZRad);
    if (!validate)
        return angle;
    return ArmElbow::validateAngle(angle);    
}

const double ArmElbow::validateAngle(const double angle) {    
    if (isnan(angle)) return ERROR_ELBOW_ANGLE_IS_NAN;    
    if (angle < ELBOW_MIN) return ERROR_ELBOW_ANGLE_LESS_MIN;            
    if (angle > ELBOW_MAX) return ERROR_ELBOW_ANGLE_ABOVE_MAX;            
    return angle;      
}

void ArmElbow::setPoints(const double rotateRad, const double elbowRad) {
    this->XZRad = elbowRad;
    this->XYRad = rotateRad;    
    this->setCoords();
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

const double ArmElbow::getAngleFromPos(ArmShoulder shoulder, const double x, const double y, const double z) {        
    const double erad = getRadFromPos(x, y, z);    
    return this->getAngleFromRad(erad - shoulder.XZRad);    
}

void ArmElbow::setPosLocal(const double localX, const double localY, const double localZ) {    
    //Serial.printf("Elbow set pos\n");
    const double erad = getRadFromPos(localX, localY, localZ);    
    setPoints(XYRad, erad);    
    //Serial.printf("Elbow XZRad: %f, x: %f(%f), y: %f(%f), z: %f(%f)\n", XZRad, localX, x, localY, y, localZ, z);
}

void ArmElbow::setRotate(ArmRotate rotate) {
    XYRad = rotate.XYRad;
    setCoords();
}

const double ArmElbow::getLocalRad(ArmShoulder shoulder) {            
    return XZRad - shoulder.XZRad;
}


//------ArmWrist------


ArmWrist::ArmWrist(ArmElbow elbow, const double wristAngle) {    
    const double wRad = this->getRad(wristAngle);        
    this->setPoints(wRad + elbow.XZRad, elbow.XYRad);
}

const double ArmWrist::validateAngle(const double angle) {    
    if (isnan(angle)) return ERROR_WRIST_ANGLE_IS_NAN;    
    if (angle < WRIST_MIN) return ERROR_WRIST_ANGLE_LESS_MIN;            
    if (angle > WRIST_MAX) return ERROR_WRIST_ANGLE_ABOVE_MAX;            
    return angle;      
}

const double ArmWrist::getAngle(ArmElbow elbow, const bool validate) {
    const double angle = this->getAngleFromRad(this->XZRad - elbow.XZRad);
    if (!validate)
        return angle;
    return ArmWrist::validateAngle(angle);    
}

void ArmWrist::setPoints(const double wristRad, const double rotateRad) {
    this->XZRad = wristRad;
    this->XYRad = rotateRad;
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
    return XZRad - elbow.XZRad;
}


void ArmWrist::setPos(ArmShoulder shoulder, ArmElbow elbow, const double x, const double y, const double z) {        
    const double wx = x - (shoulder.x + elbow.x);
    const double wy = y - (shoulder.y + elbow.y);
    const double wz = z - (shoulder.z + elbow.z + BASE_HEIGHT);
    double wrad = getRadFromPos(wx, wy, wz);
    setPoints(wrad, elbow.XYRad);    
}

//-------ArmClaw-------


ArmClaw::ArmClaw(ArmWrist wrist, const double clawAngle) {
    const double cRad = this->getRad(clawAngle);
    setPoints(wrist.XZRad, cRad, wrist.XYRad);
}

ArmClaw::ArmClaw(const double rotateRad, const double wristRad, const double clawRad) {    
    setPoints(wristRad, clawRad, rotateRad);
}

void ArmClaw::setPoints(const double rotateRad, const double wristRad, const double clawRad) {
    this->XZRad = wristRad;;
    this->XYRad = clawRad;
    this->setCoords();    
}

const double ArmClaw::getAngle() {
    return this->getAngleFromRad(this->XYRad);
}