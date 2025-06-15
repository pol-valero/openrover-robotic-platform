#ifndef SHARED_FUNCTIONS_H
#define SHARED_FUNCTIONS_H

//Shared buffer, can be used by any function that includes the sharedFunctions.h header (as the sharedBuffer is declared as extern)
extern char sharedBuffer[100];

char* intValueToString(int value, const char* unit);

char* floatValueToString(float value, const char* unit);

#endif