#include <Arduino.h>

#include "relayManager.h"

const int servosRelayPin = 28;
const int raspberryPiRelayPin = 40;


void setupRelays() {
    pinMode(servosRelayPin, OUTPUT);
    pinMode(raspberryPiRelayPin, OUTPUT);
}

void setServosRelayStatus(int status) {
    if (status == CLOSED) {
        digitalWrite(servosRelayPin, HIGH);
    } else if (status == OPEN) {
        digitalWrite(servosRelayPin, LOW);
    }
}

void setRaspberryPiRelayStatus(int status) {
    if (status == CLOSED) {
        digitalWrite(raspberryPiRelayPin, HIGH);
    } else if (status == OPEN) {
        digitalWrite(raspberryPiRelayPin, LOW);
    }
}