/*Mars rover replica - by Pol V*/

//IMPORTANT: We use software PWM which uses TIMER1, we cannot use analogWrite() on pins 13,12,11, as it uses TIMER1 too.
//We must use software PWM because there are not enough usable PWM ports in the arduino mega. 

#include <Arduino.h>

#include "servoManager.h"
#include "motorManager.h"
#include "rcValuesManager.h"
#include "opModeManager.h"
#include "serialCommunication.h"
#include "batteryManager.h"
#include "radioCommunication.h"
#include "receivedFramesHandling.h"


const int servo_relay_pin = 28;


void setup() {

  //DO NOT ALTER THE POSITION OF THESE 2 LINES//
  pinMode(servo_relay_pin, OUTPUT);
  digitalWrite(servo_relay_pin, LOW); //We immediately put the relay that powers the servos in an OPEN position to make sure that no power gets to the servos before the program initializes
  //////////////////////////////////////////////

  setupServos();
  
  setupMotors();

  setupSerial();

  setupRadio();

  setupBuzzer();

  setupBatteryMonitor();

  //DO NOT ALTER THE POSITION OF THESE 2 LINES//
  digitalWrite(servo_relay_pin, HIGH);  //We power up the servos and we send them their initial positions
  setWheelServosStraight();
  //////////////////////////////////////////////

}



void operationModeExecution() {

  switch (getOpMode()) {

    case OP_CONVENTIONAL_DRIVING:
      setMotorSpeedsConventionalControl();
      setWheelServosAnglesConventionalControl();
      break;
    case OP_360_DEGREE_TURN_CONTROL:
      setMotorSpeeds360Control();
      setWheelServosAnglesTo360();
      break;
    case OP_ROBOTIC_ARM_CONTROL:
      //roboticArmControl();
      break;
    case OP_HEAD_CONTROL:
      //headControl();
      break;
    case SUB_OP_360_DEGREE_TO_CONVENTIONAL:
      setWheelServosAnglesToConventional();
      break;

    default:
      break;

  }

}

void loop() {

  //TODO: Only call operationModeExecution inside functions every 50ms or so? (instead of calling them every loop iteration)

  //serialReceiveFrame
  //handleReceivedFrame
  //radioReceiveFrame
  //handleReceivedFrame
  //operationModeExecution
  //getStatusDataFrame  -> ex.- temp, hum, battery, RPI status...
  //radioSendFrame  //inside this function, use bool to determine wether to send the frame or not (depending if the RC is powered on or not)
  //serialSendFrame //same, but depending on the RPI powered on or not


  Frame frame = radioReceiveFrame();
  handleReceivedFrame(frame);
  operationModeExecution(); //TODO: Put this function inside "opModeManager.cpp"
  //printRcValues();

  //rc_data2 = readRcValues();
  //printRcValues();
  //chooseOperationMode(rc_data2);
  //operationModeExecution();
  //printBatteryValues();

}

