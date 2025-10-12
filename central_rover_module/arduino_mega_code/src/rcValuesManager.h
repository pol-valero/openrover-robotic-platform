#ifndef RC_VALUES_MANAGER_H
#define RC_VALUES_MANAGER_H

#define RADIO_OK true
#define RADIO_KO false

#define JOY_LEFT 1
#define JOY_RIGHT 2

#include "sharedStructs.h"


void printRcValues();

void checkRcLinkStatus(Frame frame);

bool joystickIsRight(int joystickSelection);

bool joystickIsLeft(int joystickSelection);

bool joystickIsUp(int joystickSelection);

bool joystickIsDown(int joystickSelection);

bool joystickX_isCentered(int joystickSelection);

bool joystickY_isCentered(int joystickSelection);

bool joystickXY_isCentered(int joystickSelection);

bool getRcLinkStatus();

void setRcValues(RcValues newRcValues);

RcValues getRcValues();

#endif