#include <Arduino.h>
#include <PalatisSoftPWM.h>

#include "motorManager.h"
#include "wheelServoManager.h"
#include "ackermannParameters.h"


#define FWD true
#define BCK false

#define MOTOR_1 1
#define MOTOR_2 2
#define MOTOR_3 3
#define MOTOR_4 4
#define MOTOR_5 5
#define MOTOR_6 6

#define MOT1_FWD 0  //TODO: Change names to MOT1_FWD_CH (channel)
#define MOT1_BCK 1
#define MOT2_FWD 2
#define MOT2_BCK 3
#define MOT3_FWD 4
#define MOT3_BCK 5

#define MOT4_FWD 6
#define MOT4_BCK 7
#define MOT5_FWD 8
#define MOT5_BCK 9
#define MOT6_FWD 10
#define MOT6_BCK 11


//The rover has two arms, each one with 3 motors and 2 servomotors for steering.

//Right arm motors, from front to back
const int mot1_fwd = 8; //Motor 1 forward rotation pin
const int mot1_bck = 7; //Motor 1 backward rotation pin

const int mot2_fwd = 10;
const int mot2_bck = 9; 

const int mot3_fwd = 12;
const int mot3_bck = 11; 

//Left arm motors, from front to back
const int mot4_fwd = A0; 
const int mot4_bck = A1; 

const int mot5_fwd = A2;
const int mot5_bck = A3; 

const int mot6_fwd = A4;
const int mot6_bck = A5; 

SOFTPWM_DEFINE_PIN8_CHANNEL(MOT1_FWD);
SOFTPWM_DEFINE_PIN7_CHANNEL(MOT1_BCK);
SOFTPWM_DEFINE_PIN10_CHANNEL(MOT2_FWD);
SOFTPWM_DEFINE_PIN9_CHANNEL(MOT2_BCK);
SOFTPWM_DEFINE_PIN12_CHANNEL(MOT3_FWD);
SOFTPWM_DEFINE_PIN11_CHANNEL(MOT3_BCK);

SOFTPWM_DEFINE_PINA0_CHANNEL(MOT4_FWD);
SOFTPWM_DEFINE_PINA1_CHANNEL(MOT4_BCK);
SOFTPWM_DEFINE_PINA2_CHANNEL(MOT5_FWD);
SOFTPWM_DEFINE_PINA3_CHANNEL(MOT5_BCK);
SOFTPWM_DEFINE_PINA4_CHANNEL(MOT6_FWD);
SOFTPWM_DEFINE_PINA5_CHANNEL(MOT6_BCK);

SOFTPWM_DEFINE_OBJECT(12); //Number of Software PWM Channels

int outer_wheels_speed;
int inner_front_back_wheels_speed;
int inner_middle_wheel_speed;

SpeedometerValues speedometerValues = {0, 0, 0};


void setupMotors() {
  PalatisSoftPWM.begin(15); ///15Hz PWM

  pinMode(mot1_fwd, OUTPUT);
  pinMode(mot1_bck, OUTPUT);

  pinMode(mot2_fwd, OUTPUT);
  pinMode(mot2_bck, OUTPUT);

  pinMode(mot3_fwd, OUTPUT);
  pinMode(mot3_bck, OUTPUT);

  pinMode(mot4_fwd, OUTPUT);
  pinMode(mot4_bck, OUTPUT);

  pinMode(mot5_fwd, OUTPUT);
  pinMode(mot5_bck, OUTPUT);

  pinMode(mot6_fwd, OUTPUT);
  pinMode(mot6_bck, OUTPUT);
}

