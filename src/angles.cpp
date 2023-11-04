#include "angles.h"

float getRad(const float angle) { 
  return angle / 180 * PI; 
}

float getAngle(const float rad) { 
  return rad / PI * 180; 
}