#!/usr/bin/python3
"""
This is a set of unit tests to test the function encapsulateAddressed 
in the Python DeviceWrapper

"""

import DeviceWrapper
    
def test_Mrts2TestEncapsulateAddressed(): 
    print("test_Mrts2TestEncapsulateAddressed")
    wrap = DeviceWrapper.DeviceWrapper()
    encoded = wrap.encapsulateAddressed( b"Wibble", 1, 2, 3, 4, 4)
    print("Encoded value=",encoded)
      
def test_Mrts2TestEncapsulateAddressedInvalidCrcLength(): 
    """
    This function is intended to test that an invalid CRC is rejected with an
    exception.
    """
    print("test_Mrts2TestEncapsulateAddressedInvalidCrcLength")       
    wrap = DeviceWrapper.DeviceWrapper()
    exceptionRaised=0
    try:
       encoded = wrap.encapsulateAddressed( b"Wibble", 1, 2, 9,  3, 3)
       print("Encoded value=",encoded)
    except ValueError as theException:
        exceptionRaised=1
        print("Caught Exception", theException)
    finally:
        assert(exceptionRaised==1)
       
def test_Mrts2TestEncapsulateAddressedMissingParameters(): 
    print("test_Mrts2TestEncapsulateAddressedMissingParameters")       
    wrap = DeviceWrapper.DeviceWrapper()
    exceptionRaised=0
    try:
       encoded = wrap.encapsulateAddressed( b"Wibble", 1, 2, 3)
       print("Encoded value=",encoded)
    except TypeError as theException:
        exceptionRaised=1
        print("Caught Exception", theException)
    finally:
        assert(exceptionRaised==1)

def main():    
    print("Test Encapsulate Addressed")
    test_Mrts2TestEncapsulateAddressed()
    test_Mrts2TestEncapsulateAddressedInvalidCrcLength()
    test_Mrts2TestEncapsulateAddressedMissingParameters()

if (__name__== "__main__"):
    main()        
 
