#include <Arduino.h>
#include "armParams.h"
#include "Strategy.h"
#include "Position.h"
#include "armError.h"

//------ArmRoot------
ArmRoot::ArmRoot(const double ZRad, const double l, const double z) : x(l * cos(ZRad)),
                                                                      y(l * sin(ZRad)),
                                                                      z(z)
{
}

const bool ArmRoot::isValid()
{
  return (!isnan(x)) && (!isnan(y)) && (!isnan(z));
}

//------Strategy------
Position Strategy::tryHalfLength(ArmShoulder shoulder, const double length, const double x, const double y, const double z, const double clawXAngle, const double clawAngle)
{
  const double h = length / 2.0;
  const double qh = length / 4.0;

  const double pHalf = h + qh;
  Position phPos = tryShoulderLength(shoulder, pHalf, x, y, z, clawXAngle, clawAngle);
  if (phPos.isValid())
    return phPos;

  const double nHalf = h - qh;
  return tryShoulderLength(shoulder, nHalf, x, y, z, clawXAngle, clawAngle);
}

Position Strategy::getArmPosition(
    const double x, 
    const double y, 
    const double z, 
    const double clawXRad, 
    const double clawYRad, 
    const double clawZRad, 
    const double clawRad
  )
{
  /*Position rPos = tryShoulderRad(shoulder, PI / 2, x, y, z, clawXAngle, clawAngle);
  if (rPos.isValid())
    return rPos;
  double fullLength = ELBOW_LENGTH + WRIST_LENGTH + CLAW_LENGTH;

  Position fPos = tryShoulderLength(shoulder, fullLength, x, y, z, clawXAngle, clawAngle);
  if (fPos.isValid())
    return fPos;

  Position mPos = tryShoulderLength(shoulder, ELBOW_LENGTH, x, y, z, clawXAngle, clawAngle);
  if (mPos.isValid())
    return mPos;

  int count = 1;
  const double length = WRIST_LENGTH + CLAW_LENGTH;
  for (int i = 0; i < 2; i++)
  {
    count = count * 2;
    const double step = length / count;
    for (int j = 1; j < count; j = j + 2)
    {
      const double line = ELBOW_LENGTH + (step * j);
      Position pos = tryHalfLength(shoulder, line, x, y, z, clawXAngle, clawAngle);
      if (pos.isValid())
        return pos;
    }
  }*/

  return Position();
}

Position Strategy::tryShoulderLength(ArmShoulder shoulder, const double length, const double x, const double y, const double z, const double clawXAngle, const double clawAngle)
{
  /*ArmShoulder tShoulder = shoulder;
  vector<double> rads = tShoulder.getAvailableRads(length, x, y, z);
  if (rads.size() == 0)
  {
    return Position();
  }

  for (double rad : rads)
  {
    // tShoulder.setRads(rad, tShoulder.ZRad);
    Position pos = tryElbow(tShoulder, x, y, z, clawXAngle, clawAngle);
    if (pos.isValid())
      return pos;
  }*/
  return Position();
}

Position Strategy::tryShoulderRad(const double rad, const double x, const double y, const double z, const double clawXRad, const double clawYRad, const double clawZRad, const double clawRad)
{
  
  /*if (!tShoulder.isValid())
  {
    return Position();
  }
  return tryElbow(tShoulder, x, y, z, clawXAngle, clawAngle);*/
  return Position();
}

Position Strategy::tryElbowRoot(ArmShoulder shoulder, ArmRoot root, const double x, const double y, const double z, const double clawXAngle, const double clawAngle)
{
  ArmElbow elbow = startPosition.elbow;
  elbow.setRads(elbow.YRad, shoulder.ZRad);
  // elbow.setPosLocal(root.x, root.y, root.z);

  /*if (!elbow.isValid(shoulder))
      return Position();        */
  ArmWrist wrist = startPosition.wrist;
  const double cx = x - (shoulder.x + elbow.x);
  const double cy = y - (shoulder.y + elbow.y);
  const double cz = z - (shoulder.z + elbow.z);
  // wrist.setRads(wrist.YRad, elbow.ZRad);
  const double yRad = wrist.getYRadFromPos(cx, cy, cz, CLAW_LENGTH + WRIST_LENGTH);
  // wrist.setRads(yRad, wrist.ZRad);
  /*if (!wrist.isValid(elbow)) {
      return Position();
  } */

  ArmClaw claw = startPosition.claw;
  // const double cxRad = clawXAngle / 180 * PI;
  // const double clawRad = clawAngle / 180 * PI;
  // claw.setRads(cxRad, wrist.YRad, wrist.ZRad, clawRad);
  return Position(shoulder, elbow, wrist, claw);
}

Position Strategy::tryElbow(ArmShoulder shoulder, const double x, const double y, const double z, const double clawXAngle, const double clawAngle)
{
  ArmRoots roots = Strategy::getElbowRoots(shoulder, x, y, z, WRIST_LENGTH + CLAW_LENGTH);
  if (roots.r1.isValid())
  {
    Position fPos = tryElbowRoot(shoulder, roots.r1, x, y, z, clawXAngle, clawAngle);
    if (fPos.isValid())
      return fPos;
  }

  if (roots.r2.isValid())
  {
    return tryElbowRoot(shoulder, roots.r2, x, y, z, clawXAngle, clawAngle);
  }

  return Position();
}

