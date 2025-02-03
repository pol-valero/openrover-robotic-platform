#include <Arduino.h>

#include "opModeManager.h"

//TODO: Maybe we will need to distinguish between RC_OP_WHATEVER and WEB_OP_WHATEVER (if we want to do exactly the same operations via the web as in the remote control)

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

//TODO: We will delete this function, as we will change opMode depending on the type of frame received (handled in receivedFramesHandling.cpp)
void chooseOperationMode(Rc_data rc_data) {

  if (rc_data.lever_1 == 0 && rc_data.lever_2 == 0) {

    if (op_mode == OP_360_DEGREE_TURN_CONTROL || op_mode == SUB_OP_360_DEGREE_TO_CONVENTIONAL) {
      op_mode = SUB_OP_360_DEGREE_TO_CONVENTIONAL;
    } else {
      op_mode = OP_CONVENTIONAL_DRIVING;
    }

    updateNewOperationIndicator();

  }  else if (rc_data.lever_1 == 0 && rc_data.lever_2 == 1) {

    op_mode = OP_360_DEGREE_TURN_CONTROL;
    updateNewOperationIndicator();

  } else {
      op_mode = -1;
  }
}

void setOpMode(int new_op_mode) {
  op_mode = new_op_mode;
  //TODO: Call updateNewOperationIndicator() here?
}

int getOpMode() {
  return op_mode;
}

bool hasEnteredNewOpMode() {
  return entered_new_op_mode;
}