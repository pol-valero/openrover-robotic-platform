/* Demonstrates usage of the PalatisSoftPWM library with port/bit channel configuration.
   Fades each channel from PWM value 0 to the highest level and back to 0 again.
*/

#include <PalatisSoftPWM.h>

/* pins_arduino.h defines the pin-port/bit mapping as PROGMEM so
   you have to read them with pgm_read_xxx(). That's generally okay
   for ordinary use, but really bad when you're writing super fast
   codes because the compiler doesn't treat them as constants and
   cannot optimize them away with sbi/cbi instructions.

   Therefore we have to tell the compiler the PORT and BIT here.
   Hope someday we can find a way to workaround this.

   Check the manual of your MCU for port/bit mapping.

   The following example demonstrates setting channels for all pins
   on the ATmega328P or ATmega168 used on Arduino Uno, Pro Mini,
   Nano and other boards. */
SOFTPWM_DEFINE_CHANNEL(0, DDRD, PORTD, PORTD0);  //Arduino pin 0 - this pin is used for Serial so you will be unable to use Serial in your sketch if you use it for PWM
SOFTPWM_DEFINE_CHANNEL(1, DDRD, PORTD, PORTD1);  //Arduino pin 1 - this pin is used for Serial so you will be unable to use Serial in your sketch if you use it for PWM
SOFTPWM_DEFINE_CHANNEL(2, DDRD, PORTD, PORTD2);  //Arduino pin 2
SOFTPWM_DEFINE_CHANNEL(3, DDRD, PORTD, PORTD3);  //Arduino pin 3
SOFTPWM_DEFINE_CHANNEL(4, DDRD, PORTD, PORTD4);  //Arduino pin 4
SOFTPWM_DEFINE_CHANNEL(5, DDRD, PORTD, PORTD5);  //Arduino pin 5
SOFTPWM_DEFINE_CHANNEL(6, DDRD, PORTD, PORTD6);  //Arduino pin 6
SOFTPWM_DEFINE_CHANNEL(7, DDRD, PORTD, PORTD7);  //Arduino pin 7
SOFTPWM_DEFINE_CHANNEL(8, DDRB, PORTB, PORTB0);  //Arduino pin 8
SOFTPWM_DEFINE_CHANNEL(9, DDRB, PORTB, PORTB1);  //Arduino pin 9
SOFTPWM_DEFINE_CHANNEL(10, DDRB, PORTB, PORTB2);  //Arduino pin 10
SOFTPWM_DEFINE_CHANNEL(11, DDRB, PORTB, PORTB3);  //Arduino pin 11
SOFTPWM_DEFINE_CHANNEL(12, DDRB, PORTB, PORTB4);  //Arduino pin 12
SOFTPWM_DEFINE_CHANNEL(13, DDRB, PORTB, PORTB5);  //Arduino pin 13
SOFTPWM_DEFINE_CHANNEL(14, DDRC, PORTC, PORTC0);  //Arduino pin A0
SOFTPWM_DEFINE_CHANNEL(15, DDRC, PORTC, PORTC1);  //Arduino pin A1
SOFTPWM_DEFINE_CHANNEL(16, DDRC, PORTC, PORTC2);  //Arduino pin A2
SOFTPWM_DEFINE_CHANNEL(17, DDRC, PORTC, PORTC3);  //Arduino pin A3
SOFTPWM_DEFINE_CHANNEL(18, DDRC, PORTC, PORTC4);  //Arduino pin A4
SOFTPWM_DEFINE_CHANNEL(19, DDRC, PORTC, PORTC5);  //Arduino pin A5


