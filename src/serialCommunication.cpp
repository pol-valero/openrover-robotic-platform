#include <Arduino.h>

#include "SerialTransfer.h"

SerialTransfer myTransfer;

typedef struct __attribute__((packed)) STRUCT {
    char z;
    double y;
  } TestStruct;
  
  TestStruct testStruct;
  TestStruct testStruct2;

void setupSerial() {
    Serial.begin(9600);
    Serial1.begin(115200);
    myTransfer.begin(Serial1);

    testStruct.z = 'F';
    testStruct.y = 26.5;
}

void usbSerialPrint(char* message) {
    //Prints a message to through the USB serial port, connected to the computer
    Serial.print(message);
}

void usbSerialPrintFloat(float number) {
    //Prints a float number to through the USB serial port, connected to the computer
    //This function is useful as the arduino processors do not support floats/doubles in sprintf
    Serial.print(number);
}

void testLoop() {
    ///////////////////////////////////////// Stuff buffer with struct
  static unsigned long previousMillis = 0;

  if (millis() - previousMillis >= 1000) {
    previousMillis = millis();
    myTransfer.sendDatum(testStruct);
    Serial.print("Sent: ");
  }

  ///////////////////////////////////////// Receive data
  if (myTransfer.available()) {
    myTransfer.rxObj(testStruct2);
    Serial.print("Received: ");
    Serial.print(testStruct2.z);
    Serial.print(" ");
    Serial.println(testStruct2.y);
  }

}