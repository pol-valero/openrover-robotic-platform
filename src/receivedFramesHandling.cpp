#include <Arduino.h>

#include "receivedFramesHandling.h"
#include "frameTypesDefinition.h"
#include "rcValuesManager.h"
#include "valuesFromFrameConversion.h"
#include "opModeManager.h"
#include "armServoManager.h"


void selectOperationMode(int opMode) {

    switch (opMode) {
        
        case OP_CONVENTIONAL_DRIVING:
            opConventionalDrivingSelected();
            break;
        case OP_360_DEGREE_TURN_CONTROL:
            setOpMode(OP_360_DEGREE_TURN_CONTROL);
            break;
        case OP_ROBOTIC_ARM_CONTROL:
            setOpMode(OP_ROBOTIC_ARM_CONTROL);
            break;
        default:
            break;
    }
}

void handleReceivedFrame(Frame frame) {

    switch (frame.type) {

        case INF_F_RC_VALUES:
        {
            RcValues rcValues = rcValuesFromFrame(frame);
            setRcValues(rcValues);
            break;
        }
        case CMD_F_ROVER_OP_MODE_SELECTION:
        {
            int opMode = roverOpModeSelectionFromFrame(frame);
            selectOperationMode(opMode);
            break;
        }
        case CMD_F_ARM_SERVO_SELECTION:
        {
            bool controlClawServosSelected = armServoSelectionFromFrame(frame);
            setArmServoSelection(controlClawServosSelected);
            break;
        }
        default:
            break;
    }

    checkRcLinkStatus(frame);

}