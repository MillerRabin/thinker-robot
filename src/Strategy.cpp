#include "Strategy.h"

//------ArmRoot------
ArmRoot::ArmRoot(const float ZRad, const float l, const float z) : x(l * cos(ZRad)),
                                                                      y(l * sin(ZRad)),
                                                                      z(z) {}

const bool ArmRoot::isValid()
{
  return (!isnan(x)) && (!isnan(y)) && (!isnan(z));
}

//------Strategy------
Position Strategy::tryHalfLength(ArmShoulder shoulder, const float length, const float x, const float y, const float z, const float clawXAngle, const float clawAngle)
{
  const float h = length / 2.0;
  const float qh = length / 4.0;

  const float pHalf = h + qh;
  Position phPos = tryShoulderLength(shoulder, pHalf, x, y, z, clawXAngle, clawAngle);
  if (phPos.isValid())
    return phPos;

  const float nHalf = h - qh;
  return tryShoulderLength(shoulder, nHalf, x, y, z, clawXAngle, clawAngle);
}

Position Strategy::getArmPosition(
    const float x, 
    const float y, 
    const float z, 
    const float clawXRad, 
    const float clawYRad, 
    const float clawZRad, 
    const float clawRad
  )
{
  /*Position rPos = tryShoulderRad(shoulder, PI / 2, x, y, z, clawXAngle, clawAngle);
  if (rPos.isValid())
    return rPos;
  float fullLength = ELBOW_LENGTH + WRIST_LENGTH + CLAW_LENGTH;

  Position fPos = tryShoulderLength(shoulder, fullLength, x, y, z, clawXAngle, clawAngle);
  if (fPos.isValid())
    return fPos;

  Position mPos = tryShoulderLength(shoulder, ELBOW_LENGTH, x, y, z, clawXAngle, clawAngle);
  if (mPos.isValid())
    return mPos;

  int count = 1;
  const float length = WRIST_LENGTH + CLAW_LENGTH;
  for (int i = 0; i < 2; i++)
  {
    count = count * 2;
    const float step = length / count;
    for (int j = 1; j < count; j = j + 2)
    {
      const float line = ELBOW_LENGTH + (step * j);
      Position pos = tryHalfLength(shoulder, line, x, y, z, clawXAngle, clawAngle);
      if (pos.isValid())
        return pos;
    }
  }*/

  return Position();
}

Position Strategy::tryShoulderLength(ArmShoulder shoulder, const float length, const float x, const float y, const float z, const float clawXAngle, const float clawAngle)
{
  /*ArmShoulder tShoulder = shoulder;
  vector<float> rads = tShoulder.getAvailableRads(length, x, y, z);
  if (rads.size() == 0)
  {
    return Position();
  }

  for (float rad : rads)
  {
    // tShoulder.setRads(rad, tShoulder.ZRad);
    Position pos = tryElbow(tShoulder, x, y, z, clawXAngle, clawAngle);
    if (pos.isValid())
      return pos;
  }*/
  return Position();
}

Position Strategy::tryShoulderRad(const float rad, const float x, const float y, const float z, const float clawXRad, const float clawYRad, const float clawZRad, const float clawRad)
{
  
  /*if (!tShoulder.isValid())
  {
    return Position();
  }
  return tryElbow(tShoulder, x, y, z, clawXAngle, clawAngle);*/
  return Position();
}

Position Strategy::tryElbowRoot(ArmShoulder shoulder, ArmRoot root, const float x, const float y, const float z, const float clawXAngle, const float clawAngle)
{
  ArmElbow elbow = startPosition.elbow;
  elbow.setRads(elbow.YRad, shoulder.ZRad);
  // elbow.setPosLocal(root.x, root.y, root.z);

  /*if (!elbow.isValid(shoulder))
      return Position();        */
  ArmWrist wrist = startPosition.wrist;
  const float cx = x - (shoulder.x + elbow.x);
  const float cy = y - (shoulder.y + elbow.y);
  const float cz = z - (shoulder.z + elbow.z);
  // wrist.setRads(wrist.YRad, elbow.ZRad);
  const float yRad = wrist.getYRadFromPos(cx, cy, cz, CLAW_LENGTH + WRIST_LENGTH);
  // wrist.setRads(yRad, wrist.ZRad);
  /*if (!wrist.isValid(elbow)) {
      return Position();
  } */

  ArmClaw claw = startPosition.claw;
  // const float cxRad = clawXAngle / 180 * PI;
  // const float clawRad = clawAngle / 180 * PI;
  // claw.setRads(cxRad, wrist.YRad, wrist.ZRad, clawRad);
  return Position(shoulder, elbow, wrist, claw);
}

