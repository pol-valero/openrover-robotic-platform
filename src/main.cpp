#include <Arduino.h>

#include <Wire.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include "frameTypesDefinition.h"

#define OP_CONVENTIONAL_DRIVING 1
#define OP_360_DEGREE_TURN_CONTROL 2
#define OP_ROBOTIC_ARM_CONTROL 3
#define OP_HEAD_CONTROL 4

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

typedef struct __attribute__((packed)) {
  uint8_t type; //Integer value identifying the type of frame
  int16_t data2B[4]; //Large data to be sent (2 bytes each) (ex.- joystick values -255...255)
  uint8_t data1B[5]; //Small data to be sent (1 byte each)
} Frame; //The same struct is used for radio and serial

RF24 radio(8, 7); // CE, CSN

int joyX1pin = A1;
int joyY1pin = A0;
int joyY2pin = A2;
int joyX2pin = A3;

int switch1Pin = 2;
int switch2Pin = 3;

int currentOpMode = OP_CONVENTIONAL_DRIVING;

void setup() {

    Serial.begin(38400);  

    radio.begin();
    radio.openReadingPipe(0, readAddress);
    radio.openWritingPipe(writeAddress);
    radio.setPALevel(RF24_PA_MAX);

    pinMode(joyX1pin, INPUT);
    pinMode(joyY1pin, INPUT);
    pinMode(joyX2pin, INPUT);
    pinMode(joyY2pin, INPUT);

    pinMode(switch1Pin, INPUT_PULLUP);
    pinMode(switch2Pin, INPUT_PULLUP);

}


Frame rcValuesToFrame(RcValues rcValues) {

  Frame frame;

  frame.type = INF_F_RC_VALUES;
  frame.data2B[0] = rcValues.y1;
  frame.data2B[1] = rcValues.x1;
  frame.data2B[2] = rcValues.y2;
  frame.data2B[3] = rcValues.x2;
  frame.data1B[0] = 0;  //with two physical switches, these AUXs are not used since switches are used exclusively for op mode selection
  frame.data1B[1] = 0;
  frame.data1B[2] = 0;
  frame.data1B[3] = 0;

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

  rcValues.y1 = analogRead(joyY1pin);
  rcValues.x1 = analogRead(joyX1pin);
  rcValues.y2 = analogRead(joyY2pin);
  rcValues.x2 = analogRead(joyX2pin);
  rcValues.aux1 = !digitalRead(switch1Pin);
  rcValues.aux2 = !digitalRead(switch2Pin);
  rcValues.aux3 = 0;
  rcValues.aux4 = 0;

  rcValues.dataValid = true;

  correctRcValues(rcValues);

  return rcValues;

}

RcValues getRcValuesForRadio () {
  
  RcValues rcValues;
  rcValues.dataValid = false;

  static unsigned long previousMillis = 0;

  //Gets the RC values every 50ms
  if (millis() - previousMillis >= 50) {
    previousMillis = millis();

    rcValues = getRcValues();
    rcValues.dataValid = true;

  }

  return rcValues;

}

void printRcValues(RcValues rcValues) {

  Serial.print("Y1: ");
  Serial.print(rcValues.y1);
  Serial.print(" | X1: ");
  Serial.print(rcValues.x1);
  Serial.print(" | Y2: ");
  Serial.print(rcValues.y2);
  Serial.print(" | X2: ");
  Serial.print(rcValues.x2);
  Serial.print(" | AUX1: ");
  Serial.print(rcValues.aux1);
  Serial.print(" | AUX2: ");
  Serial.println(rcValues.aux2);
}

Frame roverOpModeSelectionToFrame(int opMode) {
    
    Frame frame;
    frame.type = CMD_F_ROVER_OP_MODE_SELECTION;
    frame.data1B[0] = opMode;

    return frame;

}

Frame armServoSelectionToFrame(bool controlClawServosSelected) {
    
    Frame frame;
    frame.type = CMD_F_ARM_SERVO_SELECTION;
    if (controlClawServosSelected) {
        frame.data1B[0] = 1;
    } else {
        frame.data1B[0] = 0;
    }

    return frame;

}

void radioSendOpModeSelectionFrame(RcValues rcValues) {

    static int previousOpMode = OP_CONVENTIONAL_DRIVING;

    //We do not determine the operation mode if one second since startup has not passed (to ensure stable readings of switches)
    if (millis() < 1000) {
        return;
    }

    //Determine the operation mode based on the aux switches
    if (rcValues.aux1 == 0 && rcValues.aux2 == 1) {
        currentOpMode = OP_HEAD_CONTROL; 
    } else if (rcValues.aux1 == 1 && rcValues.aux2 == 0) {
        currentOpMode = OP_ROBOTIC_ARM_CONTROL; 
    } else if (rcValues.aux1 == 1 && rcValues.aux2 == 1) {
        currentOpMode = OP_360_DEGREE_TURN_CONTROL; 
    } else {
        currentOpMode = OP_CONVENTIONAL_DRIVING;
    }

    //Send the op mode selection frame only if there is a change in the op mode
    if (currentOpMode != previousOpMode) {

        if (previousOpMode == OP_360_DEGREE_TURN_CONTROL) {
          //When leaving the 360 degree turn control mode, we need to send again the frame to go back to conventional driving (in order to center the wheels properly and gracefully)
          Frame frame2 = roverOpModeSelectionToFrame(OP_CONVENTIONAL_DRIVING); 
          radioSendFrame(frame2);
          
          previousOpMode = currentOpMode;

          return; //We will not send the new operation mode. We will have to manually put the switches on conventional mode and then change to the desired mode again.
        }
        
        previousOpMode = currentOpMode;

        Frame frame = roverOpModeSelectionToFrame(currentOpMode);
        radioSendFrame(frame);

        //Serial.print("OpMode change: ");
        //Serial.println(currentOpMode);
    }
}

void radioSendArmServoSelectionFrame(RcValues rcValues) {

    static bool previousControlClawServosSelected = false;
    static bool currentControlClawServosSelected = false;

    if (currentOpMode != OP_ROBOTIC_ARM_CONTROL) {
        currentControlClawServosSelected = false;
        return; //We only send this frame when in robotic arm control mode
    }

    //We determine if we want to control the claw servos based on aux1 switch
    if (rcValues.y1 > 240) {
        currentControlClawServosSelected = true; 
    } else if (rcValues.y1 < -240) {
        currentControlClawServosSelected = false;
    }

    //Send the arm servo selection frame only if there is a change
    if (currentControlClawServosSelected != previousControlClawServosSelected) {

        previousControlClawServosSelected = currentControlClawServosSelected;

        Frame frame = armServoSelectionToFrame(currentControlClawServosSelected);
        radioSendFrame(frame);

        //Serial.print("Arm Servo Control change: ");
        //Serial.println(currentControlClawServosSelected);
    }
}


void loop() {

    RcValues rcValues; 
    rcValues = getRcValuesForRadio();
    radioSendRcValuesFrame(rcValues);
    radioSendOpModeSelectionFrame(rcValues);
    radioSendArmServoSelectionFrame(rcValues);
    //printRcValues(rcValues);
  
}
