#ifndef valid_rads_h
#define valid_rads_h

#include <Arduino.h>

//------ValidRads-----

class RadsList {
    private:
        vector<double> rads;
    public: 
        void addIfValid(ArmShoulder shoulder, const double rad, const double targetLength, const double x, const double y, const double z);        
        vector<double> getRads(const double rad);
};

#endif