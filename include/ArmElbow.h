#ifndef arm_elbow_h
#define arm_elbow_h

#include <armParams.h>
#include <ArmPart.h>
#include <ArmElbow.h>
#include <ArmShoulder.h>

class ArmElbow : public ArmPart {
    public:        
        ArmElbow(ArmShoulder shoulder, const float yAngle);
        virtual const String name() override {
            return "Elbow";
        };        
        void setRads(const float yRad, const float zRad);
        virtual const float getYLength() override {
            return ELBOW_Y_LENGTH;
        };
        virtual const float getYBase() override {
            return ELBOW_Y_BASE;
        };
        virtual const float getYScale() override {
            return ELBOW_Y_SCALE;
        };
        virtual const float getYMaxAngle() override {
            return ELBOW_Y_MAX;
        };
        virtual const float getYMinAngle() override {
            return ELBOW_Y_MIN;
        };    
};

#endif