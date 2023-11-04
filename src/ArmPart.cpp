#include <ArmPart.h>

void ArmPart::build(ArmBase &base, const float xAngle, const float yAngle, const float zAngle)
{
  const float xRad = getXRad(xAngle);
  const float yRad = getYRad(yAngle);
  const float zRad = getZRad(zAngle);
  setRadsLocal(xRad, yRad, zRad);
  const float pxRad = base.getXRad();
  const float pyRad = base.getYRad();
  const float pzRad = base.getZRad();
  setRads(pxRad, pyRad, pzRad);
  updateCoords();
}

const float ArmPart::getPointLength(ArmBase *prev, const float x, const float y, const float z)
{
  const float px = (prev == NULL) ? 0 : prev->x;
  const float py = (prev == NULL) ? 0 : prev->y;
  const float pz = (prev == NULL) ? 0 : prev->z;
  const float sx = x - px;
  const float sy = y - py;
  const float sz = z - pz;
  const float qx = sx * sx;
  const float qy = sy * sy;
  const float qz = sz * sz;
  return sqrt(qx + qy + qz);
}

void ArmPart::setPos(ArmBase *prev, const float x, const float y, const float z)
{
  const float px = (prev == NULL) ? 0 : prev->x;
  const float py = (prev == NULL) ? 0 : prev->y;
  const float pz = (prev == NULL) ? 0 : prev->z;
  const float xrad = (prev == NULL) ? 0 : prev->XRad;
  const float pzRad = (prev == NULL) ? 0 : prev->ZRad;
  const float localX = x - px;
  const float localY = y - py;
  const float localZ = z - pz;
  const float zrad = ArmBase::getRadFromXY(x, y);
  const float yrad = getYRadFromPos(localX, localY, localZ);
  Serial.printf("yRad: %f, zRad: %f\n", yrad, zrad);

  //setRads(xrad, yrad, pzRad);

  /*const float xLength = getXLength();
    const float yLength = getYLength();
    const float zLength = getZLength();
    const float xyLength = xLength + yLength;
    Coords xCoord = Coords{ xyLength, yRad, zRad };
    const float cxx = x - xCoord.x;
    const float cyx = x - xCoord.y;
    const float czx = x - xCoord.z;
    Serial.printf("cxx: %f, cyx: %f, czx: %f\n", cxx, cyx, czx);*/
}

const float ArmPart::getMaxHorzLength()
{      
  return 0;
}