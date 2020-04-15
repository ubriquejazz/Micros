/*! \file   barcode.c

    \brief  Decode the information stored on the 2D barcode or EEPROM

    This is stored in a structure so all data can be got at easily.
    The requested section is located and passed back.
    If the information is from an EEPROM then a write can also be performed.
*/
 
#include "barcode.h"
#include "1wirecontrol.h"   // OWDeviceInfo

unsigned char   crc8_bar;
unsigned char   check_bar;

//! Read a device chip signature
/*! @param *DevControl Structure of the device to get information from
     \return The type of device at this address
*/
OWchipTypes ReadSignature(OWDeviceInfo *DevControl)
{
    unsigned int FullSignature = 0;

    if (!System1WireReadByte(DevControl->Address,EEP_SIG_LOW,
            (unsigned char*)&FullSignature)) {
        return INACTIVE_ADDRESS;
    }
    FullSignature = FullSignature << 8;
    
    if (!System1WireReadByte(DevControl->Address,EEP_SIG_HIGH,
            (unsigned char*) &FullSignature)) {
        return INACTIVE_ADDRESS;
    }

    // Switch on the device signature
    switch (FullSignature)
    {
        // The memory is on a module device
        case DEVICE_MODULE :
            DevControl->Type = ACTIVE_MODULE;
            return DEVICE_MODULE;
        break;

        // The memory is on a adapter board
        case ADAPTER_BOARD :
            DevControl->Type = ACTIVE_ADDAPTER;
            return ADAPTER_BOARD;
        break;

        default :
            DevControl->Type = UNKNOWN_DEVICE;
            return UNKNOWN_TYPE;
    }
}

//! Return a single char from the eeprom section
/*! @param *DevControl Structure of the device to get infomation from
    @param NewContin 1 - Start a new read 0 - Read from last point
    \return Single char from the barcode section or Null termination
*/
char ModuleBoardId(OWDeviceInfo *DevControl,int NewContin)
{
    static int LastPoint;
    char       SingleVal;

    // Reset the pointer if a new read is requested
    if (NewContin)
    {
        LastPoint = EEP_BOARD_ID;
        ReadSignature(DevControl);
                
        // Check that the board type is correct to get a barcode
        #if REMOVE_ALL_CRC_CHECKS
        if ((DevControl->Type != ACTIVE_MODULE ) &&
           (DevControl->Type != UNKNOWN_DEVICE))
        #else
        if (DEVICE_MODULE != ReadSignature(DevControl))
        #endif
        {
            LastPoint = 65535;
            return '\0';
        }
    }

    if ((unsigned int)LastPoint == 65535) {
        return '\0';
    }
    
    System1WireReadByte(DevControl->Address,LastPoint++,(unsigned char*)&SingleVal);
    return SingleVal;
}

//! Return a single char from the eeprom at requested address
/*! @param *DevControl Structure of the device to get infomation from
    @param Address of the byte to read
    \return Single char from the barcode section or Null termination
*/
char ModuleBoardByteRead(OWDeviceInfo *DevControl,int Address)
{
    char SingleVal;
    System1WireReadByte(DevControl->Address,Address,(unsigned char*)&SingleVal);
    //! \todo Check the return value before sending data back
    return SingleVal;
}

//! Write a single char to the eeprom at the requested address
/*! @param *DevControl Structure of the device to get infomation from
    @param Address of the byte to write
    \return Single char from the barcode section or Null termination
*/
char ModuleBoardByteWrite(OWDeviceInfo *DevControl,int Address, unsigned char Value)
{
    int SetOK;
    SetOK = System1WireWriteByte(DevControl->Address,Address,Value);
    //! \todo Check the return value before sending data back
    return (char)SetOK;
}

