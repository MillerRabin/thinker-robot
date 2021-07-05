#include <armParams.h>
#include <strategy.h>
#include <position.h>
#include <arm.h>

//------ArmRoot------
ArmRoot::ArmRoot(const double XYRad, const double l, const double z) : 
    x(l * cos(XYRad)),
    y(l * sin(XYRad)),
    z(z)
{}

const bool ArmRoot::isValid() {
    return (!isnan(x)) && (!isnan(y)) && (!isnan(z));
}

//------Position------

Position Strategy::getArmPosition(ArmRotate rotate, ArmShoulder shoulder, const double x, const double y, const double z) {
    Serial.printf("try pi/2\n");        
    Position rPos = tryShoulderRad(rotate, shoulder, PI/2, x, y, z);        
    if (rPos.isValid()) return rPos;    
    const double eLength = ArmElbow::getLength(shoulder, x, y, z);
    const double lSum = ELBOW_LENGTH + WRIST_LENGTH;
    Serial.printf("eLength: %f, lsum: %f\n", eLength, lSum);        
    Serial.printf("try eLength\n");        
    Position fPos = tryShoulderLength(rotate, shoulder, eLength, x, y, z);        
    if (fPos.isValid()) return fPos;
    Serial.printf("eLength is Invalid.\nTry lsum\n");        
    Position sPos = tryShoulderLength(rotate, shoulder, lSum, x, y, z);
    if (sPos.isValid()) return sPos;
    Serial.printf("lSum is Invalid.\nTry ELBOW_LENGTH\n");        
    Position tPos = tryShoulderLength(rotate, shoulder, ELBOW_LENGTH, x, y, z);
    if (tPos.isValid()) return tPos;
    const double dvis = lSum - ((lSum - eLength) / 2);
    Serial.printf("eLength is Invalid.\nTry dvis: %f\n", dvis);        
    Position fourthPos = tryShoulderLength(rotate, shoulder, dvis, x, y, z);
    return fourthPos;
}

Position Strategy::tryShoulderLength(ArmRotate rotate, ArmShoulder shoulder, const double length, const double x, const double y, const double z) {
    ArmShoulder tShoulder = shoulder;
    tShoulder.setAvailableLength(length, x, y, z);
    const double sAngle = tShoulder.getAngle(false);
    Serial.printf("Start Shoulder angle is: %f\n", sAngle);    
    if (!tShoulder.isValid()) {
        Serial.printf("Shoulder is invalid\n");    
        return Position();
    }
    return tryElbow(rotate, tShoulder, x, y, z);    
}

Position Strategy::tryShoulderRad(ArmRotate rotate, ArmShoulder shoulder, const double rad, const double x, const double y, const double z) {
    ArmShoulder tShoulder = shoulder;
    tShoulder.setXZRad(rad);
    const double sAngle = tShoulder.getAngle(false);
    Serial.printf("Start Shoulder angle is: %f\n", sAngle);    
    if (!tShoulder.isValid()) {
        Serial.printf("Shoulder is invalid\n");    
        return Position();
    }
    return tryElbow(rotate, tShoulder, x, y, z);    
}


Position Strategy::tryElbowRoot(ArmRotate rotate, ArmShoulder shoulder, ArmRoot root, const double x, const double y, const double z) {
    ArmElbow elbow = position.elbow;
    elbow.setRotate(rotate);
    elbow.setPos(root.x, root.y, root.z);    
    const double swAngle = position.getWristAngle();    
    ArmWrist wrist = ArmWrist(elbow, swAngle);
    wrist.setPos(shoulder, elbow, x, y, z);    
    Serial.printf("shoulder.XZRad: %f, shoulder.XYRad: %f, shoulder x: %f, shoulder z: %f, angle: %f\n", shoulder.XZRad, shoulder.XYRad, shoulder.x, shoulder.z, shoulder.getAngle(false));
    Serial.printf("elbow.XZRad: %f, elbow.XYRad: %f, elbow x: %f, elbow z: %f, angle: %f\n", elbow.XZRad, elbow.XYRad, elbow.x, elbow.z, elbow.getAngle(shoulder, false));
    Serial.printf("wrist.XZRad: %f, wrist.XYRad: %f, wrist x: %f, wrist z: %f, angle: %f\n", wrist.XZRad, wrist.XYRad, wrist.x, wrist.z, wrist.getAngle(elbow, false));    
    return Position(rotate, shoulder, elbow, wrist, position.claw);    
}

