#ifndef arm_params_h
#define arm_params_h

#define APP_VERSION "2.0.0"

#define CLAW_ENGINE 1
#define CLAW_X_ENGINE 2
#define CLAW_Z_ENGINE 3
#define SHOULDER_Z_ENGINE 4
#define SHOULDER_Y_ENGINE 5
#define ELBOW_Y_ENGINE 6
#define WRIST_Y_ENGINE 7

//ESP32 WROOM GPIO PINS

#define CLAW_PIN 25
#define CLAW_X_PIN 26
#define CLAW_Z_PIN 33
#define SHOULDER_Z_PIN 27
#define SHOULDER_Y_PIN 14
#define WRIST_Y_PIN 12
#define ELBOW_Y_PIN 13

//Home point. Physical angles of engines
#define SHOULDER_Z_PHYSICAL_ANGLE 135
#define SHOULDER_Y_PHYSICAL_ANGLE 90
#define ELBOW_Y_PHYSICAL_ANGLE 230
#define WRIST_Y_PHYSICAL_ANGLE 235
#define CLAW_X_PHYSICAL_ANGLE 135
#define CLAW_Z_PHYSICAL_ANGLE 135
#define CLAW_PHYSICAL_ANGLE 0

//Physical length of shoulder
#define SHOULDER_LENGTH 87

//Conversion between logical angle and physical servo angle
//Calculated by formula
//logAngle = scale * physAngle + base;
//physAngle = (logicalAngle - base) / scale
#define SHOULDER_Y_BASE 225
#define SHOULDER_Y_SCALE -1
#define SHOULDER_Y_MIN 0
#define SHOULDER_Y_MAX 270

#define SHOULDER_Z_BASE -135
#define SHOULDER_Z_SCALE 1
#define SHOULDER_Z_MIN 0
#define SHOULDER_Z_MAX 270

#define ELBOW_LENGTH 95
#define ELBOW_Y_BASE 140
#define ELBOW_Y_SCALE -1
#define ELBOW_Y_MIN 0
#define ELBOW_Y_MAX 280

#define WRIST_LENGTH 70
#define WRIST_Y_BASE 135
#define WRIST_Y_SCALE -1
#define WRIST_Y_MIN 0
#define WRIST_Y_MAX 270

#define CLAW_LENGTH 210
#define CLAW_WIDTH -35
#define CLAW_X_BASE -135
#define CLAW_X_SCALE 1
#define CLAW_X_MIN 0
#define CLAW_X_MAX 270

#define CLAW_Z_BASE -135
#define CLAW_Z_SCALE 1
#define CLAW_Z_MIN 0
#define CLAW_Z_MAX 270

#define CLAW_BASE 0
#define CLAW_SCALE 1
#define CLAW_MIN 0
#define CLAW_MAX 140

#define BASE_HEIGHT 140
#define BASE_WIDTH 120

#define MIN_Z 0

#define MAX_LENGTH (SHOULDER_LENGTH + ELBOW_LENGTH + WRIST_LENGTH)
#define MAX_SUM_ANGLE 240

#define COMMAND_QUEUE_SIZE 20

//Every engine movement will split into iterations
//Affects on arm movements smooth and speed. 
#define DEFAULT_ITERATIONS 50
//Default delay between commands in queue in ms
#define DEFAULT_POST_DELAY 500

//Default delay between iterations. Affects on arm movements smooth and speed
#define DEFAULT_ITERATION_DELAY 15

enum ArmOperationResult {
    ARM_OPERATION_SUCCESS = 0,
    ERROR_SHOULDER_Y_ANGLE_IS_NAN = -1,
    ERROR_SHOULDER_Y_ANGLE_LESS_MIN = -2,
    ERROR_SHOULDER_Y_ANGLE_ABOVE_MAX = -3,
    ERROR_SHOULDER_Z_ANGLE_IS_NAN = -4,
    ERROR_SHOULDER_Z_ANGLE_LESS_MIN = -5,
    ERROR_SHOULDER_Z_ANGLE_ABOVE_MAX = -6,
    ERROR_ELBOW_Y_ANGLE_IS_NAN = -7,
    ERROR_ELBOW_Y_ANGLE_LESS_MIN = -8,
    ERROR_ELBOW_Y_ANGLE_ABOVE_MAX = -9,
    ERROR_WRIST_Y_ANGLE_IS_NAN = -10,
    ERROR_WRIST_Y_ANGLE_LESS_MIN = -11,
    ERROR_WRIST_Y_ANGLE_ABOVE_MAX = -12,
    ERROR_CLAW_X_ANGLE_IS_NAN = -13,
    ERROR_CLAW_X_ANGLE_LESS_MIN = -14,
    ERROR_CLAW_X_ANGLE_ABOVE_MAX = -15,
    ERROR_CLAW_Z_ANGLE_IS_NAN = -16,
    ERROR_CLAW_Z_ANGLE_LESS_MIN = -17,
    ERROR_CLAW_Z_ANGLE_ABOVE_MAX = -18,
    ERROR_CLAW_ANGLE_IS_NAN = -19,
    ERROR_CLAW_ANGLE_LESS_MIN = -20,
    ERROR_CLAW_ANGLE_ABOVE_MAX = -21,
    ERROR_OUT_OF_RANGE = -22,
    ERROR_POINT_UNREACHABLE = -23,
    ERROR_SUM_OF_ANGLES_ABOVE_MAX = -24,
    ERROR_COMMAND_QUEUE_IS_FULL = -25,
    ERROR_COMMAND_QUEUE_ITEM_IS_INVALID = -26, 
};

#endif