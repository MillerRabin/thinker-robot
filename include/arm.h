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
        const double getRadFromPos(const double localX, const double localY, const double localZ);        
    public:
        double YRad;
        double ZRad;        
        double x;
        double y;
        double z;
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
        const double getYAngleFromRad(const double rad);
        const double getZAngleFromRad(const double rad);
        static const double getRadFromXY(const double x, const double y);
        const bool isValid();
};

//------ArmRotate------

class ArmRotate : public ArmPoint {
    private:
        void setPoints(const double rotateRad);
    public: 
        ArmRotate(const double rotateRad);        
        const double getAngle();
        void setRadFromPos(const double x, const double y);        
        const double getLength() override {
            return 0;
        };
        const double getZBase() override {
            return ROTATE_Z_BASE;
        };
        const double getZScale() override {
            return 1;
        };
        const double getZMaxAngle() override {
            return ROTATE_Z_MAX;
        };
        const double getZMinAngle() override {
            return ROTATE_Z_MIN;
        };
};

//------ArmShoulder------

class ArmShoulder : public ArmPoint {
    private:        
        static const double validateAngle(const double angle);
    public: 
        ArmShoulder(ArmRotate rotate, const double shoulderAngle);                
        vector<double> getAvailableRads(const double maxLength, const double x, const double y, const double z);
        const bool isAvailableRadValid(const double rad, const double targetLength);
        const double getAngle(const bool validate = true);
        static const double getLength(const double x, const double y, const double z);
        void setYRad(const double rad);
        void setRotate(ArmRotate rotate);        
        void setRads(const double rotateRad, const double shoulderRad);        
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

};

//------ArmElbow------

class ArmElbow : public ArmPoint {
    private:
        void setPoints(const double rotateRad, const double elbowRad);        
        static const double validateAngle(const double angle);
    public: 
        ArmElbow(ArmShoulder shoulder, const double elbowAngle);        
        const double getAngle(ArmShoulder shoulder, const bool validate = true);
        void setToLength(ArmShoulder shoulder, const double x, const double y, const double z, const double length);
        static const double getLength(ArmShoulder shoulder, const double x, const double y, const double z);
        static const bool isAngleValid(const double angle);
        const double getAngleFromPos(ArmShoulder shoulder, const double x, const double y, const double z);        
        void setPosLocal(const double localX, const double localY, const double localZ);
        void setPos(ArmShoulder shoulder, const double x, const double y, const double z);
        void setRotate(ArmRotate rotate);
        const double getLocalRad(ArmShoulder shoulder);
        const bool isValid(ArmShoulder shoulder);
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
        void setPoints(const double wristRad, const double rotateRad);        
        static const double validateAngle(const double angle);
    public: 
        ArmWrist(ArmElbow elbow, const double wristAngle);        
        const double getAngle(ArmElbow elbow, const bool validate = true);
        static const double getLength(ArmShoulder shoulder, ArmElbow elbow, const double x, const double y, const double z);
        const double getLocalRad(ArmElbow elbow);
        void setPos(ArmShoulder shoulder, ArmElbow elbow, const double x, const double y, const double z);        
        const bool isValid(ArmElbow elbow);
        void setRotate(ArmRotate rotate);        
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
        void setPoints(const double rotateRad, const double wristRad, const double clawRad);
    public:    
        ArmClaw(ArmWrist wrist, const double clawAngle);
        ArmClaw(const double rotateRad, const double wristRad, const double clawRad);
        const double getAngle();
        const double getLength() override {
            return CLAW_LENGTH;
        };
        const double getXBase() override {
            return CLAW_X_BASE;
        };
        const double getWidth() override {
            return CLAW_WIDTH;
        };

};

#endif