/*Position Strategy::freeAngle(const double x, const double y, const double z, const double clawXAngle, const double clawAngle) {
    this->type = "Free angle";
    //const double vl = sqrt(x*x + y*y);
    if ((z <= BASE_HEIGHT) && (vl < BASE_WIDTH / 2)) {
        Position ep = Position();
        ep.setLastError(ERROR_OUT_OF_RANGE, ArmError::getBaseError(x, y, z));
        return ep;
    }

    const double sLength = ArmShoulder::getLengthFromPoint(x, y, z);
    if (sLength > MAX_LENGTH) {
        Position ep = Position();
        ep.setLastError(ERROR_OUT_OF_RANGE, ArmError::getMaxLengthError(sLength, MAX_LENGTH));
        return ep;
    }

    ArmShoulder shoulder = startPosition.shoulder;
    const double zRad = shoulder.getZRadFromXY(x, y);
    shoulder.setRads(shoulder.YRad, zRad);
    return getArmPosition(shoulder, x, y, z, clawXAngle, clawAngle);
}*/

/*Position Strategy::fixedAngle(const double x, const double y, const double z, const double clawXAngle, const double clawYAngle, const double clawZAngle,const double clawAngle) {
    this->type = "Fixed angle";
    const double rRad = clawZAngle / 180 * PI;
    const double cRad = clawYAngle / 180 * PI;

    const double cxl = CLAW_LENGTH * cos(cRad) * cos(rRad);
    const double cyl = CLAW_LENGTH * cos(cRad) * sin(rRad);
    const double czl = CLAW_LENGTH * sin(cRad) * cos(rRad);

    const double wx = x - cxl;
    const double wy = y - cyl;
    const double wz = z - czl;

    ArmShoulder shoulder = startPosition.shoulder;
    const double zRad = shoulder.getZRadFromXY(wx, wy);
    shoulder.setRads(shoulder.YRad, zRad);


    const double wxl = WRIST_LENGTH * cos(cRad) * cos(zRad);
    const double wyl = WRIST_LENGTH * cos(cRad) * sin(zRad);
    const double wzl = WRIST_LENGTH * sin(cRad);
    const double ex = wx - wxl;
    const double ey = wy - wyl;
    const double ez = wz - wzl;

    if (ez < MIN_Z) {
        Position ep = Position();
        ep.setLastError(ERROR_POINT_UNREACHABLE, ArmError::getElbowZError(ez, MIN_Z));
        return ep;
    }

    const double el = sqrt(ex*ex + ey*ey);

    if (el > SHOULDER_LENGTH + ELBOW_LENGTH) {
        Position ep = Position();
        ep.setLastError(ERROR_OUT_OF_RANGE, ArmError::getMaxLengthError(el,  SHOULDER_LENGTH + ELBOW_LENGTH + WRIST_LENGTH));
        return ep;
    }

    if ((z <= BASE_HEIGHT) &&
        ((el < BASE_WIDTH / 2) || (el < BASE_WIDTH / 2))) {
            Position ep = Position();
            ep.setLastError(ERROR_OUT_OF_RANGE, ArmError::getBaseError(x, y, z));
            return ep;
    }


    vector<double> rads = shoulder.getAvailableRads(ELBOW_LENGTH, ex, ey, ez);
    if (rads.size() == 0) {
        Position ep = Position();
        ep.setLastError(ERROR_OUT_OF_RANGE, ArmError::getShoulderError());
        return ep;
    }

    const double cxRad = clawXAngle / 180 * PI;
    const double clawRad = clawAngle / 180 * PI;

    for(double rad : rads) {
        shoulder.setRads(rad, shoulder.ZRad);
        if (!shoulder.isValid()) continue;
        ArmElbow elbow = startPosition.elbow;
        elbow.setRads(elbow.YRad, shoulder.ZRad);
        elbow.setPos(shoulder, ex, ey, ez);
        ArmWrist wrist = startPosition.wrist;
        wrist.setPos(shoulder, elbow, wx, wy, wz);
        ArmClaw claw = startPosition.claw;
        claw.setRads(cxRad, wrist.YRad, wrist.ZRad, clawRad);
        claw.setPos(shoulder, elbow, wrist, x, y, z);
        Position pos = Position(shoulder, elbow, wrist, claw);
        if (pos.isValid())
            return pos;

    }
    Position ep = Position();
    ep.setLastError(ERROR_POINT_UNREACHABLE, ArmError::getUnreachableError());
    return ep;
}*/

