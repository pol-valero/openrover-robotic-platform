#include "valuesFromFrameConversion.h"

//TODO: From a frame, convert the different fields to "values" structs like "rcValue", environmental values, RPI status values, switches values...
//ex.- EnvMonitorValues getValuesFromEnvMonitorFrame()

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