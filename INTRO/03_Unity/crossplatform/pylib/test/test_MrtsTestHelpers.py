#!/usr/bin/python3

import MrtsTestHelpers
from MrtsTestHelpers import BlockType

def test_Mrts2TestMultiBlock(): 
    print("MrtsTest")            
    trial=  MrtsTestHelpers.MrtsTest()
    trial.addBlock(BlockType.RAWDATA, b"12345")
    trial.addBlock(BlockType.ADDRESSED_PACKET, b"abcdef")
    dir(trial)
    
    print("Checking raw data type")
    blockType = trial.getBlockTypeByByteIndex(3)
    assert blockType==BlockType.RAWDATA
    
    
    print("Checking addressed data type")
    blockType = trial.getBlockTypeByByteIndex(8)
    assert blockType==BlockType.ADDRESSED_PACKET
    
    x = trial.getBytes(8)
    print("Bytes =", x)
    assert x==b"12345abc"
    
    x = trial.getBytes(2)
    print("Bytes =", x)
    assert x==b"de"
    
    trial.addBlock(BlockType.RAWDATA, b"ABCDE")
    x = trial.getBytes(2)
    print("Bytes =", x)
    assert x==b"fA"

    x = trial.getBytes(2)
    print("Bytes =", x)
    assert x==b"BC"

    x = trial.getBytes(4)
    print("Bytes =", x)
    assert x==b"DE"
    
    x = trial.getBytes(4)
    print("Bytes =", x)
    assert x==b""
    
    
    
def main():
    test_Mrts2TestMultiBlock()
    
if (__name__== "__main__"):
    main()    
    
