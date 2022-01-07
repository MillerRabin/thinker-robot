#ifndef arm_part_h
#define arm_part_h

#include <ArmBase.h>

class ArmPart : public ArmBase {    
    private:
        void setPosLocal(const double localX, const double localY, const double localZ);
        void setPos(ArmBase* prev, const double x, const double y, const double z);        
        const double getLength(ArmBase* prev, const double x, const double y, const double z);
    public:        
        ArmPart(ArmBase* prev, const double xAngle, const double yAngle, const double zAngle);        
};

#endif