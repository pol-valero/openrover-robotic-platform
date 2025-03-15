#include <Arduino.h>
//#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "sensorsManager.h"

#define SEALEVELPRESSURE_HPA (1025)

Adafruit_BME280 bmeSensor;

void setupSensors() {
    bmeSensor.begin(0x76);  //The BME280 default I2C address is 0x76
}

EnvironmentalValues getEnvironmentalValues() {
    EnvironmentalValues environmentalValues;

    environmentalValues.temperature = bmeSensor.readTemperature();
    environmentalValues.humidity = bmeSensor.readHumidity();
    environmentalValues.pressure = bmeSensor.readPressure() / 100.0F;
    environmentalValues.altitude = bmeSensor.readAltitude(SEALEVELPRESSURE_HPA);

    return environmentalValues;
}


