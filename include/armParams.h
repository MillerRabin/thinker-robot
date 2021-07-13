#ifndef arm_params_h
#define arm_params_h

//Your wi-fi credentials
#define WIFI_SSID "Your-ssid-here"
#define WIFI_PASSWORD "Your-password-here"

//Login and password for http update
#define HTTP_USERNAME "admin"
#define HTTP_PASSWORD "your-update-password-here"


#define GRIPPER_ENGINE 1
#define GRIPPER_ROTATE_ENGINE 2
#define ROTATE_ENGINE 3
#define SHOULDER_ENGINE 4
#define ELBOW_ENGINE 5
#define WRIST_ENGINE 6

//ESP32 WROOM GPIO PINS

#define GRIPPER_PIN 25
#define GRIPPER_ROTATE_PIN 26
#define ROTATE_PIN 27
#define SHOULDER_PIN 14
#define WRIST_PIN 12
#define ELBOW_PIN 13

//Physical length of shoulder
#define SHOULDER_LENGTH 105

//Conversion between logical angle and physical servo angle
//Calculated by formula
//logAngle = scale * physAngle + base;
//physAngle = (logicalAngle - base) / scale
#define SHOULDER_BASE 0
#define SHOULDER_SCALE 1
#define SHOULDER_MIN 0
#define SHOULDER_MAX 180

#define ELBOW_LENGTH 95
#define ELBOW_BASE 140
#define ELBOW_SCALE -1
#define ELBOW_MIN 0
#define ELBOW_MAX 280

#define WRIST_LENGTH 160
#define WRIST_BASE 145
#define WRIST_SCALE -1
#define WRIST_MIN 0
#define WRIST_MAX 270

#define CLAW_LENGTH 100
#define CLAW_WIDTH -35
#define CLAW_BASE -45

#define ROTATE_BASE -135
#define ROTATE_MIN 0
#define ROTATE_MAX 270

#define BASE_HEIGHT 80
#define BASE_WIDTH 120

#define MAX_LENGTH SHOULDER_LENGTH + ELBOW_LENGTH + WRIST_LENGTH
#define MAX_SUM_ANGLE 240


#define ERROR_SHOULDER_ANGLE_IS_NAN -1;
#define ERROR_SHOULDER_ANGLE_LESS_MIN -2;
#define ERROR_SHOULDER_ANGLE_ABOVE_MAX -3;
#define ERROR_ELBOW_ANGLE_IS_NAN -4;
#define ERROR_ELBOW_ANGLE_LESS_MIN -5;
#define ERROR_ELBOW_ANGLE_ABOVE_MAX -6;
#define ERROR_WRIST_ANGLE_IS_NAN -7;
#define ERROR_WRIST_ANGLE_LESS_MIN -8;
#define ERROR_WRIST_ANGLE_ABOVE_MAX -9;

#endif