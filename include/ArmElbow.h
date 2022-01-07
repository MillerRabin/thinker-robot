#ifndef arm_elbow_h
#define arm_elbow_h

#include <armParams.h>
#include <ArmPart.h>
#include <ArmElbow.h>

class ArmElbow : public ArmPart {
    public:        
        void setRads(const double yRad, const double zRad);
        const double getYLength() override {
            return ELBOW_Y_LENGTH;
        };
        const double getYBase() override {
            return ELBOW_Y_BASE;
        };
        const double getYScale() override {
            return ELBOW_Y_SCALE;
        };
        const double getYMaxAngle() override {
            return ELBOW_Y_MAX;
        };
        const double getYMinAngle() override {
            return ELBOW_Y_MIN;
        };    
};

#endif