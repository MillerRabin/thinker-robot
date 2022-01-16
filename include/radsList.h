#ifndef valid_rads_h
#define valid_rads_h

#include <Arduino.h>
#include <ArmShoulder.h>

//------ValidRads-----

class RadsList {
    private:
        std::vector<double> rads;
    public: 
        void addIfValid(ArmShoulder shoulder, const double rad, const double targetLength, const double x, const double y, const double z);        
        std::vector<double> getRads(const double rad);
};

#endif