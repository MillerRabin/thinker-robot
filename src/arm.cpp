#include <arm.h>
#include <math.h>
#include <arduino.h>
#include <armParams.h>
#include <string>

using namespace std;

//----ArmPoint-----

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

const double ArmPoint::getRadFromPos(const double localX, const double localY, const double localZ) {        
    const double length = this->getLength();    
    const double sinz = (length > 0) ? localZ / length : 0;
    const double sina = asin(sinz); 
    const double vl = (round(localX) < 0) ? -1 * sqrt(localX * localX + localY * localY) : sqrt(localX * localX + localY * localY);    
    const double tsina = (vl < 0) ?
                         (localZ < 0) ? PI + abs(sina) : PI - abs(sina) :           
                        sina;    
    Serial.printf("vl: %f, tsina: %f\n", vl, tsina);                        
    return tsina;    
}

const bool ArmPoint::isValid() {
    if (isnan(XZRad)) return false;
    return true;
}

const bool ArmPoint::isEqual(const double op1, const double op2, const double tolerance) {
    const double delta = op1 - op2;
    Serial.printf("delta is: %f\n", delta);
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

const double ArmRotate::getRadFromPos(const double x, const double y) {
    const double lengthV = sqrt(x*x + y*y);
    const double sr = (lengthV != 0) ? y / lengthV : 0;
    const double siny = asin(sr);
    Serial.printf("siny: %f, x: %f, y: %f\n", siny, x, y);
    const double res =  (x < 0) ?
                        (y < 0) ? PI + abs(siny) : PI - abs(siny) :
                        siny;    
    const double maxRad = (ROTATE_MAX + ROTATE_BASE) / 180.0 * PI;
    const double minRad = (ROTATE_MIN + ROTATE_BASE) / 180.0 * PI;
    if (res > maxRad) return res - PI;
    if (res < minRad) return res + PI;
    Serial.printf("minRad: %f, maxRad: %f, res: %f\n", minRad, maxRad, res);
    return res;
}

void ArmRotate::setPoints(const double rotateRad) {
    XZRad = 0;
    XYRad = rotateRad;    
    setCoords();   
}

void ArmRotate::setRadFromPos(const double x, const double y) {
    const double rRad = ArmRotate::getRadFromPos(x, y);
    setPoints(rRad);    
}

//----ArmShoulder-----

const double ArmShoulder::getLength(const double x, const double y, const double z) {    
    const double dz = z - TOP_BASE;        
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
        
    Serial.printf("maxLength: %f, acSum: %f, a: %f, b: %f, c: %f\n", maxLength, acSum, a, b, c);    
    const double p = (a + b + c) / 2;
    const double pa = p - a;
    const double pb = p - b;
    const double pc = p - c;
    Serial.printf("p: %f, pa: %f, pb: %f, pc: %f\n", p, pa, pb, pc);    
    const double h = 2 * sqrt(p * pa * pb * pc) / a;    
    
    const double sinh = h / c;
    const double trad = asin(sinh);
    
    const double wz = z - TOP_BASE;
    const double lsin = wz / a;
    const double lrad = asin(lsin);
        
    const double srad = trad + lrad;
    Serial.printf("shoulder h = %f, sinh = %f, trad = %f, wz = %f, lsin = %f, lrad = %f, XZRad = %f\n", h, sinh, trad, wz, lsin, lrad, srad);

    setPoints(this->XYRad, srad);

    const double tLength = ArmElbow::getLength(*this, x, y, z);
    Serial.printf("try length 1: %f, b: %f\n", tLength, b);
    if (ArmPoint::isEqual(tLength, b)) return;
    const double drad = (PI - trad) + lrad;
    setPoints(this->XYRad, drad);
    const double dLength = ArmElbow::getLength(*this, x, y, z);
    Serial.printf("try length 2: %f, b: %f\n", dLength, b);
    if (ArmPoint::isEqual(dLength, b)) return;
    const double mrad = (PI - trad) - lrad;
    setPoints(this->XYRad, mrad);
    const double mLength = ArmElbow::getLength(*this, x, y, z);
    Serial.printf("try length 3: %f, b: %f\n", mLength, b);
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
    const double sz = z - (shoulder.z + TOP_BASE);    
    const double qx = sx * sx;
    const double qy = sy * sy;
    const double qz = sz * sz;        
    return sqrt(qx + qy + qz);
}

const double ArmElbow::getAngleFromPos(ArmShoulder shoulder, const double x, const double y, const double z) {        
    const double erad = getRadFromPos(x, y, z);    
    return this->getAngleFromRad(erad - shoulder.XZRad);    
}

void ArmElbow::setPos(const double x, const double y, const double z) {    
    const double erad = getRadFromPos(x, y, z);    
    setPoints(XYRad, erad);    
}

void ArmElbow::setRotate(ArmRotate rotate) {
    XYRad = rotate.XYRad;
    setCoords();
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
    const double sz = z - (shoulder.z + elbow.z + TOP_BASE);    
    const double qx = sx * sx;
    const double qy = sy * sy;
    const double qz = sz * sz;    
    return sqrt(qx + qy + qz);
}

void ArmWrist::setPos(ArmShoulder shoulder, ArmElbow elbow, const double x, const double y, const double z) {    
    const double wx = x - shoulder.x - elbow.x;        
    const double wy = y - shoulder.y - elbow.y;
    const double wz = z - shoulder.z - elbow.z - TOP_BASE;
    Serial.printf("set wrist\n");
    Serial.printf("wx: %f, wy: %f, wz: %f, wrist rad: %f\n", wx, wy, wz, this->XZRad);
    const double wrad = getRadFromPos(wx, wy, wz);        
    Serial.printf("x: %f, y: %f, z: %f\n", x, y, z);
    Serial.printf("target wrist rad: %f\n", wrad);
    setPoints(wrad, elbow.XYRad);
    Serial.printf("wrist result: x: %f, y: %f, z: %f\n", this->x, this->y, this->z);
}

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
    //const double zwidth = this->getWidth() * cos(this->XZRad);    
    //Serial,printf("claw width: %f\n", zwidth);
    this->setCoords();    
}

const double ArmClaw::getAngle() {
    return this->getAngleFromRad(this->XYRad);
}