from threading import Thread

import logic.frameTypesDefinition as frameType
from logic.sharedStructs import Frame
from logic.sharedStructs import RoverBatteryValues
from logic.sharedStructs import EnvironmentalValues
from logic.sharedStructs import SpeedometerValues
from logic.valuesFromFrameConversion import ValuesFromFrame
import website.flaskApp as flaskApp

class ReceivedFramesHandling:

    serial = None

    def __init__(self, serial):
        self.serial = serial

    def printRoverBatteryValues(self, roverBatteryValues):
        print('Rover battery values received')
        print('Cell voltage: {} V'.format(roverBatteryValues.cellVoltage))
        print('Percentage: {} %'.format(roverBatteryValues.percentage))
        print('')

    def printEnvironmentalValues(self, environmentalValues):
        print('Environmental values received')
        print('Temperature: {}'.format(environmentalValues.temperature))
        print('Humidity: {} %'.format(environmentalValues.humidity))
        print('Pressure: {} hPa'.format(environmentalValues.pressure))
        print('Altitude: {} m'.format(environmentalValues.altitude))
        print('')

    def printSpeedometerValues(self, speedometerValues):
        print('Speedometer values received')
        print('RPM: {}'.format(speedometerValues.rpm))
        print('Meters per hour: {}'.format(speedometerValues.metersPerHour))
        print('Distance: {} m'.format(speedometerValues.distance))
        print('')

    def handleReceivedFrames(self):
        while True:
            frame = self.serial.receiveFrame()

            match frame.fType:

                case frameType.INF_F_ROVER_BAT_LEVEL:
                    roverBatteryValues = ValuesFromFrame.roverBatteryValues(frame)
                    #self.printRoverBatteryValues(roverBatteryValues)
                    flaskApp.setStatusData(None, roverBatteryValues, None, None)

                case frameType.INF_F_ENVIRONMENTAL_MONITORIZATION:
                    environmentalValues = ValuesFromFrame.environmentalValues(frame)
                    #self.printEnvironmentalValues(environmentalValues)
                    flaskApp.setStatusData(environmentalValues, None, None, None)

                case frameType.INF_F_SPEEDOMETER_VALUES:
                    speedometerValues = ValuesFromFrame.speedometerValues(frame)
                    #self.printSpeedometerValues(speedometerValues)
                    flaskApp.setStatusData(None, None, speedometerValues, None)

                case _:
                    #Do nothing
                    pass
        
    def startThread(self):
        handleReceivedFramesThread = Thread(target=self.handleReceivedFrames)  
        handleReceivedFramesThread.start()