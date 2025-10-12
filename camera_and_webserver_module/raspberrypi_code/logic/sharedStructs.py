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

class RoverBatteryValues:
    def __init__(self, cellVoltage=0.0, percentage=0):
        self.cellVoltage = cellVoltage
        self.percentage = percentage

class EnvironmentalValues:
    def __init__(self, temperature=0, humidity=0, pressure=0, altitude=0):
        self.temperature = temperature
        self.humidity = humidity
        self.pressure = pressure
        self.altitude = altitude

class SpeedometerValues:
    def __init__(self, rpm=0, metersPerHour=0, distance=0):
        self.rpm = rpm
        self.metersPerHour = metersPerHour
        self.distance = distance