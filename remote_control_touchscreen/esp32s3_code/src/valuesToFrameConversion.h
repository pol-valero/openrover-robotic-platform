#ifndef VALUES_TO_FRAME_CONVERSION_H
#define VALUES_TO_FRAME_CONVERSION_H

#include "sharedStructs.h"

Frame rcRadioEnableStatusToFrame(bool enabled);

Frame roverOpModeSelectionToFrame(int opMode);

Frame armServoSelectionToFrame(bool controlClawServosSelected);

Frame raspberryPiPowerStatusToFrame(bool powerOn);

#endif