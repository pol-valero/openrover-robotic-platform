#include <Arduino.h>
#include <Servo.h>

#include "armServoManager.h"
#include "rcValuesManager.h"

//Arm servos, in order from rotating cylinder base to gripper
#define ARM_SERVO_1 1   //Rotating cylinder base servo
#define ARM_SERVO_2 2   //First vertical control servo
#define ARM_SERVO_3 3   //Second vertical control servo
#define ARM_SERVO_4 4   //Gripper vertical control servo
#define ARM_SERVO_5 5   //Gripper open/close servo

#define ARM_SERVO_1_FOLDED_ANGLE 30
#define ARM_SERVO_2_FOLDED_ANGLE 95
#define ARM_SERVO_3_FOLDED_ANGLE 170
#define ARM_SERVO_4_FOLDED_ANGLE 40
#define ARM_SERVO_5_FOLDED_ANGLE 45


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

int armServo1Angle = ARM_SERVO_1_FOLDED_ANGLE;
int armServo2Angle = ARM_SERVO_2_FOLDED_ANGLE;
int armServo3Angle = ARM_SERVO_3_FOLDED_ANGLE;
int armServo4Angle = ARM_SERVO_4_FOLDED_ANGLE;
int armServo5Angle = ARM_SERVO_5_FOLDED_ANGLE;

bool controlClawServosSelected = false;


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
    
    arm_servo1.write(ARM_SERVO_1_FOLDED_ANGLE);
    arm_servo2.write(ARM_SERVO_2_FOLDED_ANGLE);
    arm_servo3.write(ARM_SERVO_3_FOLDED_ANGLE);
    arm_servo4.write(ARM_SERVO_4_FOLDED_ANGLE);
    arm_servo5.write(ARM_SERVO_5_FOLDED_ANGLE);

}

bool armServoAngleInBounds(int servoId, int angle) {

    switch (servoId) {
        case ARM_SERVO_1:
            if (angle >= 30 && angle <= 160) {
                return true;
            } else {
                return false;
            }
            break;
        case ARM_SERVO_2:
            if (angle >= 50 && angle <= 140) {
                return true;
            } else {
                return false;
            }
            break;
        case ARM_SERVO_3:
            if (angle >= 0 && angle <= 180) {
                return true;
            } else {
                return false;
            }
            break;
        case ARM_SERVO_4:
            if (angle >= 0 && angle <= 180) {
                return true;
            } else {
                return false;
            }
            break;
        case ARM_SERVO_5:
            if (angle >= 0 && angle <= 70) {
                return true;
            } else {
                return false;
            }
            break;
        default:
            return false;
            break;
    }

}

void setRoboticArmS1toS3Angles() {

    if (joystickIsLeft(JOY_LEFT) && armServoAngleInBounds(ARM_SERVO_1, armServo1Angle + 1)) {
        armServo1Angle++;
    } 
    
    if (joystickIsRight(JOY_LEFT) && armServoAngleInBounds(ARM_SERVO_1, armServo1Angle - 1)) {
        armServo1Angle--;
    }

    if (joystickIsDown(JOY_RIGHT) && armServoAngleInBounds(ARM_SERVO_2, armServo2Angle + 1)) {
        armServo2Angle++;
    }

    if (joystickIsUp(JOY_RIGHT) && armServoAngleInBounds(ARM_SERVO_2, armServo2Angle - 1)) {
        armServo2Angle--;
    }

    if (joystickIsLeft(JOY_RIGHT) && armServoAngleInBounds(ARM_SERVO_3, armServo3Angle + 1)) {
        armServo3Angle++;
    }

    if (joystickIsRight(JOY_RIGHT) && armServoAngleInBounds(ARM_SERVO_3, armServo3Angle - 1)) {
        armServo3Angle--;
    }

    arm_servo1.write(armServo1Angle);
    arm_servo2.write(armServo2Angle);
    arm_servo3.write(armServo3Angle);

}

void setRoboticArmS4toS5Angles() {

    if (joystickIsUp(JOY_RIGHT) && armServoAngleInBounds(ARM_SERVO_4, armServo4Angle + 1)) {
        armServo4Angle++;
    }

    if (joystickIsDown(JOY_RIGHT) && armServoAngleInBounds(ARM_SERVO_4, armServo4Angle - 1)) {
        armServo4Angle--;
    }

    if (joystickIsRight(JOY_LEFT) && armServoAngleInBounds(ARM_SERVO_5, armServo5Angle + 1)) {
        armServo5Angle++;
    }

    if (joystickIsLeft(JOY_LEFT) && armServoAngleInBounds(ARM_SERVO_5, armServo5Angle - 1)) {
        armServo5Angle--;
    }

    arm_servo4.write(armServo4Angle);
    arm_servo5.write(armServo5Angle);

}

void setRoboticArmServosAngles() {
    
    static bool firstTimeRoboticArmControl = true;

    if (firstTimeRoboticArmControl == true) {
        //We initialize the servos and send them their initial positions (before, they were not holding any position)
        setupArmServos();
        setArmServosToFullyFolded();
        firstTimeRoboticArmControl = false;

        armServo4Angle = ARM_SERVO_4_FOLDED_ANGLE + 45; //From the fully folded position, we add 45 degrees so that the gripper is freed from the 3d printed rest support on the front wall of the rover
        arm_servo4.write(armServo4Angle); 
    }
    
    static unsigned long previousMillis = 0;

    //We increment/decrement the servos angle every 40ms
    if (millis() - previousMillis >= 40) {

        previousMillis = millis();
    
        if (controlClawServosSelected == false) {
            setRoboticArmS1toS3Angles();
        } else {
            setRoboticArmS4toS5Angles();
        }
    }

}

void setArmServoSelection(bool parameterControlClawServosSelected) {
    controlClawServosSelected = parameterControlClawServosSelected;
}
