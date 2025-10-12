#include <Arduino.h>
#include <SerialTransfer.h>

#include "serialCommunication.h"
#include "frameTypesDefinition.h"


SerialTransfer myTransfer;


void initSerial() {
    Serial.begin(115200);
    Serial1.begin(38400, SERIAL_8N1, 18, 17); // RX, TX
    myTransfer.begin(Serial1);
}

void serialSendFrame(Frame frame) {
  myTransfer.sendDatum(frame);
}

Frame serialReceiveFrame() {

    Frame frame;
    frame.type = NOT_VALID;

    if (myTransfer.available()) {

        myTransfer.rxObj(frame);

        return frame;
        
    }

    return frame;

}

void usbSerialPrint(char* message) {
    //Prints a message to through the USB-C serial port, connected to the computer
    Serial.println(message);
}