#include <armParams.h>
#include <Strategy.h>
#include <Position.h>


void Strategy::case1(Position pos, double x, double z) {        
    const double dx = x - pos.x.length;    
    const double dz = z - pos.z.length;
    const double rRad = Position::getRotateRad(pos.rotateAngle);        
    const unsigned int sAngle = Position::getShoulderAngleFromX(dx, rRad);
    Serial.printf("x: %f, dx: %f, z: %f, dz: %f, Shoulder angle: %d\n", x, dx, z, dz, sAngle);
    Leverage sz = Position::getShoulderZ(pos.shoulderAngle);
    Leverage tz = Position::getShoulderZ(sAngle);
    const double dsz = tz.length - sz.length;
    Serial.printf("shoulder source: z %f, target z: %f, delta z: %f\n", sz.length, tz.length, dsz);
    Leverage sez = Position::getElbowZ(sz, pos.elbowAngle);
    Leverage tez = Position::getElbowZ(tz, pos.elbowAngle);                
    const double dez = sez.length - dsz;
    Serial.printf("elbow delta z: %f\n", dez);
    const double eAngle = Position::getElbowAngleFromZ(tz, dez);
    Serial.printf("sez.length: %f, tez.length %f, dz: %f, eAngle: %f\n", sez.length, tez.length, dez, eAngle);
    Leverage swz = Position::getWristX(sz, sez, rRad, pos.wristAngle);
    Leverage twz = Position::getWristX(tz, tez, rRad, pos.wristAngle);
    Serial.printf("source wrist.length %f, wrist.rad %f\n", swz.length, swz.rad);
    Serial.printf("target wrist.length %f, wrist.rad %f\n", twz.length, twz.rad);
    const double dwz = swz.length - dx;
    Serial.printf("Wrist delta: %f\n", dwz);
    const double swAngle = Position::getWristAngleFromX(sz, sez, swz.length);
    const double wAngle = Position::getWristAngleFromX(tz, tez, twz.length);
    Serial.printf("source wAngle: %f\n", swAngle);
    Serial.printf("target wAngle: %f\n", wAngle);
    EngineControl sEngine(SHOULDER_ENGINE, sAngle);
    EngineControl eEngine(ELBOW_ENGINE, eAngle);
    this->sequence.push_back(sEngine);
    this->sequence.push_back(eEngine);
    Serial.printf("elbow source z %f, target z %f\n", sez.length, tez.length);                    
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