#ifndef strategy_h
#define strategy_h

#include <Arduino.h>
#include "Position.h"
#include "armParams.h"

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
        Position freeAngle(const double x, const double y, const double z, const double clawXAngle, const double clawAngle);
        Position fixedAngle(const double x, const double y, const double z, const double clawXAngle, const double clawYAngle, const double clawZAngle, const double clawAngle);
        static ArmRoots getElbowRoots(ArmShoulder shoulder, const double x, const double y, const double z, const double length);
        static ArmRoot getValidElbowRoot(ArmElbow elbow, ArmShoulder shoulder, ArmRoots roots);
        Position tryElbowRoot(ArmShoulder shoulder, ArmRoot root, const double x, const double y, const double z, const double clawXAngle, const double clawAngle);
        Position tryElbow(ArmShoulder shoulder, const double x, const double y, const double z, const double clawXAngle, const double clawAngle);                
        Position tryShoulderLength(ArmShoulder shoulder, const double length, const double x, const double y, const double z, const double clawXAngle, const double clawAngle);
        Position tryHalfLength(ArmShoulder shoulder, const double length, const double x, const double y, const double z, const double clawXAngle, const double clawAngle);
        Position tryShoulderRad(ArmShoulder shoulder, const double rad, const double x, const double y, const double z, const double clawXAngle, const double clawAngle);
        Position getArmPosition(ArmShoulder shoulder, const double x, const double y, const double z, const double clawXAngle, const double clawAngle);        
        std::string type;
    public:
        Strategy(
                Position pos, 
                const double x, 
                const double y, 
                const double z, 
                const double clawXAngle, 
                const double clawYAngle, 
                const double clawZAngle, 
                const double clawAngle, 
                const double speed = DEFAULT_SPEED, 
                const unsigned int postDelay = DEFAULT_POST_DELAY                
        );
        std::vector<EngineControl> sequence;        
        Position startPosition;
        Position endPosition;
        const double speed;
        const unsigned int postDelay;        
        std::string getType() { return type; };
};

#endif