//! Look through all the devices on the bus returning the first requested type
/*! @param *DevControl Pointer to pass the found address back to
    @param ModTypeFind The type of module to be located
    \return 0 - No device found, 1 - Device found  2 = No device match found but a device
*/
int ModuleBoardLocate(OWDeviceInfo *DevControl, OWchipTypes ModTypeFind)
{
    uint8_t *AddressInfo;
    int SingleLoop;

    if (Detect_Slave_Device() == 1)
    {
        // Get the first device
        if(OWFirst())
        {   
            AddressInfo = GetAddresLoc();
            if ((*AddressInfo == TYPE_ID_MEMORY_1) ||
                (*AddressInfo == TYPE_ID_MEMORY_2))
            {
                for(SingleLoop = 0; SingleLoop < 8; SingleLoop++) {
                    DevControl->Address[SingleLoop] = *AddressInfo++;
                }
                ReadSignature(DevControl);   // Get the ID signature
                        
                // Check the signature for the module type
                #if REMOVE_ALL_CRC_CHECKS
                if ((DevControl->Type == ModTypeFind) ||
                    (DevControl->Type == UNKNOWN_DEVICE))
                #else
                if (DevControl->Type == ModTypeFind)
                #endif
                {
                    return 1;  // Device is a valid unit
                }
            }

            // Loop for the rest of the found devices
            while(OWNext())
            {
                AddressInfo = GetAddresLoc();
                if ((*AddressInfo == TYPE_ID_MEMORY_1) ||
                   (*AddressInfo == TYPE_ID_MEMORY_2))
                {
                    for(SingleLoop = 0; SingleLoop < 8; SingleLoop++)
                    {
                        DevControl->Address[SingleLoop] = *AddressInfo++;
                    }
                    ReadSignature(DevControl);   // Get the ID signature
            
                    // Check the signature for a module type
                    #if REMOVE_ALL_CRC_CHECKS
                    if ((DevControl->Type == ModTypeFind) ||
                        (DevControl->Type == UNKNOWN_DEVICE))
                    #else
                    if (DevControl->Type == ModTypeFind)
                    #endif
                    {
                        return 1; // The device is a valid unit
                    }
                }
            }
        }
        // If we get here devices have been found but nothing that matches our requirements
        return 2;  
    }
    return 0;   // No devices are on bus
}


/* BarCode */


//! Return a single char from the barcode section
/*! @param *DevControl Structure of the device to get infomation from
    @param NewContin 1 - Start a new read 0 - Read from last point
    \return Single char from the barcode section or Null termination
*/
char ModuleBarCodeGet(OWDeviceInfo *DevControl,int NewContin)
{
    static int LastPoint;
    char       SingleVal;

    // Reset the pointer if a new read is requested
    if (NewContin)
    {
        LastPoint = EEP_BARCODE;

        // Check that the board type is correct to get a barcode
        ReadSignature(DevControl);
        
        #if REMOVE_ALL_CRC_CHECKS
        if ((DevControl->Type != ACTIVE_MODULE ) &&
            (DevControl->Type != UNKNOWN_DEVICE))
        #else
        if (DEVICE_MODULE != ReadSignature(DevControl))
        #endif
        {
            return '\0';
        }
    }

    System1WireReadByte(DevControl->Address,LastPoint++,(unsigned char*)&SingleVal);
    //! \todo Check the return value before sending data back
    return SingleVal;
}

//! Set a single char from the barcode section
/*! @param *DevControl Structure of the device to get infomation from
    @param NewContin 1 - Start a new read 0 - Read from last point
    @param SetVal Value to set the barcode point to
    \return Single char from the barcode section or Null termination
*/
char ModuleBarCodeSet(OWDeviceInfo *DevControl,int NewContin, char SetVal)
{
    static int LastPoint;
    char       SingleVal;
    int        SetOK;

    // If this is a new count then find the end and start from there
    // If this is the same then the location should be correct
    
    // If this is a new pass value find the end of the barcode data
    if (NewContin)
    {
        LastPoint = EEP_BARCODE;

        ReadSignature(DevControl);
        
        // Check that the board type is correct to get a barcode
        #if REMOVE_ALL_CRC_CHECKS
        if ((DevControl->Type != ACTIVE_MODULE) &&
           (DevControl->Type != UNKNOWN_DEVICE))
        #else
        if (DEVICE_MODULE != ReadSignature(DevControl))
        #endif
        {
            return 0;
        }
        
        // Loop until the end of the barcode info is found
        do
        {
            System1WireReadByte(DevControl->Address,LastPoint++,
                    (unsigned char*)&SingleVal);
        }
        while (SingleVal != '\0');

        // Move back to the null point
        LastPoint--;
    }
    
    // From here we place the new data This must not exceed the 95 bytes permitted
    SetOK = System1WireWriteByte(DevControl->Address,LastPoint++,SetVal);
    return SetOK;
}

