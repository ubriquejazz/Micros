#!/usr/bin/python3.6
# This function sends a complete packet to the serial port and hopefully a waiting PIC32MZ

import sys
import time
import serial
import DeviceWrapper
from MrtsTestHelpers import stringToAddress 


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


def main():
    print("MrtsCommand.py")
    numRepeats=1

    if (len(sys.argv)>=5):
        thePort = sys.argv[1]
        sourceAddr = stringToAddress(sys.argv[2])
        destAddr = stringToAddress(sys.argv[3])
        command = sys.argv[4]
        print("Sending from ", sourceAddr, " to ", destAddr)
    else:
        print("Usage ", sys.argv[0], " <port> <source> <destination> <command>")
        sys.exit()
        
    wrap = DeviceWrapper.DeviceWrapper()
    wrap.ReadCallback = ReadDataCB
    wrap.WriteCallback = WriteDataCB  
    wrap.ReceivedRawDataCallback = RawDataReceived 
    wrap.ReceivedPacketCallback = PacketReceived
    wrap.ReceivedAddressedPacketCallback = AddressedPacketReceived

        
    ser = serial.Serial(thePort, 115200, timeout=0.25)
    ser.rtscts = False
    ser.xonxoff = False
    ser.bytesize = serial.EIGHTBITS
    ser.paritu  = serial.PARITY_NONE
    ser.stopbits = serial.STOPBITS_ONE
    print("Setup serial port ", thePort)

    separator=" "
    commandAndParameters=separator.join(sys.argv[4:len(sys.argv)])
    commandAndParameters=commandAndParameters+'\r'

    print("Command and parameters", commandAndParameters)
    encoded = wrap.encapsulateAddressed(commandAndParameters.encode(), 0xc002, sourceAddr, destAddr, 42, 4 )
    print("Encoded value(", len(encoded), ":: ", end="")
    for i in range(0, len(encoded)):
        print (hex(encoded[i])," ", end="");
    print(")")
        
    waitingForResponse=1  
    sent = ser.write(encoded)
    while (waitingForResponse):
        wrap.poll()

    print("** DONE **")

if (__name__== "__main__"):
    main()        