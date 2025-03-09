from time import sleep
from pySerialTransfer import pySerialTransfer as txfer
from pySerialTransfer.pySerialTransfer import Status


class Frame:
    fType = 0
    data2B = [0, 0, 0, 0]
    data1B = [0, 0, 0, 0, 0]

if __name__ == '__main__':
    try:
        frame = Frame()
        link = txfer.SerialTransfer('/dev/ttyUSB0')
        
        link.open()
        #sleep(5)

        print('Waiting for data...')
    
        while True:
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

                #We print all fields
                print('Frame Type: {}'.format(frame.fType))
                print('Data 2B: {}'.format(frame.data2B))
                print('Data 1B: {}'.format(frame.data1B))
                print()
                
               # sendSize = 0
               # testStruct.z = 'h'
               # testStruct.y = testStruct.y + 1
               # sendSize = link.tx_obj(testStruct.z, start_pos=sendSize)
               # sendSize = link.tx_obj(testStruct.y, start_pos=sendSize)
               # link.send(sendSize)

               #We send all the fields, with a +1
                sendSize = 0
                frame.fType = frame.fType + 1
                sendSize = link.tx_obj(frame.fType, start_pos=sendSize, val_type_override='B')
                for i in range(4):
                    frame.data2B[i] = frame.data2B[i] + 1
                    sendSize = link.tx_obj(frame.data2B[i], start_pos=sendSize, val_type_override='h')
                for i in range(5):
                    frame.data1B[i] = frame.data1B[i] + 1
                    sendSize = link.tx_obj(frame.data1B[i], start_pos=sendSize, val_type_override='B')
                link.send(sendSize)
                
            elif link.status.value <= 0:
                if link.status == Status.CRC_ERROR:
                    print('ERROR: CRC_ERROR')
                elif link.status == Status.PAYLOAD_ERROR:
                    print('ERROR: PAYLOAD_ERROR')
                elif link.status == Status.STOP_BYTE_ERROR:
                    print('ERROR: STOP_BYTE_ERROR')
                else:
                    print('ERROR: {}'.format(link.status.name))  
        
    except KeyboardInterrupt:
        link.close()