#ifndef STATUS_DATA_MANAGER_H
#define STATUS_DATA_MANAGER_H

#include "sharedStructs.h"

Frame getStatusDataFrame();

void enableStatusData(bool enable);

void checkRaspberryPiStatus(Frame frame);

#endif