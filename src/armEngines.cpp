#include <ArduinoJson.h>
#include "armParams.h"
#include "armEngines.h"
#include "Strategy.h"
#include "armQueue.h"

// Start engines parameter

unsigned int ArmEngines::instancesCount;
volatile float ArmEngines::shoulderZAngle = SHOULDER_Z_PHYSICAL_ANGLE;
volatile float ArmEngines::shoulderYAngle = SHOULDER_Y_PHYSICAL_ANGLE;
volatile float ArmEngines::elbowYAngle = ELBOW_Y_PHYSICAL_ANGLE;
volatile float ArmEngines::wristYAngle = WRIST_Y_PHYSICAL_ANGLE;
volatile float ArmEngines::clawXAngle = CLAW_X_PHYSICAL_ANGLE;
volatile float ArmEngines::clawYAngle = CLAW_Y_PHYSICAL_ANGLE;
volatile float ArmEngines::clawZAngle = CLAW_Z_PHYSICAL_ANGLE;
volatile float ArmEngines::clawAngle = CLAW_PHYSICAL_ANGLE;

Position ArmEngines::lastPosition = Position();
TaskHandle_t ArmEngines::loopTask = NULL;
ArmQueue ArmEngines::queue = ArmQueue();

//-------ArmEngines---------

ArmEngines::ArmEngines()
{
  ArmEngines::instancesCount++;
  if (ArmEngines::instancesCount > 1)
    return;
    
  ledcSetup(CLAW_ENGINE, TIMER_FREQ, TIMER_WIDTH);
  ledcSetup(CLAW_X_ENGINE, TIMER_FREQ, TIMER_WIDTH);
  ledcSetup(CLAW_Y_ENGINE, TIMER_FREQ, TIMER_WIDTH);
  ledcSetup(CLAW_Z_ENGINE, TIMER_FREQ, TIMER_WIDTH);
  ledcSetup(SHOULDER_Z_ENGINE, TIMER_FREQ, TIMER_WIDTH);
  ledcSetup(SHOULDER_Y_ENGINE, TIMER_FREQ, TIMER_WIDTH);
  ledcSetup(ELBOW_Y_ENGINE, TIMER_FREQ, TIMER_WIDTH);
  ledcSetup(WRIST_Y_ENGINE, TIMER_FREQ, TIMER_WIDTH);
  ledcAttachPin(CLAW_PIN, CLAW_ENGINE);
  ledcAttachPin(CLAW_X_PIN, CLAW_X_ENGINE);
  ledcAttachPin(CLAW_Y_PIN, CLAW_Y_ENGINE);
  ledcAttachPin(CLAW_Z_PIN, CLAW_Z_ENGINE);
  ledcAttachPin(SHOULDER_Z_PIN, SHOULDER_Z_ENGINE);
  ledcAttachPin(SHOULDER_Y_PIN, SHOULDER_Y_ENGINE);
  ledcAttachPin(ELBOW_Y_PIN, ELBOW_Y_ENGINE);
  ledcAttachPin(WRIST_Y_PIN, WRIST_Y_ENGINE);
  xTaskCreate(
      ArmEngines::loop,
      "ArmEnginesLoop",
      4096,
      NULL,
      tskIDLE_PRIORITY,
      &ArmEngines::loopTask);
}

ArmEngines::~ArmEngines()
{
  ArmEngines::instancesCount--;
  if (ArmEngines::instancesCount > 0)
    return;
  vTaskDelete(ArmEngines::loopTask);
}

const unsigned int ArmEngines::getIterations(const float shoulderYAngleRange, const float shoulderZAngleRange, const float elbowYAngleRange, const float wristYAngleRange, const float clawXAngleRange,
                                             const float clawYAngleRange, const float clawZAngleRange, const float clawAngleRange)
{
  const float maxRange = getMaxRange(shoulderYAngleRange, shoulderZAngleRange, elbowYAngleRange, wristYAngleRange, clawXAngleRange, clawYAngleRange, clawZAngleRange, clawAngleRange);
  const unsigned int mi = int(maxRange);
  return (mi == 0) ? 1 : mi;
}

