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
        const double validateXAngle(const double angle);
        const double validateYAngle(const double angle);
        const double validateZAngle(const double angle);
        static const double getZRadFromXY(const double x, const double y);
    public:
        ArmBase(const double XRad = 0, const double YRad = 0, const double ZRad = 0) :
            XRad(XRad),
            YRad(YRad),
            ZRad(ZRad) {}
        double XRad = 0;
        double YRad = 0;
        double ZRad = 0;        
        double x = NAN;
        double y = NAN;
        double z = NAN;  
        
        virtual const String name() {
            return "Base";
        };
        
        virtual const double getXLength() {
            return 0;
        };        
        virtual const double getYLength() {
            return 0;
        };        
        virtual const double getZLength() {
            return 0;
        };        

        virtual const double getLength()  {
            return getXLength() + getYLength() + getZLength();
        };

        virtual const double getXBase()  {
            return 0;
        };        
        virtual const double getYBase()  {
            return 0;
        };
        virtual const double getZBase()  {
            return 0;
        };
        virtual const double getXScale() {
            return 1;
        };
        virtual const double getYScale() {
            return 1;
        };
        virtual const double getZScale()  {
            return 1;
        };
        virtual const double getWidth()  {
            return 0;
        };
        virtual const double getXMaxAngle()  {
            return 270;
        };
        virtual const double getXMinAngle()  {
            return 0;
        };
        virtual const double getYMaxAngle()  {
            return 270;
        };
        virtual const double getYMinAngle()  {
            return 0;
        };
        virtual const double getZMaxAngle()  {
            return 270;
        };
        virtual const double getZMinAngle()  {
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
        void setRads(const double xRad, const double yRad, const double zRad);
        void setRadsLocal(const double xRad, const double yRad, const double zRad);
        void updateCoords();
        const double getXAngle(const bool validate = true);
        const double getYAngle(const bool validate = true);
        const double getZAngle(const bool validate = true);
};



#endif