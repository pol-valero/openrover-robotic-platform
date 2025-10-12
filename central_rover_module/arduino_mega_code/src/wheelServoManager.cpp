#include <Arduino.h>
#include <Servo.h>  

#include "wheelServoManager.h"
#include "ackermannParameters.h"
#include "opModeManager.h"
#include "rcValuesManager.h"


#define MAX_INTEGER 32766

//Right arm servos, from front to back
#define W_SERVO_1 1
#define W_SERVO_2 2
//Left arm servos, from front to back
#define W_SERVO_3 3
#define W_SERVO_4 4


//Right arm servos, from front to back
const int w_servo1_pin = 22; //Wheel servo1 pin number
const int w_servo2_pin = 23;

Servo w_servo1;
Servo w_servo2;

//Left arm servos, from front to back
const int w_servo3_pin = 24;
const int w_servo4_pin = 25;

Servo w_servo3;
Servo w_servo4;

//Wheel servos center position, in degrees. 
// {BLANK SPACE, servo1_angle, servo2_angle, servo3_angle, servo4_angle}
const int w_servo_center[] = {0,98,95,90,103};

//The turning radius of the whole rover, measured from the rover's center in mm
int turning_radius;

//To have a certain turning radius, this are the angles that we will add or subtract to each servo center angle 
int outer_servo_front_angle_beta; 
int outer_servo_back_angle_beta;
int inner_servo_front_angle_beta;
int inner_servo_back_angle_beta;

void setupWheelServos() {
    w_servo1.attach(w_servo1_pin);
    w_servo2.attach(w_servo2_pin);
    w_servo3.attach(w_servo3_pin);
    w_servo4.attach(w_servo4_pin);
}

int getTurningRadius() {
  return turning_radius;
}

//Value of the joystick used to turn, "turnJoystickValue" (-255...0...255) as argument. Negative values when joystick is left, positive values when joystick is right.
void calculateWheelServosAngle(int turnJoystickValue) {

  //Ackerman steering geometry calculations

  if (joystickX_isCentered(JOY_LEFT)) {
    turning_radius = MAX_INTEGER; //We put the turning radius to the maximum integer to indicate that we are not turning
  } else if (joystickIsLeft(JOY_LEFT)) {
      turning_radius = map(turnJoystickValue, 0, -255, max_turning_radius, min_turning_radius); //4000mm to 700mm turning radius
  } else {
      turning_radius = map(turnJoystickValue, 0, 255, max_turning_radius, min_turning_radius); 
  }

  outer_servo_front_angle_beta = round((atan(((float)d1_distance_front_middle_wheels / (turning_radius + d3_front_back_rover_width))) ) * (180 / PI)); //We multiply by 180/PI to convert radians to degrees
  outer_servo_back_angle_beta = round((atan(((float)d2_distance_back_middle_wheels / (turning_radius + d3_front_back_rover_width))) ) * (180 / PI));

  inner_servo_front_angle_beta = round((atan(((float)d1_distance_front_middle_wheels / (turning_radius - d3_front_back_rover_width))) ) * (180 / PI)); 
  inner_servo_back_angle_beta = round((atan(((float)d2_distance_back_middle_wheels / (turning_radius - d3_front_back_rover_width))) ) * (180 / PI)); 

}

void servoWrite(int servo_identifier,  int degrees) {

  const int w_servo_max_angle = 150;
  const int w_servo_min_angle = 45;

  
  switch (servo_identifier) {
    case W_SERVO_1:
      if (degrees > w_servo_min_angle && degrees < w_servo_max_angle) {
        w_servo1.write(degrees);
      } else {
        //Servo1 angle out of bounds
      }
      break;

      case W_SERVO_2:
      if (degrees > w_servo_min_angle && degrees < w_servo_max_angle) {
        w_servo2.write(degrees);
      } else {
        //Servo2 angle out of bounds
      }
      break;

      case W_SERVO_3:
      if (degrees > w_servo_min_angle && degrees < w_servo_max_angle) {
        w_servo3.write(degrees);
      } else {
        //Servo3 angle out of bounds
      }
      break;

      case W_SERVO_4:
      if (degrees > w_servo_min_angle && degrees < w_servo_max_angle) {
        w_servo4.write(degrees);
      } else {
        //Servo4 angle out of bounds
      }
      break;
  }

}

