#ifndef arm_h
#define arm_h

#include <armParams.h>
#include <math.h>
#include <string>

using namespace std;

//------ArmPoint------

class ArmPoint {
    protected:
        void setCoords();
        const double getRadFromPos(const double localX, const double localY, const double localZ);
        static const bool isEqual(const double op1, const double op2, const double tolerance = 0.00001);
    public:
        double XZRad;
        double XYRad;        
        double x;
        double y;
        double z;
        const double virtual getLength() {
            return 0;
        };
        const double virtual getBase()  {
            return 0;
        };
        const double virtual getScale() {
            return 1;
        };
        const double virtual getWidth()  {
            return 0;
        };
        const double virtual getMaxAngle()  {
            return 270;
        };
        const double virtual getMinAngle()  {
            return 0;
        };
        const double getRad(const double angle);
        const double getAngleFromRad(const double rad);
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
        const double getBase() override {
            return ROTATE_BASE;
        };
        const double getScale() override {
            return 1;
        };
        const double getMaxAngle() override {
            return ROTATE_MAX;
        };
        const double getMinAngle() override {
            return ROTATE_MIN;
        };
};

//------ArmShoulder------

class ArmShoulder : public ArmPoint {
    private:
        void setPoints(const double rotateRad, const double shoulderRad);        
        static const double validateAngle(const double angle);
    public: 
        ArmShoulder(ArmRotate rotate, const double shoulderAngle);                
        void setAvailableLength(const double maxLength, const double x, const double y, const double z);        
        const double getAngle(const bool validate = true);
        static const double getLength(const double x, const double y, const double z);
        void setXZRad(const double rad);
        void setRotate(ArmRotate rotate);
        const double getLength() override {
            return SHOULDER_LENGTH;
        };
        const double getBase() override {
            return SHOULDER_BASE;
        };
        const double getScale() override {
            return SHOULDER_SCALE;
        };
        const double getMaxAngle() override {
            return SHOULDER_MAX;
        };
        const double getMinAngle() override {
            return SHOULDER_MIN;
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
        void setPos(const double x, const double y, const double z);
        void setRotate(ArmRotate rotate);
        const double getLocalRad(ArmShoulder shoulder);
        const double getLength() override {
            return ELBOW_LENGTH;
        };
        const double getBase() override {
            return ELBOW_BASE;
        };
        const double getScale() override {
            return ELBOW_SCALE;
        };
        const double getMaxAngle() override {
            return ELBOW_MAX;
        };
        const double getMinAngle() override {
            return ELBOW_MIN;
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
        const double getLength() override {
            return WRIST_LENGTH;
        };
        const double getBase() override {
            return WRIST_BASE;
        };
        const double getScale() override {
            return WRIST_SCALE;
        };
        const double getMaxAngle() override {
            return WRIST_MAX;
        };
        const double getMinAngle() override {
            return WRIST_MIN;
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
        const double getBase() override {
            return CLAW_BASE;
        };
        const double getWidth() override {
            return CLAW_WIDTH;
        };

};

#endif