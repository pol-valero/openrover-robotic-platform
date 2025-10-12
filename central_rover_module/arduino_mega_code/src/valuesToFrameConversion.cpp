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

Frame environmentalValuesToFrame(EnvironmentalValues environmentalValues) {
    
    Frame frame;
    
    frame.type = INF_F_ENVIRONMENTAL_MONITORIZATION;
    
    frame.data2B[0] = environmentalValues.temperature;
    frame.data2B[1] = environmentalValues.humidity;
    frame.data2B[2] = environmentalValues.pressure;
    frame.data2B[3] = environmentalValues.altitude;
    
    return frame;
    
}

Frame speedometerValuesToFrame(SpeedometerValues speedometerValues) {
    
    Frame frame;
    
    frame.type = INF_F_SPEEDOMETER_VALUES;
    
    frame.data2B[0] = speedometerValues.rpm;
    frame.data2B[1] = speedometerValues.metersPerHour; 
    frame.data2B[2] = speedometerValues.distance; 
    
    return frame;
    
}

Frame raspberryPiStatusValuesToFrame(RaspberryPiStatusValues rpiStatusValues) {
    
    Frame frame;
    
    frame.type = INF_F_RASPBERRYPI_STATUS;
    
    if (rpiStatusValues.online == true) {
        frame.data1B[0] = 1;
    } else {
        frame.data1B[0] = 0;
    }
    if (rpiStatusValues.cameraOn == true) {
        frame.data1B[1] = 1;
    } else {
        frame.data1B[1] = 0;
    }
    frame.data1B[2] = rpiStatusValues.cpuTemperature;
    frame.data1B[3] = rpiStatusValues.cpuWorkload;
    
    return frame;
    
}