#ifndef arm_engines_h
#define arm_engines_h

#define ENGINE_IMPULSE_MIN 500
#define ENGINE_IMPULSE_MAX 2500
#define TIMER_WIDTH 16
#define TIMER_FREQ 200

#include "armError.h"
#include "armParams.h"
#include "Strategy.h"
#include "Position.h"
#include "armQueue.h"

//------ArmEngines------

class ArmEngines
{
private:
  static unsigned int instancesCount;
  static volatile float shoulderZAngle;
  static volatile float shoulderYAngle;
  static volatile float elbowYAngle;
  static volatile float wristYAngle;
  static volatile float clawXAngle;
  static volatile float clawYAngle;
  static volatile float clawZAngle;
  static volatile float clawAngle;        
  static Position lastPosition;
  static const float setEngine(const uint engine, const float angle);
  static const uint degToCount(const float value, const uint maxDeg, const uint impulseMin = ENGINE_IMPULSE_MIN, const uint impulseMax = ENGINE_IMPULSE_MAX);
  static TaskHandle_t loopTask;
  static void loop(void *param);
  static const float getMaxRange(const float shoulderYAngleRange, const float shoulderZAngleRange, const float elbowYAngleRange, const float wristYAngleRange,
                                  const float clawXAngleRange, const float clawYAngleRange, const float clawZAngleRange, const float clawAngleRange);
  static const unsigned int getIterations(const float shoulderYAngleRange, const float shoulderZAngleRange, const float elbowYAngleRange, const float wristYAngleRange,
                                          const float clawXAngleRange, const float clawYAngleRange, const float clawZAngleRange, const float clawAngleRange);
public:
  ArmEngines();
  ~ArmEngines();
  Position set(
      const float shoulderYAngle,
      const float shoulderZAngle,
      const float elbowYAngle,
      const float wristYAngle,
      const float clawXAngle,
      const float clawYAngle,
      const float clawZAngle,
      const float clawAngle,
      const float speed = DEFAULT_SPEED,
      const unsigned int postDelay = DEFAULT_POST_DELAY);
  Position set(
      Position pos,
      const float speed = DEFAULT_SPEED,
      const unsigned int postDelay = DEFAULT_POST_DELAY);
  Position set(JsonObject &jsonObj);
  Position applyStrategy(Strategy strategy);
  static float getfloat(JsonObject &jsonObj, const char *key);
  static float getfloatDef(JsonObject &jsonObj, const char *key, const float def);
  static unsigned int getUintDef(JsonObject &jsonObj, const char *key, const unsigned int def);
  const float getShoulderZAngle() { return ArmEngines::shoulderZAngle; };
  const float getShoulderYAngle() { return ArmEngines::shoulderYAngle; };
  const float getElbowYAngle() { return ArmEngines::elbowYAngle; };
  const float getWristYAngle() { return ArmEngines::wristYAngle; };
  const float getClawXAngle() { return ArmEngines::clawXAngle; };
  const float getClawYAngle() { return ArmEngines::clawYAngle; };
  const float getClawZAngle() { return ArmEngines::clawZAngle; };
  const float getClawAngle() { return ArmEngines::clawAngle; };
  Position getPosition();
  static ArmQueue queue;
};

#endif