#include <Arduino.h>
#include "armParams.h"
#include "strategy.h"
#include "position.h"
#include "arm.h"
#include "armError.h"

//------ArmRoot------
ArmRoot::ArmRoot(const double ZRad, const double l, const double z) : 
    x(l * cos(ZRad)),
    y(l * sin(ZRad)),
    z(z)
{}

const bool ArmRoot::isValid() {
    return (!isnan(x)) && (!isnan(y)) && (!isnan(z));
}

//------Strategy------
Position Strategy::tryHalfLength(ArmShoulder shoulder, const double length, const double x, const double y, const double z, const double clawXAngle, const double clawAngle) {
    const double h = length / 2.0;
    const double qh = length / 4.0;
    
    const double pHalf = h + qh;    
    Position phPos = tryShoulderLength(shoulder, pHalf, x, y, z, clawXAngle, clawAngle);        
    if (phPos.isValid()) return phPos;

    const double nHalf = h - qh;    
    return tryShoulderLength(shoulder, nHalf, x, y, z, clawXAngle, clawAngle);            
}

Position Strategy::getArmPosition(ArmShoulder shoulder, const double x, const double y, const double z, const double clawXAngle, const double clawAngle) {    
    Position rPos = tryShoulderRad(shoulder, PI/2, x, y, z, clawXAngle, clawAngle);        
    if (rPos.isValid()) return rPos;
    
    double fullLength = ELBOW_LENGTH + WRIST_LENGTH;
                
    Position fPos = tryShoulderLength(shoulder, fullLength, x, y, z, clawXAngle, clawAngle);        
    if (fPos.isValid()) return fPos;
        
    Position mPos = tryShoulderLength(shoulder, ELBOW_LENGTH, x, y, z, clawXAngle, clawAngle);        
    if (mPos.isValid()) return mPos;
    
    int count = 1;    
    const double length = WRIST_LENGTH;
    for (int i = 0; i < 2; i++) {
        count = count * 2;
        const double step = length / count;
        for (int j = 1; j < count; j = j + 2) {
            const double line = ELBOW_LENGTH + (step * j);            
            Position pos = tryHalfLength(shoulder, line, x, y, z, clawXAngle, clawAngle);        
            if (pos.isValid()) return pos;
        }
    }
    
    return Position();
}

Position Strategy::tryShoulderLength(ArmShoulder shoulder, const double length, const double x, const double y, const double z, const double clawXAngle, const double clawAngle) {
    ArmShoulder tShoulder = shoulder;
    vector<double> rads = tShoulder.getAvailableRads(length, x, y, z);
    if (rads.size() == 0) {        
        return Position();    
    }
        
    for (double rad : rads) {
        tShoulder.setRads(rad, tShoulder.ZRad);        
        Position pos = tryElbow(tShoulder, x, y, z, clawXAngle, clawAngle);    
        if (pos.isValid()) return pos;
    }
    return Position();
}

Position Strategy::tryShoulderRad(ArmShoulder shoulder, const double rad, const double x, const double y, const double z, const double clawXAngle, const double clawAngle) {
    ArmShoulder tShoulder = shoulder;
    tShoulder.setRads(rad, tShoulder.ZRad);        
    if (!tShoulder.isValid()) {
        return Position();
    }   
    return tryElbow(tShoulder, x, y, z, clawXAngle, clawAngle);    
}


Position Strategy::tryElbowRoot(ArmShoulder shoulder, ArmRoot root, const double x, const double y, const double z, const double clawXAngle, const double clawAngle) {
    ArmElbow elbow = position.elbow;    
    elbow.setRads(elbow.YRad, shoulder.ZRad);    
    elbow.setPosLocal(root.x, root.y, root.z);      
    
    if (!elbow.isValid(shoulder))
        return Position();
    ArmWrist wrist = position.wrist;    
    wrist.setPos(shoulder, elbow, x, y, z);     
    if (!wrist.isValid(elbow)) {
        return Position();   
    }        
    ArmClaw claw = position.claw;
    const double cxRad = clawXAngle / 180 * PI;    
    const double clawRad = clawAngle / 180 * PI;
    claw.setRads(cxRad, wrist.YRad, wrist.ZRad, clawRad);
    return Position(shoulder, elbow, wrist, claw);    
}

Position Strategy::tryElbow(ArmShoulder shoulder, const double x, const double y, const double z, const double clawXAngle, const double clawAngle) {
    ArmRoots roots = Strategy::getElbowRoots(shoulder, x, y, z, WRIST_LENGTH);
    if (roots.r1.isValid()) {
        Position fPos = tryElbowRoot(shoulder, roots.r1, x, y, z, clawXAngle, clawAngle);
        if (fPos.isValid()) return fPos;        
    }

    if (roots.r2.isValid())      
        return tryElbowRoot(shoulder, roots.r2, x, y, z, clawXAngle, clawAngle);    
    
    return Position();        
}


