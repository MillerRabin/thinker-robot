#ifndef arm_h
#define arm_h

#include <armParams.h>
#include <math.h>

class ArmRoot {
    public:    
        double x = NAN;
        double z = NAN;
};

class ArmRoots {
    public:    
        ArmRoot r1;
        ArmRoot r2;
};

class ArmPoint {
    protected:
        void setCoords();
        double getRadFromPos(const double localX, const double localZ);
    public:
        double XZRad;
        double XYRad;        
        double x;
        double y;
        double z;
        double virtual getLength() {
            return 0;
        };
        double virtual getBase()  {
            return 0;
        };
        double virtual getScale() {
            return 1;
        };
        double virtual getWidth()  {
            return 0;
        };
        double getRad(const double angle);
        double getAngleXZFromRad(const double rad);
};


class ArmRotate : public ArmPoint {
    public: 
        ArmRotate(const double rotateRad);
        double getLength() override {
            return 0;
        };
        double getBase() override {
            return ROTATE_BASE;
        };
        double getScale() override {
            return 1;
        };

};
class ArmShoulder : public ArmPoint {
    private:
        void setPoints(const double rotateRad, const double shoulderAngle);        
    public: 
        ArmShoulder(const double rotateRad, const double shoulderAngle);                
        void setToLength(const double length, const double x, const double z);
        double getAngleXZ();
        static double getLengthXZ(const double x, const double z);
        double getLength() override {
            return SHOULDER_LENGTH;
        };
        double getBase() override {
            return SHOULDER_BASE;
        };
        double getScale() override {
            return SHOULDER_SCALE;
        };
};

class ArmElbow : public ArmPoint {
    private:
        void setPoints(const double rotateRad, const double elbowRad);        
        static ArmRoots getRoots(ArmShoulder shoulder, const double x, const double z, const double length);
        static bool isAngleValid(const double angle);
        double getAngleFromPos(ArmShoulder shoulder, const double x, const double z);
        ArmRoot getValidRoot(ArmShoulder shoulder, ArmRoots roots);        
    public: 
        ArmElbow(const double rotateRad, const double shoulderRad, const double elbowAngle);        
        double getAngleXZ(ArmShoulder shoulder);
        void setToLength(ArmShoulder shoulder, const double x, const double z, const double length);
        static double getLengthXZ(ArmShoulder shoulder, const double x, const double z);
        double getLength() override {
            return ELBOW_LENGTH;
        };
        double getBase() override {
            return ELBOW_BASE;
        };
        double getScale() override {
            return ELBOW_SCALE;
        };

};

class ArmWrist : public ArmPoint {
    private:
        void setPoints(const double wristRad, const double rotateRad);        
    public: 
        ArmWrist(const double rotateRad, const double elbowRad, const double wristAngle);
        double getAngleXZ(ArmElbow elbow);
        static double getLengthXZ(ArmShoulder shoulder, ArmElbow elbow, const double x, const double z);        
        void setPos(ArmShoulder shoulder, ArmElbow elbow, const double x, const double z);
        double getLength() override {
            return WRIST_LENGTH;
        };
        double getBase() override {
            return WRIST_BASE;
        };
        double getScale() override {
            return WRIST_SCALE;
        };

};

class ArmClaw : public ArmPoint {
    private:
        void setPoints(const double rotateRad, const double wristRad, const double clawRad);
    public:    
        ArmClaw(const double rotateRad, const double wristRad, const double clawAngle);
        double getLength() override {
            return CLAW_LENGTH;
        };
        double getBase() override {
            return CLAW_BASE;
        };
        double getWidth() override {
            return CLAW_WIDTH;
        };

};

#endif