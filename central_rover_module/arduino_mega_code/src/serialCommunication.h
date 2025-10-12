#ifndef SERIAL_COMMUNICATION_H
#define SERIAL_COMMUNICATION_H

#include "sharedStructs.h"

void setupSerial();

void serialSendFrame(Frame frame);

Frame serialReceiveFrame();

void usbSerialPrint(char* message);

void usbSerialPrintFloat(float number);

void testLoop();

#endif