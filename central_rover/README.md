# Central rover module

## Code setup guide
To develop the code for this module, the PlatformIO IDE was used. This IDE is integrated into the versatile Visual Studio Code editor, and can be installed simply by searching "PlatformIO IDE" in the "Extensions" tab of Visual Studio Code. 

Thanks to the use of PlatformIO, all the code, configuration files, and dependencies are in a single package. This makes it very easy to share the complete project, enabling other people to execute it right after they download it. 

Therefore, just by opening the project present in this folder with PlatformIO, connecting the Arduino Mega development board via USB, and clicking the "upload" button, the code will be uploaded to the Arduino Mega and will start executing. 

The only thing that needs to be taking into account is making sure that the wiring connections to the Arduino Mega are exactly the same as the ones detailed in the  wiring schematic of this module. 

## Hardware and schematics
The hardware present on the central rover module includes an Arduino Mega development board that is responsible for managing all the moving elements (e.g., servomotors, motors, stepper motor), reading the environmental sensor’s values and sending/receiving communications via serial (to/from RaspberryPi Zero board) and via radio (to/from custom RC). 

In this [schematic](central_rover_module_schematic.pdf), all the hardware present on this module can be seen. 
