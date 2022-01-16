#ifndef arm_shoulder_h
#define arm_shoulder_h

#include <Arduino.h>
#include <ArmPart.h>

class ArmShoulder : public ArmPart {    
    public:        
        std::vector<double> getAvailableRads(const double maxLength, const double x, const double y, const double z);
        ArmShoulder(const double yAngle, const double zAngle);                
        virtual const String name() override {
            return "Shoulder";
        };        
        
        virtual const double getZLength() override {
            return SHOULDER_Z_LENGTH;
        };
        virtual const double getYLength() override {
            return SHOULDER_Y_LENGTH;
        };        
        virtual const double getYBase() override {
            return SHOULDER_Y_BASE;
        };
        virtual const double getYScale() override {
            return SHOULDER_Y_SCALE;
        };
        virtual const double getYMaxAngle() override {
            return SHOULDER_Y_MAX;
        };
        virtual const double getYMinAngle() override {
            return SHOULDER_Y_MIN;
        };
        virtual const double getZBase() override {
            return SHOULDER_Z_BASE;
        };
        virtual const double getZScale() override {
            return SHOULDER_Z_SCALE;
        };
        virtual const double getZMaxAngle() override {
            return SHOULDER_Z_MAX;
        };
        virtual const double getZMinAngle() override {
            return SHOULDER_Z_MIN;
        };
};

#endif