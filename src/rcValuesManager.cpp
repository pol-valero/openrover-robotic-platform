#include <Arduino.h>

#include "rcValuesManager.h"
#include "serialCommunication.h"

//TODO:TODO: Put NRF24L01 related items in another file (and put the NRF24 setupRadio() in that file)

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


RF24 radio(48, 49); //CE, CSN pins
const byte address[6] = "00001";
int radio_data[6];

static char buffer[200];

Rc_data rc_data;

bool rcLinkStatus = RADIO_KO; //TODO: Maybe instead of "RADIO" use RemoteControlLinkKO, as we will also use the same variable for web control status?

const int buzzer_pin = 29;

void setupRadio() {
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
}

void setupBuzzer() {
  pinMode(buzzer_pin, OUTPUT);
}

void printRcValues() {

  //Print it every 1 second
  static unsigned long previousMillis = 0;

  if (millis() - previousMillis >= 1000) {
    previousMillis = millis();
  
    if (rcLinkStatus == RADIO_OK) {
      sprintf(buffer, "Lever 1: %d\t\tLever 2: %d\t\tJoystick1_x: %d\t\tJoystick1_y: %d\t\tJoystick2_x: %d\t\tJoystick2_y: %d\n", 
      rc_data.lever_1, rc_data.lever_2, rc_data.joystick1_x, rc_data.joystick1_y, rc_data.joystick2_x, rc_data.joystick2_y);

      usbSerialPrint(buffer);

    } else {
      usbSerialPrint("ERROR: NO CONNECTION WITH RC TRANSMITTER OR ABNORMAL VALUES RECEIVED\n");
    }

  }

}

//TODO: Remove. Temporal function until we change the tramsitter code
void correctRcValuesReceived() {

  if (rc_data.lever_1 == 1) {
    rc_data.lever_1 = 0;
  } else {
    rc_data.lever_1 = 1;
  }

  if (rc_data.lever_2 == 1) {
    rc_data.lever_2 = 0;
  } else {
    rc_data.lever_2 = 1;
  }


  if (rc_data.joystick1_x > 45 && rc_data.joystick1_x < 75) {
   rc_data.joystick1_x  = 0;
  } else {
    if (rc_data.joystick1_x <= 45) {
      rc_data.joystick1_x = map(rc_data.joystick1_x, 45, -255, 1, 255);
    } else {
      rc_data.joystick1_x = map(rc_data.joystick1_x, 75, 255, -1, -255);
    }
  }

   if (rc_data.joystick1_y > 35 && rc_data.joystick1_y < 65) {
   rc_data.joystick1_y  = 0;
  } else {
    if (rc_data.joystick1_y <= 35) {
      rc_data.joystick1_y = map(rc_data.joystick1_y, 35, -255, -1, -255);
    } else {
      rc_data.joystick1_y = map(rc_data.joystick1_y, 65, 255, 1, 255);
    }
  }

 if (rc_data.joystick2_x > 45 && rc_data.joystick2_x < 85) {
   rc_data.joystick2_x  = 0;
  } else {
    if (rc_data.joystick2_x <= 45) {
      rc_data.joystick2_x = map(rc_data.joystick2_x, 45, -255, 1, 255);
    } else {
      rc_data.joystick2_x = map(rc_data.joystick2_x, 85, 255, -1, -255);
    }
  }

  if (rc_data.joystick2_y > 45 && rc_data.joystick2_y < 75) {
   rc_data.joystick2_y  = 0;
  } else {
    if (rc_data.joystick2_y <= 45) {
      rc_data.joystick2_y = map(rc_data.joystick2_y, 45, -255, 1, 255);
    } else {
      rc_data.joystick2_y = map(rc_data.joystick2_y, 75, 255, -1, -255);
    }
  }
  
}

Rc_data readRcValues() {

  static long msTimeSinceLastPacket = 0;

  Rc_data rc_data_received; //This variable will contain the data received when we change the trasmitter's code so that it sends a struct.
  //TODO: The fields in this variable will be checked to see if any value is off. Only if no value is off we will return RADIO_OK and we will modify the rc_data variable with the new values

  //TODO: Change transmitter code to send the Radio_data struct directly and not an array
   if(radio.available()){

    msTimeSinceLastPacket = 0;

    radio.read(&radio_data, sizeof(radio_data));

    rc_data.lever_1 = radio_data[2];
    rc_data.lever_2 = radio_data[1];

    rc_data.joystick1_x = radio_data[4];
    rc_data.joystick1_y = radio_data[5];
    rc_data.joystick2_x = radio_data[0];
    rc_data.joystick2_y = radio_data[3];

    //TODO: This function will cease to be used when I modify the transmitter code so that it sends 0 if the joysticks are within its center range and so that it sends the correct ranges (ex.- 1 to 255 if right) and so that the lever send 1 if they are up
    correctRcValuesReceived();

    //TODO: Check that ALL values are in range and return RADIO_KO if any value is off. 

    rcLinkStatus = RADIO_OK;
  
  } else {

    static unsigned long previousMillis1 = 0;

    if (millis() - previousMillis1 >= 50) {
      previousMillis1 = millis();

      msTimeSinceLastPacket += 50;
    }

    //If we spend more than 200ms without receiving a packet, the rc connection will be considered as lost
    if (msTimeSinceLastPacket >= 200) {

      rcLinkStatus = RADIO_KO;
      
      static unsigned long previousMillis2 = 0;

      if (millis() - previousMillis2 >= 3000) {
        previousMillis2 = millis();
        tone(buzzer_pin, 200, 500);
      }

    }

  }

  return rc_data;
}

bool joystickIsRight(int joystickSelection) {

  if (joystickSelection == JOY_LEFT) {

    return rc_data.joystick1_x > 0;

  } else {

    return rc_data.joystick2_x > 0;

  }

}

bool joystickIsLeft(int joystickSelection) {

  if (joystickSelection == JOY_LEFT) {

    return rc_data.joystick1_x < 0;

  } else {

    return rc_data.joystick2_x < 0;
    
  }
  
}

bool joystickIsUp(int joystickSelection) {

  if (joystickSelection == JOY_LEFT) {

    return rc_data.joystick1_y > 0;

  } else {

    return rc_data.joystick2_y > 0;
    
  }

}

bool joystickIsDown(int joystickSelection) {

   if (joystickSelection == JOY_LEFT) {

    return rc_data.joystick1_y < 0;

  } else {

    return rc_data.joystick2_y < 0;
    
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