#ifndef MOTOR_MANAGER_H
#define MOTOR_MANAGER_H

#include "rcValuesManager.h"

void setupMotors();

void setMotorSpeedsConventionalControl(Rc_data rc_data);

void setMotorSpeeds360Control(Rc_data rc_data);

#endif