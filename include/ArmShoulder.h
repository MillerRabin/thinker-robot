#ifndef arm_shoulder_h
#define arm_shoulder_h

#include <Arduino.h>
#include <ArmPart.h>
#include <vector>

class ArmShoulder : public ArmPart {    
    private:
        static const float getZRadFromXY(const float x, const float y);
    public:        
        std::vector<float> getAvailableRads(const float maxLength, const float x, const float y, const float z);
        ArmShoulder(const float yAngle, const float zAngle);                
        
        virtual const String name() override {
            return "Shoulder";
        };        
        
        virtual const float getZLength() override {
            return SHOULDER_Z_LENGTH;
        };
        virtual const float getYLength() override {
            return SHOULDER_Y_LENGTH;
        };        
        virtual const float getYBase() override {
            return SHOULDER_Y_BASE;
        };
        virtual const float getYScale() override {
            return SHOULDER_Y_SCALE;
        };
        virtual const float getYMaxAngle() override {
            return SHOULDER_Y_MAX;
        };
        virtual const float getYMinAngle() override {
            return SHOULDER_Y_MIN;
        };
        virtual const float getZBase() override {
            return SHOULDER_Z_BASE;
        };
        virtual const float getZScale() override {
            return SHOULDER_Z_SCALE;
        };
        virtual const float getZMaxAngle() override {
            return SHOULDER_Z_MAX;
        };
        virtual const float getZMinAngle() override {
            return SHOULDER_Z_MIN;
        };
};

#endif