#ifndef arm_part_h
#define arm_part_h

#include <ArmBase.h>

class ArmPart : public ArmBase {    
    private:
        void setPosLocal(const double localX, const double localY, const double localZ);    
    protected:                 
        const double getPointLength(ArmBase* prev, const double x, const double y, const double z);        
        void build(ArmBase& base, const double xAngle, const double yAngle, const double zAngle);        
    public:            
        void setPos(ArmBase* prev, const double x, const double y, const double z);
        const double getMaxHorzLength();
        virtual const String name() override {
            return "ArmPart";
        };                        
};

#endif