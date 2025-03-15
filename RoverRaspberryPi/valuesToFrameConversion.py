import frameTypesDefinition as frameType
from sharedStructs import RaspberryPiStatusValues
from sharedStructs import Frame

class ValuesToFrame:

    def raspberryPiStatus(rpiStatusValues):
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