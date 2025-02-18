#include <Arduino.h>
#include <Servo.h>

#include "armServoManager.h"

//Arm servos, in order from rotating cylinder base to gripper
#define ARM_SERVO_1 1   //Rotating cylinder base servo
#define ARM_SERVO_2 2   //First vertical control servo
#define ARM_SERVO_3 3   //Second vertical control servo
#define ARM_SERVO_4 4   //Gripper vertical control servo
#define ARM_SERVO_5 5   //Gripper open/close servo

const int arm_servo1_pin = 32;
const int arm_servo2_pin = 33;
const int arm_servo3_pin = 34;
const int arm_servo4_pin = 35;
const int arm_servo5_pin = 36;

//Arm servos, in order from rotating cylinder base to gripper
Servo arm_servo1;   //Rotating cylinder base servo
Servo arm_servo2;   //First vertical control servo
Servo arm_servo3;   //Second vertical control servo
Servo arm_servo4;   //Gripper vertical control servo
Servo arm_servo5;   //Gripper open/close servo


void setupArmServos() {

    arm_servo1.attach(arm_servo1_pin);
    arm_servo2.attach(arm_servo2_pin);
    arm_servo3.attach(arm_servo3_pin);
    arm_servo4.attach(arm_servo4_pin);
    arm_servo5.attach(arm_servo5_pin);

}

//These are the initial positions of the arm servos, 
//where the arm will be fully folded
void setArmServosToFullyFolded () {

    arm_servo5.write(45);
    arm_servo4.write(40);
    arm_servo3.write(180);
    arm_servo2.write(95);
    arm_servo1.write(30);

}