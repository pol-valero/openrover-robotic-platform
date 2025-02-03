/*Mars rover replica - by Pol V*/

//IMPORTANT: We use software PWM which uses TIMER1, we cannot use analogWrite() on pins 13,12,11, as it uses TIMER1 too.
//We must use software PWM because there are not enough usable PWM ports in the arduino mega. 

#include <Arduino.h>

#include "servoManager.h"
#include "motorManager.h"
#include "rcValuesManager.h"
#include "opModeManager.h"










//TODO: Maybe we will need to distinguish between RC_OP_WHATEVER and WEB_OP_WHATEVER (if we want to do exactly the same operations via the web as in the remote control)
//TODO:IMPORTANT: Maybe it would be interesting to put all the WEB control operations and functions in another .c. So that we would have a main.c with just the void (setup, loop), a rcControl.c, a webControl.c (with its corresponding .h) and a variables.h (or globalVariables.h) with all the shared variables.
//(it would also be interesting to have other modules motors.c, wheelServos.c... with the functions that need to be shared ex.- setMotorSpeed(), calculateWheelServosAngle()...). Alternatively, we could just have a sharedFunctions.c to group them all. 
 


Rc_data rc_data2;



























char buffer2[200];

const int batt_input_pin = A15; 

const int servo_relay_pin = 28;


void setup() {

  //DO NOT ALTER THE POSITION OF THESE 2 LINES//
  pinMode(servo_relay_pin, OUTPUT);
  digitalWrite(servo_relay_pin, LOW); //We immediately put the relay that powers the servos in an OPEN position to make sure that no power gets to the servos before the program initializes
  //////////////////////////////////////////////

  setupServos();
  
  setupMotors();

  Serial.begin(9600); //TODO: setupSerial()

  setupRadio();


  

  pinMode(batt_input_pin, INPUT);



  //DO NOT ALTER THE POSITION OF THESE 2 LINES//
  digitalWrite(servo_relay_pin, HIGH);  //We power up the servos and we send them their initial positions
  setWheelServosStraight();
  //////////////////////////////////////////////

}



void calculateBatteryPercentage() {

  //TODO: Check timer before doing function so that the procedure is only done a couple of times per second and not continuously 

  int batt_divider_voltage_analog_value;  //Analog value read from the voltage divider output, which is 4V when the battery is fully charged
  float batt_divider_voltage; //Volts of the voltage divider output
  int batt_percentage; //Percentage of the battery, from 0% (3.6V per cell) to 100% (4.2V per cell)

  batt_divider_voltage_analog_value = analogRead(batt_input_pin);

  batt_divider_voltage = (5.00 / 1023) * batt_divider_voltage_analog_value;

  //Calculations: The max charge of the battery will be 4.2V per cell, 12.6V in total. 
  //The voltage divider output when fully charged is 4V, therefore if we apply multiply by a correction factor of 4.2 / 4 = 1.05 the resulting voltage will be roughly the same as each cell of the battery. 

  batt_percentage = map(batt_divider_voltage * 1.05 * 100, 4.2 * 100, 3.6 * 100, 100, 0); //We multiply by 100 because the map() function does not accept floats.

  Serial.print("Battery cell voltage: ");
  Serial.print(batt_divider_voltage * 1.05);

  sprintf(buffer2, "\tBattery percentage: %d", batt_percentage);
  Serial.println(buffer2);

}







































void operationModeExecution() {

  switch (getOpMode()) {

    case OP_CONVENTIONAL_DRIVING:
      setMotorSpeedsConventionalControl(rc_data2);
      setWheelServosAnglesConventionalControl(rc_data2);
      break;
    case OP_360_DEGREE_TURN_CONTROL:
      setMotorSpeeds360Control(rc_data2);
      setWheelServosAnglesTo360();
      break;
    case OP_ROBOTIC_ARM_CONTROL:
      //roboticArmControl();
      break;
    case OP_HEAD_CONTROL:
      //headControl();
      break;
    case OP_WEB_CONTROL:
      //webOperationModeExecution(); //This function will be in the webControl.c
      break;
    case SUB_OP_360_DEGREE_TO_CONVENTIONAL:
      setWheelServosAnglesToConventional();
      break;

    default:
      break;

  }

}


void loop() {

  rc_data2 = readRcValues();
  printRcValues();
  chooseOperationMode(rc_data2);
  operationModeExecution();

}

