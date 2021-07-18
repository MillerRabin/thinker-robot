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
Position Strategy::tryHalfLength(ArmRotate rotate, ArmShoulder shoulder, const double length, const double x, const double y, const double z) {
    const double h = length / 2.0;
    const double qh = length / 4.0;
    
    const double pHalf = h + qh;    
    Position phPos = tryShoulderLength(rotate, shoulder, pHalf, x, y, z);        
    if (phPos.isValid()) return phPos;

    const double nHalf = h - qh;    
    return tryShoulderLength(rotate, shoulder, nHalf, x, y, z);            
}

Position Strategy::getArmPosition(ArmRotate rotate, ArmShoulder shoulder, const double x, const double y, const double z) {    
    Position rPos = tryShoulderRad(rotate, shoulder, PI/2, x, y, z);        
    if (rPos.isValid()) return rPos;
    double fullLength = ELBOW_LENGTH + WRIST_LENGTH;
                
    Position fPos = tryShoulderLength(rotate, shoulder, fullLength, x, y, z);        
    if (fPos.isValid()) return fPos;
        
    Position mPos = tryShoulderLength(rotate, shoulder, ELBOW_LENGTH, x, y, z);        
    if (mPos.isValid()) return mPos;
    
    int count = 1;    
    const double length = WRIST_LENGTH;
    for (int i = 0; i < 2; i++) {
        count = count * 2;
        const double step = length / count;
        for (int j = 1; j < count; j = j + 2) {
            const double line = ELBOW_LENGTH + (step * j);            
            Position pos = tryHalfLength(rotate, shoulder, line, x, y, z);        
            if (pos.isValid()) return pos;
        }
    }

    return Position();
}

Position Strategy::tryShoulderLength(ArmRotate rotate, ArmShoulder shoulder, const double length, const double x, const double y, const double z) {
    ArmShoulder tShoulder = shoulder;
    vector<double> rads = tShoulder.getAvailableRads(length, x, y, z);
    if (rads.size() == 0)
        return Position();    
    for (double rad : rads) {
        tShoulder.setRads(tShoulder.ZRad, rad);        
        Position pos = tryElbow(rotate, tShoulder, x, y, z);    
        if (pos.isValid()) return pos;
    }
    return Position();
}

Position Strategy::tryShoulderRad(ArmRotate rotate, ArmShoulder shoulder, const double rad, const double x, const double y, const double z) {
    ArmShoulder tShoulder = shoulder;
    tShoulder.setYRad(rad);
    
    if (!tShoulder.isValid()) {
        return Position();
    }
    return tryElbow(rotate, tShoulder, x, y, z);    
}


Position Strategy::tryElbowRoot(ArmRotate rotate, ArmShoulder shoulder, ArmRoot root, const double x, const double y, const double z) {
    ArmElbow elbow = position.elbow;
    elbow.setRotate(rotate);
    elbow.setPosLocal(root.x, root.y, root.z);  
    if (!elbow.isValid(shoulder)) {        
        return Position();
    }
    const double swAngle = position.getWristAngle();    
    ArmWrist wrist = ArmWrist(elbow, swAngle);
    wrist.setPos(shoulder, elbow, x, y, z); 
    if (!wrist.isValid(elbow)) {
        return Position();   
    }
    return Position(rotate, shoulder, elbow, wrist, position.claw);    
}

Position Strategy::tryElbow(ArmRotate rotate, ArmShoulder shoulder, const double x, const double y, const double z) {
    ArmRoots roots = Strategy::getElbowRoots(shoulder, x, y, z, WRIST_LENGTH);
    if (roots.r1.isValid()) {
        Position fPos = tryElbowRoot(rotate, shoulder, roots.r1, x, y, z);
        if (fPos.isValid()) return fPos;        
    }

    if (roots.r2.isValid()) {        
        return tryElbowRoot(rotate, shoulder, roots.r2, x, y, z);    
    }

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
                
    ArmRotate rotate = position.rotate;
    rotate.setRadFromPos(x, y);
    ArmShoulder shoulder = position.shoulder;
    shoulder.setRotate(rotate);
    Position pos = getArmPosition(rotate, shoulder, x, y, z);
    addPositionToSequence(pos);
}

void Strategy::addPositionToSequence(Position pos) {
    if (!pos.isValid()) {            
        errors.addUnreachableError();
        return;    
    }
    
    const double drAngle = pos.getRotateAngle();
    const double dsAngle = pos.getShoulderAngle();
    const double deAngle = pos.getElbowAngle();
    const double dwAngle = pos.getWristAngle();
    const double cxAngle = pos.getClawXAngle();
    const double clawAngle = pos.getClawAngle();
    
    EngineControl clawEngine(CLAW_ENGINE, round(clawAngle));
    sequence.push_back(clawEngine);

    EngineControl rEngine(ROTATE_ENGINE, round(drAngle));
    sequence.push_back(rEngine);
    
    EngineControl cxEngine(CLAW_X_ENGINE, round(cxAngle));
    sequence.push_back(cxEngine);

    EngineControl wEngine(WRIST_ENGINE, round(dwAngle));
    sequence.push_back(wEngine);
    
    EngineControl eEngine(ELBOW_ENGINE, round(deAngle));
    sequence.push_back(eEngine);    
    
    EngineControl sEngine(SHOULDER_ENGINE, round(dsAngle));
    sequence.push_back(sEngine);
}
  
void Strategy::fixedAngle(const double x, const double y, const double z, const double clawXAngle, const double clawYAngle, const double clawAngle) {        
    ArmRotate rotate = position.rotate;
    rotate.setRadFromPos(x, y);
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
    shoulder.setRotate(rotate);
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
        elbow.setRotate(rotate);
        elbow.setPos(shoulder, ex, ey, ez);
        ArmWrist wrist = position.wrist;
        wrist.setRotate(rotate);
        wrist.setPos(shoulder, elbow, x, y, z);
        ArmClaw claw = position.claw;
        claw.setRads(wrist.ZRad, wrist.YRad, cxRad, clawRad);

        Position pos = Position(rotate, shoulder, elbow, wrist, claw);
        if (pos.isValid()) {
            addPositionToSequence(pos);
            return;
        }        
    }   
    errors.addUnreachableError();
}

Strategy::Strategy(Position pos, const double x, const double y, const double z, const double clawXAngle, const double clawYAngle, const double clawAngle) : position(pos) {
    if (isnan(clawYAngle)) {
        freeAngle(x, y, z, clawXAngle, clawAngle);
        return;
    }
        
    fixedAngle(x, y, z, clawXAngle, clawYAngle, clawAngle);
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