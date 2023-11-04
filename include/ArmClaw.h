#ifndef arm_claw_h
#define arm_claw_h

#include <ArmPart.h>
#include <ArmWrist.h>
#include <ArmClaw.h>

using namespace std;


class ArmClaw : public ArmPart {
    private:
        static const float validateClawAngle(const float angle);
        const float getClawRad(const float angle);
        const float getClawAngleFromRad(const float rad);
    public:    
        float clawRad = 0;
        ArmClaw(ArmWrist wrist, const float xAngle, const float yAngle, const float zAngle, const float clawAngle);
        const float getAngle(const bool validate = true);        
        
        virtual const String name() override {
            return "Claw";
        };                
        const float getXLength() override {
            return CLAW_X_LENGTH;
        };        
        const float getYLength() override {
            return CLAW_Y_LENGTH;
        };        
        const float getZLength() override {
            return CLAW_Z_LENGTH;
        };
        const float getXBase() override {
            return CLAW_X_BASE;
        };
        const float getYBase() override {
            return CLAW_Y_BASE;
        };
        const float getZBase() override {
            return CLAW_Z_BASE;
        };
        const float getWidth() override {
            return CLAW_WIDTH;
        };
        const float getXScale() override {
            return CLAW_X_SCALE;
        };
        const float getZScale() override {
            return CLAW_Z_SCALE;
        };
};

#endif