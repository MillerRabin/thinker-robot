#ifndef arm_shoulder_h
#define arm_shoulder_h

#include <Arduino.h>
#include <ArmPart.h>

class ArmShoulder : public ArmPart {    
    public:        
        vector<double> getAvailableRads(const double maxLength, const double x, const double y, const double z);
        const String name() override {
            return "Shoulder";
        };        
        const double getZLength() override {
            return SHOULDER_Z_LENGTH;
        };
        const double getYLength() override {
            return SHOULDER_Y_LENGTH;
        };        
        const double getYBase() override {
            return SHOULDER_Y_BASE;
        };
        const double getYScale() override {
            return SHOULDER_Y_SCALE;
        };
        const double getYMaxAngle() override {
            return SHOULDER_Y_MAX;
        };
        const double getYMinAngle() override {
            return SHOULDER_Y_MIN;
        };
        const double getZBase() override {
            return SHOULDER_Z_BASE;
        };
        const double getZScale() override {
            return SHOULDER_Z_SCALE;
        };
        const double getZMaxAngle() override {
            return SHOULDER_Z_MAX;
        };
        const double getZMinAngle() override {
            return SHOULDER_Z_MIN;
        };
};

#endif