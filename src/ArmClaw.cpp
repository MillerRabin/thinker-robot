#include <ArmWrist.h>
#include <ArmClaw.h>
#include <ArmPart.h>

ArmClaw::ArmClaw(ArmWrist wrist, const double xAngle, const double yAngle, const double zAngle, const double clawAngle) {        
    build(wrist, xAngle, yAngle, zAngle);
    this->clawRad = getClawRad(clawAngle);          
}

const double ArmClaw::getClawRad(const double angle) {    
    return (CLAW_SCALE * angle + CLAW_BASE) * PI / 180.0;
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

const double ArmClaw::validateClawAngle(const double angle) {        
    if (isnan(angle)) return ERROR_CLAW_ANGLE_IS_NAN;            
    if (angle < CLAW_MIN) return ERROR_CLAW_ANGLE_LESS_MIN;     
    if (angle > CLAW_MAX) return ERROR_CLAW_ANGLE_ABOVE_MAX;            
    return angle;      
}