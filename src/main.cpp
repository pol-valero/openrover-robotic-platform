#include <Arduino.h>
#include <RcTrainer.h>
#include <AltSoftSerial.h>

#include "SerialTransfer.h"
#include <Wire.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "frameTypesDefinition.h"


// By default we listen on interrupt 0, which in Arduino nano, is digital input pin D2
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
  bool dataValid;
} RcValues;

typedef struct {
  float cellVoltage;
  int percentage;
  bool dataValid;
} BatteryValues;

typedef struct __attribute__((packed)) {
  uint8_t type; //Integer value identifying the type of frame
  int16_t data2B[4]; //Large data to be sent (2 bytes each) (ex.- joystick values -255...255)
  uint8_t data1B[5]; //Small data to be sent (1 byte each)
  //TODO: Add field for checksum/validation?
} Frame; //The same struct is used for radio and serial


AltSoftSerial mySerial; //RX = 8, TX = 9
//auto &mySerial = Serial;
SerialTransfer myTransfer;

const int batt_input_pin = A0; //Analog pin where the battery voltage is read
const int buzzer_pin = A3; //Analog pin where the buzzer is connected

bool radioControlEnabled = false;


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

  if (rcValues.dataValid) {
    
    Frame frame = rcValuesToFrame(rcValues);
    serialSendFrame(frame);

  }

}

Frame rcBattValuesToFrame(BatteryValues battValues) {

  Frame frame;
  
  frame.type = INF_F_RC_BAT_LEVEL;
  frame.data1B[0] = battValues.cellVoltage * 10; //We multiply by 10 because the float data is sent as an integer
  frame.data1B[1] = battValues.percentage;

  return frame;

}

