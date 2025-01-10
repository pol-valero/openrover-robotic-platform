#include <Arduino.h>
#include <lvgl.h>
#include <SerialTransfer.h>

#include "squareLineFiles/ui.h" //UI file header that SquareLineStudio generates
#include "screenConfig.h"


SerialTransfer myTransfer;

struct __attribute__((packed)) STRUCT {
  uint8_t type; //Hex value identifying the type of frame
  char data[48]; //Data of the frame
  uint8_t checksum; //TODO: Delete. Only for testing purposes
} frame;


void setup()
{
  Serial.begin(115200);
  Serial1.begin(38400, SERIAL_8N1, 18, 17); // RX, TX
  myTransfer.begin(Serial1);

  initScreen();

}


void sendFrame(uint8_t type, char data[48], uint8_t checksum) {

  frame.type = type;
  strcpy(frame.data, data);
  frame.checksum = checksum;

  myTransfer.sendDatum(frame);
}

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


void serialReceiveResponse() {
  //Receives the response from the other side, checks every 20ms
  //TODO: Change function name? This function will receive several types of frames in the future

  static unsigned long previousMillis = 0;

  if (millis() - previousMillis >= 20) {

    previousMillis = millis();

    if (myTransfer.available()) {

      myTransfer.rxObj(frame);

      char buffer[100];
      sprintf(buffer, "Type: %d Data: %s Checksum: %d", frame.type, frame.data, frame.checksum);
      
      Serial.println(buffer);
      lv_label_set_text(ui_Label1, buffer);

    }
  
  }

}

void runScreen() {
  
  runScreenImplementation();

}

void clickedButton(lv_event_t *e) {
    lv_label_set_text(ui_Label1, "Hey, this is a test!");
}

void loop() {

  serialReceiveResponse();
  serialSendTestValues();
  runScreen();

}