void Strategy::freeAngle(const double x, const double y, const double z, const double clawXAngle, const double clawAngle) {                
    const double vl = sqrt(x*x + y*y);
    if ((z <= BASE_HEIGHT) && (vl < BASE_WIDTH / 2)) {
        position.setLastError(ERROR_OUT_OF_RANGE, ArmError::getBaseError(x, y, z));
        return;
    }
    
    const double sLength = ArmShoulder::getLength(x, y, z);        
    if (sLength > MAX_LENGTH) {
        position.setLastError(ERROR_OUT_OF_RANGE, ArmError::getMaxLengthError(sLength, MAX_LENGTH));
        return;
    }
                        
    ArmShoulder shoulder = position.shoulder;  
    const double zRad = shoulder.getZRadFromXY(x, y);    
    shoulder.setRads(shoulder.YRad, zRad);        
    Position pos = getArmPosition(shoulder, x, y, z, clawXAngle, clawAngle); 
    addPositionToSequence(pos); 
}

void Strategy::addPositionToSequence(Position pos) {
    if (!pos.isValid()) {            
        position.setLastError(ERROR_POINT_UNREACHABLE, ArmError::getUnreachableError());        
        return;    
    }
    
    const double szAngle = pos.getShoulderZAngle();
    const double syAngle = pos.getShoulderYAngle();
    const double eyAngle = pos.getElbowYAngle();
    const double wyAngle = pos.getWristYAngle();
    const double cxAngle = pos.getClawXAngle();
    const double czAngle = pos.getClawZAngle();
    const double clawAngle = pos.getClawAngle();
        
    EngineControl clawEngine(CLAW_ENGINE, clawAngle);
    sequence.push_back(clawEngine);

    EngineControl rEngine(SHOULDER_Z_ENGINE, szAngle);
    sequence.push_back(rEngine);
    
    EngineControl cxEngine(CLAW_X_ENGINE, cxAngle);
    sequence.push_back(cxEngine);

    EngineControl czEngine(CLAW_Z_ENGINE, czAngle);
    sequence.push_back(czEngine);

    EngineControl wEngine(WRIST_Y_ENGINE, wyAngle);
    sequence.push_back(wEngine);
    
    EngineControl eEngine(ELBOW_Y_ENGINE, eyAngle);
    sequence.push_back(eEngine);    
    
    EngineControl sEngine(SHOULDER_Y_ENGINE, syAngle);
    sequence.push_back(sEngine);
}
  
void Strategy::fixedAngle(const double x, const double y, const double z, const double clawXAngle, const double clawYAngle, const double clawZAngle,const double clawAngle) {                    
    const double rRad = clawZAngle / 180 * PI;    
    const double cRad = clawYAngle / 180 * PI;
    
    const double cxl = CLAW_LENGTH * cos(cRad) * cos(rRad);
    const double cyl = CLAW_LENGTH * cos(cRad) * sin(rRad);
    const double czl = CLAW_LENGTH * sin(cRad);
    const double wx = x - cxl;
    const double wy = y - cyl;
    const double wz = z - czl;
        
    const double wRad = ArmPoint::getRadFromXY(wx, wy);
    const double wxl = WRIST_LENGTH * cos(cRad) * cos(wRad);
    const double wyl = WRIST_LENGTH * cos(cRad) * sin(wRad);
    const double wzl = WRIST_LENGTH * sin(cRad);

    const double ex = wx - wxl;
    const double ey = wy - wyl;
    const double ez = wz - wzl;

    if (ez < MIN_Z) {
        position.setLastError(ERROR_POINT_UNREACHABLE, ArmError::getElbowZError(ez, MIN_Z));        
        return;
    }
            
    const double el = sqrt(ex*ex + ey*ey);
    const double cl = sqrt(x*x + y*y);

    if (el > SHOULDER_LENGTH + ELBOW_LENGTH) {        
        position.setLastError(ERROR_OUT_OF_RANGE, ArmError::getMaxLengthError(el,  SHOULDER_LENGTH + ELBOW_LENGTH + WRIST_LENGTH));        
        return;
    }

    if ((z <= BASE_HEIGHT) && 
        ((el < BASE_WIDTH / 2) || (el < BASE_WIDTH / 2) || 
         (ex * x < 0) || (ey * y < 0))) {    
            position.setLastError(ERROR_OUT_OF_RANGE, ArmError::getBaseError(x, y, z));
            return;
    }

    
    ArmShoulder shoulder = position.shoulder;
    const double zRad = shoulder.getZRadFromXY(ex, ey);    
    shoulder.setRads(shoulder.YRad, zRad);    
    vector<double> rads = shoulder.getAvailableRads(ELBOW_LENGTH, ex, ey, ez);
    if (rads.size() == 0) {
        position.setLastError(ERROR_OUT_OF_RANGE, ArmError::getShoulderError());
        return;
    }

    const double cxRad = clawXAngle / 180 * PI;    
    const double clawRad = clawAngle / 180 * PI;
    
    for(double rad : rads) {                 
        shoulder.setRads(rad, shoulder.ZRad);
        if (!shoulder.isValid()) continue;                
        ArmElbow elbow = position.elbow;        
        elbow.setRads(elbow.YRad, shoulder.ZRad);
        elbow.setPos(shoulder, ex, ey, ez);
        ArmWrist wrist = position.wrist;        
        wrist.setPos(shoulder, elbow, wx, wy, wz);
        ArmClaw claw = position.claw;
        claw.setRads(cxRad, wrist.YRad, wrist.ZRad, clawRad);
        claw.setPos(shoulder, elbow, wrist, x, y, z);
        Position pos = Position(shoulder, elbow, wrist, claw);
        if (pos.isValid()) {
            addPositionToSequence(pos);
            return;
        }        
    }   
    position.setLastError(ERROR_POINT_UNREACHABLE, ArmError::getUnreachableError());        
}

