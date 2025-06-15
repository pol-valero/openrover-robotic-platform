#!/bin/bash

# This script will be called inside the /etc/rc.local file, which is executed at startup.
# Both the script and the rc.local file must have execution permissions (chmod +x pathToScript).

#exec > /home/polv/startup.log 2>&1     # Uncomment to create log file with all the prints that the commands below make (specially useful to see exceptions/errors)

sudo nmcli device wifi hotspot ssid RoverWiFi password marsRover2606

#cd /home/polv && source my-rover-venv/bin/activate

#cd /home/polv/RoverRaspberryPi && python -m logic.main &    # We add the & to run the process in the background, so this script can continue executing if additional commands are added.

