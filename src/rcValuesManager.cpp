#include <Arduino.h>

#include "frameTypesDefinition.h"
#include "rcValuesManager.h"
#include "valuesToFrameConversion.h"


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

  correctRcValues(rcValues);

  return rcValues;

}

Frame getRcValuesFrameForSerial() {
  
  Frame frame;
  frame.type = NOT_VALID;

  static unsigned long previousMillis = 0;

  //Gets the RC values every 130ms
  if (millis() - previousMillis >= 130) {
    previousMillis = millis();

    RcValues rcValues = getRcValues();
    frame = rcValuesToFrame(rcValues);

  }

  return frame;

}

Frame getRcValuesFrameForRadio() {
  
  Frame frame;
  frame.type = NOT_VALID;

  static unsigned long previousMillis = 0;

  //Gets the RC values every 50ms
  if (millis() - previousMillis >= 50) {
    previousMillis = millis();

    RcValues rcValues = getRcValues();
    frame = rcValuesToFrame(rcValues);
    frame.type = (radioControlEnabled == true) ? frame.type : NOT_VALID;

  }

  return frame;

}

void setRadioControlEnabled(bool enabled) {
  radioControlEnabled = enabled;
}