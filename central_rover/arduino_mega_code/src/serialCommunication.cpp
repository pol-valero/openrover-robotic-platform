#include <Arduino.h>

#include "SerialTransfer.h"
#include "serialCommunication.h"
#include "frameTypesDefinition.h"

SerialTransfer myTransfer;

void setupSerial() {
    Serial.begin(9600);
    Serial1.begin(115200);
    myTransfer.begin(Serial1);

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

//TODO: Test function. Delete. 
void testLoop() {

    static unsigned long previousMillis = 0;

  if (millis() - previousMillis >= 2000) {
    previousMillis = millis();

    //Send FRAME with all fields filled
    Frame frame;
    frame.type = 240;
    frame.data2B[0] = 5001;
    frame.data2B[1] = -5002;
    frame.data2B[2] = 5003;
    frame.data2B[3] = -5004;
    frame.data1B[0] = 205;
    frame.data1B[1] = 206;
    frame.data1B[2] = 207;
    frame.data1B[3] = 208;
    frame.data1B[4] = 209;

    serialSendFrame(frame);

  }

  ///////////////////////////////////////// Receive data
  Frame receivedFrame = serialReceiveFrame();

  if (receivedFrame.type != NOT_VALID) {
  
    //We print all the fields
        Serial.print("Received frame type: ");
        Serial.println(receivedFrame.type);
        for (int i = 0; i < 4; i++) {
            Serial.print("Received frame data2B[");
            Serial.print(i);
            Serial.print("]: ");
            Serial.println(receivedFrame.data2B[i]);
        }
        for (int i = 0; i < 5; i++) {
            Serial.print("Received frame data1B[");
            Serial.print(i);
            Serial.print("]: ");
            Serial.println(receivedFrame.data1B[i]);
        }
    }

}