#include <armParams.h>
#include <arduino.h>
#include <math.h>
#include <Position.h>


Projection Position::getX(const double sAngle, const double eAngle, const double wAngle, const double rAngle) {    
    const double sRad = (SHOULDER_INC * sAngle + SHOULDER_OFFSET) * PI / 180.0;
    const double l1 = SHOULDER_LENGTH * cos(sRad);                
    const double eRad = (ELBOW_INC * eAngle + ELBOW_OFFSET) * PI / 180.0;
    const double l2 = ELBOW_LENGTH * cos(eRad + sRad);                    
    const double wRad = (WRIST_INC * wAngle + WRIST_OFFSET) * PI / 180.0;
    const double l3 = WRIST_LENGTH * cos(wRad + sRad + eRad);        
    const double rRad = (rAngle + ROTATE_OFFSET) * PI / 180.0;    
    Projection proj;
    proj.shoulderLength = l1 * sin(rRad);
    proj.elbowLength = l2 * sin(rRad);
    proj.wristLength = l3 * sin(rRad);    
    proj.lengthSum = (l1 + l2 + l3) * sin(rRad);
    return proj;
}

Projection Position::getZ(const double sAngle, const double eAngle, const double wAngle) {
    const double sRad = (SHOULDER_INC * sAngle + SHOULDER_OFFSET) * PI / 180.0;
    const double l1 = SHOULDER_LENGTH * sin(sRad);                
    const double eRad = (ELBOW_INC * eAngle + ELBOW_OFFSET) * PI / 180.0;
    const double l2 = ELBOW_LENGTH * sin(eRad + sRad);                    
    const double wRad = (WRIST_INC * wAngle + WRIST_OFFSET) * PI / 180.0;
    const double l3 = WRIST_LENGTH * sin(wRad + sRad + eRad);
    Projection proj;
    proj.shoulderLength = l1;
    proj.elbowLength = l2;
    proj.wristLength = l3;
    proj.lengthSum = l1 + l2 + l3 + TOP_OFFSET;
    return proj;
}

Projection Position::getY(const double sAngle, const double eAngle, const double wAngle, const double rAngle) {
    const double sRad = (SHOULDER_INC * sAngle + SHOULDER_OFFSET) * PI / 180.0;
    const double l1 = SHOULDER_LENGTH * cos(sRad);                
    const double eRad = (ELBOW_INC * eAngle + ELBOW_OFFSET) * PI / 180.0;
    const double l2 = ELBOW_LENGTH * cos(eRad + sRad);                    
    const double wRad = (WRIST_INC * wAngle + WRIST_OFFSET) * PI / 180.0;
    const double l3 = WRIST_LENGTH * cos(wRad + sRad + eRad);        
    const double rRad = (rAngle + ROTATE_OFFSET) * PI / 180.0;    
    Projection proj;
    proj.shoulderLength = l1 * sin(rRad);
    proj.elbowLength = l2 * sin(rRad);
    proj.wristLength = l3 * sin(rRad);    
    proj.lengthSum = (l1 + l2 + l3) * cos(rRad);
    return proj;
}

Position::Position(const unsigned int sAngle, const unsigned int eAngle, const unsigned int wAngle, const double rAngle) {
    x = getX(sAngle, eAngle, wAngle, rAngle);
    y = getY(sAngle, eAngle, wAngle, rAngle);
    z = getZ(sAngle, eAngle, wAngle);
    shoulderAngle = sAngle;
    elbowAngle = eAngle;
    wristAngle = wAngle;
}

double Position::angleFromLength(double leverage, double length) {    
    const double dl = length / leverage;    
    const double rad = acos(dl);
    return rad * 180 / PI;    
}

bool Position::isAchievable(double leverage, double target) {    
    return leverage >= target;
}