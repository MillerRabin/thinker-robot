#include <armParams.h>
#include <arduino.h>
#include <math.h>
#include <Position.h>

double APoint::addX(const double dx) {
    this->x = x + dx;
    return this->x;
}

Leverage Position::getShoulderZ(const double sAngle) {
    const double sRad = (SHOULDER_INC * sAngle + SHOULDER_OFFSET) * PI / 180.0;
    Leverage lev;
    lev.length = SHOULDER_LENGTH * sin(sRad);
    lev.rad = sRad;
    return lev;
}

Leverage Position::getShoulderX(const double sAngle, const double rRad) {
    Leverage shoulder;
    shoulder.rad = (SHOULDER_INC * sAngle + SHOULDER_OFFSET) * PI / 180.0;
    shoulder.length = SHOULDER_LENGTH * cos(shoulder.rad) * sin(rRad);
    return shoulder;
}

Projection Position::getX(const double sAngle, const double eAngle, const double wAngle, const double rAngle) {    
    const double rRad = Position::getRotateRad(rAngle);
    Leverage shoulder = Position::getShoulderX(sAngle, rRad);
    Leverage elbow;
    elbow.rad = (ELBOW_INC * eAngle + ELBOW_OFFSET) * PI / 180.0;
    elbow.length = ELBOW_LENGTH * cos(elbow.rad + shoulder.rad) * sin(rRad);
    Leverage wrist;
    wrist.rad = (WRIST_INC * wAngle + WRIST_OFFSET) * PI / 180.0;
    wrist.length = WRIST_LENGTH * cos(wrist.rad + shoulder.rad + elbow.rad) * sin(rRad);
    Projection proj;
    proj.shoulder = shoulder;
    proj.elbow = elbow;
    proj.wrist = wrist;
    proj.length = shoulder.length + elbow.length + wrist.length;
    return proj;
}

const double Position::getRotateRad(const double rAngle) {
    return (rAngle + ROTATE_OFFSET) * PI / 180.0;
}

double Position::getShoulderAngleFromX(const double x, const double rRad) {                
    const double trad = x / SHOULDER_LENGTH / sin(rRad);    
    const double srad = acos(trad);
    const double sdeg = srad / PI * 180.0;
    const double sAngle = (sdeg - SHOULDER_OFFSET) / SHOULDER_INC;    
    return sAngle;
}

Leverage Position::getElbowZ(Leverage shoulder, const double eAngle) {
    Leverage elbow;
    elbow.rad = (ELBOW_INC * eAngle + ELBOW_OFFSET) * PI / 180.0;
    elbow.length = ELBOW_LENGTH * sin(elbow.rad + shoulder.rad);                    
    return elbow;
}

double Position::getElbowAngle(const double elbowRad) {        
    const double edeg = elbowRad / PI * 180.0;
    const double eAngle = (edeg - ELBOW_OFFSET) / ELBOW_INC;    
    return eAngle;
}

double Position::getElbowRadFromZ(Leverage shoulder, const double z) {
    const double tsum = z / ELBOW_LENGTH;    
    const double asum = asin(tsum);
    const double erad  = asum - shoulder.rad;    
    return erad;
}

Leverage Position::getWristX(Leverage shoulder, const double elbowRad, const double rotateRad, const double wristAngle) {
    Leverage wrist;
    wrist.rad = (WRIST_INC * wristAngle + WRIST_OFFSET) * PI / 180.0;
    wrist.length = WRIST_LENGTH * cos(wrist.rad + elbowRad + shoulder.rad) * sin(rotateRad);
    return wrist;
}

APoint Position::getWristPoint(const double shoulderRad, const double elbowRad, const double rotateRad, const double wristAngle) {
    APoint point;
    point.XZRad = (WRIST_INC * wristAngle + WRIST_OFFSET) * PI / 180.0;
    point.XYRad = rotateRad;
    point.sum = point.XZRad + elbowRad + shoulderRad;
    point.x = WRIST_LENGTH * cos(point.sum) * sin(rotateRad);
    point.y = WRIST_LENGTH * cos(point.sum) * cos(rotateRad);
    point.z = WRIST_LENGTH * sin(point.sum);
    return point;
}