void setMotorSpeed(int motor_num, int speed, bool direction) {

  int mot6_correction_factor = -4; //The mot6 is a bit faster than the rest at low speeds, we apply this correction factor to solve this problem

  if (motor_num == MOTOR_6) {
    speed = speed + mot6_correction_factor;
  }

  if (speed < 0) {
    speed = 0;
  }

  switch (motor_num) {

      case MOTOR_1:
        if (direction == FWD) {
           digitalWrite(mot1_bck, LOW);
           //analogWrite(mot1_fwd, speed);
           PalatisSoftPWM.set(MOT1_FWD, speed);
        } else {
          digitalWrite(mot1_fwd, LOW);
          //analogWrite(mot1_bck, speed);
          PalatisSoftPWM.set(MOT1_BCK, speed);
        }
        break;

      case MOTOR_2:
        if (direction == FWD) {
           digitalWrite(mot2_bck, LOW);
           //analogWrite(mot2_fwd, speed);
           PalatisSoftPWM.set(MOT2_FWD, speed);
        } else {
          digitalWrite(mot2_fwd, LOW);
          //analogWrite(mot2_bck, speed);
          PalatisSoftPWM.set(MOT2_BCK, speed);
        }
        break;

      case MOTOR_3:
        if (direction == FWD) {
           digitalWrite(mot3_bck, LOW);
           //analogWrite(mot3_fwd, speed);
           PalatisSoftPWM.set(MOT3_FWD, speed);
        } else {
          digitalWrite(mot3_fwd, LOW);
          //analogWrite(mot3_bck, speed);
          PalatisSoftPWM.set(MOT3_BCK, speed);
        }
        break;

      case MOTOR_4:
        if (direction == FWD) {
           digitalWrite(mot4_bck, LOW);
           //analogWrite(mot4_fwd, speed);
           PalatisSoftPWM.set(MOT4_FWD, speed);
        } else {
          digitalWrite(mot4_fwd, LOW);
          //analogWrite(mot4_bck, speed);
          PalatisSoftPWM.set(MOT4_BCK, speed);
        }
        break;

      case MOTOR_5:
        if (direction == FWD) {
           digitalWrite(mot5_bck, LOW);
           //analogWrite(mot5_fwd, speed);
           PalatisSoftPWM.set(MOT5_FWD, speed);
        } else {
          digitalWrite(mot5_fwd, LOW);
          //analogWrite(mot5_bck, speed);
          PalatisSoftPWM.set(MOT5_BCK, speed);
        }
        break;

      case MOTOR_6:
        if (direction == FWD) {
           digitalWrite(mot6_bck, LOW);
           //analogWrite(mot6_fwd, speed);
            PalatisSoftPWM.set(MOT6_FWD, speed);
        } else {
          digitalWrite(mot6_fwd, LOW);
          //analogWrite(mot6_bck, speed);
          PalatisSoftPWM.set(MOT6_BCK, speed);
        }
        break;

      default:
        break;

  }

}

void stopMotors() {

  setMotorSpeed(MOTOR_1, 0, FWD);
  setMotorSpeed(MOTOR_2, 0, FWD);
  setMotorSpeed(MOTOR_3, 0, FWD);
  setMotorSpeed(MOTOR_4, 0, FWD);
  setMotorSpeed(MOTOR_5, 0, FWD);
  setMotorSpeed(MOTOR_6, 0, FWD);

  setMotorSpeed(MOTOR_1, 0, BCK);
  setMotorSpeed(MOTOR_2, 0, BCK);
  setMotorSpeed(MOTOR_3, 0, BCK);
  setMotorSpeed(MOTOR_4, 0, BCK);
  setMotorSpeed(MOTOR_5, 0, BCK);
  setMotorSpeed(MOTOR_6, 0, BCK);

}

