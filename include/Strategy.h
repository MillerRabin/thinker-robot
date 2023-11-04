#ifndef strategy_h
#define strategy_h

#include <Arduino.h>
#include "Position.h"
#include "armParams.h"
#include "Target.h"
#include "armError.h"

//------EngineControl------

class EngineControl
{
public:
  const unsigned int engine;
  const float angle;
  EngineControl(const unsigned int engine, const float angle) : engine(engine), angle(angle){};
};

//------ArmRoot------

class ArmRoot
{
public:
  const float x;
  const float y;
  const float z;
  const bool isValid();
  ArmRoot(const float ZRad = NAN, const float l = NAN, const float z = NAN);
};

//------ArmRoots------

class ArmRoots
{
public:
  ArmRoot r1;
  ArmRoot r2;
  ArmRoots(ArmRoot r1, ArmRoot r2) : r1(r1), r2(r2){};
};

//------Strategy------

class Strategy
{
private:
  static ArmRoots getElbowRoots(ArmShoulder shoulder, const float x, const float y, const float z, const float length);
  static ArmRoot getValidElbowRoot(ArmElbow elbow, ArmShoulder shoulder, ArmRoots roots);
  Position tryElbowRoot(ArmShoulder shoulder, ArmRoot root, const float x, const float y, const float z, const float clawXAngle, const float clawAngle);
  Position tryElbow(ArmShoulder shoulder, const float x, const float y, const float z, const float clawXAngle, const float clawAngle);
  Position tryShoulderLength(ArmShoulder shoulder, const float length, const float x, const float y, const float z, const float clawXAngle, const float clawAngle);
  Position tryHalfLength(ArmShoulder shoulder, const float length, const float x, const float y, const float z, const float clawXAngle, const float clawAngle);
  Position tryShoulderRad(const float rad, const float x, const float y, const float z, const float clawXRad, const float clawYRad, const float clawZRad, const float clawRad);
  Position getArmPosition(const float x, const float y, const float z, const float clawXRad, const float clawYRad, const float clawZRad, const float clawRad);
  std::string type;

public:
  Strategy(
      Position pos,
      const Target target,
      const float clawRad,
      const float speed = DEFAULT_SPEED,
      const unsigned int postDelay = DEFAULT_POST_DELAY);
  std::vector<EngineControl> sequence;
  Position startPosition;
  Position endPosition;
  const float speed;
  const float clawRad;
  const unsigned int postDelay;
  std::string getType() { return type; };
};

#endif
