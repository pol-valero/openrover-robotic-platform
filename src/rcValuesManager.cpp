#include <Arduino.h>

#include "rcValuesManager.h"
#include "serialCommunication.h"
#include "radioCommunication.h"
#include "frameTypesDefinition.h"


static char buffer[200];

RcValues rcValues;

bool rcLinkStatus = RADIO_KO; //TODO: Maybe instead of "RADIO" use RemoteControlLinkKO, as we will also use the same variable for web control status?

const int buzzer_pin = 29;

void setupBuzzer() {
  pinMode(buzzer_pin, OUTPUT);
}

void printRcValues() {

  //Print it every 300ms
  static unsigned long previousMillis = 0;

  if (millis() - previousMillis >= 300) {
    previousMillis = millis();
  
    if (rcLinkStatus == RADIO_OK) {
      sprintf(buffer, "Joystick1_x: %d  Joystick1_y: %d  Joystick2_x: %d  Joystick2_y: %d  Aux1: %d  Aux2: %d  Aux3: %d  Aux4: %d\n", 
      rcValues.x1, rcValues.y1, rcValues.x2, rcValues.y2, rcValues.aux1, rcValues.aux2, rcValues.aux3, rcValues.aux4);

      usbSerialPrint(buffer);

    } else {
      usbSerialPrint("ERROR: NO CONNECTION WITH RC TRANSMITTER OR ABNORMAL VALUES RECEIVED\n");
    }

  }

}

//TODO: This function will cease to be used when I modify the transmitter code so that it sends 0 if the joysticks are within its center range and so that it sends the correct ranges (ex.- 1 to 255 if right) and so that the lever send 1 if they are up
//TODO: Check that ALL values are in range and return RADIO_KO if any value is off. 
//TODO: Remove. Temporal function until we change the tramsitter code
/*void correctRcValuesReceived() {
}*/

void checkRcLinkStatus(Frame frame) {

  static long msTimeSinceLastPacket = 0;

   if(frame.type == INF_F_RC_VALUES) {

    msTimeSinceLastPacket = 0;

    rcLinkStatus = RADIO_OK;
  
  } else {

    static unsigned long previousMillis1 = 0;

    if (millis() - previousMillis1 >= 50) {
      previousMillis1 = millis();

      msTimeSinceLastPacket += 50;
    }

    //If we spend more than 500ms without receiving a packet, the rc connection will be considered as lost
    if (msTimeSinceLastPacket >= 500) {

      rcLinkStatus = RADIO_KO;
      
      static unsigned long previousMillis2 = 0;

      if (millis() - previousMillis2 >= 10000) {
        previousMillis2 = millis();
        tone(buzzer_pin, 200, 500);
      }

    }

  }

}

bool joystickIsRight(int joystickSelection) {

  if (joystickSelection == JOY_LEFT) {

    return rcValues.x1 > 0;

  } else {

    return rcValues.x2 > 0;

  }

}

bool joystickIsLeft(int joystickSelection) {

  if (joystickSelection == JOY_LEFT) {

    return rcValues.x1 < 0;

  } else {

    return rcValues.x2 < 0;
    
  }
  
}

bool joystickIsUp(int joystickSelection) {

  if (joystickSelection == JOY_LEFT) {

    return rcValues.y1 > 0;

  } else {

    return rcValues.y2 > 0;
    
  }

}

bool joystickIsDown(int joystickSelection) {

   if (joystickSelection == JOY_LEFT) {

    return rcValues.y1 < 0;

  } else {

    return rcValues.y2 < 0;
    
  }

}

bool joystickX_isCentered(int joystickSelection) {
  return !joystickIsRight(joystickSelection) && !joystickIsLeft(joystickSelection);
}

bool joystickY_isCentered(int joystickSelection) {
  return !joystickIsUp(joystickSelection) && !joystickIsDown(joystickSelection);
}

bool joystickXY_isCentered(int joystickSelection) {
  return !joystickX_isCentered(joystickSelection) && !joystickY_isCentered(joystickSelection);
}

bool getRcLinkStatus() {
  return rcLinkStatus;
}

void setRcValues(RcValues newRcValues) {
  rcValues = newRcValues;
}

RcValues getRcValues() {
  return rcValues;
}