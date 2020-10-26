#ifndef position_h
#define position_h

#include <Projection.h>
#include <Position.h>

#define SHOULDER_LENGTH 105
#define ELBOW_LENGTH 95
#define WRIST_LENGTH 150

#define SHOULDER_OFFSET 0
#define SHOULDER_INC 1
#define ELBOW_OFFSET 145
#define ELBOW_INC -1
#define WRIST_OFFSET 145
#define WRIST_INC -1
#define ROTATE_OFFSET -45
#define GRIPPER_ROTATE_OFFSET -45

#define TOP_OFFSET 80


class Position {
    private:
        Projection getX(const double sAngle, const double eAngle, const double wAngle, const double rAngle);
        Projection getZ(const double sAngle, const double eAngle, const double wAngle);
        Projection getY(const double sAngle, const double eAngle, const double wAngle, const double rAngle);
        
    public: 
        Position(const double sAngle, const double eAngle, const double wAngle, const double rAngle);
        Projection x;
        Projection y;
        Projection z;
};

#endif