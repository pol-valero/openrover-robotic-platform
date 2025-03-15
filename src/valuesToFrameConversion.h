#ifndef VALUES_TO_FRAME_CONVERSION_H
#define VALUES_TO_FRAME_CONVERSION_H

#include "sharedStructs.h"

Frame roverBattValuesToFrame(BatteryValues batteryValues);

Frame environmentalValuesToFrame(EnvironmentalValues environmentalValues);

Frame speedometerValuesToFrame(SpeedometerValues speedometerValues);

Frame raspberryPiStatusValuesToFrame(RaspberryPiStatusValues rpiStatusValues);

#endif