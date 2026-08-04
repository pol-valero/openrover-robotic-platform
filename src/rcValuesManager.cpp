#include <Arduino.h>

#include "rcValuesManager.h"
#include "valuesToFrameConversion.h"
#include "serialCommunication.h"
#include "radioCommunication.h"


bool radioControlEnabled = false;

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

  /*rcValues.y1 = tx.getChannel(0);
  rcValues.x1 = tx.getChannel(3);
  rcValues.y2 = tx.getChannel(2);
  rcValues.x2 = tx.getChannel(1);
  rcValues.aux1 = tx.getChannel(4);
  rcValues.aux2 = tx.getChannel(5);
  rcValues.aux3 = tx.getChannel(6);
  rcValues.aux4 = tx.getChannel(7);*/

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

void setRadioControlEnabled(bool enabled) {
  radioControlEnabled = enabled;
}

void serialSendRcValuesFrame() {

  RcValues rcValues = getSpektrumRcValuesForSerial(); 

  if (rcValues.dataValid) {
    
    Frame frame = rcValuesToFrame(rcValues);
    serialSendFrame(frame);

  }

}

void radioSendRcValuesFrame() {
  
  RcValues rcValues = getSpektrumRcValuesForRadio();

  if (rcValues.dataValid) {
    
    Frame frame = rcValuesToFrame(rcValues);
    radioSendFrame(frame);

  }

}