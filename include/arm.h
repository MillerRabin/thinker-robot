#ifndef arm_h
#define arm_h

#include <armParams.h>
#include <math.h>
#include <string>
#include <Arduino.h>

using namespace std;

//------Coords------

class Coords {
    public:
        const double length;
        const double x;
        const double y;
        const double z;
        Coords(const double length, const double YRad, const double ZRad);
        const bool isEqual(const double x, const double y, const double z, const double tolerance = 0.00001);
};

//------ArmPoint------

class ArmPoint {    
    protected:
        void setCoords();        
        const double getYRadFromPos(const double localX, const double localY, const double localZ);     
    public:
        double YRad = NAN;
        double ZRad = NAN;
        double XRad = NAN;
        double x = NAN;
        double y = NAN;
        double z = NAN;        
        const double virtual getLength() {
            return 0;
        };
        const double virtual getXBase()  {
            return 0;
        };
        const double virtual getYBase()  {
            return 0;
        };
        const double virtual getZBase()  {
            return 0;
        };
        const double virtual getXScale() {
            return 1;
        };
        const double virtual getYScale() {
            return 1;
        };
        const double virtual getZScale()  {
            return 1;
        };
        const double virtual getWidth()  {
            return 0;
        };
        const double virtual getXMaxAngle()  {
            return 270;
        };
        const double virtual getXMinAngle()  {
            return 0;
        };
        const double virtual getYMaxAngle()  {
            return 270;
        };
        const double virtual getYMinAngle()  {
            return 0;
        };
        const double virtual getZMaxAngle()  {
            return 270;
        };
        const double virtual getZMinAngle()  {
            return 0;
        };
        static const bool isEqual(const double op1, const double op2, const double tolerance = 0.00001);
        const double getXRad(const double angle);
        const double getYRad(const double angle);
        const double getZRad(const double angle);        
        const double getXAngleFromRad(const double rad);
        const double getYAngleFromRad(const double rad);
        const double getZAngleFromRad(const double rad);
        static const double getRadFromXY(const double x, const double y);
        const bool isValid();
};

//------ArmShoulder------

class ArmShoulder : public ArmPoint {
    private:        
        static const double validateYAngle(const double angle);
        static const double validateZAngle(const double angle);        
    public: 
        ArmShoulder(const double yAngle, const double zAngle);
        vector<double> getAvailableRads(const double maxLength, const double x, const double y, const double z);
        const bool isAvailableRadValid(const double rad, const double targetLength);
        static const double getZRadFromXY(const double x, const double y);
        const double getYAngle(const bool validate = true);
        const double getZAngle(const bool validate = true);
        static const double getLength(const double x, const double y, const double z);        
        void setRads(const double yRad, const double zRad);        
        const bool isValid();        
        const double getLength() override {
            return SHOULDER_LENGTH;
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

//------ArmElbow------

class ArmElbow : public ArmPoint {
    private:        
        static const double validateYAngle(const double angle);
    public: 
        ArmElbow(ArmShoulder shoulder, const double yAngle);        
        const double getYAngle(ArmShoulder shoulder, const bool validate = true);
        void setToLength(ArmShoulder shoulder, const double x, const double y, const double z, const double length);
        static const double getLength(ArmShoulder shoulder, const double x, const double y, const double z);
        static const bool isAngleValid(const double angle);
        const double getYAngleFromPos(ArmShoulder shoulder, const double x, const double y, const double z);        
        void setPosLocal(const double localX, const double localY, const double localZ);
        void setPos(ArmShoulder shoulder, const double x, const double y, const double z);        
        const double getLocalRad(ArmShoulder shoulder);
        const bool isValid(ArmShoulder shoulder);
        void setRads(const double yRad, const double zRad);
        const double getLength() override {
            return ELBOW_LENGTH;
        };
        const double getYBase() override {
            return ELBOW_Y_BASE;
        };
        const double getYScale() override {
            return ELBOW_Y_SCALE;
        };
        const double getYMaxAngle() override {
            return ELBOW_Y_MAX;
        };
        const double getYMinAngle() override {
            return ELBOW_Y_MIN;
        };
    
};

//------ArmWrist------

class ArmWrist : public ArmPoint {
    private:        
        static const double validateYAngle(const double angle);
    public: 
        ArmWrist(ArmElbow elbow, const double yAngle);        
        const double getYAngle(ArmElbow elbow, const bool validate = true);
        static const double getLength(ArmShoulder shoulder, ArmElbow elbow, const double x, const double y, const double z);
        const double getLocalRad(ArmElbow elbow);
        void setPos(ArmShoulder shoulder, ArmElbow elbow, const double x, const double y, const double z);        
        const bool isValid(ArmElbow elbow);        
        void setRads(const double yRad, const double zRad);
        const double getLength() override {
            return WRIST_LENGTH;
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

//------ArmClaw------

class ArmClaw : public ArmPoint {
    private:
        static const double validateXAngle(const double angle);
        static const double validateYAngle(const double angle);
        static const double validateZAngle(const double angle);
        static const double validateClawAngle(const double angle);
        const double getClawRad(const double angle);
        const double getClawAngleFromRad(const double rad);
    public:    
        double clawRad = NAN;
        ArmClaw(ArmWrist wrist, const double clawXAngle, const double clawZAngle, const double clawAngle);
        ArmClaw(const double xRad, const double yRad, const double zRad, const double clawRad);
        const double getAngle(const bool validate = true);
        const double getXAngle(const bool validate = true);        
        const double getZAngle(const bool validate = true);        
        const double getYAngle(ArmWrist wrist, const bool validate = true);        
        void setRads(const double xRad, const double yRad, const double zRad, const double clawRad);
        void setPos(ArmShoulder shoulder, ArmElbow elbow, ArmWrist wrist, const double x, const double y, const double z);        
        const double getLength() override {
            return CLAW_LENGTH;
        };
        const double getXBase() override {
            return CLAW_X_BASE;
        };
        const double getWidth() override {
            return CLAW_WIDTH;
        };
        const double getXScale() override {
            return CLAW_X_SCALE;
        };
        const double getZBase() override {
            return CLAW_Z_BASE;
        };
        const double getZScale() override {
            return CLAW_Z_SCALE;
        };
};

#endif