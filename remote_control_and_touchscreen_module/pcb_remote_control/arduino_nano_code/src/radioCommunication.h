#ifndef RADIO_COMMUNICATION_H
#define RADIO_COMMUNICATION_H

#include "sharedStructs.h"

void setupRadio();

Frame radioReceiveFrame();

void radioSendFrame(Frame frame);

#endif