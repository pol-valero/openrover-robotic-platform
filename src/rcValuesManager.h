#ifndef RC_VALUES_MANAGER_H
#define RC_VALUES_MANAGER_H

#define RADIO_OK true
#define RADIO_KO false

#define JOY_LEFT 1
#define JOY_RIGHT 2

typedef struct {


  int lever_1;  //TODO: When updating the transmitter code to send structs, we will change these ints to bool
  int lever_2;

  //TODO: IMPORTANT: Maybe it is a good idea that the arduino in the RC is in charge of sending this values set to 0 if they are within a certain center range and send (1 to 255, 1 to -255) otherwise. 

  //(Positive axis: 1 to 255)
  //(Negative axis: -1 to -255)
  int joystick1_x; 
  int joystick1_y;
  int joystick2_x;
  int joystick2_y;

} Rc_data;

void setupRadio(); //Put in radio file

void printRcValues();

Rc_data readRcValues();

bool joystickIsRight(int joystickSelection);

bool joystickIsLeft(int joystickSelection);

bool joystickIsUp(int joystickSelection);

bool joystickIsDown(int joystickSelection);

bool joystickX_isCentered(int joystickSelection);

bool joystickY_isCentered(int joystickSelection);

bool joystickXY_isCentered(int joystickSelection);

bool getRcLinkStatus();

#endif