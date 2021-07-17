#ifndef strategy_h
#define strategy_h

#include <position.h>
#include <Arduino.h>
#include "armError.h"

class ArmRoot {
    public:    
        const double x;
        const double y;
        const double z;        
        const bool isValid();
        ArmRoot(const double ZRad = NAN, const double l = NAN, const double z = NAN);
};

class ArmRoots {
    public:    
        ArmRoot r1;
        ArmRoot r2;
        ArmRoots(ArmRoot r1,ArmRoot r2) : r1(r1), r2(r2) {};
};

class EngineControl {
    public:
        const unsigned int engine;
        const unsigned int angle;
        EngineControl(const unsigned int engine, const unsigned int angle) : engine(engine), angle(angle) {};
};

class Strategy {
    private:
        void freeAngle(const double x, const double y, const double z);
        void fixedAngle(const double x, const double y, const double z, const double clawXAngle, const double clawYAngle);
        static ArmRoots getElbowRoots(ArmShoulder shoulder, const double x, const double y, const double z, const double length);
        static ArmRoot getValidElbowRoot(ArmElbow elbow, ArmShoulder shoulder, ArmRoots roots);
        Position tryElbowRoot(ArmRotate rotate, ArmShoulder shoulder, ArmRoot root, const double x, const double y, const double z);
        Position tryElbow(ArmRotate rotate, ArmShoulder shoulder, const double x, const double y, const double z);                
        Position tryShoulderLength(ArmRotate rotate, ArmShoulder shoulder, const double length, const double x, const double y, const double z);
        Position tryHalfLength(ArmRotate rotate, ArmShoulder shoulder, const double length, const double x, const double y, const double z);
        Position tryShoulderRad(ArmRotate rotate, ArmShoulder shoulder, const double rad, const double x, const double y, const double z);
        Position getArmPosition(ArmRotate rotate, ArmShoulder shoulder, const double x, const double y, const double z);
        void addPositionToSequence(Position pos);
        Position position;
    public:
        Strategy(Position pos, const double x, const double y, const double z, const double clawXAngle, const double clawYAngle);
        std::vector<EngineControl> sequence;
        ArmError errors;
};

#endif
