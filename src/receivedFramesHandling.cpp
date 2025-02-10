#include <Arduino.h>

#include "receivedFramesHandling.h"
#include "frameTypesDefinition.h"
#include "rcValuesManager.h"
#include "valuesFromFrameConversion.h"

void handleReceivedFrame(Frame frame) {

    switch (frame.type) {
        case INF_F_RC_VALUES:
            RcValues rcValues = rcValuesFromFrame(frame);
            setRcValues(rcValues);
            break;

        /*case CMD_F_CONV_MODE:
            opConventionalDrivingSelected();
            break;*/

        default:
            break;
    }

    checkRcLinkStatus(frame);

}