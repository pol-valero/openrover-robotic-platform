#ifndef RC_VALUES_MANAGER_H
#define RC_VALUES_MANAGER_H

#include "sharedStructs.h"

void setupRcInputs();

void correctRcValues(RcValues &rcValues);

void setRadioControlEnabled(bool enabled);

void serialSendRcValuesFrame();

void radioSendRcValuesFrame();

#endif