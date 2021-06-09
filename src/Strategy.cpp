#include <armParams.h>
#include <Strategy.h>
#include <Position.h>
#include <arm.h>


void Strategy::case1(Position pos, double x, double z) {        
    const double dx = x - pos.x.length;    
    //const double dz = z - pos.z.length;
    ArmRotate rotate = ArmRotate(pos.rotateAngle);
    const double rRad = rotate.XYRad;    
    ArmShoulder shoulder = ArmShoulder(rRad, pos.shoulderAngle);            
    const double sz = shoulder.z;    
    const double sRad = shoulder.XZRad;
    shoulder.addX(dx);            
    const double dsz = shoulder.z - sz;    
    const unsigned int sAngle = shoulder.getAngleXZ();    
    Serial.printf("shoulder angle: %d\n", sAngle);    
    ArmElbow elbow = ArmElbow(sRad, rRad, pos.elbowAngle);            
    const double eRad = elbow.XZRad;
    elbow.addZ(shoulder.XZRad, -dsz);
    const unsigned int eAngle = elbow.getAngleXZ();
    Serial.printf("elbow z: %f, dsz: %f\n", elbow.z, dsz);
    Serial.printf("elbow angle: %d\n", eAngle);
    Serial.printf("Wrist angle %d\n", pos.wristAngle);
    ArmWrist sourceWrist = ArmWrist(sRad, eRad, rRad, pos.wristAngle);    
    ArmWrist targetWrist = ArmWrist(shoulder.XZRad, elbow.XZRad, rRad, pos.wristAngle);    
    Serial.printf("source wrist.x: %f, target wrist.x: %f\n", sourceWrist.x, targetWrist.x);    
    targetWrist.setX(shoulder.XZRad, elbow.XZRad, sourceWrist.x);
    const unsigned int wAngle = targetWrist.getAngleXZ();
    Serial.printf("target Wrist angle %d\n", wAngle);
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