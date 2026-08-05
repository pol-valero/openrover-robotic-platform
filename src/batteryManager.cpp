#include <Arduino.h>

#include "frameTypesDefinition.h"
#include "batteryManager.h"
#include "valuesToFrameConversion.h"
#include "serialCommunication.h"


const int batt_input_pin = A4; //Analog pin where the battery voltage is read

void setupBatteryMonitor() {
  pinMode(batt_input_pin, INPUT);
}

BatteryValues getRcBatteryValues() {

  //We are using 4x 1.2V AAA NiMH batteries for the RC remote (which can be seen as a 4 cell battery pack)

  BatteryValues battValues;

  int batt_divider_voltage_analog_value;  //Analog value read from the voltage divider output (0-1023)
  float batt_divider_voltage; //Volts in the voltage divider output (approximately 2.7V when the batteries are fully charged). The voltage divider in the RC halves the total voltage.
  int batt_percentage; //Percentage of the battery, from 0% (1.0V per battery/cell) to 100% (1.35V per battery/cell)

  batt_divider_voltage_analog_value = analogRead(batt_input_pin);

  batt_divider_voltage = (5.00 / 1023) * batt_divider_voltage_analog_value;

  float cell_voltage = batt_divider_voltage / 2; //The voltage divider halves the total voltage produced by the 4 batteries, so we divide by 2 to get the voltage of each battery/cell.

  batt_percentage = map(cell_voltage * 100, 1.35 * 100, 1 * 100, 100, 0); //We multiply by 100 because the map() function does not accept floats.

  if (cell_voltage <= 1) {
    batt_percentage = 0;
  }

  battValues.cellVoltage = cell_voltage;
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