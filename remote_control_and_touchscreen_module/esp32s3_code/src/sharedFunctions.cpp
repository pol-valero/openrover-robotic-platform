#include <Arduino.h>

#include "sharedFunctions.h"

//Shared buffer, can be used by any function that includes the sharedFunctions.h header (as the sharedBuffer is declared as extern)
char sharedBuffer[100];

char* intValueToString(int value, const char* unit) {

    if (strcmp(unit, "") == 0) {
        sprintf(sharedBuffer, "%d", value);
        return sharedBuffer;
    } else {
        sprintf(sharedBuffer, "%d %s", value, unit);
        return sharedBuffer;
    }
}

char* floatValueToString(float value, const char* unit) {

    if (strcmp(unit, "") == 0) {
        sprintf(sharedBuffer, "%.1f", value);
        return sharedBuffer;
    } else {
        sprintf(sharedBuffer, "%.1f%s", value, unit);
        return sharedBuffer;
    }
}