//! Remove the gap of chars from the area shifting all left
/*! @param *DevControl Structure of the device to get infomation from
    @param FirstPosChar Position of the first char in the barcode data to remove
    @param CharCount The number of chars to remove from the string
    \return Ok or not ok
*/
int ModuleBarCodeRemove(OWDeviceInfo *DevControl, int FirstPosChar, int CharCount)
{
    int StartClear, ClearTrack;
    char SingleVal;

    // Setup point values ready to clear the data
    StartClear = EEP_BARCODE + FirstPosChar;
    ClearTrack = StartClear + CharCount;
    
    // shift up until the end has been found
    do
    {
        System1WireReadByte(DevControl->Address,ClearTrack++,
                (unsigned char*)&SingleVal);
        
        System1WireWriteByte(DevControl->Address,StartClear++,SingleVal);
        // TODO  If a write fails then stop doing any more or try again.
    }
    while (SingleVal != '\0');

    // Fill the left space with nulls
    while (BARCODE_MAX > StartClear)
    {
        System1WireWriteByte(DevControl->Address,StartClear++,SingleVal);
    }
    return 0;
}

/* API */

//! Get the requested value from bar code data.
/*! @param SectionID are from the delimiter values 
    @param *Data is where the single string parameter is returned
    @param ignoreError 1 = ignore the errors 0 = return errors
    \return Pass or fail information about the action
*/
int GetValue(char SectionID, char *StringData, int ignoreError)
{
    char SingleValue;
    char StartRecord = 0;
    OWDeviceInfo ActiveDevice;
	
    int perr = PACKET_OK;

    // Reset the first char to help prevent miss read of old data
    *StringData = '\0';

    // Locate the correct module
    if ((ModuleBoardLocate(&ActiveDevice,ACTIVE_MODULE) != 1) &&
        (!ignoreError))
    {
        return PACKET_ERROR;
    }
    
    #if REMOVE_ALL_CRC_CHECKS == 0
    if ((!ignoreError) &&
        (checkCRC(&ActiveDevice,SECTION_2_START,SECTION_2_END,SECTION_2_CRC)))
    {
        return PACKET_ERROR;
    }
    #endif
    
    //Get the first value here
    SingleValue = ModuleBarCodeGet(&ActiveDevice,1);
  
    do
    {   
        // Stop recording data once the next parameter is found.
        if (((SingleValue == MODULE_TYPE) || (SingleValue == MODULE_SERIAL) ||
             (SingleValue == STOCK_CODE) ||  (SingleValue == WORK_ORDER) ||
             (SingleValue == MANUFACTURER) ||  (SingleValue == CHIP_ID) ||
             (SingleValue == PROCESS) ||  (SingleValue == TERMINATOR)   ||
             (SingleValue == CR_VALUE) || (SingleValue == LF_VALUE)) &&
             (StartRecord))
        {
            break;
        }
		
        // Take a copy of the requested section
        if(StartRecord)
        {
            *StringData = SingleValue;
            StringData++;
        }

        // Start recording once the section is found
        if (SingleValue == SectionID)
        {
            perr = PACKET_OK;
            StartRecord = 1;
        }
        
        //Get another value
        SingleValue = ModuleBarCodeGet(&ActiveDevice,0);
    }
    while(SingleValue != '\0');
    
    // If the string terminated before the end add the null
    *StringData = '\0';
    return perr;
}

//  Set the requested value to bar code data.
/*  @param SectionID is one of the delimited values
    @param *StringData single parameter is passed as a string
    \return Pass or fail information about the action
*/
int SetValue(char SectionID, char *StringData, int ignoreError) {
    OWDeviceInfo ActiveDevice;
 
    // Locate the device module information
    if ((ModuleBoardLocate(&ActiveDevice,ACTIVE_MODULE) != 1)  &&
        (!ignoreError)) {
        return NO_VALID_DEVICE;   // Fail to find a valid device
    }

    #if REMOVE_ALL_CRC_CHECKS == 0
    if (checkCRC(&ActiveDevice,SECTION_2_START,SECTION_2_END,SECTION_2_CRC))
    {
        return PACKET_ERROR;
    }
    #endif

    // Clear the value from memory that is to be set
    ClearValue(SectionID);

    // Fail if no bar code data is found
    if (!ModuleBarCodeSet(&ActiveDevice,1,SectionID)) {
        return FAILED_TO_SET_DATA;  // Failed to set data
    }
	
    // Loop until a null is located
    for( ;(*StringData != '\0') && 
          (*StringData != '\n') &&
          (*StringData != '\r'); StringData++ ) {
        // Place and check each char in turn
        if (!ModuleBarCodeSet(&ActiveDevice,0,*StringData)) {
            return FAILED_TO_SET_DATA; // Failed to set data
        }
    }

    // Set the CRC for the new data placed into the device
    setCRC(&ActiveDevice,SECTION_2_START,SECTION_2_END,SECTION_2_CRC);
    return 1;  // All went OK
}

