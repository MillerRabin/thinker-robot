#include <arduino.h>
#include <math.h>
#include <Projection.h>
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
    proj.l1 = l1 * sin(rRad);
    proj.l2 = l2 * sin(rRad);
    proj.l3 = l3 * sin(rRad);    
    proj.vector = (l1 + l2 + l3) * sin(rRad);
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
    proj.l1 = l1;
    proj.l2 = l2;
    proj.l3 = l3;
    proj.vector = l1 + l2 + l3 + TOP_OFFSET;
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
    proj.l1 = l1 * sin(rRad);
    proj.l2 = l2 * sin(rRad);
    proj.l3 = l3 * sin(rRad);    
    proj.vector = (l1 + l2 + l3) * cos(rRad);
    return proj;
}

Position::Position(const double sAngle, const double eAngle, const double wAngle, const double rAngle) {
    x = getX(sAngle, eAngle, wAngle, rAngle);
    y = getY(sAngle, eAngle, wAngle, rAngle);
    z = getZ(sAngle, eAngle, wAngle);
}