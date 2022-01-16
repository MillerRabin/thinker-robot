#ifndef arm_part_h
#define arm_part_h

#include <ArmBase.h>

class ArmPart : public ArmBase {    
    private:
        void setPosLocal(const double localX, const double localY, const double localZ);
        void setPos(ArmBase* prev, const double x, const double y, const double z);                

    protected: 
        const double getRadFromXY(const double x, const double y);
        const double getPointLength(ArmBase* prev, const double x, const double y, const double z);        
        void build(ArmBase& base, const double xAngle, const double yAngle, const double zAngle);        
    public:                    
        virtual const String name() override {
            return "ArmPart";
        };                        
};

#endif