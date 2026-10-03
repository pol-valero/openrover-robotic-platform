## Remarks
For this remote control option, the developed remote control and touchscreen module is an attachment for any generic/commercial RC that has a PPM output port (usually known as "Trainer Port"). The RC model that I have personally used is the Spektrum DX8 (if another model wants to be used, the 3D printed parts would have to be adapted). 

This trainer port usually is a 3.5mm audio jack, but it is not a standard port and could also be a mini-USB, depending on the brand. The transmitters usually use PPM (Pulse Position Modulation) signals outputted from this “Trainer port” to transmit each of the RC channel values.

With the help of an Arduino Nano, these PPM signals can be processed by the custom attachment that was developed. This way, the radio module of the generic RC is not used, and the only purpose of the generic RC is to send the values of its different channels (e.g., joysticks, switches…) via PPM, so that the custom attachment can receive them. In turn, this custom attachment uses an NRF24 radio transceiver to send the channel values to the rover using a custom communication protocol. 

## Hardware and schematics
For this option (generic remote control attachment), the hardware present in this module includes an ESP32S3 development board that is responsible for managing the screen and an Arduino Nano board that processes the RC PPM signals, reads the battery voltage, and controls the radio transceiver. 

The touchscreen model is the ESP32S3_8048S043, with a resolution of 800 x 480 px and 4.3" screen size. This model has an ESP32S3 and I/O connectors built in.

This is the [schematic](rc_and_touchscreen_module_schematic-generic_remote.pdf) for this remote control option, where all the hardware present on this module can be seen. 