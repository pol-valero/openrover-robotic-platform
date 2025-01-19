#include <Arduino.h>
#include <RcTrainer.h>
#include <AltSoftSerial.h>

#include "SerialTransfer.h"
#include <Wire.h> //TODO: Remove wire.h if not needed (it is used for I2C communication)

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "frameTypesDefinition.h"


// We listen on interrupt 0 which is digital input pin D2 (on Arduino uno)
// We can take a look at http://arduino.cc/en/Reference/attachInterrupt for mapping
// an interrupt number to pin number

RcTrainer tx;

RF24 radio(6, 5); // CE, CSN
const byte readAddress[6] = "ADDR2";
const byte writeAddress[6] = "ADDR1";

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


typedef struct __attribute__((packed)) {
  uint8_t type; //Hex value identifying the type of frame
  uint16_t data2B[4]; //Large data to be sent (2 bytes each) (ex.- joystick values 0-1023)
  uint8_t data1B[5]; //Small data to be sent (1 byte each)
  //TODO: Add field for checksum/validation?
} Frame; //The same struct is used for radio and serial

AltSoftSerial mySerial; //RX = 8, TX = 9
//auto &mySerial = Serial;
SerialTransfer myTransfer;

const int batt_input_pin = A0; //Analog pin where the battery voltage is read
const int buzzer_pin = A3; //Analog pin where the buzzer is connected


void setup() {

    Serial.begin(38400);  
    mySerial.begin(38400);
    myTransfer.begin(mySerial);

    radio.begin();
    radio.openReadingPipe(0, readAddress);
    radio.openWritingPipe(writeAddress);
    radio.setPALevel(RF24_PA_MAX);

    pinMode(batt_input_pin, INPUT);
    pinMode(buzzer_pin, OUTPUT);

    noTone(buzzer_pin);

}


void serialSendFrame(Frame frame) {

  if (frame.type != NOT_VALID) {
    myTransfer.sendDatum(frame);
  }

}

Frame rcValuesToFrame(RcValues rcValues) {

  Frame frame;

  frame.type = INF_F_RC_VALUES;
  frame.data2B[0] = rcValues.y1;
  frame.data2B[1] = rcValues.x1;
  frame.data2B[2] = rcValues.y2;
  frame.data2B[3] = rcValues.x2;
  frame.data1B[0] = rcValues.aux1;
  frame.data1B[1] = rcValues.aux2;
  frame.data1B[2] = rcValues.aux3;
  frame.data1B[3] = rcValues.aux4;

  return frame;

}

void serialSendRcValuesFrame(RcValues rcValues) {
  //Sends the Rc values every 100ms

  static unsigned long previousMillis = 0;

  if (millis() - previousMillis >= 100) {
    previousMillis = millis();
    
    Frame frame = rcValuesToFrame(rcValues);
    serialSendFrame(frame);

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

//TODO: Delete this test function
void radioSendData() {
  //Sends data to the other side, every 50ms

  static unsigned long previousMillis = 0;
  static int i = 0;

  Frame frame;

  if (millis() - previousMillis >= 50) {
    previousMillis = millis();

    frame.type = 0x04;
    frame.data2B[0] = i;
    frame.data2B[1] = i + 1;
    frame.data1B[0] = i + 2;
    frame.data1B[1] = i + 3;
    frame.data1B[2] = i + 4;

    i++;

    radioSendFrame(frame);

    if (i > 250) {
      i = 0;
    }
  
  }

}

void calculateBatteryPercentage() {

  char buffer[50]; //TODO: Lower to minimum size

  static unsigned long previousMillis = 0;

  if (millis() - previousMillis >= 100) { //TODO: Increase delay between readings 

    previousMillis = millis();

    int batt_divider_voltage_analog_value;  //Analog value read from the voltage divider output, which is approximately 4V when the battery is fully charged
    float batt_divider_voltage; //Volts of the voltage divider output. The voltage divider in the radio controller halves the voltage of the battery.
    int batt_percentage; //Percentage of the battery, from 0% (3.6V per cell) to 100% (4.2V per cell)

    batt_divider_voltage_analog_value = analogRead(batt_input_pin);

    batt_divider_voltage = (5.00 / 1023) * batt_divider_voltage_analog_value;

    //Notice: The max charge of the battery will be 4.2V per cell, 8.4V in total (2S battery).

    batt_percentage = map(batt_divider_voltage * 100, 4.2 * 100, 3.6 * 100, 100, 0); //We multiply by 100 because the map() function does not accept floats.

    Serial.print("Battery cell voltage: ");
    Serial.print(batt_divider_voltage);

    sprintf(buffer, "\tBattery percentage: %d", batt_percentage);
    Serial.println(buffer);

  }

}

//TODO: This is a test function. Delete on final or near-final version. 
void makeBuzzerSound() {

  static unsigned long previousMillis = 0;

  if (millis() - previousMillis >=10000) {
    previousMillis = millis();
    tone(buzzer_pin, 1000, 100);
  }

}

RcValues getSpektrumRcValues() {
  RcValues rcValues;

  rcValues.y1 = tx.getChannel(0);
  rcValues.x1 = tx.getChannel(3);
  rcValues.y2 = tx.getChannel(2);
  rcValues.x2 = tx.getChannel(1);
  rcValues.aux1 = tx.getChannel(4);
  rcValues.aux2 = tx.getChannel(5);
  rcValues.aux3 = tx.getChannel(6);
  rcValues.aux4 = tx.getChannel(7);

  return rcValues;
}

void handleReceivedFrame(Frame frame) {

  //Handles specific frames that must be handled by arduino nano, such as command frames 
  //to make buzzer sounds when the battery of the rc remote is low, or play a melody. 

  switch (frame.type) {

    case CMD_F_TEST:
      tone(buzzer_pin, 500, 100);

      //TODO: Delete. Just for testing
      Frame responseFrame;
      responseFrame.type = CMD_F_TEST;
      responseFrame.data2B[1] = frame.data2B[1];
      responseFrame.data1B[3] = frame.data1B[3];

      serialSendFrame(responseFrame);
      //
      break;

    default:
      break;
      
  }

}

void loop() {

    RcValues rcValues = getSpektrumRcValues();

    serialSendRcValuesFrame(rcValues);
    Frame serialFrame = serialReceiveFrame();
    //radioSendFrame(serialFrame);
    Frame radioFrame = radioReceiveFrame();
    radioFrame.type = CMD_F_TEST; //TODO: Delete. Just for testing
    serialSendFrame(radioFrame);

    radioSendData(); //TODO: Delete this test function

    handleReceivedFrame(serialFrame);
    //handleReceivedFrame(radioFrame);  //Right now not necessary, but may be if we expand functionalities

    calculateBatteryPercentage();
    makeBuzzerSound();
  
}
