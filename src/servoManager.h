#ifndef SERVO_MANAGER_H
#define SERVO_MANAGER_H

#include "rcValuesManager.h"

void setupServos();

int getTurningRadius();

void setWheelServosStraight();

void setWheelServosAnglesConventionalControl(Rc_data rc_data);

void setWheelServosAnglesTo360();

void setWheelServosAnglesToConventional();

#endif