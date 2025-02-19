#ifndef ARM_SERVO_MANAGER_H
#define ARM_SERVO_MANAGER_H

void setupArmServos();

void setArmServosToFullyFolded ();

void setRoboticArmServosAngles();

void setArmServoSelection(bool parameterControlClawServosSelected);

#endif