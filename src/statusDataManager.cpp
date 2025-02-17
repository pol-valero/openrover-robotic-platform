#include <Arduino.h>

#include "statusDataManager.h"
#include "frameTypesDefinition.h"
#include "batteryManager.h"
#include "valuesToFrameConversion.h"

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

Frame getStatusDataFrame() {
    
    Frame frame;
    frame.type = NOT_VALID;

    frame = getRoverBatteryFrame();

    //TODO:
    //frame = getRpiStatusFrame();
    //frame = getEnvironmentalMonitoringFrame();
    //frame = getOdometerFrame();

    return frame;
    
}