#include <armParams.h>
#include <Strategy.h>
#include <Position.h>

void Strategy::case1(Position pos, double x) {        
        const unsigned int angle = Position::angleFromLength(SHOULDER_LENGTH, x);        
        int dAngle = angle - pos.shoulderAngle;
        EngineControl sEngine(SHOULDER_ENGINE, angle);                
        this->sequence.push_back(sEngine);    
        const unsigned int eAngle = pos.elbowAngle + dAngle;
        EngineControl eEngine(ELBOW_ENGINE, eAngle);
        this->sequence.push_back(eEngine);    
        Serial.printf("shoulder angle %d elbow angle %d\n", angle, eAngle);
}

EngineControl::EngineControl(unsigned int engine, unsigned int angle) {
    this->engine = engine;
    this->angle = angle;
}
  
Strategy::Strategy(Position pos, double x, double y, double z) {
    const double dx = x - pos.x.lengthSum;
    const double dy = y - pos.y.lengthSum;
    const double dz = z - pos.z.lengthSum;
    const double xOffset = pos.x.shoulderLength + dx;
    const bool canShoulder = Position::isAchievable(SHOULDER_LENGTH, xOffset);
    if (canShoulder)
        this->case1(pos, xOffset);
        
    Serial.printf("l1 - %f, l2 - %f, l3 - %f, vector - %f\n", pos.x.shoulderLength, pos.x.elbowLength, pos.x.wristLength, pos.x.lengthSum);
    Serial.printf("dx - %f, dy - %f, dz - %f\n", dx, dy, dz);
}