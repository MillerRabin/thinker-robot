#ifndef arm_claw_h
#define arm_claw_h

#include <ArmPart.h>
#include <ArmWrist.h>
#include <ArmClaw.h>

using namespace std;


class ArmClaw : public ArmPart {
    private:
        static const double validateClawAngle(const double angle);
        const double getClawRad(const double angle);
        const double getClawAngleFromRad(const double rad);
    public:    
        double clawRad = 0;
        ArmClaw(ArmWrist wrist, const double xAngle, const double yAngle, const double zAngle, const double clawAngle);
        const double getAngle(const bool validate = true);        

        virtual const String name() override {
            return "Claw";
        };                
        const double getXLength() override {
            return CLAW_X_LENGTH;
        };        
        const double getYLength() override {
            return CLAW_Y_LENGTH;
        };        
        const double getZLength() override {
            return CLAW_Z_LENGTH;
        };
        const double getXBase() override {
            return CLAW_X_BASE;
        };
        const double getYBase() override {
            return CLAW_Y_BASE;
        };
        const double getZBase() override {
            return CLAW_Z_BASE;
        };
        const double getWidth() override {
            return CLAW_WIDTH;
        };
        const double getXScale() override {
            return CLAW_X_SCALE;
        };
        const double getZScale() override {
            return CLAW_Z_SCALE;
        };
};

#endif