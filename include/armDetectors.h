#ifndef arm_detectors_h
#define arm_detectors_h

#include <armParams.h>
#include <MPU9250_WE.h>
#include <Wire.h>
#include <Adafruit_BME280.h>

//
class ArmDetectorsData 
{
public:
  float baseAltitude;  
  float baseBMETemperature;
  xyzFloat baseGValues;
  xyzFloat baseGyroValues;
  xyzFloat baseMagValues;
  float baseMPUTemperature;
  float baseResultantG;
  xyzFloat baseAngles;
  int baseBMEError;
  int baseMPUError; 
};


//------ArmDetectors------

class ArmDetectors
{
private:
  static unsigned int instancesCount;  
  static Adafruit_BME280 baseBME;
  static MPU9250_WE baseMPU;  
  static TwoWire baseLine;
  static bool baseMPUSetup();
  static bool baseBMESetup();
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