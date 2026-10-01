#include "valuesFromFrameConversion.h"

bool rcRadioEnableStatusFromFrame(Frame frame) {
    return frame.data1B[0] == 1;
}