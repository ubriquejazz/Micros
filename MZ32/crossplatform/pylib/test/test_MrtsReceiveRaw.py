#!/usr/bin/python3

import DeviceWrapper
import MrtsTestHelpers
from MrtsTestHelpers import BlockType

print("Adding blocks")
bufferLine = 0
helper =  MrtsTestHelpers.MrtsTest()
helper.addBlock(BlockType.RAWDATA, b"12345")
helper.addBlock(BlockType.RAWDATA, b"abcdef")

def WriteDataCB(buffer, bufferLength):
    print("WriteDataCB")
    return b"Wibble",
    
def ReadDataCB(bufferLength):
    global readCount
    retVal = helper.getBytes(bufferLength)
    print("ReadDataCB, bufferLength=", bufferLength )
    return retVal,

def DummyReadDataCB(bufferLength):
    return b"0123"

def RawDataReceived(buffer):
    print("RawDataReceived ", buffer.hex())
    return 0

def PacketReceived(payload, type, crcOk):
    print("PacketReceived, payload=", payload, " type= ", type, "crcOK=")

def test_Mrts2TestMultiBlock(): 
    print("MrtsTest")       
    
    print("Testing wrapper")
    wrap = DeviceWrapper.DeviceWrapper()
    wrap.ReadCallback = ReadDataCB
    wrap.WriteCallback = WriteDataCB  
    wrap.ReceivedRawDataCallback = RawDataReceived   
      
    for  i in range(0,100):
        wrap.poll()  
    return
 
if __name__ == "__main__":
    print("Test multiblock")
    test_Mrts2TestMultiBlock()

