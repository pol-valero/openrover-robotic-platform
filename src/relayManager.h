#ifndef RELAY_MANAGER_H
#define RELAY_MANAGER_H

#define OPEN 1
#define CLOSED 2

void setServosRelayStatus(int status);

void setupRelays();

void setRaspberryPiRelayStatus(int status);

#endif