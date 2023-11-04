#include <ArmBase.h>

//------Coords------

Coords::Coords(const float length, const float YRad, const float ZRad) : length(length),
  x(ArmBase::getXLength(length, YRad, ZRad)),
  y(ArmBase::getYLength(length, YRad, ZRad)),
  z(ArmBase::getZLength(length, YRad, ZRad))
{}

const bool Coords::isEqual(const float x, const float y, const float z, const float tolerance)
{
  const float deltaX = this->x - x;
  const float deltaY = this->y - y;
  const float deltaZ = this->z - z;
  return (deltaX < tolerance) && (deltaX > -tolerance) &&
         (deltaY < tolerance) && (deltaY > -tolerance) &&
         (deltaZ < tolerance) && (deltaZ > -tolerance);
}

//------ArmBase------

void ArmBase::updateCoords()
{
  const float zLength = getZLength();  
  const float zxLocal = zLength * cos(ZRadLocal);
  const float zyLocal = zLength * sin(ZRadLocal);
  const float zzLocal = 0;
  
  const float yLength = getYLength();  
  const float yxLocal = yLength * cos(YRadLocal) * cos(ZRadLocal);
  const float yyLocal = yLength * cos(YRadLocal) * sin(ZRadLocal);
  const float yzLocal = yLength * sin(YRadLocal);
  
  const float xLength = getXLength();  
  const float xxLocal = xLength * cos(YRadLocal) * cos(ZRadLocal);
  const float xyLocal = xLength * cos(YRadLocal) * sin(ZRadLocal);
  const float xzLocal = xLength * sin(YRadLocal);

  /*Serial.printf("\nname: %s\n", name().c_str());
  Serial.printf("ZRadLocal: %f, YRadLocal: %f, XRadLocal: %f\n", ZRadLocal, YRadLocal, ZRadLocal);
  Serial.printf("ZRad: %f, YRad: %f, XRad: %f\n", ZRad, YRad, XRad);
  Serial.printf("zxLocal: %f, zyLocal: %f, zzLocal: %f\n", zxLocal, zyLocal, zzLocal);
  Serial.printf("yxLocal: %f, yyLocal: %f, yzLocal: %f\n", yxLocal, yyLocal, yzLocal);
  Serial.printf("xxLocal: %f, xyLocal: %f, xzLocal: %f\n", xxLocal, xyLocal, xzLocal);*/

  this->x = zxLocal + yxLocal + xxLocal;
  this->y = zyLocal + yyLocal + xyLocal;
  this->z = zzLocal + yzLocal + xzLocal;
  //Serial.printf("x: %f, y: %f, z: %f\n", this->x, this->y, this->z);  
  rotateX(XRad);
  //Serial.printf("rotate x x: %f, y: %f, z: %f\n", this->x, this->y, this->z);  
  rotateY(YRad);
  //Serial.printf("rotate y x: %f, y: %f, z: %f\n", this->x, this->y, this->z);  
  rotateZ(ZRad);
  //Serial.printf("rotate z x: %f, y: %f, z: %f\n", this->x, this->y, this->z);  
}

void ArmBase::rotateZ(const float zRad) { 
  const float sx = this->x;  
  const float sy = this->y;
  const float sz = this->z;    
  this->x = sx * cos(zRad) - sy * sin(zRad);
  this->y = sx * sin(zRad) + sy * cos(zRad);
  this->z = sz;
}

void ArmBase::rotateX(const float xRad) { 
  const float sx = this->x;
  const float sy = this->y;
  const float sz = this->z;    
  this->x = sx;
  this->y = sy * cos(xRad) - sz * sin(xRad);
  this->z = sy * sin(xRad) + sz * cos(xRad);
  
}

void ArmBase::rotateY(const float yRad) { 
  const float sx = this->x;
  const float sy = this->y;
  const float sz = this->z;    
  this->x = sx * cos(yRad) - sz * sin(yRad);
  this->y = sy;
  this->z = sx * sin(yRad) + sz * cos(yRad);
}

