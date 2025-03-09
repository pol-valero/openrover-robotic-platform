from time import sleep
from pySerialTransfer import pySerialTransfer as txfer
from pySerialTransfer.pySerialTransfer import Status


class Frame:
    fType = 0
    data2B = [0, 0, 0, 0]
    data1B = [0, 0, 0, 0, 0]


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

    frame.fType = 0 # Later we will equal it to the define "not valid"

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
    if frame.fType != 0:    # Later we will use the define "not valid"
        sendSize = 0
        sendSize = link.tx_obj(frame.fType, start_pos=sendSize, val_type_override='B')
        for i in range(4):
            sendSize = link.tx_obj(frame.data2B[i], start_pos=sendSize, val_type_override='h')
        for i in range(5):
            sendSize = link.tx_obj(frame.data1B[i], start_pos=sendSize, val_type_override='B')
        link.send(sendSize)


if __name__ == '__main__':
    try:
        frame = Frame()
        frame2 = Frame()
        link = txfer.SerialTransfer('/dev/ttyUSB0')
        
        link.open()

        print('Waiting for data...')
    
        while True:
                frame = receiveFrame(link)

                if frame.fType != 0:
                    printFrame(frame)

                    #We send all the fields, with a +2 (only when we receive a frame)
                    frame2.fType = frame.fType + 2
                    for i in range(4):
                        frame2.data2B[i] = frame.data2B[i] + 2
                    for i in range(5):
                        frame2.data1B[i] = frame.data1B[i] + 2
                    sendFrame(link, frame2)
        
    except KeyboardInterrupt:
        link.close()