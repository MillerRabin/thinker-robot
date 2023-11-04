#include <ArmWrist.h>

ArmWrist::ArmWrist(ArmElbow elbow, const float yAngle) {
    build(elbow, 0.0, yAngle, 0.0);
}

