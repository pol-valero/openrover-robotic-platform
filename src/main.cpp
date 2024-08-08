#include <Arduino.h>
#include <RcTrainer.h>

// We listen on interrupt 0 which is digital input pin D2 (on Arduino uno)
// We can take a look at http://arduino.cc/en/Reference/attachInterrupt for mapping
// an interrupt number to pin number

RcTrainer tx;

typedef struct {
  int y1;
  int x1;
  int y2;
  int x2;
  int aux1;
  int aux2;
  int aux3;
  int aux4;
} RcValues;

void setup() {
    Serial.begin(19200);	
}

void loop() {

    char buffer[200];
    RcValues rcvalues;

    rcvalues.y1 = tx.getChannel(0);
    rcvalues.x1 = tx.getChannel(3);
    rcvalues.y2 = tx.getChannel(2);
    rcvalues.x2 = tx.getChannel(1);
    rcvalues.aux1 = tx.getChannel(4);
    rcvalues.aux2 = tx.getChannel(5);
    rcvalues.aux3 = tx.getChannel(6);
    rcvalues.aux4 = tx.getChannel(7);
    
    Serial.println("----------------------");

    sprintf(buffer, "X1: %d | Y1: %d | X2: %d | Y2: %d | AUX1: %d | AUX2: %d | AUX3: %d | AUX4: %d", rcvalues.x1, rcvalues.y1, rcvalues.x2, rcvalues.y2, rcvalues.aux1, rcvalues.aux2, rcvalues.aux3, rcvalues.aux4);

    Serial.println(buffer);
    
    delay(50);
}