void serialSendBattValuesFrame(BatteryValues battValues) {

  if (battValues.dataValid) {

    Frame frame = rcBattValuesToFrame(battValues);

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

void radioSendRcValuesFrame(RcValues rcValues) {

  if (rcValues.dataValid) {
    
    Frame frame = rcValuesToFrame(rcValues);
    radioSendFrame(frame);

  }

}


BatteryValues getBatteryValues() {

  //We are using a Lithium-Ion battery for the RC remote
  //We get the battery level every second

  static unsigned long previousMillis = 0;
  BatteryValues battValues;

  battValues.dataValid = false;

  if (millis() - previousMillis >= 1000) { 

    previousMillis = millis();

    int batt_divider_voltage_analog_value;  //Analog value read from the voltage divider output, which is approximately 4V when the battery is fully charged
    float batt_divider_voltage; //Volts of the voltage divider output. The voltage divider in the radio controller halves the voltage of the battery.
    int batt_percentage; //Percentage of the battery, from 0% (3.3V per cell) to 100% (4.2V per cell)

    batt_divider_voltage_analog_value = analogRead(batt_input_pin);

    batt_divider_voltage = (5.00 / 1023) * batt_divider_voltage_analog_value;

    //Notice: The max charge of the battery will be 4.2V per cell, 8.4V in total (2S battery).

    batt_percentage = map(batt_divider_voltage * 100, 4.2 * 100, 3.3 * 100, 100, 0); //We multiply by 100 because the map() function does not accept floats.

    if (batt_divider_voltage <= 3.3) {
      batt_percentage = 0;
    }

    battValues.cellVoltage = batt_divider_voltage;
    battValues.percentage = batt_percentage;
    battValues.dataValid = true;

  }

  return battValues;

}

//TODO: This is a test function. Delete on final or near-final version. 
void makeBuzzerSound() {

  static unsigned long previousMillis = 0;

  if (millis() - previousMillis >=10000) {
    previousMillis = millis();
    tone(buzzer_pin, 1000, 100);
  }

}

//We need to correct the values of the RC remote, because the values are not centered at 0, and we want a range of -255..255 in each axis
void correctRcValues(RcValues &rcValues) {

  if (rcValues.x1 > 470 && rcValues.x1 < 550) {
   rcValues.x1 = 0;
  } else {
    if (rcValues.x1 <= 470) {
      rcValues.x1 = map(rcValues.x1, 470, 0, -1, -255);
    } else {
      rcValues.x1 = map(rcValues.x1, 550, 1023, 0, 255);
    }
  }

  if (rcValues.y1 > 470 && rcValues.y1 < 550) {
   rcValues.y1 = 0;
  } else {
    if (rcValues.y1 <= 470) {
      rcValues.y1 = map(rcValues.y1, 470, 0, -1, -255);
    } else {
      rcValues.y1 = map(rcValues.y1, 550, 1023, 0, 255);
    }
  }

  if (rcValues.x2 > 470 && rcValues.x2 < 550) {
   rcValues.x2 = 0;
  } else {
    if (rcValues.x2 <= 470) {
      rcValues.x2 = map(rcValues.x2, 470, 0, -1, -255);
    } else {
      rcValues.x2 = map(rcValues.x2, 550, 1023, 0, 255);
    }
  }

  if (rcValues.y2 > 470 && rcValues.y2 < 550) {
   rcValues.y2 = 0;
  } else {
    if (rcValues.y2 <= 470) {
      rcValues.y2 = map(rcValues.y2, 470, 0, -1, -255);
    } else {
      rcValues.y2 = map(rcValues.y2, 550, 1023, 0, 255);
    }
  }

  //TODO: Once the final use of each AUX is determined, we will map the values to the correct range (ex.- 0..1)
  rcValues.aux1 = map(rcValues.aux1, 0, 1023, 0, 255); 
  rcValues.aux2 = map(rcValues.aux2, 0, 1023, 0, 255);
  rcValues.aux3 = map(rcValues.aux3, 0, 1023, 0, 255);
  rcValues.aux4 = map(rcValues.aux4, 0, 1023, 0, 255); 

  if (rcValues.aux1 < 10) {
    rcValues.aux1 = 0;
  } else {
    rcValues.aux1 = 1;
  }

  if (rcValues.aux2 < 10) {
    rcValues.aux2 = 0;
  } else if (rcValues.aux2 > 245) {
    rcValues.aux2 = 2;
  } else {
    rcValues.aux2 = 1;
  }

  if (rcValues.aux3 < 10) {
    rcValues.aux3 = 0;
  } else {
    rcValues.aux3 = 1;
  }

  if (rcValues.aux4 < 10) {
    rcValues.aux4 = 0;
  } else if(rcValues.aux4 > 245) {
    rcValues.aux4 = 2;
  } else {
    rcValues.aux4 = 1;
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

  rcValues.dataValid = true;

  correctRcValues(rcValues);

  return rcValues;

}

RcValues getSpektrumRcValuesForSerial () {
  
  RcValues rcValues;
  rcValues.dataValid = false;

  static unsigned long previousMillis = 0;

  //Gets the RC values every 130ms
  if (millis() - previousMillis >= 130) {
    previousMillis = millis();

    rcValues = getSpektrumRcValues();

  }

  return rcValues;

}

RcValues getSpektrumRcValuesForRadio () {
  
  RcValues rcValues;
  rcValues.dataValid = false;

  static unsigned long previousMillis = 0;

  //Gets the RC values every 50ms
  if (millis() - previousMillis >= 50) {
    previousMillis = millis();

    rcValues = getSpektrumRcValues();
    rcValues.dataValid = radioControlEnabled;

  }

  return rcValues;

}

bool rcRadioEnableStatusFromFrame(Frame frame) {
  return frame.data1B[0] == 1;
}

void handleReceivedFrame(Frame frame) {

  //Handles specific frames that must be handled by arduino nano, such as command frames 
  //to make buzzer sounds when the battery of the rc remote is low, or play a melody. 

  switch (frame.type) {

    case CMD_F_RC_BUZZER:
      tone(buzzer_pin, 1000, 100);
      break;

    case CMD_F_RC_RADIO_ENABLING:
      radioControlEnabled = rcRadioEnableStatusFromFrame(frame);
      break;

    case CMD_F_TEST:
      //TODO: Delete. Just for testing
      tone(buzzer_pin, 500, 100);

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

    RcValues rcValues; 
    rcValues = getSpektrumRcValuesForSerial();
    serialSendRcValuesFrame(rcValues);
    rcValues = getSpektrumRcValuesForRadio();
    radioSendRcValuesFrame(rcValues);

    BatteryValues battValues = getBatteryValues();
    serialSendBattValuesFrame(battValues);

    Frame serialFrame = serialReceiveFrame();
    radioSendFrame(serialFrame);
    Frame radioFrame = radioReceiveFrame();
    serialSendFrame(radioFrame);

    handleReceivedFrame(serialFrame);
    //handleReceivedFrame(radioFrame);  //Right now not necessary, but may be if we expand functionalities
  
}
