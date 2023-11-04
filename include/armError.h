#ifndef arm_error_h
#define arm_error_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <string>
#include "armParams.h"

class ArmError
{
public:
  static std::string getBaseError(const float x, const float y, const float z);
  static std::string getMaxLengthError(const float length, const float maxLength);
  static std::string getShoulderError();
  static std::string getUnreachableError();
  static std::string getElbowZError(const float z, const float minZ);
  static std::string getSumError(const float sum);
  static std::string getErrorText(ArmOperationResult res);
};

#endif