const float ArmEngines::getMaxRange(
    const float shoulderYAngleRange,
    const float shoulderZAngleRange,
    const float elbowYAngleRange,
    const float wristYAngleRange,
    const float clawXAngleRange,
    const float clawYAngleRange,
    const float clawZAngleRange,
    const float clawAngleRange)
{
  const float aShoulderYAngleRange = abs(shoulderYAngleRange);
  const float aShoulderZAngleRange = abs(shoulderZAngleRange);
  const float aElbowYAngleRange = abs(elbowYAngleRange);
  const float aWristYAngleRange = abs(wristYAngleRange);
  const float aClawXAngleRange = abs(clawXAngleRange);
  const float aClawYAngleRange = abs(clawYAngleRange);
  const float aClawZAngleRange = abs(clawZAngleRange);
  const float aClawAngleRange = abs(clawAngleRange);

  float max = aShoulderYAngleRange;
  max = (aShoulderZAngleRange > max) ? aShoulderZAngleRange : max;
  max = (aElbowYAngleRange > max) ? aElbowYAngleRange : max;
  max = (aWristYAngleRange > max) ? aWristYAngleRange : max;
  max = (aClawXAngleRange > max) ? aClawXAngleRange : max;
  max = (aClawYAngleRange > max) ? aClawYAngleRange : max;
  max = (aClawZAngleRange > max) ? aClawZAngleRange : max;
  max = (aClawAngleRange > max) ? aClawAngleRange : max;
  return max;
}

