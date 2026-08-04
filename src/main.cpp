#include <Arduino.h>

#include "radioCommunication.h"
#include "serialCommunication.h"
#include "batteryManager.h"
#include "buzzerManager.h"
#include "receivedFramesHandling.h"
#include "rcValuesManager.h"


void setup() {

  setupSerial();
  setupRadio();
  setupBatteryMonitor();
  setupRcInputs();
  setupBuzzer();

}

void loop() {

    serialSendRcValuesFrame();  //TODO: Make these functions return a Frame and then sending it to radio or serial? This change is optional, just to improve modularity
    radioSendRcValuesFrame();
    serialSendBattValuesFrame();

    Frame serialFrame = serialReceiveFrame();
    radioSendFrame(serialFrame);
    Frame radioFrame = radioReceiveFrame();
    serialSendFrame(radioFrame);

    handleReceivedFrame(serialFrame);
    //handleReceivedFrame(radioFrame);  //Right now not necessary, but may be if we expand functionalities in the future
  
}
