#include <Arduino.h>

#include "rcValuesManager.h"
#include "valuesToFrameConversion.h"
#include "serialCommunication.h"
#include "radioCommunication.h"


int joyX1Pin = A1;
int joyY1Pin = A0;
int joyY2Pin = A2;
int joyX2Pin = A3;

int switch1Pin = 2;
int switch2Pin = 3;

int pushButtonPin = 4;

bool radioControlEnabled = false;

void setupRcInputs() {
  pinMode(joyX1Pin, INPUT);
  pinMode(joyY1Pin, INPUT);
  pinMode(joyY2Pin, INPUT);
  pinMode(joyX2Pin, INPUT);

  pinMode(switch1Pin, INPUT_PULLUP);
  pinMode(switch2Pin, INPUT_PULLUP);

  pinMode(pushButtonPin, INPUT_PULLUP);
}

//We need to correct the values of the RC remote, because the values are not centered at 0, and we want a range of -255..255 in each axis
void correctRcValues(RcValues &rcValues) {

  if (rcValues.x1 > 560 && rcValues.x1 < 650) {
   rcValues.x1 = 0;
  } else {
    if (rcValues.x1 <= 560) {
      rcValues.x1 = map(rcValues.x1, 560, 0, 0, 255);
    } else {
      rcValues.x1 = map(rcValues.x1, 650, 1023, 0, -255);
    }
  }

  if (rcValues.y1 > 550 && rcValues.y1 < 630) {
   rcValues.y1 = 0;
  } else {
    if (rcValues.y1 <= 550) {
      rcValues.y1 = map(rcValues.y1, 550, 0, -1, -255);
    } else {
      rcValues.y1 = map(rcValues.y1, 630, 1023, 0, 255);
    }
  }

  if (rcValues.x2 > 560 && rcValues.x2 < 650) {
   rcValues.x2 = 0;
  } else {
    if (rcValues.x2 <= 560) {
      rcValues.x2 = map(rcValues.x2, 560, 0, 0, 255);
    } else {
      rcValues.x2 = map(rcValues.x2, 650, 1023, 0, -255);
    }
  }

  if (rcValues.y2 > 550 && rcValues.y2 < 640) {
   rcValues.y2 = 0;
  } else {
    if (rcValues.y2 <= 550) {
      rcValues.y2 = map(rcValues.y2, 550, 0, 0, 255);
    } else {
      rcValues.y2 = map(rcValues.y2, 640, 1023, 0, -255);
    }
  }
  
}

RcValues getRcValues() {
  
  RcValues rcValues;

  rcValues.y1 = analogRead(joyY1Pin);
  rcValues.x1 = analogRead(joyX1Pin);
  rcValues.y2 = analogRead(joyY2Pin);
  rcValues.x2 = analogRead(joyX2Pin);
  rcValues.aux1 = !digitalRead(switch1Pin);
  rcValues.aux2 = !digitalRead(switch2Pin);
  rcValues.aux3 = !digitalRead(pushButtonPin);
  rcValues.aux4 = 0;

  rcValues.dataValid = true;

  correctRcValues(rcValues);

  return rcValues;

}

RcValues getRcValuesForSerial() {
  
  RcValues rcValues;
  rcValues.dataValid = false;

  static unsigned long previousMillis = 0;

  //Gets the RC values every 130ms
  if (millis() - previousMillis >= 130) {
    previousMillis = millis();

    rcValues = getRcValues();

  }

  return rcValues;

}

RcValues getRcValuesForRadio() {
  
  RcValues rcValues;
  rcValues.dataValid = false;

  static unsigned long previousMillis = 0;

  //Gets the RC values every 50ms
  if (millis() - previousMillis >= 50) {
    previousMillis = millis();

    rcValues = getRcValues();
    rcValues.dataValid = radioControlEnabled;

  }

  return rcValues;

}

void setRadioControlEnabled(bool enabled) {
  radioControlEnabled = enabled;
}

void serialSendRcValuesFrame() {

  RcValues rcValues = getRcValuesForSerial(); 

  if (rcValues.dataValid) {
    
    Frame frame = rcValuesToFrame(rcValues);
    serialSendFrame(frame);

  }

}

void radioSendRcValuesFrame() {
  
  RcValues rcValues = getRcValuesForRadio();

  if (rcValues.dataValid) {
    
    Frame frame = rcValuesToFrame(rcValues);
    radioSendFrame(frame);

  }

}