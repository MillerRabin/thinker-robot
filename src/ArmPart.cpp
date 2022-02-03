#include <ArmPart.h>

void ArmPart::build(ArmBase &base, const double xAngle, const double yAngle, const double zAngle)
{
  const double xRad = getXRad(xAngle);
  const double yRad = getYRad(yAngle);
  const double zRad = getZRad(zAngle);
  setRadsLocal(xRad, yRad, zRad);
  const double pxRad = base.getXRad();
  const double pyRad = base.getYRad();
  const double pzRad = base.getZRad();
  setRads(pxRad, pyRad, pzRad);
  updateCoords();
}

const double ArmPart::getPointLength(ArmBase *prev, const double x, const double y, const double z)
{
  const double px = (prev == NULL) ? 0 : prev->x;
  const double py = (prev == NULL) ? 0 : prev->y;
  const double pz = (prev == NULL) ? 0 : prev->z;
  const double sx = x - px;
  const double sy = y - py;
  const double sz = z - pz;
  const double qx = sx * sx;
  const double qy = sy * sy;
  const double qz = sz * sz;
  return sqrt(qx + qy + qz);
}

void ArmPart::setPos(ArmBase *prev, const double x, const double y, const double z)
{
  const double px = (prev == NULL) ? 0 : prev->x;
  const double py = (prev == NULL) ? 0 : prev->y;
  const double pz = (prev == NULL) ? 0 : prev->z;
  const double xrad = (prev == NULL) ? 0 : prev->XRad;
  const double pzRad = (prev == NULL) ? 0 : prev->ZRad;
  const double localX = x - px;
  const double localY = y - py;
  const double localZ = z - pz;
  const double zrad = ArmBase::getRadFromXY(x, y);
  const double yrad = getYRadFromPos(localX, localY, localZ);
  Serial.printf("yRad: %f, zRad: %f\n", yrad, zrad);

  //setRads(xrad, yrad, pzRad);

  /*const double xLength = getXLength();
    const double yLength = getYLength();
    const double zLength = getZLength();
    const double xyLength = xLength + yLength;
    Coords xCoord = Coords{ xyLength, yRad, zRad };
    const double cxx = x - xCoord.x;
    const double cyx = x - xCoord.y;
    const double czx = x - xCoord.z;
    Serial.printf("cxx: %f, cyx: %f, czx: %f\n", cxx, cyx, czx);*/
}

const double ArmPart::getMaxHorzLength()
{  
  const double zxMax = this->getZXLength(ZRadLocal);
  const double zyMax = this->getZYLength(ZRadLocal);
  const double yxMax = this->getYXLength(0, ZRadLocal);
  const double yyMax = this->getYYLength(0, ZRadLocal);  
  const double xxMax = this->getXXLength(0, ZRadLocal);
  const double xyMax = this->getXYLength(0, ZRadLocal);  
  const double xLn = zxMax + yxMax + xxMax;
  const double yLn = zyMax + yyMax + xyMax;  
  const double maxLength  = sqrt(xLn * xLn + yLn * yLn);  
  return maxLength;
}