#include <Arduino.h>
#include <RcTrainer.h>
#include <AltSoftSerial.h>

#include "SerialTransfer.h"
#include <Wire.h> //TODO: Remove wire.h if not needed (it is used for I2C communication)

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


// We listen on interrupt 0 which is digital input pin D2 (on Arduino uno)
// We can take a look at http://arduino.cc/en/Reference/attachInterrupt for mapping
// an interrupt number to pin number

RcTrainer tx;

RF24 radio(6, 5); // CE, CSN
const byte address[6] = "00001";
int datos[6];

typedef struct {
  int y1;
  int x1;
  int y2;
  int x2;
  int aux1;
  int aux2;
  int aux3;
  int aux4;
} RcValues;

struct __attribute__((packed)) STRUCT {
  uint8_t type; //Hex value identifying the type of frame
  char data[48]; //Data of the frame
  uint8_t checksum; //TODO: Delete. Only for testing purposes
} frame;

AltSoftSerial mySerial; //RX = 8, TX = 9
//auto &mySerial = Serial;
SerialTransfer myTransfer;


void setup() {
    Serial.begin(38400);  
    mySerial.begin(38400);
    myTransfer.begin(mySerial);

    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MAX);
    radio.startListening();

}

void sendFrame(uint8_t type, char data[48], uint8_t checksum) {

  frame.type = type;
  strcpy(frame.data, data);
  frame.checksum = checksum;

  myTransfer.sendDatum(frame);
}

void serialSendRcValues(RcValues rcvalues) {
  //Sends the Rc values every 100ms

  static unsigned long previousMillis = 0;

  if (millis() - previousMillis >= 100) {
    previousMillis = millis();
    char buffer[50]; //TODO: Lower to minimum
    sprintf(buffer, "X1:%d Y1:%d X2:%d Y2:%d AUX1:%d", rcvalues.x1, rcvalues.y1, rcvalues.x2, rcvalues.y2, rcvalues.aux1);
    sendFrame(0x03, buffer, 26);
  }

}

void serialReceiveResponse() {
  //Receives the response from the other side, checks every 5ms
  //TODO: We check every 5ms because we are testing with high volume of information; when we are done, we can increase this value
  //TODO: Change function name? This function will receive several types of frames in the future

  static unsigned long previousMillis = 0;

  if (millis() - previousMillis >= 5) {
    previousMillis = millis();
    if (myTransfer.available()) {
      myTransfer.rxObj(frame);

      char buffer[50];
      sprintf(buffer, "Type: %d Data: %s Checksum: %d", frame.type, frame.data, frame.checksum);
      Serial.println(buffer);
    }
  }
}

void radioReceiveResponse() {
  //Receives the response from the other side, checks every 5ms
  //TODO: Change function name? This function will receive several types of radio responses in the future

  static unsigned long previousMillis = 0;

  if (millis() - previousMillis >= 5) {
    previousMillis = millis();
    if(radio.available()){

      char buffer[200]; //TODO: Lower to minimum size

      radio.read(&datos, sizeof(datos));

      sprintf(buffer, "Palanca 1: %d Palanca 2: %d Joystick_X_1: %d Joystick_Y_1: %d Joystick_Y_2: %d Joystick_X_2: %d", datos[2], datos[1], datos[4], datos[5], datos[3], datos[0]);

      Serial.println(buffer);

    }
  }

}


void loop() {

    RcValues rcvalues;

    rcvalues.y1 = tx.getChannel(0);
    rcvalues.x1 = tx.getChannel(3);
    rcvalues.y2 = tx.getChannel(2);
    rcvalues.x2 = tx.getChannel(1);
    rcvalues.aux1 = tx.getChannel(4);
    rcvalues.aux2 = tx.getChannel(5);
    rcvalues.aux3 = tx.getChannel(6);
    rcvalues.aux4 = tx.getChannel(7);

    serialSendRcValues(rcvalues);
    serialReceiveResponse();
    radioReceiveResponse();
  
}
