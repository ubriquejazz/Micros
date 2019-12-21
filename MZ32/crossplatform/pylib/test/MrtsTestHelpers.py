#!/usr/bin/python3
from enum import Enum

class BlockType(Enum):
    UNKNOWN = 0
    RAWDATA = 1
    PACKET = 2
    ADDRESSED_PACKET = 3
    
ENC_DLE = 0x10
ENC_STX = 0x02
ENC_ETX = 0x03

class MrtsDataBlock(object):
    def __init__(self, newBlockType, newStartPos):
        self.blockType = newBlockType
        self.startPos = newStartPos 
        
        
class MrtsTest(object):
    
    def __init__(self):
        self.readPosition = 0
        self.blockNumber = 0
        self.blockList = []
        self.dataBytes = b""
    
    def addBlock(self, blockType, newDataBytes):
        newBlock = MrtsDataBlock(blockType, len(self.dataBytes))
        self.dataBytes += newDataBytes
        self.blockList.append(newBlock)
        
    def getBytes(self, numBytes):
        retVal = b""
        endPosition = self.readPosition + numBytes
        if (endPosition>len(self.dataBytes)):
            endPosition=len(self.dataBytes)
        if (self.readPosition<len(self.dataBytes)):
            retVal=self.dataBytes[self.readPosition:endPosition]
        self.readPosition = endPosition   
        return retVal

    def getBlockTypeByByteIndex(self, byteIndex):          
        lastBlockType = BlockType.UNKNOWN
        for block in self.blockList:
            if (block.startPos>byteIndex):
                return lastBlockType
            lastBlockType=block.blockType
        return lastBlockType
        
        
# This function is used to allow the address to be split into subnet and local
# net address.        
def stringToAddress(addressString):
    separatedAddress= addressString.split(":")
    if (len(separatedAddress)==2):
        numberedAddress=int(separatedAddress[0])*256+int(separatedAddress[1])
    elif (len(separatedAddress)==1):
        numberedAddress = int(separatedAddress[0])
    else:
        raise ValueError("Address must be in form <subnet><:><address> where subnet and dot prefix are optional")
    return numberedAddress
      
      
def main():
    print("MrtsTestHelpers.py")

if (__name__== "__main__"):
    main()        
        

    