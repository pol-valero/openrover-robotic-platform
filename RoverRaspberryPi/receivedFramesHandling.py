from threading import Thread

import frameTypesDefinition as frameType
from sharedStructs import Frame

class ReceivedFramesHandling:

    serial = None

    def __init__(self, serial):
        self.serial = serial


    def handleReceivedFrames(self):
        while True:
            frame = self.serial.receiveFrame()

            if frame.fType != frameType.NOT_VALID:
                self.serial.printFrame(frame)

    def startThread(self):
        handleReceivedFramesThread = Thread(target=self.handleReceivedFrames)  
        handleReceivedFramesThread.start()