void ArmEngines::loop(void *param)
{
  while (true)
  {
    ArmQueueItem *aq = ArmEngines::queue.dequeue();

    if (aq == NULL)
    {
      vTaskDelay(1000 / portTICK_RATE_MS);
      continue;
    }

    const unsigned int speed = aq->speed;
    const unsigned int postDelay = aq->postDelay;

    const float targetShoulderYAngle = aq->shoulderYAngle;
    const float targetShoulderZAngle = aq->shoulderZAngle;
    const float targetElbowYAngle = aq->elbowYAngle;
    const float targetWristYAngle = aq->wristYAngle;
    const float targetClawXAngle = aq->clawXAngle;
    const float targetClawYAngle = aq->clawYAngle;
    const float targetClawZAngle = aq->clawZAngle;
    const float targetClawAngle = aq->clawAngle;

    delete aq;

    const float sourceShoulderYAngle = ArmEngines::shoulderYAngle;
    const float sourceShoulderZAngle = ArmEngines::shoulderZAngle;
    const float sourceElbowYAngle = ArmEngines::elbowYAngle;
    const float sourceWristYAngle = ArmEngines::wristYAngle;    
    const float sourceClawXAngle = ArmEngines::clawXAngle;    
    const float sourceClawYAngle = ArmEngines::clawYAngle;    
    const float sourceClawZAngle = ArmEngines::clawZAngle;    
    const float sourceClawAngle = ArmEngines::clawAngle;
    
    const float shoulderYAngleRange = targetShoulderYAngle - sourceShoulderYAngle;
    const float shoulderZAngleRange = targetShoulderZAngle - sourceShoulderZAngle;
    const float elbowYAngleRange = targetElbowYAngle - sourceElbowYAngle;
    const float wristYAngleRange = targetWristYAngle - sourceWristYAngle;
    const float clawXAngleRange = targetClawXAngle - sourceClawXAngle;
    const float clawYAngleRange = targetClawYAngle - sourceClawYAngle;
    const float clawZAngleRange = targetClawZAngle - sourceClawZAngle;
    const float clawAngleRange = targetClawAngle - sourceClawAngle;

    const unsigned int iterations = getIterations(shoulderYAngleRange, shoulderZAngleRange, elbowYAngleRange, wristYAngleRange, clawXAngleRange, clawYAngleRange, clawZAngleRange, clawAngleRange) / speed;
    const unsigned int iterationDelay = DEFAULT_ITERATION_DELAY;

    for (unsigned int counter = 1; counter <= iterations; counter++)
    {
      const float shoulderYAngleInc = shoulderYAngleRange / iterations * counter;
      const float shoulderZAngleInc = shoulderZAngleRange / iterations * counter;
      const float elbowYAngleInc = elbowYAngleRange / iterations * counter;
      const float wristYAngleInc = wristYAngleRange / iterations * counter;
      const float clawXAngleInc = clawXAngleRange / iterations * counter;
      const float clawYAngleInc = clawYAngleRange / iterations * counter;
      const float clawZAngleInc = clawZAngleRange / iterations * counter;
      const float clawAngleInc = clawAngleRange / iterations * counter;

      ArmEngines::shoulderYAngle = (counter <= iterations) ? sourceShoulderYAngle + shoulderYAngleInc : targetShoulderYAngle;
      ArmEngines::shoulderZAngle = (counter <= iterations) ? sourceShoulderZAngle + shoulderZAngleInc : targetShoulderZAngle;
      ArmEngines::elbowYAngle = (counter <= iterations) ? sourceElbowYAngle + elbowYAngleInc : targetElbowYAngle;
      ArmEngines::wristYAngle = (counter <= iterations) ? sourceWristYAngle + wristYAngleInc : targetWristYAngle;
      ArmEngines::clawXAngle = (counter <= iterations) ? sourceClawXAngle + clawXAngleInc : targetClawXAngle;
      ArmEngines::clawYAngle = (counter <= iterations) ? sourceClawYAngle + clawYAngleInc : targetClawYAngle;
      ArmEngines::clawZAngle = (counter <= iterations) ? sourceClawZAngle + clawZAngleInc : targetClawZAngle;
      ArmEngines::clawAngle = (counter <= iterations) ? sourceClawAngle + clawAngleInc : targetClawAngle;

      const unsigned int shoulderY = degToCount(ArmEngines::shoulderYAngle, SHOULDER_Y_PHYSICAL_MAX, SHOULDER_Y_COUNT_LOW, SHOULDER_Y_COUNT_HIGH);
      const unsigned int shoulderZ = degToCount(ArmEngines::shoulderZAngle, 270);
      const unsigned int elbowY = degToCount(ArmEngines::elbowYAngle, 270, ELBOW_Y_COUNT_LOW, ELBOW_Y_COUNT_HIGH);
      const unsigned int wristY = degToCount(ArmEngines::wristYAngle, 270);
      const unsigned int clawX = degToCount(ArmEngines::clawXAngle, 270);
      const unsigned int clawY = degToCount(ArmEngines::clawYAngle, CLAW_Y_PHYSICAL_MAX, CLAW_Y_COUNT_LOW, CLAW_Y_COUNT_HIGH);
      const unsigned int clawZ = degToCount(ArmEngines::clawZAngle, 270);
      const unsigned int claw = degToCount(ArmEngines::clawAngle, 180);

      ledcWrite(SHOULDER_Z_ENGINE, shoulderZ);
      ledcWrite(SHOULDER_Y_ENGINE, shoulderY);
      ledcWrite(ELBOW_Y_ENGINE, elbowY);
      ledcWrite(WRIST_Y_ENGINE, wristY);
      ledcWrite(CLAW_X_ENGINE, clawX);
      ledcWrite(CLAW_Y_ENGINE, clawY);
      ledcWrite(CLAW_Z_ENGINE, clawZ);
      ledcWrite(CLAW_ENGINE, claw);
      vTaskDelay(iterationDelay / portTICK_RATE_MS);
    }
    vTaskDelay(postDelay / portTICK_RATE_MS);
  }
}

Position ArmEngines::set(
    const float shoulderYAngle,
    const float shoulderZAngle,
    const float elbowYAngle,
    const float wristYAngle,
    const float clawXAngle,
    const float clawYAngle,
    const float clawZAngle,
    const float clawAngle,
    const float speed,
    const unsigned int postDelay)
{
  Position pos = Position(shoulderYAngle, shoulderZAngle, elbowYAngle, wristYAngle, clawXAngle, clawYAngle, clawZAngle, clawAngle);
  if (!pos.isValid())
    return pos;
  ArmOperationResult res = ArmEngines::queue.enqueue(
      shoulderYAngle,
      shoulderZAngle,
      elbowYAngle,
      wristYAngle,
      clawXAngle,
      clawYAngle,
      clawZAngle,
      clawAngle,
      speed, postDelay);
  pos.setLastError(res, ArmError::getErrorText(res));
  if (res == ARM_OPERATION_SUCCESS)
    ArmEngines::lastPosition = pos;
  return pos;
}

