#include <ArmWrist.h>
#include <ArmClaw.h>

ArmClaw::ArmClaw(ArmWrist wrist, const double clawXAngle, const double clawYAngle, const double clawZAngle, const double clawAngle) {    
    const double cxRad = getXRad(clawXAngle);    
    const double cyRad = getYRad(clawYAngle);    
    const double czRad = getZRad(clawZAngle);
    const double clawRad = getClawRad(clawAngle);     
    Serial.printf("cyRad: %f, wrist.YRad: %f\n", cyRad, wrist.YRad);
    setRadsLocal(cxRad, cyRad, czRad);    
    setRads(wrist.XRad, wrist.YRad, wrist.ZRad, clawRad);    
}

const double ArmClaw::getClawRad(const double angle) {    
    return (CLAW_SCALE * angle + CLAW_BASE) * PI / 180.0;
}

void ArmClaw::setRads(const double xRad, const double yRad, const double zRad, const double clawRad) {    
    this->YRad = yRad;
    this->ZRad = zRad;
    if (!isnan(xRad))
        this->XRad = xRad;
    if (!isnan(clawRad))
        this->clawRad = clawRad;
    setCoords();    
}

void ArmClaw::setPos(ArmShoulder shoulder, ArmElbow elbow, ArmWrist wrist, const double x, const double y, const double z) {        
    const double cx = x - (shoulder.x + elbow.x + wrist.x);
    const double cy = y - (shoulder.y + elbow.y + wrist.y);
    const double zRad = getRadFromXY(cx, cy);
    setRads(XRad, YRad, zRad, clawRad);
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


const double ArmClaw::getXAngle(const bool validate) {                 
    const double delta = YRad * ZRad;
    const double tXRad = this->XRad + delta;    
    const double angle = getXAngleFromRad(tXRad);
    if (!validate)
        return angle;
    return ArmClaw::validateXAngle(angle);    
}

const double ArmClaw::getZAngle(ArmWrist wrist, const bool validate) {         
    const double angle = getZAngleFromRad(this->ZRad - wrist.ZRad);     
    if (!validate)
        return angle;
    return ArmClaw::validateZAngle(angle);    
}

const double ArmClaw::getYAngle(ArmWrist wrist, const bool validate) {
    const double angle = getYAngleFromRad(wrist.YRad);
    if (!validate)
        return angle;
    return ArmClaw::validateYAngle(angle);    
}

const double ArmClaw::validateYAngle(const double angle) {    
    if (isnan(angle)) return ERROR_WRIST_Y_ANGLE_IS_NAN;        
    if (angle < WRIST_Y_MIN) return ERROR_WRIST_Y_ANGLE_LESS_MIN;     
    if (angle > WRIST_Y_MAX) return ERROR_WRIST_Y_ANGLE_ABOVE_MAX;            
    return angle;      
}

const double ArmClaw::validateXAngle(const double angle) {        
    if (isnan(angle)) return ERROR_CLAW_X_ANGLE_IS_NAN;                
    if (angle < CLAW_X_MIN) return ERROR_CLAW_X_ANGLE_LESS_MIN;     
    if (angle > CLAW_X_MAX) return ERROR_CLAW_X_ANGLE_ABOVE_MAX;            
    return angle;      
}

const double ArmClaw::validateZAngle(const double angle) {        
    if (isnan(angle)) return ERROR_CLAW_Z_ANGLE_IS_NAN;            
    if (angle < CLAW_Z_MIN) return ERROR_CLAW_Z_ANGLE_LESS_MIN;     
    if (angle > CLAW_Z_MAX) return ERROR_CLAW_Z_ANGLE_ABOVE_MAX;            
    return angle;      
}

const double ArmClaw::validateClawAngle(const double angle) {        
    if (isnan(angle)) return ERROR_CLAW_ANGLE_IS_NAN;            
    if (angle < CLAW_MIN) return ERROR_CLAW_ANGLE_LESS_MIN;     
    if (angle > CLAW_MAX) return ERROR_CLAW_ANGLE_ABOVE_MAX;            
    return angle;      
}
