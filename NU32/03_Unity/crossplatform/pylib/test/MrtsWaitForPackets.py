#!/usr/bin/python3.6
# This function sends a complete packet to the serial port and hopefully a waiting PIC32MZ

import serial
import DeviceWrapper

ser = serial.Serial("/dev/pts/3", 9600, timeout=0)
ser.rtscts = False
ser.xonxoff = False

ser.timeout = 3

def WriteDataCB(buffer, bufferLength):
    print("WriteDataCB")
    return b"Wibble",
    

def ReadDataCB(bufferLength):
    global readCount
    global ser
    retVal = None
    #print("Polling, len=", bufferLength)
    if (ser.in_waiting):
        retVal = ser.read(1)
        print("Received=", retVal)
    return retVal,


def RawDataReceived(buffer):
    print("RawDataReceived ", buffer.hex())
    return 0

def PacketReceived(payload, type, crcOk):
    print("PacketReceived, payload=", payload, " type= ", type, "crcOK=", crcOk)


def AddressedPacketReceived(payload, src, dst, type, crcOk):
    print("AddressedPacketReceived, payload=", payload, " src=", src, " dst=", dst, " type= ", type, "crcOK=", crcOk)



print("Waiting for incomming packets")
wrap = DeviceWrapper.DeviceWrapper()
wrap.ReadCallback = ReadDataCB
wrap.WriteCallback = WriteDataCB  
wrap.ReceivedRawDataCallback = RawDataReceived   
wrap.ReceivedPacketCallback = PacketReceived
wrap.ReceivedAddressedPacketCallback = AddressedPacketReceived

while (1):
    wrap.poll()
    

