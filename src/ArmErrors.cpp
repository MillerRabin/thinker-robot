#include "ArmErrors.h"

ArmErrors::Error ArmErrors::get(unsigned int errorCode) {
    unsigned int ln = ArmErrors::table.size();
    if (errorCode >= ln) {
        ArmErrors::Error err = ArmErrors::Error();
        err.code = 500;
        err.message = "No error in table";
        return err;
    }
    return ArmErrors::table[errorCode];
}