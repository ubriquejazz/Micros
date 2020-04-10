#!/usr/bin/python3.6
# This function sends a complete packet to the serial port and hopefully a waiting PIC32MZ

import sys
import serial
import DeviceWrapper
from _pytest._code import source

numRepeats=1

if (len(sys.argv)>=4):
    thePort = sys.argv[1]
    sourceAddr = int(sys.argv[2])
    destAddr = int(sys.argv[3])
    if (len(sys.argv)>=5):
        numRepeats = int(sys.argv[4])
else:
    print("MrtsPing <port> <src> <dst> <num repeats>")
    sys.exit()
    
testPayload=bytes([   0x00, 0x00, # Encapsulated Protocol = 0 (Echo Request_
                     0x65, 0x66, # Payload 0, 1
                     0x67, 0x68, # Payload 2, 3
                     0x69, 0x70  # Payload 4, 5
                 ])

ser = serial.Serial(thePort, 9600, timeout=0.125)
ser.rtscts = False
ser.xonxoff = False

wrap = DeviceWrapper.DeviceWrapper()
encoded = wrap.encapsulateAddressed( testPayload, 0xc002, sourceAddr, destAddr, 4)
print("Encoded value=",encoded)
     

for transmission in range(0, numRepeats):
    sent = ser.write(encoded)
    print ("Pass ", transmission, " Sent bytes ", sent)
    receiveData = bytearray(160)
    numRead = ser.readinto(receiveData)
    print ("Read ", numRead, "bytes \n\r")
    for i in range( 0, numRead ):
        print( chr(receiveData[i]), " =  ", hex(receiveData[i]))

