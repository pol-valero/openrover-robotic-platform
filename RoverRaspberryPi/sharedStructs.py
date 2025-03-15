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