void ArmBase::setRads(const float xRad, const float yRad, const float zRad)
{
  this->YRad = yRad;
  this->ZRad = zRad;
  this->XRad = xRad;
}

void ArmBase::setRadsLocal(const float xRad, const float yRad, const float zRad)
{
  this->XRadLocal = xRad;
  this->YRadLocal = yRad;
  this->ZRadLocal = zRad;
}

const float ArmBase::getXRad(const float angle)
{
  return (this->getXScale() * angle + this->getXBase()) * PI / 180.0;
}

const float ArmBase::getYRad(const float angle)
{
  return (this->getYScale() * angle + this->getYBase()) * PI / 180.0;
}

const float ArmBase::getZRad(const float angle)
{
  return (this->getZScale() * angle + this->getZBase()) * PI / 180.0;
}

const float ArmBase::getXRad()
{
  return this->XRad + this->XRadLocal;
}

const float ArmBase::getYRad()
{
  return this->YRad + this->YRadLocal;
}

const float ArmBase::getZRad()
{
  return this->ZRad + this->ZRadLocal;
}

const float ArmBase::getXAngleFromRad(const float rad)
{
  if (rad == NAN)
    return NAN;
  const float sdeg = rad / PI * 180.0;
  float sAngle = (sdeg - this->getXBase()) / this->getXScale();
  if (sAngle < 0)
    sAngle = 360 + sAngle;
  const float tAngle = sAngle - (trunc(sAngle / 360) * 360);
  return tAngle;
}

const float ArmBase::getYAngleFromRad(const float rad)
{
  if (rad == NAN)
    return NAN;
  const float sdeg = rad / PI * 180.0;
  float sAngle = (sdeg - this->getYBase()) / this->getYScale();
  if (sAngle < 0)
    sAngle = 360 + sAngle;
  const float tAngle = sAngle - (trunc(sAngle / 360) * 360);
  return tAngle;
}

const float ArmBase::getZAngleFromRad(const float rad)
{
  if (rad == NAN)
    return NAN;
  const float sdeg = rad / PI * 180.0;
  float sAngle = (sdeg - this->getZBase()) / this->getZScale();
  if (sAngle < 0)
    sAngle = 360 + sAngle;
  const float tAngle = sAngle - (trunc(sAngle / 360) * 360);
  return tAngle;
}

const float ArmBase::getRadFromXY(const float x, const float y)
{
  const float lengthV = sqrt(x * x + y * y);
  const float sr = (lengthV != 0) ? y / lengthV : 0;
  const float siny = asin(sr);
  const float res = (x < 0) ? (y < 0) ? -PI - siny : PI - siny
                             : siny;
  return res;
}

const float ArmBase::getYRadFromPos(const float localX, const float localY, const float localZ, const float length)
{
  const float tLength = isnan(length) ? getLength() : length;
  Serial.printf("ZRad: %f, localZ: %f, tLength: %f\n", ZRad, localZ, tLength);
  const float sinz = (tLength > 0) ? localZ / tLength : 0;
  const float sina = asin(sinz);
  const float asina = abs(sina);
  Serial.printf("sinz: %f, asinz: %f\n", sinz, sina);
  Coords coord = Coords(tLength, sina, ZRad);
  Serial.printf("coord.x: %f, coord.y: %f, coord.z: %f\n", coord.x, coord.y, coord.z);
  Serial.printf("localX: %f, localY: %f, localZ: %f\n", localX, localY, localZ);
  if (coord.isEqual(localX, localY, localZ))
    return sina;
  const float a1 = PI + asina;
  Coords coord2 = Coords(tLength, a1, ZRad);
  if (coord2.isEqual(localX, localY, localZ))
    return a1;
  const float a2 = PI - asina;
  Coords coord3 = Coords(tLength, a2, ZRad);
  if (coord3.isEqual(localX, localY, localZ))
    return a2;
  return NAN;
}

