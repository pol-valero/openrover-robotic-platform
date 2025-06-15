from pySerialTransfer import pySerialTransfer as txfer
from pySerialTransfer.pySerialTransfer import Status

import logic.frameTypesDefinition as frameType
from logic.sharedStructs import Frame

class SerialCommunication:

    link = None

    def __init__(self):
        self.link = txfer.SerialTransfer('/dev/ttyUSB0')   
        self.link.open()

        #print("Serial initialized")

    def receiveFrame(self):
        frame = Frame()

        frame.fType = frameType.NOT_VALID

        if self.link.available():
            recSize = 0
            
            frame.fType = self.link.rx_obj(obj_type='B', start_pos=recSize)
            recSize += txfer.STRUCT_FORMAT_LENGTHS['B']
            
            for i in range(4):
                frame.data2B[i] = self.link.rx_obj(obj_type='h', start_pos=recSize)
                recSize += txfer.STRUCT_FORMAT_LENGTHS['h']

            for i in range(5):
                frame.data1B[i] = self.link.rx_obj(obj_type='B', start_pos=recSize)
                recSize += txfer.STRUCT_FORMAT_LENGTHS['B']

        elif self.link.status.value <= 0:
            self.printSerialErrors() 

        return frame

    def sendFrame(self, frame):
        if frame.fType != frameType.NOT_VALID:
            sendSize = 0
            sendSize = self.link.tx_obj(frame.fType, start_pos=sendSize, val_type_override='B')
            for i in range(4):
                sendSize = self.link.tx_obj(frame.data2B[i], start_pos=sendSize, val_type_override='h')
            for i in range(5):
                sendSize = self.link.tx_obj(frame.data1B[i], start_pos=sendSize, val_type_override='B')
            self.link.send(sendSize)

    def printSerialErrors(self):
        if self.link.status == Status.CRC_ERROR:
            print('ERROR: CRC_ERROR')
        elif self.link.status == Status.PAYLOAD_ERROR:
            print('ERROR: PAYLOAD_ERROR')
        elif self.link.status == Status.STOP_BYTE_ERROR:
            print('ERROR: STOP_BYTE_ERROR')
        else:
            print('ERROR: {}'.format(self.link.status.name)) 

    def printFrame(self, frame):
        #We print all fields
        print('Frame Type: {}'.format(frame.fType))
        print('Data 2B: {}'.format(frame.data2B))
        print('Data 1B: {}'.format(frame.data1B))
        print()