#include "valuesFromFrameConversion.h"

RcValues rcValuesFromFrame(Frame frame) {

    RcValues rcValues;
    
    rcValues.y1 = frame.data2B[0];
    rcValues.x1 = frame.data2B[1];
    rcValues.y2 = frame.data2B[2];
    rcValues.x2 = frame.data2B[3];
    rcValues.aux1 = frame.data1B[0];
    rcValues.aux2 = frame.data1B[1];
    rcValues.aux3 = frame.data1B[2];
    rcValues.aux4 = frame.data1B[3];
    
    return rcValues;

}

int roverOpModeSelectionFromFrame(Frame frame) {
    return frame.data1B[0];
}

bool armServoSelectionFromFrame(Frame frame) {
    if (frame.data1B[0] == 1) {
        return true;
    } else {
        return false;
    }
}

bool raspberryPiPowerStatusFromFrame(Frame frame) {
    if (frame.data1B[0] == 1) {
        return true;    //Power ON
    } else {
        return false;   //Power OFF
    }
}