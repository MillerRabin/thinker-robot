#ifndef ArmErrors_h
#define ArmErrors_h

#include<vector>
#include <Arduino.h>


using namespace std;

namespace ArmErrors {
    struct Error {
        unsigned int code;
        std::string message;
    };

    static vector<Error> table {
        { 200, "OK" },
        { 400, "Must be 4 parameters for 4 engines" }
    };

    Error get(unsigned int errorCode);

}

#endif