#ifndef arm_part_h
#define arm_part_h

#include <ArmBase.h>

class ArmPart : public ArmBase {
private:
  void setPosLocal(const float localX, const float localY, const float localZ);

protected:
  const float getPointLength(ArmBase *prev, const float x, const float y, const float z);
  void build(ArmBase &base, const float xAngle, const float yAngle, const float zAngle);

public:
  void setPos(ArmBase *prev, const float x, const float y, const float z);
  const float getMaxHorzLength();
  virtual const String name() override {
    return "ArmPart";
  };
};

#endif