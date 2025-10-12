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

BatteryValues battValuesFromFrame(Frame frame) {

    BatteryValues battValues;
    
    battValues.cellVoltage = frame.data1B[0] / 10.0;  //We divide by 10.0 because the initial value was multiplied by 10, to be sent as an integer
    battValues.percentage = frame.data1B[1];
    
    return battValues;

}

EnvironmentalValues environmentalValuesFromFrame(Frame frame) {

    EnvironmentalValues envValues;
    
    envValues.temperature = frame.data2B[0];
    envValues.humidity = frame.data2B[1];
    envValues.pressure = frame.data2B[2];
    envValues.altitude = frame.data2B[3];
    
    return envValues;

}

SpeedometerValues speedometerValuesFromFrame(Frame frame) {

    SpeedometerValues speedValues;
    
    speedValues.rpm = frame.data2B[0];
    speedValues.metersPerHour = frame.data2B[1];
    speedValues.distance = frame.data2B[2];
    
    return speedValues;

}

RaspberryPiStatusValues rpiStatusValuesFromFrame(Frame frame) {

    RaspberryPiStatusValues rpiStatusValues;
    
    if (frame.data1B[0] == 1) {
        rpiStatusValues.online = true;
    } else {
        rpiStatusValues.online = false;
    }

    if (frame.data1B[1] == 1) {
        rpiStatusValues.cameraOn = true;
    } else {
        rpiStatusValues.cameraOn = false;
    }

    rpiStatusValues.cpuTemperature = frame.data1B[2];
    rpiStatusValues.cpuWorkload = frame.data1B[3];
    
    return rpiStatusValues;

}