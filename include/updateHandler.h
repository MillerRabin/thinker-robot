#include <ESPAsyncWebServer.h>
#include "initServer.h"

#ifndef update_handler_h
#define update_handler_h

extern bool shouldReboot;
void enableUpdate();

#endif