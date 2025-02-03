#include <Arduino.h>
#include "serialCommunication.h"

const int batt_input_pin = A15; 
static char buffer[100];


void setupBatteryMonitor() {
    pinMode(batt_input_pin, INPUT);
}

//TODO: Change name to getBatteryValues() and return values, and reuse struct of RC arduino nano code
void printBatteryValues() {

  static unsigned long previousMillis = 0;

  //We calculate and print the battery percentage every 2 seconds
  if (millis() - previousMillis >= 2000) {
    previousMillis = millis();

    int batt_divider_voltage_analog_value;  //Analog value read from the voltage divider output, which is 4V when the battery is fully charged
    float batt_divider_voltage; //Volts of the voltage divider output
    int batt_percentage; //Percentage of the battery, from 0% (3.6V per cell) to 100% (4.2V per cell)

    batt_divider_voltage_analog_value = analogRead(batt_input_pin);

    batt_divider_voltage = (5.00 / 1023) * batt_divider_voltage_analog_value;

    //Calculations: The max charge of the battery will be 4.2V per cell, 12.6V in total. 
    //The voltage divider output when fully charged is 4V, therefore if we apply multiply by a correction factor of 4.2 / 4 = 1.05 the resulting voltage will be roughly the same as each cell of the battery. 

    batt_percentage = map(batt_divider_voltage * 1.05 * 100, 4.2 * 100, 3.6 * 100, 100, 0); //We multiply by 100 because the map() function does not accept floats.

    usbSerialPrint("\tBattery cell voltage: ");
    usbSerialPrintFloat(batt_divider_voltage * 1.05);

    sprintf(buffer, "\n\tBattery percentage: %d\n", batt_percentage);
    usbSerialPrint(buffer);

  }

}
