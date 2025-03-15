#ifndef VALUES_FROM_FRAME_CONVERSION_H
#define VALUES_FROM_FRAME_CONVERSION_H

#include "sharedStructs.h"

RcValues rcValuesFromFrame(Frame frame);

int roverOpModeSelectionFromFrame(Frame frame);

bool armServoSelectionFromFrame(Frame frame);

bool raspberryPiPowerStatusFromFrame(Frame frame);

#endif