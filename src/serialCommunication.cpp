#include <Arduino.h>
#include <SerialTransfer.h>

//TODO: Delete these two includes (interaction with screen will be done outside of this file)
#include <lvgl.h>
#include "squareLineFiles/ui.h" //UI file header that SquareLineStudio generates


SerialTransfer myTransfer;

struct __attribute__((packed)) STRUCT {
    uint8_t type; //Hex value identifying the type of frame
    char data[48]; //Data of the frame
    uint8_t checksum; //TODO: Delete. Only for testing purposes
} frame; //TODO: Make typedef. Also, change structure to smaller new one with arrays. 


void initSerial() {
    Serial.begin(115200);
    Serial1.begin(38400, SERIAL_8N1, 18, 17); // RX, TX
    myTransfer.begin(Serial1);
}

void sendFrame(uint8_t type, char data[48], uint8_t checksum) {

    frame.type = type;
    strcpy(frame.data, data);
    frame.checksum = checksum;

    myTransfer.sendDatum(frame);
}

//TODO: Create receiveFrame function (with timeout as parameter?)

void serialSendTestValues() {
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

}

void serialReceiveResponse(char* response) {
    //Receives the response from the other side, checks every 20ms
    //TODO: Change function name? This function will receive several types of frames in the future

    sprintf(response, "");

    if (myTransfer.available()) {

        myTransfer.rxObj(frame);

        sprintf(response, "Type: %d Data: %s Checksum: %d", frame.type, frame.data, frame.checksum);
        
    }

}

void usbSerialPrint(char* message) {
    //Prints a message to through the USB-C serial port, connected to the computer
    Serial.println(message);
}