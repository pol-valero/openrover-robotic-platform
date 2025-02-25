#include <Arduino.h>

#include "statusDataManager.h"
#include "frameTypesDefinition.h"
#include "batteryManager.h"
#include "valuesToFrameConversion.h"
#include "sensorsManager.h"
#include "motorManager.h"

Frame getRoverBatteryFrame() {

    Frame frame;
    frame.type = NOT_VALID;

    static unsigned long previousMillis = 0;

    //We calculate and get the battery values every 1 second
    if (millis() - previousMillis >= 1000) {
        previousMillis = millis();

        BatteryValues roverBatteryValues = getRoverBatteryValues();
        frame = roverBattValuesToFrame(roverBatteryValues);

    }

    return frame;
    
}

Frame getEnvironmentalMonitoringFrame() {
    
    Frame frame;
    frame.type = NOT_VALID;

    static unsigned long previousMillis = 0;

    //We get the environmental monitoring values every 2 seconds
    if (millis() - previousMillis >= 2000) {
        previousMillis = millis();

        EnvironmentalValues environmentalValues = getEnvironmentalValues();
        frame = environmentalValuesToFrame(environmentalValues);

    }

    return frame;
    
}

Frame getSpeedometerFrame() {
    
    Frame frame;
    frame.type = NOT_VALID;

    static unsigned long previousMillis = 0;

    //We get the speedometer values every 1 second
    if (millis() - previousMillis >= 1000) {
        previousMillis = millis();

        SpeedometerValues speedometerValues = getSpeedometerValues();
        frame = speedometerValuesToFrame(speedometerValues);

    }

    return frame;
}

Frame getStatusDataFrame() {
    
    Frame frame;
    frame.type = NOT_VALID;

    frame = getRoverBatteryFrame();
    if (frame.type != NOT_VALID) {  //If frame is valid, we return it immediately (so that in a single iteration of this function, only one timer is reseted and one valid frame is gotten)
        return frame;
    }

    frame = getEnvironmentalMonitoringFrame();
    if (frame.type != NOT_VALID) {
        return frame;
    }

    frame = getSpeedometerFrame();
    if (frame.type != NOT_VALID) {
        return frame;
    }

    //TODO:
    //frame = getRpiStatusFrame();

    return frame;
    
}