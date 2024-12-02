#!/usr/bin/python3
"""
This is a set of unit tests to ensure the callback functions have error checking

"""

import DeviceWrapper
    
def test_BadReceivedRawDataCallback(): 
    print("test_BadReceivedRawDataCallback")
    wrap = DeviceWrapper.DeviceWrapper()
    exceptionCaught=0
    try:
        wrap.ReceivedRawDataCallback = 9
        pass
    except Exception as theException:
        print("Exception caught :", theException)
        exceptionCaught=1
    else:
        print("No exception caught")
    finally:
        assert exceptionCaught==1
         
def test_BadReceivedAddressedPacketCallback(): 
    print("test_BadReceivedAddressedPacketCallback")
    wrap = DeviceWrapper.DeviceWrapper()
    exceptionCaught=0
    try:
        wrap.ReceivedAddressedPacketCallback = "A string is an error."
        pass
    except Exception as theException:
        print("Exception caught :", theException)
        exceptionCaught=1
    else:
        print("No exception caught")
    finally:
        assert exceptionCaught==1
        
def test_BadReceivedPacketCallback(): 
    print("test_BadReceivedPacketCallback")
    wrap = DeviceWrapper.DeviceWrapper()
    exceptionCaught=0
    try:
        wrap.ReceivedPacketCallback = wrap
        pass
    except Exception as theException:
        print("Exception caught :", theException)
        exceptionCaught=1
    else:
        print("No exception caught")
    finally:
        assert exceptionCaught==1
        
def test_BadReadCallback(): 
    print("test_BadReadCallback")
    wrap = DeviceWrapper.DeviceWrapper()
    exceptionCaught=0
    try:
        wrap.ReadCallback = 0
        pass
    except Exception as theException:
        print("Exception caught :", theException)
        exceptionCaught=1
    else:
        print("No exception caught")
    finally:
        assert exceptionCaught==1      
        
def test_BadWriteCallback(): 
    print("test_BadWriteCallback")
    wrap = DeviceWrapper.DeviceWrapper()
    exceptionCaught=0
    try:
        wrap.WriteCallback = 0
        pass
    except Exception as theException:
        print("Exception caught :", theException)
        exceptionCaught=1
    else:
        print("No exception caught")
    finally:
        assert exceptionCaught==1      
                     
def test_BadInitChannelCallback(): 
    print("test_BadInitChannelCallback")
    wrap = DeviceWrapper.DeviceWrapper()
    exceptionCaught=0
    try:
        wrap.InitChannelCallback = 0
        pass
    except Exception as theException:
        print("Exception caught :", theException)
        exceptionCaught=1
    else:
        print("No exception caught")
    finally:
        assert exceptionCaught==1      
        
def test_BadCloseChannelCallback(): 
    print("test_BadCloseChannelCallback")
    wrap = DeviceWrapper.DeviceWrapper()
    exceptionCaught=0
    try:
        wrap.CloseChannelCallback = 0
        pass
    except Exception as theException:
        print("Exception caught :", theException)
        exceptionCaught=1
    else:
        print("No exception caught")
    finally:
        assert exceptionCaught==1            

def main():
    print("Test Encapsulate Addressed")
    test_BadReceivedRawDataCallback()
    test_BadReceivedPacketCallback()
    test_BadReceivedAddressedPacketCallback()
    test_BadReadCallback()
    test_BadWriteCallback()
    test_BadInitChannelCallback()
    test_BadCloseChannelCallback()
    
if (__name__== "__main__"):
    main()
