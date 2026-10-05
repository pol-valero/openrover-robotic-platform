## Remarks
For this remote control option, the developed remote control and touchscreen module is built using a custom double-layered PCB. The [PCB design](remote_control_pcb_design_easyeda.json) has been created using the EasyEDA app, and the PCB has been manufactured by JLCPCB. 

In order to manufacture the PCB, a [Gerber fabrication file](remote_control_pcb_gerber.zip) has to be generated from the design and then has to be uploaded to the manufacturer's website (e.g., JLCPCB, PCBWAY...). If you want to make any modifications to the EasyEDA PCB design, you will have to regenerate the Gerber by going into the top menu and selecting "Fabrication -> PCB Fabrication File (Gerber) -> Generate Gerber".

Other parts of the remote control that have to be manufactured are the top and bottom acrylic plates, which should be cut using a laser or CNC machine. In my case, I used a workshop near my home that had a laser machine, but there are also online websites where you can order laser cut designs in acrylic. These are the designs for the [top](rc_top_plate.dxf) and [bottom](rc_bottom_plate.dxf) acrylic plates. I would recommend using 3mm clear acrylic.     

## Hardware and schematics
For this option (custom remote control PCB), the hardware present in this module includes an ESP32S3 development board that is responsible for managing the screen and an Arduino Nano board that reads the analog or digital signals from the joysticks/switches/buttons, reads the battery voltage, and controls the radio transceiver. 

The touchscreen model is the ESP32S3_8048S043, with a resolution of 800 x 480 px and 4.3" screen size. This model has an ESP32S3 and I/O connectors built in.

This is the [schematic](esp32s3_touchscreen_submodule_schematic-pcb_remote.pdf) of the "ESP32S3 touchscreen" submodule for this remote control option. The [EasyEDA PCB design](remote_control_pcb_design_easyeda.json) can be seen as the schematic of the "Arduino Nano" submodule. 

Here is an image of the top EasyEDA PCB design:
![Alt text](/images/rc_pcb_design_front.png)

Here are some images of the assembled PCB:
| ![](/images/custom_rc_pcb_assembly_images/assembly9.jpeg) | ![](/images/custom_rc_pcb_assembly_images/assembly11.jpeg) |
| -------------------------- | ---------------------- |

## Short demo
![Alt text](/images/rc_pcb_control_screen_demo.gif)