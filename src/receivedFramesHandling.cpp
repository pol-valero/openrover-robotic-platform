#include <Arduino.h>

#include "receivedFramesHandling.h"
#include "frameTypesDefinition.h"
#include "rcValuesManager.h"
#include "valuesFromFrameConversion.h"
#include "opModeManager.h"


void selectOperationMode(int opMode) {

    switch (opMode) {
        
        case OP_CONVENTIONAL_DRIVING:
            opConventionalDrivingSelected();
            break;
        case OP_360_DEGREE_TURN_CONTROL:
            op360DegreeTurnControlSelected();
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
        default:
            break;
    }

    checkRcLinkStatus(frame);

}