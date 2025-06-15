/* Demonstrates usage of the PalatisSoftPWM library.
   Fades Arduino pin 13 from PWM value 0 to the highest level and back to 0 again.
*/

#include <PalatisSoftPWM.h>

SOFTPWM_DEFINE_PIN13_CHANNEL(0);  //Configure Arduino pin 13 as PWM channel 0
SOFTPWM_DEFINE_OBJECT(1);

const unsigned int fadeDuration = 1000;  // (ms)The length of time for to go from PWM value 0 to the highest level and back to 0 again. The maximum allowed value is 8388.

void setup() {
  PalatisSoftPWM.begin(60);  // begin with 60 Hz PWM frequency
}

void loop() {
  // fade from PWM value 0 to the highest value
  for (byte value = 0; value < PalatisSoftPWM.PWMlevels() - 1; value++) {
    delayMicroseconds(fadeDuration * 1000UL / PalatisSoftPWM.PWMlevels() / 2);
    PalatisSoftPWM.set(0, value);
  }
  // fade back to PWM value 0
  for (int value = PalatisSoftPWM.PWMlevels() - 1; value >= 0; value--) {
    delayMicroseconds(fadeDuration * 1000UL / PalatisSoftPWM.PWMlevels() / 2);
    PalatisSoftPWM.set(0, value);
  }
}
