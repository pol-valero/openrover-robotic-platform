from serialCommunication import SerialCommunication 
from receivedFramesHandling import ReceivedFramesHandling
from statusDataManager import StatusDataManager


if __name__ == '__main__':

    try:

        serial = SerialCommunication()  

        receivedFramesHandling = ReceivedFramesHandling(serial)
        receivedFramesHandling.startThread()

        statusDataManager = StatusDataManager(serial)
        statusDataManager.startThread()
        
    except KeyboardInterrupt:
        link.close()