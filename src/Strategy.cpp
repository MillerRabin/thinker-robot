#include <armParams.h>
#include <Strategy.h>
#include <Position.h>
#include <arm.h>


void Strategy::case1(Position pos, double x, double z) {        
    Serial.printf("source x: %f, z: %f\n", pos.x.length, pos.z.length);
    Serial.printf("target x: %f, z: %f\n", x, z);
        
    ArmRotate rotate = ArmRotate(pos.rotateAngle);
    const double rRad = rotate.XYRad;    
    ArmShoulder shoulder = ArmShoulder(rRad, pos.shoulderAngle);            
    ArmElbow elbow = ArmElbow(shoulder.XZRad, rRad, pos.elbowAngle);
    
    const double sLength = ArmShoulder::getLengthXZ(x, z);
    Serial.printf("sLength: %f\n", sLength);    
    const double eLength = ArmElbow::getLengthXZ(shoulder, x, z);    
    Serial.printf("eLength: %f\n", eLength);    
    const double wLength = ArmWrist::getLengthXZ(shoulder, elbow, x, z);
    Serial.printf("wLength: %f\n", wLength);    
    const double lSum = ELBOW_LENGTH + WRIST_LENGTH;
    Serial.printf("eLength: %f, lsum: %f\n", eLength, lSum);    


    elbow.setToLength(shoulder, x, z, WRIST_LENGTH);        
    Serial.printf("source shoulder x: %f, shoulder z: %f\n", shoulder.x, shoulder.z);
    Serial.printf("source elbow x: %f, elbow z: %f\n", elbow.x, elbow.z);    
    ArmWrist wrist = ArmWrist(shoulder.XZRad, elbow.XZRad, rRad, pos.wristAngle);    
    wrist.setZ(shoulder, elbow, z);
    Serial.printf("shoulder x: %f, shoulder z: %f\n", shoulder.x, shoulder.z);
    Serial.printf("elbow x: %f, elbow z: %f\n", elbow.x, elbow.z);
    Serial.printf("wrist x: %f, wrist z: %f\n", wrist.x, wrist.z);

    const unsigned int sAngle = shoulder.getAngleXZ();
    const unsigned int wAngle = wrist.getAngleXZ();    
    const unsigned int eAngle = elbow.getAngleXZ();
    Serial.printf("shoulder angle: %d, elbow angle: %d, wrist angle %d\n", sAngle, eAngle, wAngle);

    EngineControl sEngine(SHOULDER_ENGINE, sAngle);
    EngineControl eEngine(ELBOW_ENGINE, eAngle);
    EngineControl wEngine(WRIST_ENGINE, wAngle);
    this->sequence.push_back(sEngine);
    this->sequence.push_back(eEngine);
    this->sequence.push_back(wEngine);
}

EngineControl::EngineControl(unsigned int engine, unsigned int angle) {
    this->engine = engine;
    this->angle = angle;
}
  
Strategy::Strategy(Position pos, double x, double y, double z) {
    //const double dx = x - pos.x.length;
    //const double dy = y - pos.y.length;
    //const double dz = z - pos.z.length;
    //const double xOffset = pos.x.shoulder.length + dx;
    //const double zOffset = pos.z.shoulder.length + dz;           
    this->case1(pos, x, z);
        
    //Serial.printf("l1 - %f, l2 - %f, l3 - %f, vector - %f\n", pos.x.shoulder.length, pos.x.elbow.length, pos.x.wrist.length, pos.x.length);
    //Serial.printf("dx - %f, dy - %f, dz - %f\n", dx, dy, dz);
}