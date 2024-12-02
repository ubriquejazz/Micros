#!/usr/bin/python3.6
# This function sends a complete packet to the serial port and hopefully a waiting PIC32MZ

import sys
import serial
import DeviceWrapper

numRepeats=1

if (len(sys.argv)>=4):
    thePort = sys.argv[1]
    sourceAddr = int(sys.argv[2])
    destAddr = int(sys.argv[3])
    if (len(sys.argv)>=5):
        numRepeats = int(sys.argv[4])
else:
    print("MrtsGetVersion <port> <src> <dst> <num repeats>")
    sys.exit()


ser = serial.Serial("/dev/ttyUSB0", 9600, timeout=0.125)
ser.rtscts = False
ser.xonxoff = False

versionQuery = bytes([ 0x56, 0x45, # Payload 0, 1 V E
                      0x52,0x00 # Payload 2, 3 R \0
                     ])

wrap = DeviceWrapper.DeviceWrapper()
encoded = wrap.encapsulateAddressed(versionQuery, 0xc002, sourceAddr, destAddr, 4 )
print("Encoded value=",encoded)
     

for transmission in range(0, numRepeats):
    sent = ser.write(encoded)
    print ("Pass ", transmission, "Sent bytes ", sent)
    receiveData = bytearray(160)
    numRead = ser.readinto(receiveData)
    print ("Pass ", transmission, "Read ", numRead, "bytes \n\r")
    for i in range( 0, numRead ):
        print( chr(receiveData[i]), " =  ", hex(receiveData[i]))
