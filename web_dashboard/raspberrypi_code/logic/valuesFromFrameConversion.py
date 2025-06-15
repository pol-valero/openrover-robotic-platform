import logic.frameTypesDefinition as frameType
from logic.sharedStructs import RoverBatteryValues
from logic.sharedStructs import EnvironmentalValues
from logic.sharedStructs import SpeedometerValues
from logic.sharedStructs import Frame

class ValuesFromFrame:

    def roverBatteryValues(frame):
        roverBatteryValues = RoverBatteryValues()
        roverBatteryValues.cellVoltage = frame.data1B[0] / 10.0     #We divide by 10 because the initial value was multiplied by 10, to be sent as an integer
        roverBatteryValues.percentage = frame.data1B[1]
        return roverBatteryValues

    def environmentalValues(frame):
        environmentalValues = EnvironmentalValues()
        environmentalValues.temperature = frame.data2B[0]
        environmentalValues.humidity = frame.data2B[1]
        environmentalValues.pressure = frame.data2B[2]
        environmentalValues.altitude = frame.data2B[3]
        return environmentalValues

    def speedometerValues(frame):
        speedometerValues = SpeedometerValues()
        speedometerValues.rpm = frame.data2B[0]
        speedometerValues.metersPerHour = frame.data2B[1]
        speedometerValues.distance = frame.data2B[2]
        return speedometerValues

