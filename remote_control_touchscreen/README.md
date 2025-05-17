# Code setup guide for the "remote control and touchscreen" module

This module is formed by two submodules, the "Arduino Nano" and the "ESP32S3 touchscreen" submodules.

To develop the code for these submodules, the PlatformIO IDE was used. This IDE is integrated into the versatile Visual Studio Code editor, and can be installed simply by searching "PlatformIO IDE" in the "Extensions" tab of Visual Studio Code. 

Thanks to the use of PlatformIO, all the code, configuration files, and dependencies are in a single package. This makes it very easy to share the complete project, enabling other people to execute it right after they download it. 

Each of the two submodules is a standalone PlatformIO project. Therefore, just by opening each project present in this folder with PlatformIO, connecting the ESP32S3 or Arduino Nano development board via USB, and clicking the "upload" button, the code will be uploaded to the ESP32S3 or Arduino Nano and will start executing. 

The only thing that needs to be taking into account is making sure that the wiring connections of the ESP32S3 and the Arduino Nano are exactly the same as the ones detailed in the wiring schematics of this module. 