/* Or you may want inverted outputs: */
/*
  SOFTPWM_DEFINE_CHANNEL_INVERT(0, DDRD, PORTD, PORTD0);  //Arduino pin 0 - this pin is used for Serial so you will be unable to use Serial in your sketch if you use it for PWM
  SOFTPWM_DEFINE_CHANNEL_INVERT(1, DDRD, PORTD, PORTD1);  //Arduino pin 1 - this pin is used for Serial so you will be unable to use Serial in your sketch if you use it for PWM
  SOFTPWM_DEFINE_CHANNEL_INVERT(2, DDRD, PORTD, PORTD2);  //Arduino pin 2
  SOFTPWM_DEFINE_CHANNEL_INVERT(3, DDRD, PORTD, PORTD3);  //Arduino pin 3
  SOFTPWM_DEFINE_CHANNEL_INVERT(4, DDRD, PORTD, PORTD4);  //Arduino pin 4
  SOFTPWM_DEFINE_CHANNEL_INVERT(5, DDRD, PORTD, PORTD5);  //Arduino pin 5
  SOFTPWM_DEFINE_CHANNEL_INVERT(6, DDRD, PORTD, PORTD6);  //Arduino pin 6
  SOFTPWM_DEFINE_CHANNEL_INVERT(7, DDRD, PORTD, PORTD7);  //Arduino pin 7
  SOFTPWM_DEFINE_CHANNEL_INVERT(8, DDRB, PORTB, PORTB0);  //Arduino pin 8
  SOFTPWM_DEFINE_CHANNEL_INVERT(9, DDRB, PORTB, PORTB2);  //Arduino pin 9
  SOFTPWM_DEFINE_CHANNEL_INVERT(10, DDRB, PORTB, PORTB2);  //Arduino pin 10
  SOFTPWM_DEFINE_CHANNEL_INVERT(11, DDRB, PORTB, PORTB3);  //Arduino pin 11
  SOFTPWM_DEFINE_CHANNEL_INVERT(12, DDRB, PORTB, PORTB4);  //Arduino pin 12
  SOFTPWM_DEFINE_CHANNEL_INVERT(13, DDRB, PORTB, PORTB5);  //Arduino pin 13
  SOFTPWM_DEFINE_CHANNEL_INVERT(14, DDRC, PORTC, PORTC0);  //Arduino pin A0
  SOFTPWM_DEFINE_CHANNEL_INVERT(15, DDRC, PORTC, PORTC1);  //Arduino pin A1
  SOFTPWM_DEFINE_CHANNEL_INVERT(16, DDRC, PORTC, PORTC2);  //Arduino pin A2
  SOFTPWM_DEFINE_CHANNEL_INVERT(17, DDRC, PORTC, PORTC3);  //Arduino pin A3
  SOFTPWM_DEFINE_CHANNEL_INVERT(18, DDRC, PORTC, PORTC4);  //Arduino pin A4
  SOFTPWM_DEFINE_CHANNEL_INVERT(19, DDRC, PORTC, PORTC5);  //Arduino pin A5
*/

/* Here you make an instance of desired channel counts you want
   with the default 256 PWM levels (0 ~ 255). */
SOFTPWM_DEFINE_OBJECT(20);

const unsigned int fadeDuration = 1000;  // (ms)The length of time for each channel to go from PWM value 0 to the highest level and back to 0 again. The maximum allowed value is 8388.

void setup() {
  PalatisSoftPWM.begin(60);  // begin with 60 Hz PWM frequency
}

void loop() {
  for (byte channel = 0; channel < PalatisSoftPWM.size(); channel++) {  // cycle through the channels
    // fade from PWM value 0 to the highest value
    for (byte value = 0; value < PalatisSoftPWM.PWMlevels() - 1; value++) {
      delayMicroseconds(fadeDuration * 1000UL / PalatisSoftPWM.PWMlevels() / 2);
      PalatisSoftPWM.set(channel, value);
    }
    // fade back to PWM value 0
    for (int value = PalatisSoftPWM.PWMlevels() - 1; value >= 0; value--) {
      delayMicroseconds(fadeDuration * 1000UL / PalatisSoftPWM.PWMlevels() / 2);
      PalatisSoftPWM.set(channel, value);
    }
  }
}