//  clear the requested value to bar code data.
/*  @param SectionID is one of the delimited values
    @param *StringData single parameter is passed as a string
    \return Pass or fail information about the action
*/
int ClearValue(char SectionID)
{
    OWDeviceInfo ActiveDevice;
    char SingleValue;
    int ClearStart= 0, ClearCount, FullCount;
    int StartRecord = 1;
        
    if (ModuleBoardLocate(&ActiveDevice,ACTIVE_MODULE) != 1) {
        return PACKET_ERROR;
    }

    // Fail if not the correct board type
    if (!(SingleValue = ModuleBarCodeGet(&ActiveDevice,1))) {
        return NO_BOARD_TYPE_FOUND;  // Value not found
    }

    FullCount = 0;  // Set to the first value in the bar code
    ClearCount = 0;  // Set the number of values to be cleared

    while(SingleValue != '\0')
    {
        // Stop recording data once the next parameter is found
        // only if already recording
        if (((SingleValue == MODULE_TYPE) ||
             (SingleValue == MODULE_SERIAL) ||
             (SingleValue == STOCK_CODE) ||
             (SingleValue == WORK_ORDER) ||
             (SingleValue == MANUFACTURER) ||
             (SingleValue == CHIP_ID) ||
             (SingleValue == PROCESS) ||
             (SingleValue == TERMINATOR) ||
             (SingleValue == CR_VALUE)) &&
             (!StartRecord)) {

            // Get the number of chars to remove
            ClearCount = FullCount - ClearStart;
            
            // Call to remove the bar code values
            // Only remove if a value has been found
            if ((!StartRecord) &&
                (!ModuleBarCodeRemove(&ActiveDevice,ClearStart,ClearCount))) {
                
                setCRC(&ActiveDevice,SECTION_2_START,
                        SECTION_2_END,SECTION_2_CRC);
            }
            break;
        }
		
        // Log the position once the selection is found
        if (SingleValue == SectionID)
        {
            StartRecord = 0;
            ClearStart = FullCount;  // Log the start of the data
        }

        // Move to the next section here
        SingleValue = ModuleBarCodeGet(&ActiveDevice,0);
        FullCount++;
    }

    return 1;
}

//  Take the data between the two points and check the CRC value
/*  @param start First byte to start the check
    @param end Last byte to check in the list
    @param crcPoint Location the CRC is placed
    \return Pass or fail information about the action
*/
int checkCRC(OWDeviceInfo *ActiveDevice, unsigned int start,
             unsigned int end, unsigned int crcPoint)
{
    int loop;

    crc8_bar = 0x00;   // Reset the CRC value
    
    // Loop on all the requested data
    for (loop = start; loop <= end; loop++)
    {
       do_crc8(ModuleBoardByteRead(ActiveDevice,loop));
    }

    // Add in the CRC value as well to get a 0 on valid data
    do_crc8(ModuleBoardByteRead(ActiveDevice,crcPoint));

    return crc8_bar;
}

//  Take the data between the two points and check the CRC value
/*  @param start First byte to start the check
    @param end Last byte to check in the list
    @param crcPoint Location the CRC is placed
    \return Pass or fail information about the action
*/
int setCRC(OWDeviceInfo *ActiveDevice, unsigned int start,
           unsigned int end, unsigned int crcPoint)
{
    int loop;

    crc8_bar = 0x00;   // Reset the CRC value

    // Loop on all the requested data
    for (loop = start; loop <= end; loop++)
    {
       do_crc8(ModuleBoardByteRead(ActiveDevice,loop));
    }

    // Write the CRC value to the correct memory location
    do_crc8(ModuleBoardByteWrite(ActiveDevice,crcPoint,crc8_bar));

    return crc8_bar;
}

//! 1 byte CRC check
/*! @param value Value to add to the byte CRC
    \return last calculated CRC
*/
unsigned char do_crc8(unsigned char value)
{
    extern const unsigned char dscrc_table[];
    
    crc8_bar = dscrc_table[crc8_bar ^ value];
    return crc8_bar;
}

/*******************************************************************************
 End of File
 */
