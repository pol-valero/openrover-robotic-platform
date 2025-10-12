#include <Arduino.h>

#include "screenConfig.h"
#include "serialCommunication.h"
#include "screenController.h"
#include "receivedFramesHandling.h"


void setup() {
  
  initSerial();
  initScreen();

}


void loop() {

  runScreen();

  Frame frame = serialReceiveFrame();
  handleReceivedFrame(frame);

}
