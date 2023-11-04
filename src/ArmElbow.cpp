#include <ArmBase.h>
#include <ArmElbow.h>

ArmElbow::ArmElbow(ArmShoulder shoulder, const float yAngle) {
    build(shoulder, 0.0, yAngle, 0.0);
}