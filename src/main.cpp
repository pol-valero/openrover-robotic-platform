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
} Frame; //The same struct is used for radio and serial)

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

//TODO: Create function "radioSendFrame" with same parameters
void serialSendFrame(Frame frame) {
  myTransfer.sendDatum(frame);
}



Frame rcValuesToFrame(RcValues rcvalues) {

  Frame frame;

  frame.type = 0x01;
  frame.data2B[0] = rcvalues.y1;
  frame.data2B[1] = rcvalues.x1;
  frame.data2B[2] = rcvalues.y2;
  frame.data2B[3] = rcvalues.x2;
  frame.data1B[0] = rcvalues.aux1;
  frame.data1B[1] = rcvalues.aux2;
  frame.data1B[2] = rcvalues.aux3;
  frame.data1B[3] = rcvalues.aux4;

  return frame;

}


void serialSendRcValuesFrame(RcValues rcvalues) {
  //Sends the Rc values every 100ms

  static unsigned long previousMillis = 0;

  if (millis() - previousMillis >= 100) {
    previousMillis = millis();
    
    Frame frame = rcValuesToFrame(rcvalues);
    serialSendFrame(frame);

  }

}

void serialReceiveFrame() {
  //TODO: Add handle frame function inside?

  if (myTransfer.available()) {
    Frame frame;
    myTransfer.rxObj(frame);

    char buffer[100];
    sprintf(buffer, "Type: %d Data2B[0]: %d Data1B[0]: %d", frame.type, frame.data2B[0], frame.data1B[0]);  //TODO: Delete. Just for testing
    Serial.println(buffer);
  }
  
}

/*void radioReceiveFrame() {
  //Receives the response from the other side, checks every 20ms
  //TODO: Change function name? This function will receive several types of radio responses in the future

  static unsigned long previousMillis = 0;

  if (millis() - previousMillis >= 20) {
    previousMillis = millis();

    radio.startListening();

    if(radio.available()){

      char buffer[200]; //TODO: Lower to minimum size

      radio.read(&tinyFrame, sizeof(tinyFrame));

      sprintf(buffer, "Type:%d Data2B[0]:%d Data2B[1]:%d Data1B[0]:%d Data1B[2]: %d", tinyFrame.type, tinyFrame.data2B[0], tinyFrame.data2B[1], tinyFrame.data1B[0], tinyFrame.data1B[2]);

      Serial.println(buffer);

    }
  }

}*/

/*void radioSendData() {
  //Sends data to the other side, every 50ms
  //TODO: Change function name? This function will send several types of radio responses in the future

  static unsigned long previousMillis = 0;
  static int i = 0;

  if (millis() - previousMillis >= 50) {
    previousMillis = millis();

    tinyFrame.type = 0x04;
    tinyFrame.data2B[0] = i;
    tinyFrame.data2B[1] = i + 1;
    tinyFrame.data1B[0] = i + 2;
    tinyFrame.data1B[1] = i + 3;
    tinyFrame.data1B[2] = i + 4;

    i++;

    radio.stopListening();

    radio.write(&tinyFrame, sizeof(tinyFrame));

    if (i > 250) {
      i = 0;
    }
  
  }

}*/

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

    serialSendRcValuesFrame(rcvalues);
    //radioSendRcValuesFrame(rcvalues);
    serialReceiveFrame();
    //radioReceiveFrame();

    calculateBatteryPercentage();
    makeBuzzerSound();
  
}
