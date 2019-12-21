#!/usr/bin/python3

import DeviceWrapper

readCount = 1

def WriteDataCB(buffer, bufferLength):
    print("WriteDataCB")
    return b"Wibble",
    

def ReadDataCB(bufferLength):
    global readCount
    retVal = None
    if (readCount==1):
        retVal = bytes([0x01, 0x02, 0x03, 0x04, 0x05, 0x06])
    readCount=readCount - 1
    print("ReadDataCB, bufferLength=", bufferLength )
    return retVal,


def RawDataReceived(buffer):
    print("RawDataReceived ", buffer.hex())
    return 0


print("Testing wrapper")
wrap = DeviceWrapper.DeviceWrapper()
wrap.ReadCallback = ReadDataCB
wrap.WriteCallback = WriteDataCB  
wrap.ReceivedRawDataCallback = RawDataReceived          
for  i in range(0,10):
    wrap.poll()