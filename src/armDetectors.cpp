#include "armParams.h"
#include "armDetectors.h"

unsigned int ArmDetectors::instancesCount;
TwoWire ArmDetectors::baseLine = TwoWire(1);
MPU9250_WE ArmDetectors::baseMPU = MPU9250_WE(&ArmDetectors::baseLine, BASE_MPU_ADDR);
Adafruit_VL53L0X ArmDetectors::clawRange = Adafruit_VL53L0X();

ArmDetectorsData ArmDetectors::data = ArmDetectorsData();

TaskHandle_t ArmDetectors::loopTask = NULL;
const volatile SemaphoreHandle_t ArmDetectors::iicSemaphore = xSemaphoreCreateBinary();

bool ArmDetectors::vl53L0x_setup() {
  if (!ArmDetectors::clawRange.begin()) {
    Serial.print("\nFailed to boot claw range VL53L0X\n");
    ArmDetectors::data.clawRangeError = RANGE_ERROR_NOT_DETECTED;
    return false;
  }  
  ArmDetectors::clawRange.startRangeContinuous();  
  ArmDetectors::data.clawRangeError = RANGE_ERROR_OK;
  return true;
}


bool ArmDetectors::baseMPUSetup() {
  bool status = ArmDetectors::baseMPU.init();
  if (!status) {
    Serial.print("\nbaseMPU not detected");
    ArmDetectors::data.baseMPUError = MPU_ERROR_NOT_DETECTED;
    return status;
  }
      
  if(!ArmDetectors::baseMPU.initMagnetometer()) {
    ArmDetectors::data.baseMPUError = MPU_ERROR_MAGNETOMOTER_NOT_DETECTED;
  }

  ArmDetectors::data.baseMPUError = MPU_ERROR_OK;

  ArmDetectors::baseMPU.setAccOffsets(-14240.0, 18220.0, -17280.0, 15590.0, -20930.0, 12080.0);
  ArmDetectors::baseMPU.setGyrOffsets(45.0, 145.0, -105.0);
  ArmDetectors::baseMPU.enableGyrDLPF();
  ArmDetectors::baseMPU.setGyrDLPF(MPU9250_DLPF_6);

  ArmDetectors::baseMPU.setSampleRateDivider(5);  
  ArmDetectors::baseMPU.setGyrRange(MPU9250_GYRO_RANGE_250);

  ArmDetectors::baseMPU.setAccRange(MPU9250_ACC_RANGE_2G);

  ArmDetectors::baseMPU.enableAccDLPF(true);
  ArmDetectors::baseMPU.setAccDLPF(MPU9250_DLPF_6);

  ArmDetectors::baseMPU.enableAccAxes(MPU9250_ENABLE_XYZ);
  ArmDetectors::baseMPU.enableGyrAxes(MPU9250_ENABLE_XYZ);
  
  if (ArmDetectors::data.baseMPUError != MPU_ERROR_OK)
    ArmDetectors::baseMPU.setMagOpMode(AK8963_CONT_MODE_100HZ);
  return true;
}

ArmDetectors::ArmDetectors()
{
  ArmDetectors::instancesCount++;
  if (ArmDetectors::instancesCount > 1)
    return;
  
  bool fl = Wire.begin();
  if (!fl) {    
    Serial.print("\nFirst iic not initialized");  
  }
  bool sl = ArmDetectors::baseLine.begin(SECOND_LINE_SDA, SECOND_LINE_SCL);
  if (!sl) {
    Serial.print("\nSecond iic not initialized");
  }
  baseMPUSetup();
  vl53L0x_setup();
      
  xTaskCreate(
      ArmDetectors::loop,
      "ArmEnginesLoop",
      4096,
      NULL,
      tskIDLE_PRIORITY,
      &ArmDetectors::loopTask);
}

ArmDetectors::~ArmDetectors()
{
  ArmDetectors::instancesCount--;
  if (ArmDetectors::instancesCount > 0)
    return;
  Wire.end();
  ArmDetectors::baseLine.end();
  vTaskDelete(ArmDetectors::loopTask);
}

void ArmDetectors::loop(void *param)
{
  xSemaphoreGive(ArmDetectors::iicSemaphore);
  while (true)
  {
    xSemaphoreTake(ArmDetectors::iicSemaphore, portMAX_DELAY);

    if (ArmDetectors::data.baseMPUError == MPU_ERROR_OK) {
      ArmDetectors::data.baseGValues = ArmDetectors::baseMPU.getGValues();
      ArmDetectors::data.baseGyroValues = ArmDetectors::baseMPU.getGyrValues();
      ArmDetectors::data.baseMagValues = ArmDetectors::baseMPU.getMagValues();
      ArmDetectors::data.baseMPUTemperature = ArmDetectors::baseMPU.getTemperature();
      ArmDetectors::data.baseResultantG = ArmDetectors::baseMPU.getResultantG(ArmDetectors::data.baseGValues);  
      ArmDetectors::data.baseAngles = ArmDetectors::baseMPU.getAngles();
    } else {
      ArmDetectors::data.baseGValues = xyzFloat(NAN, NAN, NAN);
      ArmDetectors::data.baseGyroValues = xyzFloat(NAN, NAN, NAN);
      ArmDetectors::data.baseMagValues = xyzFloat(NAN, NAN, NAN);
      ArmDetectors::data.baseMPUTemperature = NAN;
      ArmDetectors::data.baseResultantG = NAN;
      ArmDetectors::data.baseAngles = xyzFloat(NAN, NAN, NAN);
    }

    if (ArmDetectors::data.clawRangeError == RANGE_ERROR_OK) {
      if (!ArmDetectors::clawRange.isRangeComplete()) {
        ArmDetectors::data.clawRange = ArmDetectors::clawRange.readRange();
      } else {
        ArmDetectors::data.clawRange = -1;
      }
    }
    xSemaphoreGive(ArmDetectors::iicSemaphore);
    vTaskDelay(DETECTORS_LOOP_DELAY);
  }
}


ArmDetectorsData ArmDetectors::getData() {
  //Serial.printf("\nget data");
  xSemaphoreTake(ArmDetectors::iicSemaphore, portMAX_DELAY);
  //Serial.printf("\nget data semaphore taken");
  const ArmDetectorsData data = ArmDetectors::data;
  xSemaphoreGive(ArmDetectors::iicSemaphore);
  //Serial.printf("\nget data semaphore given");
  return data;  
}