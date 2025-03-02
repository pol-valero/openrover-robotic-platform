#include <Arduino.h>

#include "opModeManager.h"
#include "wheelServoManager.h"
#include "motorManager.h"
#include "armServoManager.h"
#include "headMovementsManager.h"

//TODO: Maybe we will need to distinguish between RC_OP_WHATEVER and WEB_OP_WHATEVER (if we want to do exactly the same operations via the web as in the remote control)
//TODO: Maybe we won't need to distinguish between RC_OP_WHATEVER and WEB_OP_WHATEVER, as exactly the same operations will be done via the web as in the remote control (the only difference will be if the frames are 
//received via serial (RPI - Web) or via radio (NANO - ESP32 - RC)

int op_mode = OP_CONVENTIONAL_DRIVING;
bool entered_new_op_mode;


//Sets "entered_new_op_mode" to true if we have just changed modes and are executing the first iteration of a operation mode
void updateNewOperationIndicator() {

  static int last_op_mode = OP_CONVENTIONAL_DRIVING;

  if (last_op_mode != op_mode) {
    entered_new_op_mode = true;
  } else {
    entered_new_op_mode = false;
  }

  last_op_mode = op_mode;

}

void opConventionalDrivingSelected() {

  //If we come from the 360 degree control operation, we will go to the sub-operation to turn the wheels to the conventional position
  //The same applies if we come from the sub-operation to turn the wheels to the conventional position
  if (op_mode == OP_360_DEGREE_TURN_CONTROL || op_mode == SUB_OP_360_DEGREE_TO_CONVENTIONAL) {
      op_mode = SUB_OP_360_DEGREE_TO_CONVENTIONAL;
    } else {
      op_mode = OP_CONVENTIONAL_DRIVING;
    }

    updateNewOperationIndicator();
}

void setOpMode(int new_op_mode) {
  op_mode = new_op_mode;
  updateNewOperationIndicator();
}

bool hasEnteredNewOpMode() {
  
  bool enteredNewOpModeAux = entered_new_op_mode;

  //We want the reading to be "destructive", so that "entered_new_op_mode = true" is only returned when we have changed opmodes
  //and we are executing the first iteration of the new opmode (where this function will be called)
  if (entered_new_op_mode == true) {
    entered_new_op_mode = false;
  }

  return enteredNewOpModeAux;
}

void operationModeExecution() {

  switch (op_mode) {

    case OP_CONVENTIONAL_DRIVING:
    {
      static unsigned long previousMillis = 0;
      if (millis() - previousMillis >= 50) { //We only calculate and set the speeds and angles every 50ms, to increase performance of other concurrent parts of the code
        previousMillis = millis();
        setMotorSpeedsConventionalControl();
        setWheelServosAnglesConventionalControl();
      }
      break;
    }
    case OP_360_DEGREE_TURN_CONTROL:
    {
      static unsigned long previousMillis = 0;
      if (millis() - previousMillis >= 50) { //We only calculate and set the speeds every 50ms, to increase performance of other concurrent parts of the code
        previousMillis = millis();
        setMotorSpeeds360Control();
      }
      setWheelServosAnglesTo360();
      break;
    }
    case OP_ROBOTIC_ARM_CONTROL:
      setRoboticArmServosAngles();
      break;
    case OP_HEAD_CONTROL:
      setStepperHeadPanAngle();
      setServoHeadTiltAngle();
      break;
    case SUB_OP_360_DEGREE_TO_CONVENTIONAL:
      setWheelServosAnglesToConventional();
      break;

    default:
      break;

  }

}