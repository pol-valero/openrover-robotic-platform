#include <Arduino.h>
//#include <SerialTransfer.h>   //TODO: Uncomment and put SerialTransfer related items for RPI Serial communication

//SerialTransfer myTransfer;

void setupSerial() {
    Serial.begin(9600);
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