# Remote control and touchscreen module

## Remote control options
The OpenRover project has two remote control options available:

1) Custom remote control created from scratch, using a manufactured double-layered PCB, some 3D printed parts, and acrylic plastic. Code, schematics, and further info can be found in this [folder](pcb_remote_control/). 

2) Custom attachment for a generic/commercial remote control that has a PPM output port, like the Spektrum DX8, using a home-made PCB and some 3D printed parts. Code, schematics, and further info can be found in this [folder](generic_remote_control_attachment/).

For anyone trying to replicate the project, I would definitely recommend choosing the first option. The second option would only be suited for those who already have a Spektrum DX8 (or similar) remote control, really like how it feels/handles, and would like to tinker with it. For the second option, also note that from my experience, the PPM readings can be a little unreliable sometimes.

//TODO: Image of Spektrum RC, Image of PCB RC with status screen

## Code setup guide
No matter which remote control option is selected, this module is formed by two submodules, the "Arduino Nano" and the "ESP32S3 touchscreen" submodules. The [code](esp32s3_code/) and [embedded UI design](touchscreen_ui_design_squareline/) of the "ESP32S3 touchscreen" submodule is exactly the same for the two remote control options, that's why they are located in this folder. The code and schematics of the "Arduino Nano" submodule and the schematics of the "ESP32S3 touchscreen" submodule differ depending on the chosen remote control option, that's why they are located inside the folder of each remote control option.

To develop the code for these submodules, the PlatformIO IDE was used. This IDE is integrated into the versatile Visual Studio Code editor, and can be installed simply by searching "PlatformIO IDE" in the "Extensions" tab of Visual Studio Code. 

Thanks to the use of PlatformIO, all the code, configuration files, and dependencies are in a single package. This makes it very easy to share the complete project, enabling other people to execute it right after they download it. 

Each of the two submodules is a standalone PlatformIO project. Therefore, just by opening the project present in this folder (and in the chosen RC option subfolder) with PlatformIO, connecting the ESP32S3 or Arduino Nano development board via USB, and clicking the "upload" button, the code will be uploaded to the ESP32S3 or Arduino Nano and will start executing. 

The only thing that needs to be taking into account is making sure that the wiring connections of the ESP32S3 and the Arduino Nano are exactly the same as the ones detailed in the wiring schematics of this module, which can be found inside the folder of each remote control option. 

## Embedded UI design
To create the embedded UI design of the touchscreen, the SquareLine Studio platform and LVGL library were used. To edit the UI, these steps have to be followed:

- Step 1: Open the [SquareLine Studio project](touchscreen_ui_design_squareline/) and make the desired visual changes

- Step 2: Click on Export -> Export UI Files. The UI files export path must be previously specified in the "Project Settings"

- Step 3: Copy all the exported UI Files into the "squareLineFiles" folder of the [ESP32S3 PlatformIO project](esp32s3_code/), making sure to overwrite the previous files that were in this folder

- Step 4: Click the "upload" button in PlatformIO to upload the code to the ESP32S3. The modified UI should now appear on the touchscreen. 

Three screens have been implemented in the embedded UI: Control, Monitor and Configuration. These screens can be seen in the images below. 

![Alt text](/images/embedded_UI_designs.png)