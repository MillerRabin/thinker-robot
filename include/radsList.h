#ifndef valid_rads_h
#define valid_rads_h

#include <Arduino.h>
#include <ArmShoulder.h>

//------ValidRads-----

class RadsList {
    private:
        std::vector<float> rads;
    public: 
        void addIfValid(ArmShoulder shoulder, const float rad, const float targetLength, const float x, const float y, const float z);        
        std::vector<float> getRads(const float rad);
};

#endif