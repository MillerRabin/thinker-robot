#ifndef position_h
#define position_h

#include <armParams.h>
#include <Position.h>

class Projection {
    public:
        double shoulderLength;
        double elbowLength;
        double wristLength;
        double lengthSum;
};

class Position {
    private:
        Projection getX(const double sAngle, const double eAngle, const double wAngle, const double rAngle);
        Projection getZ(const double sAngle, const double eAngle, const double wAngle);
        Projection getY(const double sAngle, const double eAngle, const double wAngle, const double rAngle);        
    public: 
        Position(const unsigned int sAngle, const unsigned int eAngle, const unsigned int wAngle, const double rAngle);
        Projection x;
        Projection y;
        Projection z;
        unsigned int shoulderAngle;
        unsigned int elbowAngle;
        unsigned int wristAngle;
        static double angleFromLength(double leverage, double length);
        static bool isAchievable(double leverage, double length);
};

#endif