//speed (0...255) as argument
void calculateMotorsSpeed(int speed) {
  //Ackerman steering geometry calculations

  int correctionFactor1 = 0;  //Value that we will subtract from the inner wheels speed, so that tire slipping does not occur when having high speed and high angle
  //These correction factors are necessary because even tough the wheel speed equations are correct, the inner tires slip a bit (due to unknown physical factors) at high speed and high angle
  int correctionFactor2 = 0;

  int turning_radius;

  if (joystickX_isCentered(JOY_LEFT)) {

    outer_wheels_speed = inner_front_back_wheels_speed = inner_middle_wheel_speed = speed;
    
  } else {
    
    speed = map(speed, 0, 255, 0, 100); //we convert the speed to a 0-100 range to make the calculations

    if (speed >= 30) {
      correctionFactor1 = map(speed, 30, 100, 0, 40);
      correctionFactor2 = map(speed, 30, 100, 0, 30);
    }

    outer_wheels_speed = speed;

    //TODO: Test with this code and apply correction factor to inner speeds if necessary (ex.- doing inner_.. -= correctionFactor)

    turning_radius = getTurningRadius();

    inner_front_back_wheels_speed = (float) (speed * (sqrt(pow(d1_distance_front_middle_wheels, 2) + pow((turning_radius - d3_front_back_rover_width), 2)))) / (turning_radius + d4_center_rover_width);

    if (correctionFactor1 < inner_front_back_wheels_speed) { //We make sure that when we apply the correctionFactor it does not result in a negative speed, which could cause unexpected motor behaviour
      inner_front_back_wheels_speed -= correctionFactor1;
    }

    inner_middle_wheel_speed = speed * ((float) (turning_radius - d4_center_rover_width) / (turning_radius + d4_center_rover_width));

    if (correctionFactor2 < inner_middle_wheel_speed) { //We make sure that when we apply the correctionFactor it does not result in a negative speed, which could cause unexpected motor behaviour
      inner_middle_wheel_speed -= correctionFactor2;
    }

    //TODO: Adjust "toMin" if necessary so that when doing a close turn the inner middle wheel spins
    outer_wheels_speed = map(outer_wheels_speed, 0, 100, 0, 255);
    inner_front_back_wheels_speed = map(inner_front_back_wheels_speed, 0, 100, 0, 255);
    inner_middle_wheel_speed = map(inner_middle_wheel_speed, 0, 100, 0, 255);

  }

}

void updateSpeedometerMetrics(int speed) {

  //NOTE: This function is just an aproximation of speed and distance. For greater accuracy, a hall sensor would be used to count the RPMs of the wheel

  if (speed < 30) {
    speedometerValues.rpm = 0;
  } else {
    speedometerValues.rpm = map(speed, 30, 255, 10, 25);  //The motor, when loaded with the rover weight, can reach aproximately 25 rpm at full speed.
  }

  speedometerValues.metersPerHour = (speedometerValues.rpm * 0.4 * 60); //The wheel perimeter is 40cm (0.4m), so we multiply the rpm by 0.4 to get the meters per minute, then we multiply by 60 to get the meters per hour

  //Every second, we increase the distance
  static unsigned long previousMillis = 0;

  if (millis() - previousMillis >= 1000) {
    previousMillis = millis();

    static float distance = 0;
    distance += speedometerValues.rpm * 0.4 / 60;
    
    speedometerValues.distance = distance;   //We add the meters travelled in the last second (suposing the RPMs were constant)
  }

}

