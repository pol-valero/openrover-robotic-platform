#ifndef VALUES_FROM_FRAME_CONVERSION_H
#define VALUES_FROM_FRAME_CONVERSION_H

#include "sharedStructs.h"

RcValues rcValuesFromFrame(Frame frame);

BatteryValues battValuesFromFrame(Frame frame);

EnvironmentalValues environmentalValuesFromFrame(Frame frame);

SpeedometerValues speedometerValuesFromFrame(Frame frame);

RaspberryPiStatusValues rpiStatusValuesFromFrame(Frame frame);

#endif