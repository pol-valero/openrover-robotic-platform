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

  Frame rcValuesFrameForSerial = getRcValuesFrameForSerial();
  serialSendFrame(rcValuesFrameForSerial);
  Frame rcValuesFrameForRadio = getRcValuesFrameForRadio();
  radioSendFrame(rcValuesFrameForRadio);
  
  Frame rcBatteryValuesFrame = getRcBatteryValuesFrame();
  serialSendFrame(rcBatteryValuesFrame);

  //The Arduino Nano acts as a bridge between the serial communications coming from the ESP32S3 touscreen module and the radio communications coming from the NRF24L01 in the rover. 
  //It receives frames from the ESP32S3 via serial and sends them to the rover via radio, and vice versa.
  Frame receivedSerialFrame = serialReceiveFrame();
  radioSendFrame(receivedSerialFrame);
  Frame receivedRadioFrame = radioReceiveFrame();
  serialSendFrame(receivedRadioFrame);
  //

  handleReceivedFrame(receivedSerialFrame);
  //handleReceivedFrame(receivedRadioFrame);  //Right now not necessary, but may be if we expand functionalities in the future
  
}
