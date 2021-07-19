#ifndef arm_error_h
#define arm_error_h

#include <Arduino.h>
#include <ArduinoJson.h>

class ArmError {
    private:
        std::vector<std::string> errors;
    public:    
        void addBaseError(const double x, const double y, const double z);
        void addMaxLengthError(const double length, const double maxLength);
        void addShoulderError();
        void addUnreachableError();
        void addElbowZError(const double z, const double minZ);
        void flushToJsonArray(JsonArray& arr);
        size_t size();
        void clear();
};



#endif