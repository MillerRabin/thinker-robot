#ifndef position_h
#define position_h

#include <armParams.h>
#include <Position.h>
#include <ArmShoulder.h>
#include <ArmElbow.h>
#include <ArmWrist.h>
#include <ArmClaw.h>

class Position
{
private:
  ArmOperationResult lastError;
  std::string lastErrorText;

public:
  ArmShoulder shoulder;
  ArmElbow elbow;
  ArmWrist wrist;
  ArmClaw claw;
  Position(
      const float shoulderYAngle = NAN,
      const float shoulderZAngle = NAN,
      const float elbowYAngle = NAN,
      const float wristYAngle = NAN,
      const float clawXAngle = NAN,
      const float clawYAngle = NAN,
      const float clawZAngle = NAN,
      const float clawAngle = NAN);
  Position(ArmShoulder shoulder, ArmElbow elbow, ArmWrist wrist, ArmClaw claw);
  const bool isValid();
  const float getX();
  const float getY();
  const float getZ();
  ArmOperationResult getLastError() { return lastError; };
  std::string getLastErrorText() { return lastErrorText; };
  void setLastError(ArmOperationResult error, std::string text)
  {
    lastError = error;
    lastErrorText = text;
  };
};

#endif