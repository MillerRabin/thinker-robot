#ifndef position_h
#define position_h

#include <armParams.h>
#include <Position.h>

class APoint {
    public:
        double XZRad;
        double XYRad;
        double sum;
        double x;
        double y;
        double z;
        double addX(const double dx);
};

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
        static double getElbowAngle(const double elbowRad);
        static double getElbowRadFromZ(Leverage shoulder, const double z);        
        static Leverage getWristX(Leverage shoulder, const double elbowRad, const double rotateRad, const double wristAngle);        
        static Leverage getWristZ(Leverage shoulder, Leverage elbow, const double wAngle);        
        static APoint getWristPoint(const double shoulderRad, const double elbowRad, const double rotateRad, const double wristAngle);
        static APoint getShoulderPoint(const double rotateRad, const double shoulderAngle);
        static double getWristAngleFromX(const double shoulderRad, const double elbowRad, APoint point);
        static double getWristAngleFromZ(const double shoulderRad, const double elbowRad, APoint point);
};

#endif