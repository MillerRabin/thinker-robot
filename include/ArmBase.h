#ifndef arm_base_h
#define arm_base_h

#include <math.h>
#include <string>
#include <Arduino.h>
#include "armParams.h"

//------Coords------

class Coords
{
public:
  const float length;
  const float x;
  const float y;
  const float z;
  Coords(const float length, const float YRad, const float ZRad);
  const bool isEqual(const float x, const float y, const float z, const float tolerance = 0.00001);
};

//------Point------
class Point
{
public:
  const float x;
  const float y;
  const float z;
  Point(const float x, const float y, const float z) : x(x), y(y), z(z){};
};

//------ArmBase------

class ArmBase
{
protected:
  float XRadLocal = 0;
  float YRadLocal = 0;
  float ZRadLocal = 0;
  const float validateXAngle(const float angle);
  const float validateYAngle(const float angle);
  const float validateZAngle(const float angle);
  void rotateX(const float xRad);
  void rotateY(const float yRad);
  void rotateZ(const float zRad);

public:
  ArmBase(const float XRad = 0, const float YRad = 0, const float ZRad = 0) : XRad(XRad),
                                                                              YRad(YRad),
                                                                              ZRad(ZRad) {}
  float XRad = 0;
  float YRad = 0;
  float ZRad = 0;
  float x = NAN;
  float y = NAN;
  float z = NAN;

  virtual const String name()
  {
    return "Base";
  };

  virtual const float getXLength()
  {
    return 0;
  };
  virtual const float getYLength()
  {
    return 0;
  };
  virtual const float getZLength()
  {
    return 0;
  };

  virtual const float getLength()
  {
    return getXLength() + getYLength() + getZLength();
  };

  virtual const float getXBase()
  {
    return 0;
  };
  virtual const float getYBase()
  {
    return 0;
  };
  virtual const float getZBase()
  {
    return 0;
  };
  virtual const float getXScale()
  {
    return 1;
  };
  virtual const float getYScale()
  {
    return 1;
  };
  virtual const float getZScale()
  {
    return 1;
  };
  virtual const float getWidth()
  {
    return 0;
  };
  virtual const float getXMaxAngle()
  {
    return 270;
  };
  virtual const float getXMinAngle()
  {
    return 0;
  };
  virtual const float getYMaxAngle()
  {
    return 270;
  };
  virtual const float getYMinAngle()
  {
    return 0;
  };
  virtual const float getZMaxAngle()
  {
    return 270;
  };
  virtual const float getZMinAngle()
  {
    return 0;
  };
  const float getYRadFromPos(const float localX, const float localY, const float localZ, const float length = NAN);
  static const bool isEqual(const float op1, const float op2, const float tolerance = 0.00001);
  const float getXRad(const float angle);
  const float getYRad(const float angle);
  const float getZRad(const float angle);
  const float getXRad();
  const float getYRad();
  const float getZRad();
  const float getXAngleFromRad(const float rad);
  const float getYAngleFromRad(const float rad);
  const float getZAngleFromRad(const float rad);
  static const float getRadFromXY(const float x, const float y);
  const bool isValid();
  static const float getLengthFromPoint(const float x, const float y, const float z);
  void setRads(const float xRad, const float yRad, const float zRad);
  void setRadsLocal(const float xRad, const float yRad, const float zRad);
  void updateCoords();
  const float getXAngle(const bool validate = true);
  const float getYAngle(const bool validate = true);
  const float getZAngle(const bool validate = true);
  static const float getXLength(const float length, const float YRad, const float ZRad);
  static const float getYLength(const float length, const float YRad, const float ZRad);
  static const float getZLength(const float length, const float YRad, const float ZRad);
};

#endif