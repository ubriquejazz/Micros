#!/usr/bin/python3

import DeviceWrapper
import MrtsTestHelpers
from MrtsTestHelpers import BlockType

rubbishStart = b"pq"
moreRubbish = b"12345"
totalGarbage = b"abcdefghijklm"

realPacket = bytes ([ 
    MrtsTestHelpers.ENC_DLE,
    MrtsTestHelpers.ENC_STX, 
    0x00, 0x03,                    # length = 3
    0x80, 0x00,                    # type = 4 byte crc
    0x01, 0x02, 0x03,              # some data
    0x0a, 0x0b, 0x0c, 0x0d,        # 4 byte crc
    MrtsTestHelpers.ENC_DLE,
    MrtsTestHelpers.ENC_ETX])

bufferLine = 0
helper =  MrtsTestHelpers.MrtsTest()

def WriteDataCB(buffer, bufferLength):
    print("WriteDataCB")
    return b"Wibble",
    
def ReadDataCB(bufferLength):
    global readCount
    retVal = helper.getBytes(bufferLength)
    return retVal,

def DummyReadDataCB(bufferLength):
    return b"0123"

def RawDataReceived(buffer):
    print("RawDataReceived ", buffer.hex())
    return 0

def PacketReceived(payload, type, crcOk):
    print("PacketReceived, payload=", payload, " type= ", type, "crcOK=", crcOk)

def DoRealPacket():
    helper.addBlock(BlockType.RAWDATA,rubbishStart)
    helper.addBlock(BlockType.PACKET, realPacket)   
    helper.addBlock(BlockType.RAWDATA, moreRubbish)
    helper.addBlock(BlockType.PACKET, realPacket)   
    helper.addBlock(BlockType.RAWDATA, moreRubbish)
    
def test_Mrts2TestMultiBlock(): 
    print("test_Mrts2TestMultiBlock")       
    wrap = DeviceWrapper.DeviceWrapper()
    wrap.ReadCallback = ReadDataCB
    wrap.WriteCallback = WriteDataCB  
    wrap.ReceivedRawDataCallback = RawDataReceived   
    wrap.ReceivedPacketCallback = PacketReceived
    DoRealPacket()
      
    for  i in range(0,50):
        wrap.poll()  
    return
 
if __name__ == "__main__":
    print("Test multiblock")
    test_Mrts2TestMultiBlock()