Position ArmEngines::set(
    Position pos,
    const float speed,
    const unsigned int postDelay)
{

  if (!pos.isValid())
    return pos;
  ArmOperationResult res = ArmEngines::queue.enqueue(
      pos.shoulder.getYAngle(),
      pos.shoulder.getZAngle(),
      pos.elbow.getYAngle(),
      pos.wrist.getYAngle(),
      pos.claw.getXAngle(),
      pos.claw.getYAngle(),
      pos.claw.getZAngle(),
      pos.claw.getAngle(),
      speed,
      postDelay);
  pos.setLastError(res, ArmError::getErrorText(res));
  if (res == ARM_OPERATION_SUCCESS)
    ArmEngines::lastPosition = pos;
  return pos;
}

Position ArmEngines::set(JsonObject &jsonObj)
{
  Position pos = ArmEngines::getPosition();
  return set(
      getfloatDef(jsonObj, "shoulder-y", pos.shoulder.getYAngle()),
      getfloatDef(jsonObj, "shoulder-z", pos.shoulder.getZAngle()),
      getfloatDef(jsonObj, "elbow-y", pos.elbow.getYAngle()),
      getfloatDef(jsonObj, "wrist-y", pos.wrist.getYAngle()),
      getfloatDef(jsonObj, "claw-x", pos.claw.getXAngle()),
      getfloatDef(jsonObj, "claw-y", pos.claw.getYAngle()),
      getfloatDef(jsonObj, "claw-z", pos.claw.getAngle()),
      getfloatDef(jsonObj, "claw", pos.claw.getAngle()),
      getfloatDef(jsonObj, "speed", DEFAULT_SPEED),
      getfloatDef(jsonObj, "post-delay", DEFAULT_POST_DELAY));
}

float ArmEngines::getfloat(JsonObject &jsonObj, const char *key)
{
  if (!jsonObj.containsKey(key))
    return NAN;
  const char *str = jsonObj[key];
  return atof(str);
}

unsigned int ArmEngines::getUintDef(JsonObject &jsonObj, const char *key, unsigned int def)
{
  if (!jsonObj.containsKey(key))
    return def;
  const char *str = jsonObj[key];
  return atoi(str);
}

float ArmEngines::getfloatDef(JsonObject &jsonObj, const char *key, const float def)
{
  if (!jsonObj.containsKey(key))
    return def;
  const char *str = jsonObj[key];
  return atof(str);
}

const float ArmEngines::setEngine(const uint engine, const float angle)
{
  const uint tDeg = degToCount(angle, 270);
  ledcWrite(engine, tDeg);
  return angle;
}

const uint ArmEngines::degToCount(const float value, const uint maxDeg, const uint impulseMin, const uint impulseMax)
{
  const uint range = impulseMax - impulseMin;
  const float impulseLength = value / maxDeg * (range) + impulseMin;
  const float maxImpulseLength = (1.0 / TIMER_FREQ) * 1000000;
  const float diff = impulseLength / maxImpulseLength;
  const unsigned count = 65535 * diff;
  return count;
}

Position ArmEngines::applyStrategy(Strategy strategy)
{
  ArmOperationResult res = strategy.endPosition.getLastError();
  if (res != ARM_OPERATION_SUCCESS)
    return strategy.endPosition;
  return set(
      strategy.endPosition,
      strategy.speed,
      strategy.postDelay);
}

Position ArmEngines::getPosition()
{
  if (ArmEngines::lastPosition.isValid())
    return ArmEngines::lastPosition;

  ArmEngines::lastPosition = Position(
      ArmEngines::shoulderYAngle,
      ArmEngines::shoulderZAngle,
      ArmEngines::elbowYAngle,
      ArmEngines::wristYAngle,
      ArmEngines::clawXAngle,
      ArmEngines::clawZAngle,
      ArmEngines::clawAngle);

  return ArmEngines::lastPosition;
}