void setMotorSpeedsConventionalControl() {
  //Depending on the channels values (steering LEFT/RIGHT and throttle FWD/BCK) we will set each motor to its according 
  //(outer_wheels_speed, inner_front_back_wheels_speed, inner_middle_wheel_speed) and rotation direction using the "setMotorSpeed" function
  //(outer_wheels_speed, inner_front_back_wheels_speed, inner_middle_wheel_speed) will have the speed (0-100) calculated at "calculateMotorsSpeed"
  //We will map each of these speeds to a range of (0-255) and put them into the corresponding motorX_pwm_speed in this function. 

  if (getRcLinkStatus() == RADIO_KO) {
    stopMotors();
    return;
  }

  RcValues rcValues = getRcValues();
  int speedAndDirection = rcValues.y2;  //speedAndDirection (-255...0...255). Negative values are backwards speed.
  
  //NOTICE: If we want to change the control joystick (RIGHT/LEFT) or axis (X/Y) we will have to change related functions and arguments (ex.- joystickIsUp(JOY_LEFT))
  //both in this function and the "calculateMotorsSpeed" function

  updateSpeedometerMetrics(abs(speedAndDirection));

  if (joystickY_isCentered(JOY_RIGHT)) {

    stopMotors();

  } else {

      calculateMotorsSpeed(abs(speedAndDirection));

      if (joystickIsUp(JOY_RIGHT)) {

        if (joystickIsLeft(JOY_LEFT)) {

          setMotorSpeed(MOTOR_1, outer_wheels_speed, FWD);
          setMotorSpeed(MOTOR_2, outer_wheels_speed, FWD);
          setMotorSpeed(MOTOR_3, outer_wheels_speed, FWD);

          setMotorSpeed(MOTOR_4, inner_front_back_wheels_speed, FWD);
          setMotorSpeed(MOTOR_5, inner_middle_wheel_speed, FWD);
          setMotorSpeed(MOTOR_6, inner_front_back_wheels_speed, FWD);

        } else {

          setMotorSpeed(MOTOR_1, inner_front_back_wheels_speed, FWD);
          setMotorSpeed(MOTOR_2, inner_middle_wheel_speed, FWD);
          setMotorSpeed(MOTOR_3, inner_front_back_wheels_speed, FWD);

          setMotorSpeed(MOTOR_4, outer_wheels_speed, FWD);
          setMotorSpeed(MOTOR_5, outer_wheels_speed, FWD);
          setMotorSpeed(MOTOR_6, outer_wheels_speed, FWD);

        }

      } else {

        if (joystickIsLeft(JOY_LEFT)) {

          setMotorSpeed(MOTOR_1, outer_wheels_speed, BCK);
          setMotorSpeed(MOTOR_2, outer_wheels_speed, BCK);
          setMotorSpeed(MOTOR_3, outer_wheels_speed, BCK);

          setMotorSpeed(MOTOR_4, inner_front_back_wheels_speed, BCK);
          setMotorSpeed(MOTOR_5, inner_middle_wheel_speed, BCK);
          setMotorSpeed(MOTOR_6, inner_front_back_wheels_speed, BCK);

        } else {

          setMotorSpeed(MOTOR_1, inner_front_back_wheels_speed, BCK);
          setMotorSpeed(MOTOR_2, inner_middle_wheel_speed, BCK);
          setMotorSpeed(MOTOR_3, inner_front_back_wheels_speed, BCK);

          setMotorSpeed(MOTOR_4, outer_wheels_speed, BCK);
          setMotorSpeed(MOTOR_5, outer_wheels_speed, BCK);
          setMotorSpeed(MOTOR_6, outer_wheels_speed, BCK);

        }

      }

  }

}

void setMotorSpeeds360Control() {

  int speed; //From 0 to 255

  if (getRcLinkStatus() == RADIO_KO) {
    stopMotors();
    return;
  }

  RcValues rcValues = getRcValues();
  int speedAndDirection = rcValues.x2;  //speedAndDirection (-255...0...255). Negative values are counter-clockwise speed.

  //NOTICE: If we want to change the control joystick (RIGHT/LEFT) or axis (X/Y) we will have to change related functions and arguments (ex.- joystickIsUp(JOY_LEFT))

  updateSpeedometerMetrics(abs(speedAndDirection));

  if (joystickX_isCentered(JOY_RIGHT)) {

    stopMotors();

  } else {

    speed = abs(speedAndDirection);

    if (joystickIsLeft(JOY_RIGHT)) {

      setMotorSpeed(MOTOR_1, speed, FWD);
      setMotorSpeed(MOTOR_2, speed, FWD);
      setMotorSpeed(MOTOR_3, speed, FWD);
      setMotorSpeed(MOTOR_4, speed, BCK);
      setMotorSpeed(MOTOR_5, speed, BCK);
      setMotorSpeed(MOTOR_6, speed, BCK);

    } else {

      setMotorSpeed(MOTOR_1, speed, BCK);
      setMotorSpeed(MOTOR_2, speed, BCK);
      setMotorSpeed(MOTOR_3, speed, BCK);
      setMotorSpeed(MOTOR_4, speed, FWD);
      setMotorSpeed(MOTOR_5, speed, FWD);
      setMotorSpeed(MOTOR_6, speed, FWD);

    }

  }

}

SpeedometerValues getSpeedometerValues() {
  return speedometerValues;
}