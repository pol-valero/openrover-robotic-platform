#include <Arduino.h>

#include "frameTypesDefinition.h"
#include "receivedFramesHandling.h"
#include "rcValuesManager.h"
#include "buzzerManager.h"
#include "serialCommunication.h"
#include "valuesFromFrameConversion.h"


void handleReceivedFrame(Frame frame) {

  //Handles specific frames that must be handled by arduino nano, such as command frames 
  //to make buzzer sounds

  switch (frame.type) {

    case CMD_F_RC_BUZZER:
      shortTestBuzz();
      break;

    case CMD_F_RC_RADIO_ENABLING:
      bool radioControlEnabled = rcRadioEnableStatusFromFrame(frame);
      setRadioControlEnabled(radioControlEnabled);
      break;

    case CMD_F_TEST:
      //TODO: Delete. Just for testing
      shortTestBuzz();

      Frame responseFrame;
      responseFrame.type = CMD_F_TEST;
      responseFrame.data2B[1] = frame.data2B[1];
      responseFrame.data1B[3] = frame.data1B[3];

      serialSendFrame(responseFrame);
      //
      break;

    default:
      break;

  }

}