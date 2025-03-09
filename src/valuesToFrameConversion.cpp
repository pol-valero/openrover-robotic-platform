#include <Arduino.h>

#include "valuesToFrameConversion.h"
#include "frameTypesDefinition.h"

Frame rcRadioEnableStatusToFrame(bool enabled) {
    
    Frame frame;
    frame.type = CMD_F_RC_RADIO_ENABLING;
    if (enabled) {
        frame.data1B[0] = 1;
    } else {
        frame.data1B[0] = 0;
    }

    return frame;
}

Frame roverOpModeSelectionToFrame(int opMode) {
    
    Frame frame;
    frame.type = CMD_F_ROVER_OP_MODE_SELECTION;
    frame.data1B[0] = opMode;

    return frame;

}

Frame armServoSelectionToFrame(bool controlClawServosSelected) {
    
    Frame frame;
    frame.type = CMD_F_ARM_SERVO_SELECTION;
    if (controlClawServosSelected) {
        frame.data1B[0] = 1;
    } else {
        frame.data1B[0] = 0;
    }

    return frame;

}

Frame raspberryPiPowerStatusToFrame(bool powerOn) {
    
    Frame frame;
    frame.type = CMD_F_RASPBERRYPI_POWER;
    if (powerOn) {
        frame.data1B[0] = 1;
    } else {
        frame.data1B[0] = 0;
    }

    return frame;

}
