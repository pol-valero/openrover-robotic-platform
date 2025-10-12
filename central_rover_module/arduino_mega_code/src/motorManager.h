#ifndef MOTOR_MANAGER_H
#define MOTOR_MANAGER_H

#include "rcValuesManager.h"

void setupMotors();

void setMotorSpeedsConventionalControl();

void setMotorSpeeds360Control();

SpeedometerValues getSpeedometerValues();

#endif