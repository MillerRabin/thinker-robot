#ifndef arm_h
#define arm_h

#include <armParams.h>

class ArmPoint {
    protected:
        void setCoords();
    public:
        double XZRad;
        double XYRad;
        double XZCurrent;
        double x;
        double y;
        double z;
        virtual double getLength() = 0;
        virtual double getOffset() = 0;
        virtual double getInc() = 0;
        double getRad(const double angle) ;
        double getAngleXZ();
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
        void addX(const double dx);        
        static double getAngleFromX();
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
    public: 
        ArmElbow(const double shoulderRad, const double rotateRad, const double elbowAngle);
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
        ArmWrist(const double shoulderRad, const double elbowRad, const double rotateRad, const double wristAngle);        
        void setX(const double shoulderRad, const double elbowRad, const double x);        
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

#endif