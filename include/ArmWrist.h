#ifndef arm_wrist_h
#define arm_wrist_h

#include <armParams.h>
#include <ArmPart.h>
#include <ArmElbow.h>
#include <ArmWrist.h>


class ArmWrist : public ArmPart {
    public: 
        ArmWrist(ArmElbow elbow, const float yAngle);
        virtual const String name() override {
            return "Wrist";
        };        
        virtual const float getYLength() override {
            return WRIST_Y_LENGTH;
        };
        virtual const float getYBase() override {
            return WRIST_Y_BASE;
        };
        virtual const float getYScale() override {
            return WRIST_Y_SCALE;
        };
        virtual const float getYMaxAngle() override {
            return WRIST_Y_MAX;
        };
        virtual const float getYMinAngle() override {
            return WRIST_Y_MIN;
        };

};

#endif