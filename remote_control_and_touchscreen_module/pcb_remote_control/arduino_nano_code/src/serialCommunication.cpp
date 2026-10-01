#include <Arduino.h>
#include <AltSoftSerial.h>

#include "SerialTransfer.h"
#include "serialCommunication.h"
#include "frameTypesDefinition.h"

AltSoftSerial mySerial; //For Arduino Uno/Nano, RX = 8, TX = 9. Cannot be changed.
//auto &mySerial = Serial;
SerialTransfer myTransfer;

void setupSerial() {
    Serial.begin(38400);  
    mySerial.begin(38400);
    myTransfer.begin(mySerial);
}

void serialSendFrame(Frame frame) {

    if (frame.type != NOT_VALID) {
        myTransfer.sendDatum(frame);
    }

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