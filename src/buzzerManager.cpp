#include <Arduino.h>

const int buzzer_pin = A3; //Analog pin where the buzzer is connected

void setupBuzzer() {
    pinMode(buzzer_pin, OUTPUT);
    noTone(buzzer_pin);
}

void lowBatteryBuzz() {
    tone(buzzer_pin, 300, 250);
}

void shortTestBuzz() {
    tone(buzzer_pin, 1000, 100);
}