#include <Arduino.h>

const int buzzer_pin = 29;

void setupBuzzer() {
  pinMode(buzzer_pin, OUTPUT);
}

void lostRcLinkBuzz() {
  tone(buzzer_pin, 200, 500);
}

void lowBatteryBuzz() {
  tone(buzzer_pin, 300, 250);
}



