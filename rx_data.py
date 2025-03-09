from pySerialTransfer import pySerialTransfer as txfer
from pySerialTransfer.pySerialTransfer import Status
from threading import Thread
import subprocess
import psutil
import time

import frameTypesDefinition as frameType


class Frame:
    fType = 0
    data2B = [0, 0, 0, 0]
    data1B = [0, 0, 0, 0, 0]

class RaspberryPiStatusValues:
    def __init__(self, online=False, cameraOn=False, cpuTemperature=0, cpuWorkload=0):
        self.online = online
        self.cameraOn = cameraOn
        self.cpuTemperature = cpuTemperature
        self.cpuWorkload = cpuWorkload


def getCPUtemperature():
    temp = subprocess.check_output(['vcgencmd', 'measure_temp']).decode('utf-8')
    temp = float(temp.replace("temp=", "").replace("'C\n", ""))
    temp = int(temp)
    return temp

def getCPUworkload():
    return psutil.cpu_percent(interval=0)   # We put interval=0 to get the CPU workload at the moment (for more accuracy, we would need to call increase the interval and put this call on a thread)

def printSerialErrors(link):
    if link.status == Status.CRC_ERROR:
        print('ERROR: CRC_ERROR')
    elif link.status == Status.PAYLOAD_ERROR:
        print('ERROR: PAYLOAD_ERROR')
    elif link.status == Status.STOP_BYTE_ERROR:
        print('ERROR: STOP_BYTE_ERROR')
    else:
        print('ERROR: {}'.format(link.status.name)) 

def printFrame(frame):
    #We print all fields
    print('Frame Type: {}'.format(frame.fType))
    print('Data 2B: {}'.format(frame.data2B))
    print('Data 1B: {}'.format(frame.data1B))
    print()

def receiveFrame(link):
    frame = Frame()

    frame.fType = frameType.NOT_VALID

    if link.available():
        recSize = 0
        
        frame.fType = link.rx_obj(obj_type='B', start_pos=recSize)
        recSize += txfer.STRUCT_FORMAT_LENGTHS['B']
        
        for i in range(4):
            frame.data2B[i] = link.rx_obj(obj_type='h', start_pos=recSize)
            recSize += txfer.STRUCT_FORMAT_LENGTHS['h']

        for i in range(5):
            frame.data1B[i] = link.rx_obj(obj_type='B', start_pos=recSize)
            recSize += txfer.STRUCT_FORMAT_LENGTHS['B']

    elif link.status.value <= 0:
        printSerialErrors(link) 

    return frame

def sendFrame(link, frame):
    if frame.fType != frameType.NOT_VALID:
        sendSize = 0
        sendSize = link.tx_obj(frame.fType, start_pos=sendSize, val_type_override='B')
        for i in range(4):
            sendSize = link.tx_obj(frame.data2B[i], start_pos=sendSize, val_type_override='h')
        for i in range(5):
            sendSize = link.tx_obj(frame.data1B[i], start_pos=sendSize, val_type_override='B')
        link.send(sendSize)

def handleReceivedFrames(link):
    while True:
        frame = receiveFrame(link)

        if frame.fType != frameType.NOT_VALID:
            printFrame(frame)

def raspberryPiStatusValuesToFrame(rpiStatusValues):
    frame = Frame()
    frame.fType = frameType.INF_F_RASPBERRYPI_STATUS

    if rpiStatusValues.online:
        frame.data1B[0] = 1
    else :
        frame.data1B[0] = 0

    if rpiStatusValues.cameraOn:
        frame.data1B[1] = 1
    else :
        frame.data1B[1] = 0

    frame.data1B[2] = rpiStatusValues.cpuTemperature
    frame.data1B[3] = rpiStatusValues.cpuWorkload

    return frame


def sendRaspberryPiStatusValues(link):
    while True:

        cpuTemp = int(getCPUtemperature())
        cpuWorkload = int(getCPUworkload())

        print('CPU Temperature: {}'.format(cpuTemp))
        print('CPU Workload: {}'.format(cpuWorkload))
        rpiStatusValues = RaspberryPiStatusValues(online=True, cameraOn=False, cpuTemperature=cpuTemp, cpuWorkload=cpuWorkload)
        frame = raspberryPiStatusValuesToFrame(rpiStatusValues)
        sendFrame(link, frame)

        time.sleep(2.5)


if __name__ == '__main__':

    try:
        link = txfer.SerialTransfer('/dev/ttyUSB0')   
        link.open()

        print('Waiting for data...')
    
        handleReceivedFramesThread = Thread(target=handleReceivedFrames, args=(link,))
        sendRaspberryPiStatusValuesThread = Thread(target=sendRaspberryPiStatusValues, args=(link,))
        handleReceivedFramesThread.start()
        sendRaspberryPiStatusValuesThread.start()
        
    except KeyboardInterrupt:
        link.close()