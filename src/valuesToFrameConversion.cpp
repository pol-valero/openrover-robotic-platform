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