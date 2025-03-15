from threading import Thread
import time
import subprocess
import psutil

import frameTypesDefinition as frameType
from sharedStructs import RaspberryPiStatusValues
from sharedStructs import Frame
from valuesToFrameConversion import ValuesToFrame

class StatusDataManager:

    serial = None

    def __init__(self, serial):
        self.serial = serial

    def getCPUtemperature(self):
        temp = subprocess.check_output(['vcgencmd', 'measure_temp']).decode('utf-8')
        temp = float(temp.replace("temp=", "").replace("'C\n", ""))
        temp = int(temp)
        return temp

    def getCPUworkload(self):
        return psutil.cpu_percent(interval=0)   # We put interval=0 to get the CPU workload at the moment (for more accuracy, we would need to increase the interval and put this call on a thread)

    def sendRaspberryPiStatusValues(self):
        while True:

            cpuTemp = int(self.getCPUtemperature())
            cpuWorkload = int(self.getCPUworkload())

            print('CPU Temperature: {}'.format(cpuTemp))
            print('CPU Workload: {}'.format(cpuWorkload))
            rpiStatusValues = RaspberryPiStatusValues(online=True, cameraOn=False, cpuTemperature=cpuTemp, cpuWorkload=cpuWorkload)
            frame = ValuesToFrame.raspberryPiStatus(rpiStatusValues)
            self.serial.sendFrame(frame)

            time.sleep(2.5)

    def startThread(self):
        sendRaspberryPiStatusValuesThread = Thread(target=self.sendRaspberryPiStatusValues)
        sendRaspberryPiStatusValuesThread.start()
        