Position Strategy::tryElbow(ArmRotate rotate, ArmShoulder shoulder, const double x, const double y, const double z) {
    const double sx = shoulder.x;
    const double sy = shoulder.y;
    const double sz = shoulder.z + BASE_HEIGHT;
    Serial.printf("sx: %f, sy: %f, sz: %f, R: %d\n", sx, sy, sz, ELBOW_LENGTH);
    Serial.printf("x: %f, y: %f, z: %f, r: %d\n", x, y, z, WRIST_LENGTH);
        
    ArmRoots roots = Strategy::getElbowRoots(shoulder, x, y, z, WRIST_LENGTH);
    const double x1 = roots.r1.x + shoulder.x;
    const double y1 = roots.r1.y + shoulder.y;
    const double z1 = roots.r1.z + shoulder.z + BASE_HEIGHT;
    const double x2 = roots.r2.x + shoulder.x;
    const double y2 = roots.r2.y + shoulder.y;
    const double z2 = roots.r2.z + shoulder.z + BASE_HEIGHT;    
    Serial.printf("roots r1x: %f, r1y: %f, r1z: %f, r2x: %f, r2y: %f, r2z: %f\n", roots.r1.x, roots.r1.y, roots.r1.z, roots.r2.x, roots.r2.y, roots.r2.z);
    Serial.printf("r1x: %f, r1y: %f, r1z: %f, r2x: %f, r2y: %f, r2z: %f\n", x1, y1, z1, x2, y2, z2);
    
    if (roots.r1.isValid()) {
        Serial.printf("Try root 1\n");
        Position fPos = tryElbowRoot(rotate, shoulder, roots.r1, x, y, z);
        if (fPos.isValid()) return fPos;        
    }

    if (roots.r2.isValid()) {
        Serial.printf("Root 1 is invalid.\nTry root 2\n");
        return tryElbowRoot(rotate, shoulder, roots.r2, x, y, z);    
    }

    return Position();        
}


void Strategy::freeAngle(const double x, const double y, const double z) {        
    Serial.printf("\nSTART: source x: %f, y: %f, z: %f\n", position.getX(), position.getY(), position.getZ());
    Serial.printf("target x: %f, y: %f, z: %f\n", x, y, z);    
    const double vl = sqrt(x*x + y*y);
    if ((z <= BASE_HEIGHT) && (vl < BASE_WIDTH / 2)) {
        char sx[25]; 
        dtostrf(x, 1, 3, sx);
        char sy[25]; 
        dtostrf(y, 1, 3, sy);
        char sz[25]; 
        dtostrf(z, 1, 3, sz);
        string rString = string("EOUTOFRANGE: The Arm can`t move through it`s base. x: ") + sx + string(", y: ") + sy + string(", z: ") + sz;        
        errors.push_back(rString);
        return;
    }
    
    const double sLength = ArmShoulder::getLength(x, y, z);        
    if (sLength > MAX_LENGTH) {
        char ss[25]; 
        dtostrf(sLength, 1, 3, ss);
        char ms[25]; 
        dtostrf(MAX_LENGTH, 1, 3, ms);
        string rString = string("EOUTOFRANGE: shoulder length: ") + ss + string(" is out of range. Max length is: ") + ms;        
        errors.push_back(rString);
        return;
    }
    
            
    ArmRotate rotate = position.rotate;
    rotate.setRadFromPos(x, y);
    Serial.printf("Rotate rad is: %f\n", rotate.XYRad);
    ArmShoulder shoulder = position.shoulder;
    shoulder.setRotate(rotate);
    Position fPos = getArmPosition(rotate, shoulder, x, y, z);
    if (!fPos.isValid()) {            
        string rString = "EUNREACHABLE: The point is unreachable";        
        errors.push_back(rString);
        return;    
    }

    Serial.printf("fPos.rotate.XYRad is: %f\n", fPos.rotate.XYRad);
    const double drAngle = fPos.getRotateAngle();
    const double dsAngle = fPos.getShoulderAngle();
    const double deAngle = fPos.getElbowAngle();
    const double dwAngle = fPos.getWristAngle();

    Serial.printf("Rotate angle: %f, Shoulder angle: %f, elbow angle: %f, wrist angle: %f\n", drAngle, dsAngle, deAngle, dwAngle);    
            
    EngineControl rEngine(ROTATE_ENGINE, round(drAngle));
    this->sequence.push_back(rEngine);
    
    EngineControl wEngine(WRIST_ENGINE, round(dwAngle));
    this->sequence.push_back(wEngine);
    
    EngineControl eEngine(ELBOW_ENGINE, round(deAngle));
    this->sequence.push_back(eEngine);    
    
    EngineControl sEngine(SHOULDER_ENGINE, round(dsAngle));
    this->sequence.push_back(sEngine);
}
  
Strategy::Strategy(Position pos, const double x, const double y, const double z) : position(pos) {
    freeAngle(x, y, z);
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
        return ArmRoots(ArmRoot(shoulder.XYRad, l1, z1), ArmRoot(shoulder.XYRad, l2, z2));
    }
        
    return ArmRoots(ArmRoot(), ArmRoot());
}