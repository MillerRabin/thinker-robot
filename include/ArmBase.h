#ifndef arm_base_h
#define arm_base_h

#include <armParams.h>
#include <math.h>
#include <string>
#include <Arduino.h>


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

//------ArmBase------

class ArmBase {    
    protected:
        double XRadLocal = 0;
        double YRadLocal = 0;
        double ZRadLocal = 0;                    
        void setCoords();            
        void setRadsLocal(const double xRad, const double yRad, const double zRad);
        const double validateXAngle(const double angle);
        const double validateYAngle(const double angle);
        const double validateZAngle(const double angle);
    public:
        double YRad = 0;
        double ZRad = 0;
        double XRad = 0;
        double x = NAN;
        double y = NAN;
        double z = NAN;  
        
        const String virtual name() {
            return "Base";
        };
        
        const double virtual getXLength() {
            return 0;
        };        
        const double virtual getYLength() {
            return 0;
        };        
        const double virtual getZLength() {
            return 0;
        };        

        const double virtual getLength()  {
            return getXLength() + getYLength() + getZLength();
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
        const double getYRadFromPos(const double localX, const double localY, const double localZ, const double length = NAN);             
        static const bool isEqual(const double op1, const double op2, const double tolerance = 0.00001);
        const double getXRad(const double angle);
        const double getYRad(const double angle);
        const double getZRad(const double angle);        
        const double getXRad();
        const double getYRad();
        const double getZRad(); 
        const double getXAngleFromRad(const double rad);
        const double getYAngleFromRad(const double rad);
        const double getZAngleFromRad(const double rad);        
        static const double getRadFromXY(const double x, const double y);
        const bool isValid();
        const double getLengthFromPoint(const double x, const double y, const double z);
        void setRads(const double xRad, const double yRad, const double zRad, const double clawRad);
        const double getXAngle(const bool validate);
        const double getYAngle(const bool validate);
        const double getZAngle(const bool validate);
};



#endif