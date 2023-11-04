#include <ArmWrist.h>
#include <ArmClaw.h>
#include <ArmPart.h>

ArmClaw::ArmClaw(ArmWrist wrist, const float xAngle, const float yAngle, const float zAngle, const float clawAngle) {        
    build(wrist, xAngle, yAngle, zAngle);
    this->clawRad = getClawRad(clawAngle);          
}

const float ArmClaw::getClawRad(const float angle) {    
    return (CLAW_SCALE * angle + CLAW_BASE) * PI / 180.0;
}

const float ArmClaw::getAngle(const bool validate) {     
     const float angle = getClawAngleFromRad(clawRad);
    if (!validate)
        return angle;
    return ArmClaw::validateClawAngle(angle);    
}

const float ArmClaw::getClawAngleFromRad(const float rad) {
    if (rad == NAN) return NAN;
    const float sdeg = rad / PI * 180.0;
    float sAngle = (sdeg - CLAW_BASE) / CLAW_SCALE;        
    if (sAngle < 0)
        sAngle = 360 + sAngle;
    const float tAngle = sAngle - (trunc(sAngle / 360) * 360);
    return tAngle;
}

const float ArmClaw::validateClawAngle(const float angle) {        
    if (isnan(angle)) return ERROR_CLAW_ANGLE_IS_NAN;            
    if (angle < CLAW_MIN) return ERROR_CLAW_ANGLE_LESS_MIN;     
    if (angle > CLAW_MAX) return ERROR_CLAW_ANGLE_ABOVE_MAX;            
    return angle;      
}