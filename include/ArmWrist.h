#ifndef arm_wrist_h
#define arm_wrist_h

#include <armParams.h>
#include <ArmPart.h>
#include <ArmWrist.h>

class ArmWrist : public ArmPart {
    public: 
        const double getYLength() override {
            return WRIST_Y_LENGTH;
        };
        const double getYBase() override {
            return WRIST_Y_BASE;
        };
        const double getYScale() override {
            return WRIST_Y_SCALE;
        };
        const double getYMaxAngle() override {
            return WRIST_Y_MAX;
        };
        const double getYMinAngle() override {
            return WRIST_Y_MIN;
        };

};

#endif