Strategy::Strategy(
    Position pos, 
    const double x, 
    const double y, 
    const double z, 
    const double clawXAngle, 
    const double clawYAngle, 
    const double clawZAngle, 
    const double clawAngle,
    const unsigned int iterations,
    const unsigned int postDelay,
    const unsigned int iterationDelay
) : 
    position(pos),
    iterations(iterations),
    postDelay(postDelay),
    iterationDelay(iterationDelay)
{        
    const double sca = pos.claw.clawRad / PI * 180;
    const double scxa = pos.claw.XRad / PI * 180;
    const double scza = pos.claw.ZRad / PI * 180;
    const double lClawAngle = isnan(clawAngle) ? sca : clawAngle;
    const double lClawXAngle = isnan(clawXAngle) ? scxa : clawXAngle;
    const double lClawZAngle = isnan(clawZAngle) ? scza : clawZAngle;

    if (isnan(clawYAngle)) {
        freeAngle(x, y, z, lClawXAngle, lClawAngle);
        return;
    }
        
    fixedAngle(x, y, z, lClawXAngle, clawYAngle, lClawZAngle, lClawAngle);
    return;
}


ArmRoots Strategy::getElbowRoots(ArmShoulder shoulder, const double x, const double y, const double z, const double length) {    
    const double sx = x - shoulder.x;
    const double sy = y - shoulder.y;
    const double sz = z - (shoulder.z + BASE_HEIGHT);
    const double qx = sx * sx;
    const double qy = sy * sy;
    const double qz = sz * sz;
    
    const double sl = (x < 0) ? - 1 * sqrt(qx + qy) : sqrt(qx + qy);
    const double ql = sl * sl;
    
    const double od = sqrt(ql + qz);
    const double qd = od * od;    
    const double R = ELBOW_LENGTH;
    const double qR = R * R;
    const double r = length;
    const double qr = r * r;
    const double rSum = R + r;
    const double rDiff = abs(R - r);
    const double tolerance = 0.00001;
    const double d = ((od < (rSum + tolerance)) && (od > (rSum - tolerance))) ? rSum : od;
        
    if ((d <= rSum) && (d > rDiff)) {
        const double b = (qr - qR + qd) / (2 * d);           
        const double a = d - b;      
        const double qa = a * a;
        const double h = sqrt(abs(qR - qa));
        const double pos0L = a / d * sl;
        const double pos0Z = a / d * sz;        
        const double l1 = pos0L + h / d * sz;
        const double z1 = pos0Z - h / d * sl;
        const double l2 = pos0L - h / d * sz;
        const double z2 = pos0Z + h / d * sl;        
        const double dPI = PI / 2;
        const double sRad = (shoulder.ZRad > dPI) ? shoulder.ZRad + PI :
                            (shoulder.ZRad > 0) ? shoulder.ZRad :
                            (shoulder.ZRad >= -dPI) ? shoulder.ZRad :
                            (shoulder.ZRad > -PI) ? shoulder.ZRad + PI:
                            shoulder.ZRad;                
        return ArmRoots(ArmRoot(sRad, l1, z1), ArmRoot(sRad, l2, z2));
    }
        
    return ArmRoots(ArmRoot(), ArmRoot());
}