void setWheelServosStraight() {

  servoWrite(W_SERVO_1, w_servo_center[1]);
  servoWrite(W_SERVO_2, w_servo_center[2]);
  servoWrite(W_SERVO_3, w_servo_center[3]);
  servoWrite(W_SERVO_4, w_servo_center[4]);

}

void setWheelServosAnglesConventionalControl() {
  //Depending on the channels values (steering LEFT/RIGHT) we will add or subtract each servos'
  //(outer_servos_angle_beta, inner_servos_angle_beta) to its w_servo_center constant, so that it goes to the correct position
  //(outer_servos_angle_beta, inner_servos_angle_beta) will have the angle calculated at "calculateMotorsSpeed"
  //We will add or subtract each "angle beta" to the w_servo_center constant and put the result into the corresponding 
  //servoX_angle. Then we will write each servoX_angle to the corresponding servo using the Servo.write function

  RcValues rcValues = getRcValues();
  int turnJoystickValue = rcValues.x1;  //Value of the joystick used to turn, "turnJoystickValue" (-255...0...255). Negative values when joystick is left, positive values when joystick is right.

  //NOTICE: If we want to change the control joystick (RIGHT/LEFT) or axis (X/Y) we will have to change related functions and arguments (ex.- joystickIsUp(JOY_LEFT))
  //both in this function and the "calculateWheelServosAngle" function

  if (joystickX_isCentered(JOY_LEFT)) {

    setWheelServosStraight();

  } else {
    
    calculateWheelServosAngle(turnJoystickValue);

    if (joystickIsLeft(JOY_LEFT)) {
      servoWrite(W_SERVO_1, w_servo_center[1] - outer_servo_front_angle_beta);
      servoWrite(W_SERVO_2, w_servo_center[2] + outer_servo_back_angle_beta);
      servoWrite(W_SERVO_3, w_servo_center[3] - inner_servo_front_angle_beta);
      servoWrite(W_SERVO_4, w_servo_center[4] + inner_servo_back_angle_beta);

    } else {
      servoWrite(W_SERVO_1, w_servo_center[1] + inner_servo_front_angle_beta);
      servoWrite(W_SERVO_2, w_servo_center[2] - inner_servo_back_angle_beta);
      servoWrite(W_SERVO_3, w_servo_center[3] + outer_servo_front_angle_beta);
      servoWrite(W_SERVO_4, w_servo_center[4] - outer_servo_back_angle_beta);
    }

  }

}

void setWheelServosAnglesTo360() {
  
  static int currentAngle = 0;
  const int finalAngle = 45;

  if (hasEnteredNewOpMode() == true) { 
    currentAngle = 0;
  }

  static unsigned long previousMillis = 0;

  //We increment/decrement the servos angle every 10ms
  if (millis() - previousMillis >= 10) {
    previousMillis = millis();

    if (currentAngle != finalAngle) {

        currentAngle++;

        servoWrite(W_SERVO_1, w_servo_center[1] - currentAngle);
        servoWrite(W_SERVO_2, w_servo_center[2] + currentAngle);
        servoWrite(W_SERVO_3, w_servo_center[3] + currentAngle);
        servoWrite(W_SERVO_4, w_servo_center[4] - currentAngle);

    }

  }

}

void setWheelServosAnglesToConventional() {
  
  static int currentAngle = 0;
  const int finalAngle = 45;

  if (hasEnteredNewOpMode() == true) { 
    currentAngle = 0;
  }

  static unsigned long previousMillis = 0;

  //We increment/decrement the servos angle every 10ms
  if (millis() - previousMillis >= 10) {
    previousMillis = millis();
  
    if (currentAngle != finalAngle) {
      currentAngle++;
      servoWrite(W_SERVO_1, (w_servo_center[1] - finalAngle) + currentAngle);
      servoWrite(W_SERVO_2, (w_servo_center[2] + finalAngle) - currentAngle);
      servoWrite(W_SERVO_3, (w_servo_center[3] + finalAngle) - currentAngle);
      servoWrite(W_SERVO_4, (w_servo_center[4] - finalAngle) + currentAngle);
    } else {
      //If we have reached the final angle, we can switch to the conventional driving mode
      setOpMode(OP_CONVENTIONAL_DRIVING);
    }

  }

}