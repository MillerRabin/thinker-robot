#ifndef arm_queue_h
#define arm_queue_h

#include "Arduino.h"
#include "armParams.h"

//------ArmQueueItem------

class ArmQueueItem
{
private:
  const bool isValid();

public:
  const float shoulderZAngle;
  const float shoulderYAngle;
  const float elbowYAngle;
  const float wristYAngle;
  const float clawXAngle;
  const float clawYAngle;
  const float clawZAngle;
  const float clawAngle;
  const unsigned int speed;
  const unsigned int postDelay;
  const bool valid;

  ArmQueueItem(
      const float shoulderYAngle = NAN,
      const float shoulderZAngle = NAN,
      const float elbowYAngle = NAN,
      const float wristYAngle = NAN,
      const float clawXAngle = NAN,
      const float clawYAngle = NAN,
      const float clawZAngle = NAN,
      const float clawAngle = NAN,
      const float speed = DEFAULT_SPEED,
      const unsigned int postDelay = DEFAULT_POST_DELAY);
};

class ArmQueue
{
private:
  ArmQueueItem *storage[COMMAND_QUEUE_SIZE];
  unsigned int head = 0;
  unsigned int tail = 0;
  unsigned int inc(const unsigned int val);
  const volatile SemaphoreHandle_t syncSemaphore;
  portMUX_TYPE qMux = portMUX_INITIALIZER_UNLOCKED;

public:
  ArmQueue();
  const bool isFull();
  const unsigned int size();
  ArmOperationResult enqueue(
      const float shoulderYAngle,
      const float shoulderZAngle,
      const float elbowYAngle,
      const float wristYAngle,
      const float clawXAngle,
      const float clawYAngle,
      const float clawZAngle,
      const float clawAngle,
      const float speed,
      const unsigned int postDelay);
  ArmQueueItem *dequeue();
};

#endif