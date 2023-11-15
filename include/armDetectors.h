#ifndef arm_detectors_h
#define arm_detectors_h

#include <armParams.h>
#include <MPU9250_WE.h>
#include <Wire.h>
#include <Adafruit_VL53L0X.h>

//
class ArmDetectorsData 
{
public:
  xyzFloat baseGValues;
  xyzFloat baseGyroValues;
  xyzFloat baseMagValues;
  float baseMPUTemperature;
  float baseResultantG;
  xyzFloat baseAngles;
  int baseMPUError;
  int clawRangeError;
  uint16_t clawRange;
  float getBaseAzimuth();
};


//------ArmDetectors------

class ArmDetectors
{
private:
  static unsigned int instancesCount;  
  static MPU9250_WE baseMPU;  
  static TwoWire baseLine;
  static bool baseMPUSetup();
  static bool vl53L0x_setup();
  static Adafruit_VL53L0X clawRange;
  static TaskHandle_t loopTask;
  static void loop(void *param);
  static ArmDetectorsData data;  
  static const volatile SemaphoreHandle_t iicSemaphore;  
public:
  ArmDetectors();
  ~ArmDetectors();
  ArmDetectorsData getData();
};

#endif