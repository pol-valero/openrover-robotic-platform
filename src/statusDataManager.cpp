#include <Arduino.h>

#include "statusDataManager.h"
#include "frameTypesDefinition.h"
#include "batteryManager.h"
#include "valuesToFrameConversion.h"
#include "sensorsManager.h"
#include "motorManager.h"
#include "radioCommunication.h"

bool statusDataEnabled = true;

Frame getRoverBatteryFrame() {

    Frame frame;
    frame.type = NOT_VALID;

    static unsigned long previousMillis = 0;

    //We calculate and get the battery values every 1.2 second
    if (millis() - previousMillis >= 1200) {
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

    //We get the speedometer values every 1.35 second
    if (millis() - previousMillis >= 1350) {
        previousMillis = millis();

        SpeedometerValues speedometerValues = getSpeedometerValues();
        frame = speedometerValuesToFrame(speedometerValues);

    }

    return frame;
}

Frame getStatusDataFrame() {
    
    Frame frame;
    frame.type = NOT_VALID;

    if (statusDataEnabled == false) {
        return frame;
    }

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
    //frame = getRpiStatusFrame(); -> It is better than instead of doing this here, the RPI itself sends its status data

    return frame;
    
}

void enableStatusData(bool enable) {
    statusDataEnabled = enable;
}

void checkRaspberryPiStatus(Frame frame) {

    static long msTimeSinceLastPacket = 0;

   if(frame.type == INF_F_RASPBERRYPI_STATUS) {

    msTimeSinceLastPacket = 0;
  
  } else {

    static unsigned long previousMillis1 = 0;

    if (millis() - previousMillis1 >= 1000) {
      previousMillis1 = millis();

      msTimeSinceLastPacket += 1000;
    }

    //If we spend more than 6 seconds without receiving a packet, the RPI status will be considered as offline
    if (msTimeSinceLastPacket >= 6000) {

        msTimeSinceLastPacket = 0;

        //RPI offline
        RaspberryPiStatusValues rpiStatusValues;
        rpiStatusValues.online = false;
        rpiStatusValues.cameraOn = false;
        rpiStatusValues.cpuTemperature = 0;
        rpiStatusValues.cpuWorkload = 0;

        Frame rpiStatusFrame = raspberryPiStatusValuesToFrame(rpiStatusValues);
        radioSendFrame(rpiStatusFrame);
        
    }

  }

}