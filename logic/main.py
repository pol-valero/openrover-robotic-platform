import time

from logic.serialCommunication import SerialCommunication 
from logic.receivedFramesHandling import ReceivedFramesHandling
from logic.statusDataManager import StatusDataManager
import website.flaskApp as flaskApp

if __name__ == '__main__':

    try:

        flaskApp.startThread()
        #time.sleep(3)

        serial = SerialCommunication()  

        receivedFramesHandling = ReceivedFramesHandling(serial)
        receivedFramesHandling.startThread()

        statusDataManager = StatusDataManager(serial)
        statusDataManager.startThread()
        
    except KeyboardInterrupt:
        link.close()