Position Strategy::tryElbow(ArmShoulder shoulder, const float x, const float y, const float z, const float clawXAngle, const float clawAngle)
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

/*Position Strategy::freeAngle(const float x, const float y, const float z, const float clawXAngle, const float clawAngle) {
    this->type = "Free angle";
    //const float vl = sqrt(x*x + y*y);
    if ((z <= BASE_HEIGHT) && (vl < BASE_WIDTH / 2)) {
        Position ep = Position();
        ep.setLastError(ERROR_OUT_OF_RANGE, ArmError::getBaseError(x, y, z));
        return ep;
    }

    const float sLength = ArmShoulder::getLengthFromPoint(x, y, z);
    if (sLength > MAX_LENGTH) {
        Position ep = Position();
        ep.setLastError(ERROR_OUT_OF_RANGE, ArmError::getMaxLengthError(sLength, MAX_LENGTH));
        return ep;
    }

    ArmShoulder shoulder = startPosition.shoulder;
    const float zRad = shoulder.getZRadFromXY(x, y);
    shoulder.setRads(shoulder.YRad, zRad);
    return getArmPosition(shoulder, x, y, z, clawXAngle, clawAngle);
}*/

/*Position Strategy::fixedAngle(const float x, const float y, const float z, const float clawXAngle, const float clawYAngle, const float clawZAngle,const float clawAngle) {
    this->type = "Fixed angle";
    const float rRad = clawZAngle / 180 * PI;
    const float cRad = clawYAngle / 180 * PI;

    const float cxl = CLAW_LENGTH * cos(cRad) * cos(rRad);
    const float cyl = CLAW_LENGTH * cos(cRad) * sin(rRad);
    const float czl = CLAW_LENGTH * sin(cRad) * cos(rRad);

    const float wx = x - cxl;
    const float wy = y - cyl;
    const float wz = z - czl;

    ArmShoulder shoulder = startPosition.shoulder;
    const float zRad = shoulder.getZRadFromXY(wx, wy);
    shoulder.setRads(shoulder.YRad, zRad);


    const float wxl = WRIST_LENGTH * cos(cRad) * cos(zRad);
    const float wyl = WRIST_LENGTH * cos(cRad) * sin(zRad);
    const float wzl = WRIST_LENGTH * sin(cRad);
    const float ex = wx - wxl;
    const float ey = wy - wyl;
    const float ez = wz - wzl;

    if (ez < MIN_Z) {
        Position ep = Position();
        ep.setLastError(ERROR_POINT_UNREACHABLE, ArmError::getElbowZError(ez, MIN_Z));
        return ep;
    }

    const float el = sqrt(ex*ex + ey*ey);

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


    vector<float> rads = shoulder.getAvailableRads(ELBOW_LENGTH, ex, ey, ez);
    if (rads.size() == 0) {
        Position ep = Position();
        ep.setLastError(ERROR_OUT_OF_RANGE, ArmError::getShoulderError());
        return ep;
    }

    const float cxRad = clawXAngle / 180 * PI;
    const float clawRad = clawAngle / 180 * PI;

    for(float rad : rads) {
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
    Target target,
    float clawRad,
    const float speed,
    const unsigned int postDelay) : startPosition(pos),
                                    clawRad(clawRad),
                                    speed(speed),
                                    postDelay(postDelay)
{
  const float sca = pos.claw.clawRad;
  const float scxa = pos.claw.XRad;
  const float scya = pos.claw.YRad;
  const float scza = pos.claw.ZRad;
  const float cr = isnan(clawRad) ? sca : clawRad;
  const float cx = target.angleX.middle();
  const float cy = target.angleY.middle();
  const float cz = target.angleZ.middle();
  const float clawXRad = isnan(cx) ? scxa : cx;
  const float clawYRad = isnan(cy) ? scya : cy;
  const float clawZRad = isnan(cz) ? scza : cz;
  Serial.printf("x: %f, y: %f, z: %f\n", target.x.middle(), target.y.middle(), target.z.middle());
  Serial.printf("clawRad: %f, clawXRad: %f, clawYRad: %f, clawZRad: %f\n", cr, clawXRad, clawYRad, clawZRad);
  Serial.printf("shoulder.length: %f, elbow.length: %f, wrist.length: %f, claw.length: %f\n",
                pos.shoulder.getLength(), pos.elbow.getLength(), pos.wrist.getLength(), pos.claw.getLength());

  const float cxLength = pos.claw.getXLength();
  const float cyLength = pos.claw.getYLength();
  const float szLength = pos.shoulder.getZLength();
  const float cxyLength = cxLength + cyLength;
  Coords xCoord = Coords{ cxyLength, clawYRad, clawZRad };
  const float cxx = target.x.middle() - xCoord.x;
  const float cyx = target.y.middle() - xCoord.y;
  const float czx = target.z.middle() - xCoord.z - szLength;
  const float pLength = ArmBase::getLengthFromPoint(cxx, cyx, czx);
  const float zRadTotal = ArmBase::getRadFromXY(cxx, cyx);

  Serial.printf("cxyLength: %f, xCoord.x: %f, xCoord.y: %f, xCoord.z: %f\n", cxyLength, xCoord.x, xCoord.y, xCoord.z);
  Serial.printf("cxx: %f, cyx: %f, czx: %f, pLength: %f\n", cxx, cyx, czx, pLength);
  Serial.printf("zRadTotal: %f, clawZRad: %f\n", zRadTotal, clawZRad);

  const float sHorzLength = pos.shoulder.getYLength();
  const float eHorzLength = pos.elbow.getYLength();
  const float wHorzLength = pos.wrist.getYLength();
  const float cHorzLength = pos.claw.getZLength();
  const float maxHorzLength = sHorzLength + eHorzLength + wHorzLength + cHorzLength;
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
  
  //claw.setRadsLocal(clawXRad, clawYRad, scza, sca);
  return;
}

ArmRoots Strategy::getElbowRoots(ArmShoulder shoulder, const float x, const float y, const float z, const float length)
{
  const float sx = x - shoulder.x;
  const float sy = y - shoulder.y;
  const float sz = z - (shoulder.z);
  const float qx = sx * sx;
  const float qy = sy * sy;
  const float qz = sz * sz;

  const float sl = (x < 0) ? -1 * sqrt(qx + qy) : sqrt(qx + qy);
  const float ql = sl * sl;

  const float od = sqrt(ql + qz);
  const float qd = od * od;
  const float R = ELBOW_LENGTH;
  const float qR = R * R;
  const float r = length;
  const float qr = r * r;
  const float rSum = R + r;
  const float rDiff = abs(R - r);
  const float tolerance = 0.00001;
  const float d = ((od < (rSum + tolerance)) && (od > (rSum - tolerance))) ? rSum : od;

  if ((d <= rSum) && (d > rDiff))
  {
    const float b = (qr - qR + qd) / (2 * d);
    const float a = d - b;
    const float qa = a * a;
    const float h = sqrt(abs(qR - qa));
    const float pos0L = a / d * sl;
    const float pos0Z = a / d * sz;
    const float l1 = pos0L + h / d * sz;
    const float z1 = pos0Z - h / d * sl;
    const float l2 = pos0L - h / d * sz;
    const float z2 = pos0Z + h / d * sl;
    const float dPI = PI / 2;
    const float sRad = (shoulder.ZRad > dPI) ? shoulder.ZRad + PI : (shoulder.ZRad > 0)   ? shoulder.ZRad
                                                                 : (shoulder.ZRad >= -dPI) ? shoulder.ZRad
                                                                 : (shoulder.ZRad > -PI)   ? shoulder.ZRad + PI
                                                                                           : shoulder.ZRad;
    return ArmRoots(ArmRoot(sRad, l1, z1), ArmRoot(sRad, l2, z2));
  }

  return ArmRoots(ArmRoot(), ArmRoot());
}