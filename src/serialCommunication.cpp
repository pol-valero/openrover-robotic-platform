#include <Arduino.h>
#include <SerialTransfer.h>

#include "serialCommunication.h"


SerialTransfer myTransfer;


void initSerial() {
    Serial.begin(115200);
    Serial1.begin(38400, SERIAL_8N1, 18, 17); // RX, TX
    myTransfer.begin(Serial1);
}

void serialSendFrame(Frame frame) {
  myTransfer.sendDatum(frame);
}


/*void serialSendTestValues() {
    //Sends test values to the other side every 200ms

    static unsigned long previousMillis = 0;
    static int i = 0;

    if (millis() - previousMillis >= 200) {
        previousMillis = millis();

        char buffer[10];
        sprintf(buffer, "%d", i++);

        sendFrame(0x08, buffer, 0x12);

        if (i > 250) {
        i = 0;
        }
        
    }

}*/

Frame serialReceiveFrame() {

    if (myTransfer.available()) {

        Frame frame;

        myTransfer.rxObj(frame);

        return frame;
        
    }

}

void usbSerialPrint(char* message) {
    //Prints a message to through the USB-C serial port, connected to the computer
    Serial.println(message);
}