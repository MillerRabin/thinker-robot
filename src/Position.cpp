#include <armParams.h>
#include <arduino.h>
#include <math.h>
#include <Position.h>


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

double Position::getShoulderAngleFromX(const double x, const double rRad) {                
    const double trad = x / SHOULDER_LENGTH / sin(rRad);    
    const double srad = acos(trad);
    const double sdeg = srad / PI * 180.0;
    const double sAngle = (sdeg - SHOULDER_OFFSET) / SHOULDER_INC;    
    return sAngle;
}


double Position::getWristAngleFromX(Leverage shoulder, Leverage elbow, const double x) {
    const double tsum = x / WRIST_LENGTH;    
    const double asum = acos(tsum);
    const double wrad  = asum;
    const double wdeg = wrad / PI * 180.0;
    const double wAngle = (wdeg + WRIST_OFFSET + 90);
    Serial.printf("sum: %f, asum: %F, wrad: %f, wdeg: %f, wAngle: %f\n", tsum, asum, wrad, wdeg, wAngle);
    return wAngle;
}


Leverage Position::getElbowZ(Leverage shoulder, const double eAngle) {
    Leverage elbow;
    elbow.rad = (ELBOW_INC * eAngle + ELBOW_OFFSET) * PI / 180.0;
    elbow.length = ELBOW_LENGTH * sin(elbow.rad + shoulder.rad);                    
    return elbow;
}

double Position::getElbowAngleFromZ(Leverage shoulder, const double z) {
    const double tsum = z / ELBOW_LENGTH;    
    const double asum = asin(tsum);
    const double erad  = asum - shoulder.rad;
    const double edeg = erad / PI * 180.0;
    const double eAngle = (edeg - ELBOW_OFFSET) * ELBOW_INC;
    Serial.printf("shoulder: %f, sum: %f, asum: %F, erad: %f, edeg: %f, eAngle: %f\n", shoulder.rad, tsum, asum, erad, edeg, eAngle);
    return eAngle;
}

Leverage Position::getWristX(Leverage shoulder, Leverage elbow, const double rRad, const double wAngle) {
    Leverage wrist;
    wrist.rad = (WRIST_INC * wAngle + WRIST_OFFSET) * PI / 180.0;
    wrist.length = WRIST_LENGTH * cos(wrist.rad + elbow.rad + shoulder.rad) * sin(rRad);
    return wrist;
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