#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "radioCommunication.h"
#include "frameTypesDefinition.h"

RF24 radio(48, 49); //CE, CSN pins
const byte readAddress[6] = "ADDR1";
const byte writeAddress[6] = "ADDR2";

void setupRadio() {
  radio.begin();
  radio.openReadingPipe(0, readAddress);
  radio.openWritingPipe(writeAddress);
  radio.setPALevel(RF24_PA_MAX);
  //radio.startListening();
}

Frame radioReceiveFrame() {

  Frame frame;
  frame.type = NOT_VALID;

  radio.startListening();

  if(radio.available()) {

    radio.read(&frame, sizeof(frame));

    return frame;

  }

  return frame;

}

void radioSendFrame(Frame frame) {

  if (frame.type != NOT_VALID) {
    radio.stopListening();
    radio.write(&frame, sizeof(frame));
  }
  
}