APoint Position::getShoulderPoint(const double rotateRad, const double shoulderAngle) {
    APoint point;
    point.XZRad = (SHOULDER_INC * shoulderAngle + SHOULDER_OFFSET) * PI / 180.0;
    point.XYRad = rotateRad;
    point.sum = point.XZRad;
    point.x = SHOULDER_LENGTH * cos(point.sum) * sin(rotateRad);
    point.y = SHOULDER_LENGTH * cos(point.sum) * cos(rotateRad);
    point.z = SHOULDER_LENGTH * sin(point.sum);
    return point;
}

double Position::getWristAngleFromX(const double shoulderRad, const double elbowRad, APoint point) {    
    const double sign = (point.sum < 0) ? -1 : 1;
    const double tsum = point.x / (WRIST_LENGTH * sin(point.XYRad));
    const double asum = acos(tsum) * sign;
    const double wrad  = asum - elbowRad - shoulderRad;
    const double wdeg = wrad / PI * 180.0;
    const double wAngle = (wdeg - WRIST_OFFSET) / WRIST_INC;    
    return wAngle;
}

double Position::getWristAngleFromZ(const double shoulderRad, const double elbowRad, APoint point) {
    const double tsum = point.z / WRIST_LENGTH;
    const double asum = asin(tsum);
    const double wrad  = asum - elbowRad - shoulderRad;
    const double wdeg = wrad / PI * 180.0;
    const double wAngle = (wdeg - WRIST_OFFSET) / WRIST_INC;
    return wAngle;
}

Leverage Position::getWristZ(Leverage shoulder, Leverage elbow, const double wAngle) {
    Leverage wrist;
    wrist.rad = (WRIST_INC * wAngle + WRIST_OFFSET) * PI / 180.0;
    wrist.length = WRIST_LENGTH * sin(wrist.rad  + elbow.rad + shoulder.rad);
    return wrist;
}

Projection Position::getZ(const double sAngle, const double eAngle, const double wAngle) {
    Leverage shoulder = Position::getShoulderZ(sAngle);
    Leverage elbow = Position::getElbowZ(shoulder, eAngle);
    Leverage wrist = Position::getWristZ(shoulder, elbow, wAngle);    
    Projection proj;
    proj.shoulder = shoulder;
    proj.elbow = elbow;
    proj.wrist = wrist;
    proj.length = shoulder.length + elbow.length + wrist.length + TOP_OFFSET;
    return proj;
}

Projection Position::getY(const double sAngle, const double eAngle, const double wAngle, const double rAngle) {
    Leverage shoulder;
    const double rRad = (rAngle + ROTATE_OFFSET) * PI / 180.0;    
    shoulder.rad = (SHOULDER_INC * sAngle + SHOULDER_OFFSET) * PI / 180.0;
    shoulder.length = SHOULDER_LENGTH * cos(shoulder.rad) * cos(rRad);
    Leverage elbow;
    elbow.rad = (ELBOW_INC * eAngle + ELBOW_OFFSET) * PI / 180.0;
    elbow.length = ELBOW_LENGTH * cos(elbow.rad + shoulder.rad) * cos(rRad);
    Leverage wrist;
    wrist.rad = (WRIST_INC * wAngle + WRIST_OFFSET) * PI / 180.0;
    wrist.length = WRIST_LENGTH * cos(wrist.rad + shoulder.rad + elbow.rad) * cos(rRad);    
    Projection proj;
    proj.shoulder = shoulder;
    proj.elbow = elbow;
    proj.wrist = wrist;
    proj.length = shoulder.length + elbow.length + wrist.length;
    return proj;
}

Position::Position(const unsigned int sAngle, const unsigned int eAngle, const unsigned int wAngle, const double rAngle) {
    x = getX(sAngle, eAngle, wAngle, rAngle);
    y = getY(sAngle, eAngle, wAngle, rAngle);
    z = getZ(sAngle, eAngle, wAngle);
    shoulderAngle = sAngle;
    elbowAngle = eAngle;
    wristAngle = wAngle;
    rotateAngle = rAngle;
}

