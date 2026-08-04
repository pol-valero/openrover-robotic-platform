#include <Arduino.h>

#include "frameTypesDefinition.h"
#include "batteryManager.h"
#include "valuesToFrameConversion.h"
#include "serialCommunication.h"


const int batt_input_pin = A0; //Analog pin where the battery voltage is read

void setupBatteryMonitor() {
  pinMode(batt_input_pin, INPUT);
}

BatteryValues getRcBatteryValues() {

  //We are using a Lithium-Ion battery for the RC remote

  BatteryValues battValues;

  int batt_divider_voltage_analog_value;  //Analog value read from the voltage divider output, which is approximately 4V when the battery is fully charged
  float batt_divider_voltage; //Volts of the voltage divider output. The voltage divider in the radio controller halves the voltage of the battery.
  int batt_percentage; //Percentage of the battery, from 0% (3.3V per cell) to 100% (4.2V per cell)

  batt_divider_voltage_analog_value = analogRead(batt_input_pin);

  batt_divider_voltage = (5.00 / 1023) * batt_divider_voltage_analog_value;

  //Notice: The max charge of the battery will be 4.2V per cell, 8.4V in total (2S battery).

  batt_percentage = map(batt_divider_voltage * 100, 4.2 * 100, 3.3 * 100, 100, 0); //We multiply by 100 because the map() function does not accept floats.

  if (batt_divider_voltage <= 3.3) {
    batt_percentage = 0;
  }

  battValues.cellVoltage = batt_divider_voltage;
  battValues.percentage = batt_percentage;

  return battValues;

}

Frame getRcBatteryValuesFrame() {
  
  Frame frame;
  frame.type = NOT_VALID;

  static unsigned long previousMillis = 0;

  //We calculate and get the battery values every 2 seconds
  if (millis() - previousMillis >= 2000) {
    previousMillis = millis();

    BatteryValues rcBatteryValues = getRcBatteryValues();
    frame = rcBattValuesToFrame(rcBatteryValues);

  }

  return frame;

}