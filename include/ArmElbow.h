#ifndef arm_elbow_h
#define arm_elbow_h

#include <armParams.h>
#include <ArmPart.h>
#include <ArmElbow.h>
#include <ArmShoulder.h>

class ArmElbow : public ArmPart {
    public:        
        ArmElbow(ArmShoulder shoulder, const double yAngle);
        virtual const String name() override {
            return "Elbow";
        };        
        void setRads(const double yRad, const double zRad);
        virtual const double getYLength() override {
            return ELBOW_Y_LENGTH;
        };
        virtual const double getYBase() override {
            return ELBOW_Y_BASE;
        };
        virtual const double getYScale() override {
            return ELBOW_Y_SCALE;
        };
        virtual const double getYMaxAngle() override {
            return ELBOW_Y_MAX;
        };
        virtual const double getYMinAngle() override {
            return ELBOW_Y_MIN;
        };    
};

#endif