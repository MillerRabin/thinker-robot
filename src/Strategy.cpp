#include <armParams.h>
#include <strategy.h>
#include <position.h>
#include <arm.h>
#include <Arduino.h>

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
    if (rads.size() == 0)
        return Position();    
    for (double rad : rads) {
        tShoulder.setRads(tShoulder.ZRad, rad);        
        Position pos = tryElbow(tShoulder, x, y, z, clawXAngle, clawAngle);    
        if (pos.isValid()) return pos;
    }
    return Position();
}

Position Strategy::tryShoulderRad(ArmShoulder shoulder, const double rad, const double x, const double y, const double z, const double clawXAngle, const double clawAngle) {
    ArmShoulder tShoulder = shoulder;
    tShoulder.setRads(rad, tShoulder.ZRad);    
    Serial.printf("rad: %f, shoulder.x: %f, shoulder.y: %f, shoulder.z: %f, YRad: %f, ZRad: %f\n", rad, tShoulder.x, tShoulder.y, tShoulder.z, tShoulder.YRad, tShoulder.ZRad);  
    if (!tShoulder.isValid()) {
        return Position();
    }
    Serial.printf("valid\n");  
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
    claw.setRads(wrist.ZRad, wrist.YRad, cxRad, clawRad);
    return Position(shoulder, elbow, wrist, claw);    
}

Position Strategy::tryElbow(ArmShoulder shoulder, const double x, const double y, const double z, const double clawXAngle, const double clawAngle) {
    ArmRoots roots = Strategy::getElbowRoots(shoulder, x, y, z, WRIST_LENGTH);
    Serial.printf("root1 roots.r1.x: %f, roots.r1.y: %f, roots.r1.z: %f\n", roots.r1.x, roots.r1.y, roots.r1.z);
    Serial.printf("root2 roots.r2.x: %f, roots.r2.y: %f, roots.r2.z: %f\n", roots.r2.x, roots.r2.y, roots.r2.z);
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
        errors.addBaseError(x, y, z);
        return;
    }
    
    const double sLength = ArmShoulder::getLength(x, y, z);        
    if (sLength > MAX_LENGTH) {
        errors.addMaxLengthError(sLength, MAX_LENGTH);
        return;
    }
                        
    ArmShoulder shoulder = position.shoulder;  
    const double zRad = shoulder.getZRadFromXY(x, y);
    Serial.printf("zRad is: %f\n", zRad);
    shoulder.setRads(shoulder.YRad, zRad);    
    Position pos = getArmPosition(shoulder, x, y, z, clawXAngle, clawAngle);
    addPositionToSequence(pos);
}

void Strategy::addPositionToSequence(Position pos) {
    if (!pos.isValid()) {            
        errors.addUnreachableError();
        return;    
    }
    
    const double szAngle = pos.getShoulderZAngle();
    const double syAngle = pos.getShoulderYAngle();
    const double eyAngle = pos.getElbowYAngle();
    const double wyAngle = pos.getWristYAngle();
    const double cxAngle = pos.getClawXAngle();
    const double clawAngle = pos.getClawAngle();
    
    Serial.printf("szAngle: %f, syAngle: %f, eyAngle: %f, wyAngle: %f, cxAngle: %f, clawAngle: %f\n", szAngle, syAngle, eyAngle, wyAngle, cxAngle, clawAngle);  
    
    EngineControl clawEngine(CLAW_ENGINE, clawAngle);
    sequence.push_back(clawEngine);

    EngineControl rEngine(SHOULDER_Z_ENGINE, szAngle);
    sequence.push_back(rEngine);
    
    EngineControl cxEngine(CLAW_X_ENGINE, cxAngle);
    sequence.push_back(cxEngine);

    EngineControl wEngine(WRIST_Y_ENGINE, wyAngle);
    sequence.push_back(wEngine);
    
    EngineControl eEngine(ELBOW_Y_ENGINE, eyAngle);
    sequence.push_back(eEngine);    
    
    EngineControl sEngine(SHOULDER_Y_ENGINE, syAngle);
    sequence.push_back(sEngine);
}
  
void Strategy::fixedAngle(const double x, const double y, const double z, const double clawXAngle, const double clawYAngle, const double clawAngle) {        
    const double rRad = ArmPoint::getRadFromXY(x, y);       
    const double wRad = clawYAngle / 180 * PI;
        
    const double wxl = WRIST_LENGTH * cos(wRad) * cos(rRad);
    const double wyl = WRIST_LENGTH * cos(wRad) * sin(rRad);
    const double wzl = WRIST_LENGTH * sin(wRad);
    const double ex = x - wxl;
    const double ey = y - wyl;
    const double ez = z - wzl;
    
    if (ez < MIN_Z) {
        errors.addElbowZError(ez, MIN_Z);
        return;
    }
            
    const double el = sqrt(ex*ex + ey*ey);
    const double wl = sqrt(x*x + y*y);
    if (el > SHOULDER_LENGTH + ELBOW_LENGTH) {
        errors.addMaxLengthError(el, SHOULDER_LENGTH + ELBOW_LENGTH);
        return;
    }

    if ((z <= BASE_HEIGHT) && 
        ((wl < BASE_WIDTH / 2) || (el < BASE_WIDTH / 2) || 
         (ex * x < 0) || (ey * y < 0))) {    
            errors.addBaseError(x, y, z);
            return;
    }

    
    ArmShoulder shoulder = position.shoulder;
    const double zRad = shoulder.getZRadFromXY(x, y);    
    shoulder.setRads(shoulder.YRad, zRad);
    vector<double> rads = shoulder.getAvailableRads(ELBOW_LENGTH, ex, ey, ez);
    if (rads.size() == 0) {
        errors.addShoulderError();
        return;
    }

    const double cxRad = clawXAngle / 180 * PI;    
    const double clawRad = clawAngle / 180 * PI;
    
    for(double rad : rads) {          
        shoulder.setRads(shoulder.ZRad, rad);        
        if (!shoulder.isValid()) continue;
        ArmElbow elbow = position.elbow;        
        elbow.setPos(shoulder, ex, ey, ez);
        ArmWrist wrist = position.wrist;        
        wrist.setPos(shoulder, elbow, x, y, z);
        ArmClaw claw = position.claw;
        claw.setRads(wrist.ZRad, wrist.YRad, cxRad, clawRad);

        Position pos = Position(shoulder, elbow, wrist, claw);
        if (pos.isValid()) {
            addPositionToSequence(pos);
            return;
        }        
    }   
    errors.addUnreachableError();
}

Strategy::Strategy(Position pos, const double x, const double y, const double z, const double clawXAngle, const double clawYAngle, const double clawAngle) : position(pos) {
    Serial.printf("\nx: %f, y: %f, z: %f, clawXAngle: %f, clawYAngle: %f, clawAngle: %f\n", x, y, z, clawXAngle, clawYAngle, clawAngle);
    
    if (isnan(clawYAngle)) {
        freeAngle(x, y, z, clawXAngle, clawAngle);
        return;
    }
        
    fixedAngle(x, y, z, clawXAngle, clawYAngle, clawAngle);
    return;
}


ArmRoots Strategy::getElbowRoots(ArmShoulder shoulder, const double x, const double y, const double z, const double length) {
    Serial.printf("zRad is: %f\n", shoulder.ZRad);
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