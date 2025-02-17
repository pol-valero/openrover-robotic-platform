#include <Arduino.h>

#include "valuesToFrameConversion.h"
#include "frameTypesDefinition.h"

Frame roverBattValuesToFrame(BatteryValues batteryValues) {
    
    Frame frame;
    
    frame.type = INF_F_ROVER_BAT_LEVEL;
    
    frame.data1B[0] = batteryValues.cellVoltage * 10;   //We multiply by 10 because the float data is sent as an integer
    frame.data1B[1] = batteryValues.percentage;
    
    return frame;
    
}