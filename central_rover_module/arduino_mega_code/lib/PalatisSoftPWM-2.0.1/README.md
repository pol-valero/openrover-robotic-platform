PalatisSoftPWM
===============

Software PWM library for [Arduino](http://arduino.cc)

AVR microcontrollers provide hardware PWM on some of the pins but if you need PWM on other pins then it must be implemented in software. This library provides easy and efficient software PWM on any pin. Each channel can be set to a different PWM duty cycle.


#### Differences from the original library
This is a fork of the excellent https://github.com/Palatis/arduino-softpwm. It is intended to be easier to use while retaining the same level of efficiency and flexibility achieved by the original author.
- Easy configuration: Arduino pin numbers for the most popular microcontrollers on any board that uses a standard pinout can now be used to configure PWM channels in addition to the previous PORT/BIT configuration system.
- Documentation: Installation, Usage, and Troubleshooting.
- Simplified example: Usage is now fully documented so the example can demonstrate the library without being overly confusing.
- Optional output delay. This feature of the original library may be useful for some applications but can also have an undesirable effect so I allow the user to decide whether to use it.


<a id="installation"></a>
#### Installation
- Download the most recent release from: https://github.com/per1234/PalatisSoftPWM/releases
- Using Arduino IDE 1.0.x:
  - Sketch > Import Library... > Add Library... > select the downloaded file > Open
- Using Arduino IDE 1.5+:
  - Sketch > Include Library > Add ZIP Library... > select the downloaded file > Open


<a id="usage"></a>
#### Usage
See **File > Examples > PalatisSoftPWM** for demonstration of library usage.

**`#define SOFTPWM_OUTPUT_DELAY`** - Add this line above `#include <PalatisSoftPWM.h>` for a 1 PWM clock cycle delay between outputs to prevent large in-rush currents.

**`SOFTPWM_DEFINE_PINn_CHANNEL(CHANNEL)`** - Configure Arduino pin n for software PWM use. This supports boards that use ATmega328P, ATmega168, ATmega32U4, ATmega2560, ATtiny85, ATmega1284P and related microcontrollers. For other microcontrollers use `SOFTPWM_DEFINE_CHANNEL()` instead. With ATmega1284P, etc. PalatisSoftPWM will default to the [Mighty 1284P standard variant](https://github.com/JChristensen/mighty-1284p/blob/v1.6.3/avr/variants/standard/pins_arduino.h) if the specific pinout can't be automatically determined.
- Parameter: **CHANNEL** - The channel number is used as an ID for the pin.

**`SOFTPWM_DEFINE_PINn_CHANNEL_INVERT(CHANNEL)`** - Depending on your application you may prefer to invert the output of the channel. This will cause PWM level 0 to produce a 100% duty cycle.
- Parameter: **CHANNEL** - The channel number is used as an ID for the pin.

**`SOFTPWM_DEFINE_CHANNEL(CHANNEL, PMODE, PORT, BIT)`** - Configure a pin for software PWM use. Consult the datasheet for your microcontroller for the appropriate `PORT` and `BIT` values for the physical pin. This information is shown in the pinout diagram, for example: [ATmega328P datasheet](http://www.atmel.com/Images/Atmel-8271-8-bit-AVR-Microcontroller-ATmega48A-48PA-88A-88PA-168A-168PA-328-328P_datasheet_Summary.pdf) **Figure 1-1** found on page 3. If you want to determine the Arduino pin assigned to the physical pin http://www.pighixxx.com/test/pinoutspg/boards/ provides this information for the most popular Arduino boards or you can look at the pins_arduino.h file in the **variant** folder used by your board. Usage is demonstrated in **File > Examples > PalatisSoftPWM > PalatisSoftPWM_example2**.
- Parameter: **CHANNEL** - The channel number is used as an ID for the pin.
- Parameter: **PMODE** - DDRx register of the pin's port. Append the port letter to `DDR`. For example: The [Arduino UNO diagram](http://www.pighixxx.com/test/portfolio-items/uno/?portfolioID=314) shows that Arduino pin 13 is **PB5** which means that the port is **B** so you should use the value `DDRB` for that pin.
- Parameter: **PORT** - The port of the pin. For example: Arduino pin 13 is **PB5** so you should use the value `PORTB` for that pin.
- Parameter: **BIT** - The bit of the pin. For example: Arduino pin 13 is **PB5** so you should use the value `PORTB5` for that pin.

**`SOFTPWM_DEFINE_CHANNEL_INVERT(CHANNEL, PMODE, PORT, BIT)`** - Depending on your application you may prefer to invert the output of the channel. This will cause PWM level 0 to produce a 100% duty cycle. See `SOFTPWM_DEFINE_CHANNEL()` for description of parameters.

**`SOFTPWM_DEFINE_OBJECT(CHANNEL_CNT)`** - Define the softPWM object with the default 256 PWM levels.
- Parameter: **CHANNEL_CNT** - The number of channels that are defined.

**`SOFTPWM_DEFINE_OBJECT_WITH_PWM_LEVELS(CHANNEL_CNT, PWM_LEVELS)`** - Define the softPWM object with the specified number of PWM levels.
- Parameter: **CHANNEL_CNT** - The number of channels that are defined.
- Parameter: **PWM_LEVELS** - The number of PWM levels. Using less PWM levels may allow a higher PWM frequency. The maximum value is 256.

**`SOFTPWM_DEFINE_EXTERN_OBJECT(CHANNEL_CNT)`** - Add this if you want to use the SoftPWM object outside where it's defined. See `SOFTPWM_DEFINE_OBJECT()` for description of the parameter.

**`SOFTPWM_DEFINE_EXTERN_OBJECT_WITH_PWM_LEVELS(CHANNEL_CNT, PWM_LEVELS)`** - Add this if you want to use the SoftPWM object outside where it's defined. See `SOFTPWM_DEFINE_OBJECT_WITH_PWM_LEVELS()` for description of parameters.

`PalatisSoftPWM.begin(hertz)` - Initialize PalatisSoftPWM.
- Parameter: **hertz** - The PWM frequency. Setting the value too high will cause incorrect operation, too low will cause a visible flicker.
  - Type: long

**`PalatisSoftPWM.printInterruptLoad()`** - Prints diagnostic information to the serial monitor. This can be used to find the optimal PWM frequency by setting different PWM frequency values in `PalatisSoftPWM.begin()` and then checking the resulting interrupt load. Calling this function will momentarily turn off the PWM on all channels.
- Output:
  - **Load of interrupt** - The portion of the time that the microcontroller spends doing soft PWM. A value of 1 would indicate that 100% of the time is spent in the PalatisSoftPWM ISR, meaning that no clock cycles are left to execute any other code.
  - **Clock cycles per interrupt** - The number of clock cycles required to run the PalatisSoftPWM ISR.
  - **Interrupt frequency** - The frequency at which the PalatisSoftPWM ISR is triggered.
  - **PWM frequency** - Actual PWM frequency. This may vary slightly from the value set in `PalatisSoftPWM.begin()`.
  - **PWM levels** - Value set in `SOFTPWM_DEFINE_OBJECT_WITH_PWM_LEVELS()`/`SOFTPWM_DEFINE_EXTERN_OBJECT_WITH_PWM_LEVELS)` or 256 if `SOFTPWM_DEFINE_OBJECT()`/`SOFTPWM_DEFINE_EXTERN_OBJECT()` was used.

**`PalatisSoftPWM.set(channel_idx, value)`** - Set the PWM level of the given channel.
- Parameter: **channel_idx** - The channel to set.
  - Type: int
- Parameter: **value** - The PWM level to set.
  - Type: byte

**`PalatisSoftPWM.size()`**
- Returns: Number of channels defined.
  - Type: size_t

**`PalatisSoftPWM.PWMlevels()`**
- Returns: The number of PWM levels.
  - Type: unsigned int

**`PalatisSoftPWM.allOff()`** - Set the PWM value of all channels to 0.


<a id="troubleshooting"></a>
#### Troubleshooting
- LEDs have a visible flicker, especially noticeable when the LED is moving relative to the viewer.
  - The PWM frequency set in `PalatisSoftPWM.begin()` is too low. Use `PalatisSoftPWM.printInterruptLoad()` to determine the optimum PWM frequency. You may be able to achieve a higher PWM frequency by setting less PWM levels with `SOFTPWM_DEFINE_OBJECT_WITH_PWM_LEVELS()` or `SOFTPWM_DEFINE_EXTERN_OBJECT_WITH_PWM_LEVELS()`.
- Erratic PWM operation
  - The interrupt load is too high. Use `PalatisSoftPWM.printInterruptLoad()` to determine the interrupt load. You can decrease the interrupt load by setting less PWM levels with `SOFTPWM_DEFINE_OBJECT_WITH_PWM_LEVELS()` or `SOFTPWM_DEFINE_EXTERN_OBJECT_WITH_PWM_LEVELS()` or setting the PWM frequency lower in `PalatisSoftPWM.begin()`.
- LED brightness changes between low brightness PWM values are larger than at brighter PWM values.
  - This is caused by the way LEDs work. If possible, use more PWM levels or never allow the LED to get dimmer than the level below which the difference between PWM levels is too distinct.
- `error: expected constructor, destructor, or type conversion before '(' token` compile error  when using `SOFTPWM_DEFINE_PINn_CHANNEL()` or `SOFTPWM_DEFINE_PINn_CHANNEL_INVERT()`.
  - The Arduino pins for the microcontroller model you're using are not defined. Use `SOFTPWM_DEFINE_CHANNEL()` or `SOFTPWM_DEFINE_CHANNEL_INVERT()` instead.


#### Contributing
Pull requests or issue reports are welcome! Please see the [contribution rules](https://github.com/per1234/PalatisSoftPWM/blob/master/CONTRIBUTING.md) for instructions.
