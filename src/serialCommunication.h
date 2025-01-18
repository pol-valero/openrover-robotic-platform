#ifndef SERIAL_COMMUNICATION_H
#define SERIAL_COMMUNICATION_H

typedef struct __attribute__((packed)) {
  uint8_t type; //Hex value identifying the type of frame
  uint16_t data2B[4]; //Large data to be sent (2 bytes each) (ex.- joystick values 0-1023)
  uint8_t data1B[5]; //Small data to be sent (1 byte each)
  //TODO: Add field for checksum/validation?
} Frame;

void initSerial();

Frame serialReceiveFrame();

void serialSendTestValues();

void usbSerialPrint(char* message);

//TODO: Send and receive frame headers

#endif