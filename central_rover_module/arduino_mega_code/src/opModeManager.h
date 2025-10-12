#ifndef OP_MODE_MANAGER_H
#define OP_MODE_MANAGER_H

#include "rcValuesManager.h"

#define OP_CONVENTIONAL_DRIVING 1
#define OP_360_DEGREE_TURN_CONTROL 2
#define OP_ROBOTIC_ARM_CONTROL 3
#define OP_HEAD_CONTROL 4
//We will use the RC remote to enable this operations and then the RC can be turned off. When turned on again, we will be able to change the operation to another one. 
#define SUB_OP_360_DEGREE_TO_CONVENTIONAL 5

void opConventionalDrivingSelected();

void setOpMode(int new_op_mode);

bool hasEnteredNewOpMode();

void operationModeExecution();

#endif