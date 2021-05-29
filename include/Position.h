#ifndef position_h
#define position_h

#include <armParams.h>
#include <Position.h>

class Leverage {
    public:
        double length;
        double rad;
};

class Projection {
    public:
        Leverage shoulder;
        Leverage elbow;
        Leverage wrist;
        double length;
};

class Position {
    private:
        static Projection getX(const double sAngle, const double eAngle, const double wAngle, const double rAngle);
        static Projection getZ(const double sAngle, const double eAngle, const double wAngle);
        static Projection getY(const double sAngle, const double eAngle, const double wAngle, const double rAngle);        
    public: 
        Position(const unsigned int sAngle, const unsigned int eAngle, const unsigned int wAngle, const double rAngle);
        Projection x;
        Projection y;
        Projection z;
        unsigned int shoulderAngle;
        unsigned int elbowAngle;
        unsigned int wristAngle;
        unsigned int rotateAngle;
        static const double getRotateRad(const double rAngle);
        static Leverage getShoulderX(const double sAngle, const double rRad);
        static double getShoulderAngleFromX(const double x, const double rRad);
        static Leverage getShoulderZ(const double sAngle);        
        static Leverage getElbowZ(Leverage shoulder, const double eAngle);
        static double getElbowAngleFromZ(Leverage shoulder, const double z);
        static double getWristAngleFromX(Leverage shoulder, Leverage elbow, const double x);
        static Leverage getWristX(Leverage shoulder, Leverage elbow, const double rRad, const double wAngle);
        static Leverage getWristZ(Leverage shoulder, Leverage elbow, const double wAngle);        
};

#endif