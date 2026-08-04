#include <Arduino.h>

#include "valuesToFrameConversion.h"
#include "frameTypesDefinition.h"

Frame rcValuesToFrame(RcValues rcValues) {

  Frame frame;

  frame.type = INF_F_RC_VALUES;
  frame.data2B[0] = rcValues.y1;
  frame.data2B[1] = rcValues.x1;
  frame.data2B[2] = rcValues.y2;
  frame.data2B[3] = rcValues.x2;
  frame.data1B[0] = rcValues.aux1;
  frame.data1B[1] = rcValues.aux2;
  frame.data1B[2] = rcValues.aux3;
  frame.data1B[3] = rcValues.aux4;

  return frame;

}

Frame rcBattValuesToFrame(BatteryValues battValues) {

  Frame frame;
  
  frame.type = INF_F_RC_BAT_LEVEL;
  frame.data1B[0] = battValues.cellVoltage * 10; //We multiply by 10 because the float data is sent as an integer
  frame.data1B[1] = battValues.percentage;

  return frame;

}