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
  static volatile double shoulderZAngle;
  static volatile double shoulderYAngle;
  static volatile double elbowYAngle;
  static volatile double wristYAngle;
  static volatile double clawXAngle;
  static volatile double clawYAngle;
  static volatile double clawZAngle;
  static volatile double clawAngle;        
  static Position lastPosition;
  static const double setEngine(const uint engine, const double angle);
  static const uint degToCount(const double value, const uint maxDeg, const uint impulseMin = ENGINE_IMPULSE_MIN, const uint impulseMax = ENGINE_IMPULSE_MAX);
  static TaskHandle_t loopTask;
  static void loop(void *param);
  static const double getMaxRange(const double shoulderYAngleRange, const double shoulderZAngleRange, const double elbowYAngleRange, const double wristYAngleRange,
                                  const double clawXAngleRange, const double clawYAngleRange, const double clawZAngleRange, const double clawAngleRange);
  static const unsigned int getIterations(const double shoulderYAngleRange, const double shoulderZAngleRange, const double elbowYAngleRange, const double wristYAngleRange,
                                          const double clawXAngleRange, const double clawYAngleRange, const double clawZAngleRange, const double clawAngleRange);
public:
  ArmEngines();
  ~ArmEngines();
  Position set(
      const double shoulderYAngle,
      const double shoulderZAngle,
      const double elbowYAngle,
      const double wristYAngle,
      const double clawXAngle,
      const double clawYAngle,
      const double clawZAngle,
      const double clawAngle,
      const double speed = DEFAULT_SPEED,
      const unsigned int postDelay = DEFAULT_POST_DELAY);
  Position set(
      Position pos,
      const double speed = DEFAULT_SPEED,
      const unsigned int postDelay = DEFAULT_POST_DELAY);
  Position set(JsonObject &jsonObj);
  Position applyStrategy(Strategy strategy);
  static double getDouble(JsonObject &jsonObj, const char *key);
  static double getDoubleDef(JsonObject &jsonObj, const char *key, const double def);
  static unsigned int getUintDef(JsonObject &jsonObj, const char *key, const unsigned int def);
  const double getShoulderZAngle() { return ArmEngines::shoulderZAngle; };
  const double getShoulderYAngle() { return ArmEngines::shoulderYAngle; };
  const double getElbowYAngle() { return ArmEngines::elbowYAngle; };
  const double getWristYAngle() { return ArmEngines::wristYAngle; };
  const double getClawXAngle() { return ArmEngines::clawXAngle; };
  const double getClawYAngle() { return ArmEngines::clawYAngle; };
  const double getClawZAngle() { return ArmEngines::clawZAngle; };
  const double getClawAngle() { return ArmEngines::clawAngle; };
  Position getPosition();
  static ArmQueue queue;
};

#endif