const bool ArmBase::isEqual(const float op1, const float op2, const float tolerance)
{
  const float delta = op1 - op2;
  return (delta < tolerance) && (delta > -tolerance);
}

const float ArmBase::getLengthFromPoint(const float x, const float y, const float z)
{
  const float dz = z;
  const float qz = dz * dz;
  const float qx = x * x;
  const float qy = y * y;
  return sqrt(qx + qy + qz);
}

const float ArmBase::validateXAngle(const float angle)
{
  if (isnan(angle))
    return ERROR_X_ANGLE_IS_NAN;
  if (angle < getXMinAngle())
    return ERROR_X_ANGLE_LESS_MIN;
  if (angle > getXMaxAngle())
    return ERROR_X_ANGLE_ABOVE_MAX;
  return angle;
}

const float ArmBase::validateYAngle(const float angle)
{
  if (isnan(angle))
    return ERROR_Y_ANGLE_IS_NAN;
  if (angle < getYMinAngle())
    return ERROR_Y_ANGLE_LESS_MIN;
  if (angle > getYMaxAngle())
    return ERROR_Y_ANGLE_ABOVE_MAX;
  return angle;
}

const float ArmBase::validateZAngle(const float angle)
{
  if (isnan(angle))
    return ERROR_Z_ANGLE_IS_NAN;
  if (angle < getZMinAngle())
    return ERROR_Z_ANGLE_LESS_MIN;
  if (angle > getZMaxAngle())
    return ERROR_Z_ANGLE_ABOVE_MAX;
  return angle;
}

const float ArmBase::getXAngle(const bool validate)
{
  const float angle = getXAngleFromRad(XRadLocal);
  if (!validate)
    return angle;
  return ArmBase::validateXAngle(angle);
}

const float ArmBase::getYAngle(const bool validate)
{
  const float angle = getYAngleFromRad(YRadLocal);
  if (!validate)
    return angle;
  return ArmBase::validateYAngle(angle);
}

const float ArmBase::getZAngle(const bool validate)
{
  const float angle = getZAngleFromRad(ZRadLocal);
  if (!validate)
    return angle;
  return ArmBase::validateZAngle(angle);
}

const bool ArmBase::isValid()
{
  if (isnan(XRad) || isnan(YRad) || isnan(ZRad))
    return false;
  const float sMinXRad = getXRad(getXMinAngle());
  const float sMaxXRad = getXRad(getXMaxAngle());
  const float minXRad = (sMinXRad > sMaxXRad) ? sMaxXRad : sMinXRad;
  const float maxXRad = (sMinXRad > sMaxXRad) ? sMinXRad : sMaxXRad;
  if ((XRad < minXRad) || (XRad > maxXRad))
    return false;
  const float sMinYRad = getYRad(getYMinAngle());
  const float sMaxYRad = getYRad(getYMaxAngle());
  const float minYRad = (sMinYRad > sMaxYRad) ? sMaxYRad : sMinYRad;
  const float maxYRad = (sMinYRad > sMaxYRad) ? sMinYRad : sMaxYRad;
  if ((YRad < minYRad) || (YRad > maxYRad))
    return false;
  const float sMinZRad = getZRad(getZMinAngle());
  const float sMaxZRad = getZRad(getZMaxAngle());
  const float minZRad = (sMinZRad > sMaxZRad) ? sMaxZRad : sMinZRad;
  const float maxZRad = (sMinZRad > sMaxZRad) ? sMinZRad : sMaxZRad;
  if ((ZRad < minZRad) || (ZRad > maxZRad))
    return false;
  return true;
}

const float ArmBase::getXLength(const float length, const float YRad, const float ZRad)
{
  return length * cos(YRad) * cos(ZRad);
}

const float ArmBase::getYLength(const float length, const float YRad, const float ZRad)
{
  return length * cos(YRad) * sin(ZRad);
}

const float ArmBase::getZLength(const float length, const float YRad, const float ZRad)
{  
  return length * sin(YRad);
}