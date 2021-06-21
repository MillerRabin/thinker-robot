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
        double XZCurrent;
        double x;
        double y;
        double z;
        double virtual getLength() {
            return 0;
        };
        double virtual getOffset()  {
            return 0;
        };
        double virtual getInc() {
            return 1;
        };
        double virtual getWidth()  {
            return 0;
        };
        double getRad(const double angle);
        double getAngleXZ();        
        double getAngleXZ(const double rad);
};


class ArmRotate : public ArmPoint {
    public: 
        ArmRotate(const double rotateRad);
        double getLength() override {
            return 0;
        };
        double getOffset() override {
            return ROTATE_OFFSET;
        };
        double getInc() override {
            return 1;
        };

};
class ArmShoulder : public ArmPoint {
    private:
        void setPoints(const double rotateRad, const double shoulderAngle);        
    public: 
        ArmShoulder(const double rotateRad, const double shoulderAngle);        
        static double getLengthXZ(const double x, const double z);        
        void addX(const double dx);        
        double getLength() override {
            return SHOULDER_LENGTH;
        };
        double getOffset() override {
            return SHOULDER_OFFSET;
        };
        double getInc() override {
            return SHOULDER_INC;
        };
};

class ArmElbow : public ArmPoint {
    private:
        void setPoints(const double shoulderRad, const double rotateRad, const double elbowRad);        
        static ArmRoots getRoots(ArmShoulder shoulder, const double x, const double z, const double length);
        static bool isAngleValid(const double angle);
        double getAngleFromPos(ArmShoulder shoulder, const double x, const double z);
        ArmRoot getValidRoot(ArmShoulder shoulder, ArmRoots roots);        
    public: 
        ArmElbow(const double rotateRad, const double shoulderRad, const double elbowAngle);        
        void setToLength(ArmShoulder shoulder, const double x, const double z, const double length);
        static double getLengthXZ(ArmShoulder shoulder, const double x, const double z);                
        void addZ(const double shoulderRad, const double dz);
        double getLength() override {
            return ELBOW_LENGTH;
        };
        double getOffset() override {
            return ELBOW_OFFSET;
        };
        double getInc() override {
            return ELBOW_INC;
        };

};

class ArmWrist : public ArmPoint {
    private:
        void setPoints(const double shoulderRad, const double elbowRad, const double wristRad, const double rotateRad);        
    public: 
        ArmWrist(const double rotateRad, const double shoulderRad, const double elbowRad, const double wristAngle);
        static double getLengthXZ(ArmShoulder shoulder, ArmElbow elbow, const double x, const double z);        
        void setPos(ArmShoulder shoulder, ArmElbow elbow, const double x, const double z);
        double getLength() override {
            return WRIST_LENGTH;
        };
        double getOffset() override {
            return WRIST_OFFSET;
        };
        double getInc() override {
            return WRIST_INC;
        };

};

class ArmClaw : public ArmPoint {
    private:
        void setPoints(const double rotateRad, const double shoulderRad, const double elbowRad, const double wristRad, const double clawRad);
    public:    
        ArmClaw(const double rotateRad, const double shoulderRad, const double elbowRad, const double wristRad, const double clawAngle);        
        double getLength() override {
            return CLAW_LENGTH;
        };
        double getOffset() override {
            return CLAW_OFFSET;
        };
        double getWidth() override {
            return CLAW_WIDTH;
        };

};

#endif