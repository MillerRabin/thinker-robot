#include <arm.h>
#include <math.h>
#include <arduino.h>
#include <armParams.h>

double ArmPoint::getRad(const double angle) {    
    return (this->getInc() * angle + this->getOffset()) * PI / 180.0;    
}

void ArmPoint::setCoords() {    
    const double length = this->getLength();
    this->x = length * cos(this->XZCurrent) * sin(this->XYRad);
    this->y = length * cos(this->XZCurrent) * cos(this->XYRad);
    this->z = length * sin(this->XZCurrent);
}

double ArmPoint::getAngleXZ() {
    const double sdeg = this->XZRad / PI * 180.0;
    const double sAngle = (sdeg - this->getOffset()) / this->getInc();        
    return sAngle;
}

double ArmPoint::getAngleXZ(const double rad) {
    const double sdeg = rad / PI * 180.0;
    const double sAngle = (sdeg - this->getOffset()) / this->getInc();        
    return sAngle;
}

double ArmPoint::getRadFromPos(const double localX, const double localZ) {    
    Serial.printf("localX: %f, localZ: %f\n", localX, localZ);
    const double length = this->getLength();    
    const double sinz = localZ / length;        
    const double sina = asin(sinz);    
    return (localX < 0) ? PI - sina :  sina;    
}

ArmRotate::ArmRotate(const double rotateAngle) {            
    this->XYRad = this->getRad(rotateAngle);
}

double ArmShoulder::getLengthXZ(const double x, const double z) {
    const double dz = z - TOP_OFFSET;    
    const double qx = x * x;
    const double qz = dz * dz;
    return sqrt(qz + qx);
}

ArmShoulder::ArmShoulder(const double rotateRad, const double shoulderAngle) {            
    const double sRad = this->getRad(shoulderAngle);    
    this->setPoints(rotateRad, sRad);
}

void ArmShoulder::setPoints(const double rotateRad, const double shoulderRad) {
    this->XZRad = shoulderRad;
    this->XYRad = rotateRad;
    this->XZCurrent = this->XZRad; 
    this->setCoords();   
}

void ArmShoulder::addX(const double dx) {
    const double tx = this->x + dx;
    const double trad = tx / SHOULDER_LENGTH / sin(this->XYRad);
    const double sRad = acos(trad);
    this->setPoints(this->XYRad, sRad);
}

ArmElbow::ArmElbow(const double rotateRad, const double shoulderRad, const double elbowAngle) {    
    const double eRad = this->getRad(elbowAngle);        
    this->setPoints(shoulderRad, rotateRad, eRad);
}

void ArmElbow::setPoints(const double shoulderRad, const double rotateRad, const double elbowRad) {
    this->XZRad = elbowRad;
    this->XYRad = rotateRad;
    this->XZCurrent = this->XZRad + shoulderRad;    
    this->setCoords();
}

void ArmElbow::addZ(const double shoulderRad, const double dz) {
    const double tz = this->z + dz;    
    const double trad = tz / ELBOW_LENGTH;
    const double erad = asin(trad) - shoulderRad;    
    this->setPoints(shoulderRad, this->XYRad, erad);
}

double ArmElbow::getLengthXZ(ArmShoulder shoulder, const double x, const double z) {    
    const double sx = x - shoulder.x ;
    const double sz = z - (shoulder.z + TOP_OFFSET);
    const double qx = sx * sx;
    const double qz = sz * sz;
    return sqrt(qz + qx);    
}

ArmRoots ArmElbow::getRoots(ArmShoulder shoulder, const double x, const double z, const double length) {
    const double sx = x - shoulder.x ;
    const double sz = z - (shoulder.z + TOP_OFFSET);
    const double qx = sx * sx;
    const double qz = sz * sz;
    const double R = ELBOW_LENGTH;
    const double qR = R * R;
    const double r = length;
    const double qr = r * r;
    const double c = (qr - qx - qz - qR) / -2;    
    const double qc = c * c;
    const double a = qx + qz;
    ArmRoots roots;
    if (sx != 0) {
        const double b = -2 * sz * c;
        const double e = c * c - qR * qx;
        const double D = b * b - 4 * a * e;
        if (D > 0) {            
            roots.r1.z = (-b + sqrt(D)) / (2 * a);
            roots.r2.z = (-b - sqrt(D)) / (2 * a);            
            roots.r1.x = (c - roots.r1.z * sz) / sx;
            roots.r2.x = (c - roots.r2.z * sz) / sx;                        
            return roots;
        } if (D == 0) {
            roots.r1.z = (-b + sqrt(D)) / (2 * a);
            roots.r1.x = (c - roots.r1.z * sz) / sx;
            return roots;
        }
        return roots;
    }
    const double D = qR - qc / qz;
    if (D > 0) {            
        roots.r1.x = sqrt(D);
        roots.r2.x = -1 * sqrt(D);
        roots.r1.z = c / sz;
        roots.r2.x = c / sz;
        return roots;
    } if (D == 0) {
        roots.r1.x = 0;                    
        roots.r1.z = c / sz;
        return roots;
    }
    return roots;
}


