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
      const double shoulderYAngle = NAN,
      const double shoulderZAngle = NAN,
      const double elbowYAngle = NAN,
      const double wristYAngle = NAN,
      const double clawXAngle = NAN,
      const double clawYAngle = NAN,
      const double clawZAngle = NAN,
      const double clawAngle = NAN);
  Position(ArmShoulder shoulder, ArmElbow elbow, ArmWrist wrist, ArmClaw claw);
  const bool isValid();
  const double getX();
  const double getY();
  const double getZ();
  ArmOperationResult getLastError() { return lastError; };
  std::string getLastErrorText() { return lastErrorText; };
  void setLastError(ArmOperationResult error, std::string text)
  {
    lastError = error;
    lastErrorText = text;
  };
};

#endif