Strategy::Strategy(
    Position pos,
    const double x,
    const double y,
    const double z,
    const double clawXAngle,
    const double clawYAngle,
    const double clawZAngle,
    const double clawAngle,
    const double speed,
    const unsigned int postDelay) : startPosition(pos),
                                    speed(speed),
                                    postDelay(postDelay)
{
  const double sca = pos.claw.clawRad;
  const double scxa = pos.claw.XRad;
  const double scya = pos.claw.YRad;
  const double scza = pos.claw.ZRad;
  const double clawRad = isnan(clawAngle) ? sca : clawAngle / 180 * PI;
  const double clawXRad = isnan(clawXAngle) ? scxa : clawXAngle / 180 * PI;
  const double clawYRad = isnan(clawYAngle) ? scya : clawYAngle / 180 * PI;
  const double clawZRad = isnan(clawZAngle) ? scza : clawZAngle / 180 * PI;
  Serial.printf("x: %f, y: %f, z: %f\n", x, y, z);
  Serial.printf("clawRad: %f, clawXRad: %f, clawYRad: %f, clawZRad: %f\n", clawRad, clawXRad, clawYRad, clawZRad);
  Serial.printf("shoulder.length: %f, elbow.length: %f, wrist.length: %f, claw.length: %f\n",
                pos.shoulder.getLength(), pos.elbow.getLength(), pos.wrist.getLength(), pos.claw.getLength());

  const double cxLength = pos.claw.getXLength();
  const double cyLength = pos.claw.getYLength();
  const double szLength = pos.shoulder.getZLength();
  const double cxyLength = cxLength + cyLength;
  Coords xCoord = Coords{ cxyLength, clawYRad, clawZRad };
  const double cxx = x - xCoord.x;
  const double cyx = y - xCoord.y;
  const double czx = z - xCoord.z - szLength;
  const double pLength = ArmBase::getLengthFromPoint(cxx, cyx, czx);
  const double zRadTotal = ArmBase::getRadFromXY(cxx, cyx);

  Serial.printf("cxyLength: %f, xCoord.x: %f, xCoord.y: %f, xCoord.z: %f\n", cxyLength, xCoord.x, xCoord.y, xCoord.z);
  Serial.printf("cxx: %f, cyx: %f, czx: %f, pLength: %f\n", cxx, cyx, czx, pLength);
  Serial.printf("zRadTotal: %f, clawZRad: %f\n", zRadTotal, clawZRad);

  const double sHorzLength = pos.shoulder.getYLength();
  const double eHorzLength = pos.elbow.getYLength();
  const double wHorzLength = pos.wrist.getYLength();
  const double cHorzLength = pos.claw.getZLength();
  const double maxHorzLength = sHorzLength + eHorzLength + wHorzLength + cHorzLength;
  Serial.printf("sHorzLength: %f, eHorzLength: %f, wHorzlength: %f, cHorzLength: %f\n", sHorzLength, eHorzLength, wHorzLength, cHorzLength);
  Serial.printf("maxHorzLength: %f\n", maxHorzLength);
  if (pLength < maxHorzLength)
  {
    this->endPosition = Position();
    this->endPosition.setLastError(ERROR_POINT_UNREACHABLE, ArmError::getMaxLengthError(pLength, maxHorzLength));
    return;
  }
  
  ArmShoulder shoulder = pos.shoulder;
  ArmElbow elbow = pos.elbow;
  ArmWrist wrist = pos.wrist;
  ArmClaw claw = pos.claw;
  
  claw.setRadsLocal(clawXRad, clawYRad, scza, sca);
  return;
}

ArmRoots Strategy::getElbowRoots(ArmShoulder shoulder, const double x, const double y, const double z, const double length)
{
  const double sx = x - shoulder.x;
  const double sy = y - shoulder.y;
  const double sz = z - (shoulder.z);
  const double qx = sx * sx;
  const double qy = sy * sy;
  const double qz = sz * sz;

  const double sl = (x < 0) ? -1 * sqrt(qx + qy) : sqrt(qx + qy);
  const double ql = sl * sl;

  const double od = sqrt(ql + qz);
  const double qd = od * od;
  const double R = ELBOW_LENGTH;
  const double qR = R * R;
  const double r = length;
  const double qr = r * r;
  const double rSum = R + r;
  const double rDiff = abs(R - r);
  const double tolerance = 0.00001;
  const double d = ((od < (rSum + tolerance)) && (od > (rSum - tolerance))) ? rSum : od;

  if ((d <= rSum) && (d > rDiff))
  {
    const double b = (qr - qR + qd) / (2 * d);
    const double a = d - b;
    const double qa = a * a;
    const double h = sqrt(abs(qR - qa));
    const double pos0L = a / d * sl;
    const double pos0Z = a / d * sz;
    const double l1 = pos0L + h / d * sz;
    const double z1 = pos0Z - h / d * sl;
    const double l2 = pos0L - h / d * sz;
    const double z2 = pos0Z + h / d * sl;
    const double dPI = PI / 2;
    const double sRad = (shoulder.ZRad > dPI) ? shoulder.ZRad + PI : (shoulder.ZRad > 0)   ? shoulder.ZRad
                                                                 : (shoulder.ZRad >= -dPI) ? shoulder.ZRad
                                                                 : (shoulder.ZRad > -PI)   ? shoulder.ZRad + PI
                                                                                           : shoulder.ZRad;
    return ArmRoots(ArmRoot(sRad, l1, z1), ArmRoot(sRad, l2, z2));
  }

  return ArmRoots(ArmRoot(), ArmRoot());
}