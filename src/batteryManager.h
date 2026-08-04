#ifndef BATERY_MANAGER_H
#define BATERY_MANAGER_H

#include "sharedStructs.h"

BatteryValues getRcBatteryValues();

void setupBatteryMonitor();

void serialSendBattValuesFrame();

#endif