double ArmElbow::getAngleFromPos(ArmShoulder shoulder, const double x, const double z) {        
    const double tsum = getRadFromPos(x, z);    
    const double erad = tsum - shoulder.XZRad;
    return this->getAngleXZ(erad);    
}

bool ArmElbow::isAngleValid(const double angle) {
    if (angle == NAN) return false;
    if (angle < ELBOW_MIN) return false;
    if (angle > ELBOW_MAX) return false;
    return true;
}

ArmRoot ArmElbow::getValidRoot(ArmShoulder shoulder, ArmRoots roots) {
    const double angle1 = this->getAngleFromPos(shoulder, roots.r1.x, roots.r1.z);
    const double angle2 = this->getAngleFromPos(shoulder, roots.r2.x, roots.r2.z);
    Serial.printf("angle1: %f, angle2: %f\n", angle1, angle2);
    const bool av1 = ArmElbow::isAngleValid(angle1);
    const bool av2 = ArmElbow::isAngleValid(angle2);
    if (av1 && !av2) return roots.r1;
    if (!av1 && av2) return roots.r2;
    return roots.r1;
}

void ArmElbow::setToLength(ArmShoulder shoulder, const double x, const double z, const double length) {
    const double sx = shoulder.x;
    const double sz = shoulder.z + TOP_OFFSET;    
    Serial.printf("sx: %f, sz: %f, R: %d\n", sx, sz, ELBOW_LENGTH);
    Serial.printf("x: %f, z: %f, r: %f\n", x, z, length);
    ArmRoots roots = ArmElbow::getRoots(shoulder, x, z, length);
    const double x1 = roots.r1.x + shoulder.x;
    const double z1 = roots.r1.z + shoulder.z + TOP_OFFSET;
    const double x2 = roots.r2.x + shoulder.x;
    const double z2 = roots.r2.z + shoulder.z + TOP_OFFSET;    
    Serial.printf("roots r1x: %f, r1z: %f, r2x: %f, r2z: %f\n", roots.r1.x, roots.r1.z, roots.r2.x, roots.r2.z);
    Serial.printf("r1x: %f, r1z: %f, r2x: %f, r2z: %f\n", x1, z1, x2, z2);
    ArmRoot root = this->getValidRoot(shoulder, roots);
    const double tsum = this->getRadFromPos(root.x, root.z);
    const double erad = tsum - shoulder.XZRad;    
    this->setPoints(shoulder.XZRad, this->XYRad, erad);    
}

ArmWrist::ArmWrist( const double rotateRad, const double shoulderRad, const double elbowRad,const double wristAngle) {    
    const double wRad = this->getRad(wristAngle);        
    this->setPoints(shoulderRad, elbowRad, wRad, rotateRad);
}

void ArmWrist::setPoints(const double shoulderRad, const double elbowRad, const double wristRad, const double rotateRad) {
    this->XZRad = wristRad;
    this->XYRad = rotateRad;
    this->XZCurrent = this->XZRad + shoulderRad + elbowRad;
    this->setCoords();
}

double ArmWrist::getLengthXZ(ArmShoulder shoulder, ArmElbow elbow, const double x, const double z) {    
    const double sx = x - (shoulder.x + elbow.x);
    const double sz = z - (shoulder.z + elbow.z + TOP_OFFSET);
    const double qx = sx * sx;
    const double qz = sz * sz;    
    return sqrt(qz + qx);
}


void ArmWrist::setPos(ArmShoulder shoulder, ArmElbow elbow, const double x, const double z) {
    const double wz = z - shoulder.z - elbow.z - TOP_OFFSET;
    const double wx = x - shoulder.x - elbow.x;
    const double tsum = getRadFromPos(wx, wz);
    const double wrad = tsum - shoulder.XZRad - elbow.XZRad;
    this->setPoints(shoulder.XZRad, elbow.XZRad, wrad, shoulder.XYRad);
}

ArmClaw::ArmClaw(const double rotateRad, const double shoulderRad, const double elbowRad, const double wristRad, const double clawAngle) {
    const double cRad = this->getRad(clawAngle);
    this->setPoints(shoulderRad, elbowRad, wristRad, cRad, rotateRad);
}

void ArmClaw::setPoints(const double shoulderRad, const double elbowRad, const double wristRad, const double clawRad, const double rotateRad) {
    this->XZRad = wristRad;
    this->XYRad = clawRad;        
    this->XZCurrent = this->XZRad + shoulderRad + elbowRad;
    //const double zwidth = this->getWidth() * cos(this->XZRad);    
    //Serial,printf("claw width: %f\n", zwidth);
    this->setCoords();
    
}