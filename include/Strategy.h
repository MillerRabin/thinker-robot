#ifndef strategy_h
#define strategy_h

#include <Arduino.h>
#include "arm.h"
#include "position.h"
#include "ArmParams.h"

//------EngineControl------

class EngineControl {
    public:
        const unsigned int engine;
        const double angle;
        EngineControl(const unsigned int engine, const double angle) : engine(engine), angle(angle) {};
};

//------ArmRoot------

class ArmRoot {
    public:    
        const double x;
        const double y;
        const double z;        
        const bool isValid();
        ArmRoot(const double ZRad = NAN, const double l = NAN, const double z = NAN);
};

//------ArmRoots------

class ArmRoots {
    public:    
        ArmRoot r1;
        ArmRoot r2;
        ArmRoots(ArmRoot r1,ArmRoot r2) : r1(r1), r2(r2) {};
};

//------Strategy------

class Strategy {
    private:
        void freeAngle(const double x, const double y, const double z, const double clawXAngle, const double clawAngle);
        void fixedAngle(const double x, const double y, const double z, const double clawXAngle, const double clawYAngle, const double clawAngle);
        static ArmRoots getElbowRoots(ArmShoulder shoulder, const double x, const double y, const double z, const double length);
        static ArmRoot getValidElbowRoot(ArmElbow elbow, ArmShoulder shoulder, ArmRoots roots);
        Position tryElbowRoot(ArmShoulder shoulder, ArmRoot root, const double x, const double y, const double z, const double clawXAngle, const double clawAngle);
        Position tryElbow(ArmShoulder shoulder, const double x, const double y, const double z, const double clawXAngle, const double clawAngle);                
        Position tryShoulderLength(ArmShoulder shoulder, const double length, const double x, const double y, const double z, const double clawXAngle, const double clawAngle);
        Position tryHalfLength(ArmShoulder shoulder, const double length, const double x, const double y, const double z, const double clawXAngle, const double clawAngle);
        Position tryShoulderRad(ArmShoulder shoulder, const double rad, const double x, const double y, const double z, const double clawXAngle, const double clawAngle);
        Position getArmPosition(ArmShoulder shoulder, const double x, const double y, const double z, const double clawXAngle, const double clawAngle);
        void addPositionToSequence(Position pos);              
    public:
        Strategy(
                Position pos, 
                const double x, 
                const double y, 
                const double z, 
                const double clawXAngle, 
                const double clawYAngle, 
                const double clawAngle, 
                const unsigned int iterations = DEFAULT_ITERATIONS, 
                const unsigned int postDelay = DEFAULT_POST_DELAY, 
                const unsigned int iterationDelay = DEFAULT_ITERATION_DELAY);
        std::vector<EngineControl> sequence;        
        Position position;
        const unsigned int iterations;
        const unsigned int postDelay;
        const unsigned int iterationDelay;
};

#endif
