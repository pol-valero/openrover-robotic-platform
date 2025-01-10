#include <Arduino.h>

#include "screenConfig.h"
#include "serialCommunication.h"
#include "screenController.h"


void setup() {
  
  initSerial();
  initScreen();

}


void loop() {

  runScreen();
  screenControllerExecution();

}
