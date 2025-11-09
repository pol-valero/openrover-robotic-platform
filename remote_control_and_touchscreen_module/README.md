# Remote control and touchscreen module

## Remarks
The developed remote control and touchscreen module is currently an add-on to any generic RC that has a PPM output port (usually known as "Trainer Port"). The RC model that I have personally used is the Spektrum DX8. 

This trainer port usually is a 3.5mm audio jack, but it is not a standard port and can also be a mini-USB, depending on the brand. The transmitters usually use PPM (Pulse Position Modulation) signals outputted from this “Trainer port” to transmit each of the RC channel values.

With the help of an Arduino Nano, these PPM signals are processed by the custom RC module of the OpenRover project. This way, the radio module of the generic RC is not used, and the only purpose of the generic RC is to send the values of its different channels (e.g., joysticks, switches…) via PPM, so that the custom RC module can receive them. In turn, this custom module uses an NRF24 radio transceiver to send the channel values to the rover using a custom communication protocol. 

If instead of a generic RC, a custom RC with joysticks and switches wanted to be used, the Arduino Nano would directly read the values of these joysticks and switches instead of the PPM channel values. This adaptation would require very few modifications on the developed OpenRover hardware and software for this remote control and touchscreen module. 


## Code setup guide
This module is formed by two submodules, the "Arduino Nano" and the "ESP32S3 touchscreen" submodules.

To develop the code for these submodules, the PlatformIO IDE was used. This IDE is integrated into the versatile Visual Studio Code editor, and can be installed simply by searching "PlatformIO IDE" in the "Extensions" tab of Visual Studio Code. 

Thanks to the use of PlatformIO, all the code, configuration files, and dependencies are in a single package. This makes it very easy to share the complete project, enabling other people to execute it right after they download it. 

Each of the two submodules is a standalone PlatformIO project. Therefore, just by opening each project present in this folder with PlatformIO, connecting the ESP32S3 or Arduino Nano development board via USB, and clicking the "upload" button, the code will be uploaded to the ESP32S3 or Arduino Nano and will start executing. 

The only thing that needs to be taking into account is making sure that the wiring connections of the ESP32S3 and the Arduino Nano are exactly the same as the ones detailed in the wiring schematics of this module. 

## Embedded UI design
To create the embedded UI design of the touchscreen, the SquareLine Studio platform and LVGL library were used. To edit the UI, these steps have to be followed:

- Step 1: Open the [SquareLine Studio project](touchscreen_ui_design_squareline/) and make the desired visual changes

- Step 2: Click on Export -> Export UI Files. The UI files export path must be previously specified in the "Project Settings"

- Step 3: Copy all the exported UI Files into the "squareLineFiles" folder of the [ESP32S3 PlatformIO project](esp32s3_code/), making sure to overwrite the previous files that were in this folder

- Step 4: Click the "upload" button in PlatformIO to upload the code to the ESP32S3. The modified UI should now appear on the touchscreen. 

Three screens have been implemented in the embedded UI: Control, Monitor and Configuration. These screens can be seen in the images below. 

![Alt text](/images/embedded_UI_designs.png)

## Hardware and schematics
The hardware present on the remote control and touchscreen module includes an ESP32S3 development board that is responsible for managing the screen and an Arduino Nano board that helps processing the RC PPM signals and controlling the radio transceiver. 

The touchscreen model is the ESP32S3_8048S043, with a resolution of 800 x 480 px and 4.3" screen size. This model has an ESP32S3 and I/O connectors built in.

In this [schematic](rc_and_touchscreen_module_schematic.pdf), all the hardware present on this module can be seen. 