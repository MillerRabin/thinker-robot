#include <ArmPart.h>

void ArmPart::build(ArmBase& base, const double xAngle, const double yAngle, const double zAngle) {                
    const double xRad = getXRad(xAngle);    
    const double yRad = getYRad(yAngle);    
    const double zRad = getZRad(zAngle);            
    setRadsLocal(xRad, yRad, zRad);    
    const double pxRad = base.getXRad();
    const double pyRad = base.getYRad();
    const double pzRad = base.getZRad();    
    setRads(pxRad, pyRad, pzRad);  
    updateCoords();  
}

const double ArmPart::getPointLength(ArmBase* prev, const double x, const double y, const double z) {    
    const double px = (prev == NULL) ? 0 : prev->x;
    const double py = (prev == NULL) ? 0 : prev->y;
    const double pz = (prev == NULL) ? 0 : prev->z;        
    const double sx = x - px;
    const double sy = y - py;
    const double sz = z - pz;
    const double qx = sx * sx;
    const double qy = sy * sy;
    const double qz = sz * sz;        
    return sqrt(qx + qy + qz);
}

void ArmPart::setPos(ArmBase* prev, const double x, const double y, const double z) {        
    const double px = (prev == NULL) ? 0 : prev->x;
    const double py = (prev == NULL) ? 0 : prev->y;
    const double pz = (prev == NULL) ? 0 : prev->z;        
    const double pzRad = (prev == NULL) ? 0 : prev->ZRad;        
    const double localX = x - px;
    const double localY = y - py;
    const double localZ = z - pz;
    const double erad = getYRadFromPos(localX, localY, localZ);    
    setRads(0, erad, pzRad);    
}