#include <ArmWrist.h>

ArmWrist::ArmWrist(ArmElbow elbow, const double yAngle) {
    build(elbow, 0.0, yAngle, 0.0);
}

