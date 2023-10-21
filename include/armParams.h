#ifndef arm_params_h
#define arm_params_h

#define APP_VERSION "3.0.1"

#define CLAW_ENGINE 1
#define CLAW_X_ENGINE 2
#define CLAW_Y_ENGINE 3
#define CLAW_Z_ENGINE 4
#define SHOULDER_Y_ENGINE 5
#define SHOULDER_Z_ENGINE 6
#define ELBOW_Y_ENGINE 7
#define WRIST_Y_ENGINE 8

//ESP32 WROOM GPIO PINS

#define SHOULDER_Z_PIN 23
#define SHOULDER_Y_PIN 27
#define ELBOW_Y_PIN 32
#define CLAW_PIN 18
#define CLAW_X_PIN 33
#define CLAW_Y_PIN 19
#define CLAW_Z_PIN 25
#define WRIST_Y_PIN 26

//Home point. Physical angles of engines
#define SHOULDER_Z_PHYSICAL_ANGLE 135
#define SHOULDER_Y_PHYSICAL_ANGLE 90
#define ELBOW_Y_PHYSICAL_ANGLE 135
#define WRIST_Y_PHYSICAL_ANGLE 45
#define CLAW_X_PHYSICAL_ANGLE 135
#define CLAW_Y_PHYSICAL_ANGLE 0
#define CLAW_Z_PHYSICAL_ANGLE 135
#define CLAW_PHYSICAL_ANGLE 0

#define SHOULDER_Y_LENGTH 97
#define SHOULDER_Y_BASE -90
#define SHOULDER_Y_SCALE 1
#define SHOULDER_Y_MIN 0
#define SHOULDER_Y_MAX 180
#define SHOULDER_Y_PHYSICAL_MAX 180
#define SHOULDER_Y_COUNT_LOW 566
#define SHOULDER_Y_COUNT_HIGH 2455

#define SHOULDER_Z_LENGTH 127
#define SHOULDER_Z_BASE -135
#define SHOULDER_Z_SCALE 1
#define SHOULDER_Z_MIN 0
#define SHOULDER_Z_MAX 270
#define SHOULDER_LENGTH SHOULDER_Y_LENGTH + SHOULDER_Z_LENGTH

#define ELBOW_Y_LENGTH 97
#define ELBOW_Y_BASE -135
#define ELBOW_Y_SCALE 1
#define ELBOW_Y_MIN 20
#define ELBOW_Y_MAX 260
#define ELBOW_Y_COUNT_LOW ENGINE_IMPULSE_MIN
#define ELBOW_Y_COUNT_HIGH ENGINE_IMPULSE_MAX
#define ELBOW_LENGTH ELBOW_Y_LENGTH

#define WRIST_Y_LENGTH 97
#define WRIST_Y_BASE -135
#define WRIST_Y_SCALE 1
#define WRIST_Y_MIN 10
#define WRIST_Y_MAX 270
#define WRIST_LENGTH WRIST_Y_LENGTH

#define CLAW_X_LENGTH 97
#define CLAW_X_BASE -135
#define CLAW_X_SCALE 1
#define CLAW_X_MIN 0
#define CLAW_X_MAX 270

#define CLAW_Y_LENGTH 80
#define CLAW_Y_BASE -90
#define CLAW_Y_SCALE 1
#define CLAW_Y_MIN 0
#define CLAW_Y_MAX 180
#define CLAW_Y_PHYSICAL_MAX 180
#define CLAW_Y_COUNT_LOW 500
#define CLAW_Y_COUNT_HIGH 2455

#define CLAW_Z_LENGTH 97
#define CLAW_Z_BASE -135
#define CLAW_Z_SCALE 1
#define CLAW_Z_MIN 10
#define CLAW_Z_MAX 260

#define CLAW_LENGTH CLAW_Z_LENGTH + CLAW_X_LENGTH

#define CLAW_WIDTH -35
#define CLAW_BASE 0
#define CLAW_SCALE 1
#define CLAW_MIN 0
#define CLAW_MAX 140

#define BASE_WIDTH 120

#define MIN_Z 0

#define MAX_LENGTH (SHOULDER_LENGTH + ELBOW_LENGTH + WRIST_LENGTH + CLAW_Z_LENGTH + CLAW_X_LENGTH)
#define MAX_SUM_ANGLE 240

#define COMMAND_QUEUE_SIZE 20

#define DEFAULT_SPEED 1
//Default delay between commands in queue in ms
#define DEFAULT_POST_DELAY 500

//Default delay between iterations. Affects on arm movements smooth and speed
#define DEFAULT_ITERATION_DELAY 15

#define SECOND_LINE_SDA 5
#define SECOND_LINE_SCL 4

#define BASE_MPU_ADDR 0x68
#define BASE_BME_ADDR 0x76

#define DETECTORS_LOOP_DELAY (500 / portTICK_RATE_MS)
#define SEALEVELPRESSURE_HPA (1013.25)

enum ArmOperationResult {
    ARM_OPERATION_SUCCESS = 0,
    ERROR_X_ANGLE_IS_NAN = -1,
    ERROR_X_ANGLE_LESS_MIN = -2,
    ERROR_X_ANGLE_ABOVE_MAX = -3,
    ERROR_Y_ANGLE_IS_NAN = -4,
    ERROR_Y_ANGLE_LESS_MIN = -5,
    ERROR_Y_ANGLE_ABOVE_MAX = -6,
    ERROR_Z_ANGLE_IS_NAN = -7,
    ERROR_Z_ANGLE_LESS_MIN = -8,
    ERROR_Z_ANGLE_ABOVE_MAX = -9,    
    ERROR_CLAW_ANGLE_IS_NAN = -10,
    ERROR_CLAW_ANGLE_LESS_MIN = -11,
    ERROR_CLAW_ANGLE_ABOVE_MAX = -12,    
    ERROR_OUT_OF_RANGE = -13,
    ERROR_POINT_UNREACHABLE = -14,
    ERROR_SUM_OF_ANGLES_ABOVE_MAX = -15,
    ERROR_COMMAND_QUEUE_IS_FULL = -16,
    ERROR_COMMAND_QUEUE_ITEM_IS_INVALID = -17
};

enum BMEError {
  BME_ERROR_OK = 0,
  BME_ERROR_NOT_DETECTED = -1
};

enum MPUError {
  MPU_ERROR_OK = 0,
  MPU_ERROR_NOT_DETECTED = -1,
  MPU_ERROR_MAGNETOMOTER_NOT_DETECTED = -2
};


#endif