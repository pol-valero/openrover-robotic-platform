from time import sleep
from pySerialTransfer import pySerialTransfer as txfer
from pySerialTransfer.pySerialTransfer import Status


class Struct:
    z = ''
    y = 0.0

if __name__ == '__main__':
    try:
        testStruct = Struct
        link = txfer.SerialTransfer('/dev/ttyUSB0')
        
        link.open()
        #sleep(5)

        print('Waiting for data...')
    
        while True:
            if link.available():
                recSize = 0
                
                testStruct.z = link.rx_obj(obj_type='c', start_pos=recSize)
                recSize += txfer.STRUCT_FORMAT_LENGTHS['c']
                
                testStruct.y = link.rx_obj(obj_type='f', start_pos=recSize)
                
                print('{}{}'.format(testStruct.z, testStruct.y))
                sendSize = 0
                testStruct.z = 'h'
                testStruct.y = testStruct.y + 1
                sendSize = link.tx_obj(testStruct.z, start_pos=sendSize)
                sendSize = link.tx_obj(testStruct.y, start_pos=sendSize)
                link.send(sendSize)

                #print('Test1')
                
            elif link.status.value <= 0:
                if link.status == Status.CRC_ERROR:
                    print('ERROR: CRC_ERROR')
                elif link.status == Status.PAYLOAD_ERROR:
                    print('ERROR: PAYLOAD_ERROR')
                elif link.status == Status.STOP_BYTE_ERROR:
                    print('ERROR: STOP_BYTE_ERROR')
                else:
                    print('ERROR: {}'.format(link.status.name))
            
            #print('Test2')
  
        
    except KeyboardInterrupt:
        link.close()