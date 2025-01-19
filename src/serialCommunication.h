#ifndef SERIAL_COMMUNICATION_H
#define SERIAL_COMMUNICATION_H

#include "sharedStructs.h"

void initSerial();

void serialSendFrame(Frame frame);

Frame serialReceiveFrame();

void usbSerialPrint(char* message);

#endif