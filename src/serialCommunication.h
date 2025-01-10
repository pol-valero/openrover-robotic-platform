#ifndef SERIAL_COMMUNICATION_H
#define SERIAL_COMMUNICATION_H

void initSerial();

void serialReceiveResponse(char* response);

void serialSendTestValues();

//TODO: Send and receive frame headers

#endif