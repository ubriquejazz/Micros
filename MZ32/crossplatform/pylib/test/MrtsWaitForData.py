#!/usr/bin/python3.6
# This is just to wait for messages coming down from the serial port.

import sys
import time
import serial
import DeviceWrapper


def WriteDataCB(buffer, bufferLength):
    print("WriteDataCB")
    


def ReadDataCB(bufferLength):
    global readCount
    global ser
    retVal = None
    #print("Polling, len=", bufferLength)
    if (ser.in_waiting):
        retVal = ser.read(1)
        print("Received=", retVal);
    return retVal,

def RawDataReceived(buffer):
    print("RawDataReceived (", end="")
    for i in range(0, len(buffer)):
        print (hex(buffer[i])," ", end="")
    print(")")
    return 0


def PacketReceived(payload, type, crcOk):
    print("PacketReceived, payload=", payload, " type= ", type, "crcOK=", crcOk)


def AddressedPacketReceived(payload, src, dst, sequence, type, crcOk):
    global waitingForResponse
    print("AddressedPacketReceived src=", src, " dst=", dst, " sequence=", sequence, " type= ", type, "crcOK=", crcOk, end="")
    print(" payload <", payload, ">")
    waitingForResponse=0



numRepeats=1

if (len(sys.argv)>=2):
    thePort = sys.argv[1]
else:
    print("Usage ", sys.argv[0], " <port>")
    sys.exit()
    
wrap = DeviceWrapper.DeviceWrapper()
wrap.ReadCallback = ReadDataCB
wrap.WriteCallback = WriteDataCB  
wrap.ReceivedRawDataCallback = RawDataReceived 
wrap.ReceivedPacketCallback = PacketReceived
wrap.ReceivedAddressedPacketCallback = AddressedPacketReceived
wrap.UseRawTerminator=False

    
ser = serial.Serial(thePort, 115200, timeout=0.25)
ser.rtscts = False
ser.xonxoff = False
ser.bytesize = serial.EIGHTBITS
ser.paritu  = serial.PARITY_NONE
ser.stopbits = serial.STOPBITS_ONE
print("Setup and waiting on serial port ", thePort)


waitingForResponse=1  
while (1):
    time.sleep(0.005)
    wrap.poll()

print("** DONE **")
