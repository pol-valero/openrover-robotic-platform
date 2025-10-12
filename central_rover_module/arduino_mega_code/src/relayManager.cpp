#include <Arduino.h>

#include "relayManager.h"

const int servosRelayPin = 28;
const int raspberryPiRelayPin = 40;


void setupRelays() {
    pinMode(servosRelayPin, OUTPUT);
    pinMode(raspberryPiRelayPin, OUTPUT);
}

void setServosRelayStatus(bool powerOn) {
    if (powerOn == true) {
        digitalWrite(servosRelayPin, HIGH);
    } else {
        digitalWrite(servosRelayPin, LOW);
    }
}

void setRaspberryPiRelayStatus(bool powerOn) {
    if (powerOn == true) {
        digitalWrite(raspberryPiRelayPin, HIGH);
    } else {
        digitalWrite(raspberryPiRelayPin, LOW);
    }
}