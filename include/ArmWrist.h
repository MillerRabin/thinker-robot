#ifndef arm_wrist_h
#define arm_wrist_h

#include <armParams.h>
#include <ArmPart.h>
#include <ArmElbow.h>
#include <ArmWrist.h>


class ArmWrist : public ArmPart {
    public: 
        ArmWrist(ArmElbow elbow, const double yAngle);
        virtual const String name() override {
            return "Wrist";
        };        
        virtual const double getYLength() override {
            return WRIST_Y_LENGTH;
        };
        virtual const double getYBase() override {
            return WRIST_Y_BASE;
        };
        virtual const double getYScale() override {
            return WRIST_Y_SCALE;
        };
        virtual const double getYMaxAngle() override {
            return WRIST_Y_MAX;
        };
        virtual const double getYMinAngle() override {
            return WRIST_Y_MIN;
        };

};

#endif