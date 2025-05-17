# Code setup guide for the "camera and web dashboard" module

Before uploading this module's project code in the RaspberryPi Zero 2W, it is necessary to install and OS in the board. Specifically, the RaspberryPi OS Lite (headless version) has to be installed. This OS and an install guide can be found in the [RaspberryPi website](https://www.raspberrypi.com/software/operating-systems/).

Once the Raspberry Pi OS is running and SSH connections can be made to be the board, it is possible to upload all the module's project code. To upload an edit the code, the best way is to install the SFTP plugin for the Visual Studio Code editor. This plugin allows to easily set up and modify the project’s file structure and file’s contents remotely from a standard PC. 

Once the whole project is uploaded, you have to go inside the main project folder and execute the following command. 

```python -m logic.main```

If it is the first time you execute this command, several errors will displayed, detailing the dependencies used by the code need that to be installed. It is recommended to install this dependencies on a "virtual environment". Once these dependencies are installed, if we execute the previous command, this module's program will start running, stablishing serial communication with the Arduino Mega and generating the web dashboard. 

The web dashboard will be available on the following URL. 
```http://pizero.local:5000/monitor```

If instead of the RaspberryPi connecting to an existing WiFi, you want the RaspberryPi to generate a WiFi network on its own, the folowing command needs to be used.   
```sudo nmcli device wifi hotspot ssid RoverWiFi password marsRover```

Optionally, the commands to generate the WiFi network and to execute the module's program can be written into a script. This script can then be called inside the ```/etc/rc.local``` file, which is executed at startup. This way, once the RaspberryPi Zero 2W is powered, it will generate the WiFi network and execute the module's program automatically. 

It is important to make sure that the wiring connections to the RaspberryPi Zero 2W are exactly the same as the ones detailed in the wiring schematic of this module.