#include <armParams.h>
#include <Strategy.h>
#include <Position.h>
#include <arm.h>


void Strategy::case1(Position pos, double x, double z) {        
    Serial.printf("source x: %f, z: %f\n", pos.getX(), pos.getZ());
    Serial.printf("target x: %f, z: %f\n", x, z);
        
    ArmRotate rotate = ArmRotate(pos.rotateAngle);
    const double rRad = rotate.XYRad;    
    ArmShoulder shoulder = ArmShoulder(rRad, pos.shoulderAngle);            
    
    const double eLength = ArmElbow::getLengthXZ(pos.shoulder, x, z);    
    Serial.printf("eLength: %f\n", eLength);    
    const double wLength = ArmWrist::getLengthXZ(pos.shoulder, pos.elbow, x, z);
    Serial.printf("wLength: %f\n", wLength);    
    const double lSum = ELBOW_LENGTH + WRIST_LENGTH;
    Serial.printf("eLength: %f, lsum: %f\n", eLength, lSum);        
    if (lSum < eLength) { 
        shoulder.setToLength(lSum, x, z);        
        const double eLength = ArmElbow::getLengthXZ(shoulder, x, z);    
        Serial.printf("new eLength: %f\n", eLength);    
    }
        
    ArmElbow elbow = ArmElbow(rRad, shoulder.XZRad, pos.elbowAngle);
    elbow.setToLength(shoulder, x, z, WRIST_LENGTH);        
    ArmWrist wrist = ArmWrist(rRad, elbow.XZRad, pos.wristAngle);
    wrist.setPos(shoulder, elbow, x, z);
    Serial.printf("shoulder.XZRad: %f, shoulder x: %f, shoulder z: %f\n", shoulder.XZRad, shoulder.x, shoulder.z);
    Serial.printf("elbow.XZRad: %f, elbow x: %f, elbow z: %f\n", elbow.XZRad, elbow.x, elbow.z);
    Serial.printf("wrist.XZRad: %f, wrist x: %f, wrist z: %f\n", wrist.XZRad, wrist.x, wrist.z);

    const double dsAngle = shoulder.getAngleXZ();
    const double deAngle = elbow.getAngleXZ(shoulder);
    const double dwAngle = wrist.getAngleXZ(elbow);
    
    Serial.printf("shoulder angle: %f, elbow angle: %f, wrist angle %f\n", dsAngle, deAngle, dwAngle);
        
    if (isnan(dsAngle)) 
        errors.push_back("Shoulder angle is NaN");
    
    if (isnan(deAngle))
        errors.push_back("Elbow angle is NaN");
        
    if (isnan(dwAngle))
        errors.push_back("Wrist angle is NaN");

    if (errors.size() > 0) return;
        
    EngineControl wEngine(WRIST_ENGINE, round(dwAngle));
    this->sequence.push_back(wEngine);
    
    EngineControl eEngine(ELBOW_ENGINE, round(deAngle));
    this->sequence.push_back(eEngine);    
    
    EngineControl sEngine(SHOULDER_ENGINE, round(dsAngle));
    this->sequence.push_back(sEngine);
    
}

EngineControl::EngineControl(unsigned int engine, unsigned int angle) {
    this->engine = engine;
    this->angle = angle;
}
  
Strategy::Strategy(Position pos, double x, double y, double z) {
    this->case1(pos, x, z);
}