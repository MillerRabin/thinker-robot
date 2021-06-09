#include <arm.h>
#include <math.h>
#include <arduino.h>
#include <armParams.h>

double ArmPoint::getRad(const double angle) {    
    return (this->getInc() * angle + this->getOffset()) * PI / 180.0;    
}

void ArmPoint::setCoords() {
    this->x = this->getLength() * cos(this->XZCurrent) * sin(this->XYRad);
    this->y = this->getLength() * cos(this->XZCurrent) * cos(this->XYRad);
    this->z = this->getLength() * sin(this->XZCurrent);
}

double ArmPoint::getAngleXZ() {
    const double sdeg = this->XZRad / PI * 180.0;
    const double sAngle = (sdeg - this->getOffset()) / this->getInc();        
    return sAngle;
}

ArmRotate::ArmRotate(const double rotateAngle) {            
    this->XYRad = this->getRad(rotateAngle);
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

ArmElbow::ArmElbow(const double shoulderRad, const double rotateRad, const double elbowAngle) {    
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

ArmWrist::ArmWrist(const double shoulderRad, const double elbowRad, const double rotateRad, const double wristAngle) {    
    const double wRad = this->getRad(wristAngle);        
    this->setPoints(shoulderRad, elbowRad, wRad, rotateRad);
}

void ArmWrist::setPoints(const double shoulderRad, const double elbowRad, const double wristRad, const double rotateRad) {
    this->XZRad = wristRad;
    this->XYRad = rotateRad;
    this->XZCurrent = this->XZRad + shoulderRad + elbowRad;
    this->setCoords();
}

void ArmWrist::setX(const double shoulderRad, const double elbowRad, const double x) {
    this->x = x;
    const double trad = this->x / WRIST_LENGTH;
    const double sign = (this->XZCurrent < 0) ? -1 : 1;
    const double wrad = sign * acos(trad) - shoulderRad - elbowRad;    
    this->setPoints(shoulderRad, elbowRad, wrad, this->XYRad);
}