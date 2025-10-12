/*Mars rover replica - by Pol V*/

//IMPORTANT: We use software PWM which uses TIMER1, we cannot use analogWrite() on pins 13,12,11, as it uses TIMER1 too.
//We must use software PWM because there are not enough usable PWM ports in the arduino mega. 

#include <Arduino.h>

#include "wheelServoManager.h"
#include "motorManager.h"
#include "rcValuesManager.h"
#include "opModeManager.h"
#include "serialCommunication.h"
#include "batteryManager.h"
#include "radioCommunication.h"
#include "receivedFramesHandling.h"
#include "statusDataManager.h"
#include "armServoManager.h"
#include "sensorsManager.h"
#include "headMovementsManager.h"
#include "relayManager.h"
#include "buzzerManager.h"


void setup() {

  //DO NOT ALTER THE POSITION OF THESE 2 LINES//
  setupRelays();
  setServosRelayStatus(false);  //We immediately put the relay that powers the servos in an OPEN position to make sure that no power gets to the servos before the program initializes
  //////////////////////////////////////////////

  setRaspberryPiRelayStatus(false);  //We power off the Raspberry Pi

  setupWheelServos();
  
  setupMotors();

  setupSerial();

  setupRadio();

  setupBuzzer();

  setupBatteryMonitor();

  setupSensors();

  setupHeadServoStepper();

  //DO NOT ALTER THE POSITION OF THESE 2 LINES//
  setServosRelayStatus(true);  //We power up the servos and we send them their initial positions
  setWheelServosStraight();
  //////////////////////////////////////////////

}

void loop() {

  Frame receivedRadioFrame = radioReceiveFrame();
  Frame receivedSerialFrame = serialReceiveFrame();
  handleReceivedFrame(receivedRadioFrame);
  handleReceivedFrame(receivedSerialFrame);
  operationModeExecution();
  Frame statusDataFrame = getStatusDataFrame();
  radioSendFrame(statusDataFrame);
  serialSendFrame(statusDataFrame);

}

