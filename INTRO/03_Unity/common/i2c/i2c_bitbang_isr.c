/*!\name      i2c_bitbang_isr.c
 *
 * \brief       This is a sample project demonstrating an I2C Master
 *              device simulated in software.  The project consists of an 
 *              initialization function, functions to perform I2C bus operations  
 *              and an Interrupt Service Routine (ISR) that contains the I2C Master 
 *              state machine.  The ISR is responsible for performing the START, 
 *              STOP, RESTART, WRITE byte and READ byte operation.              
 *              This project uses the IO pins of I2C-2 peripheral. If another
 *              I2C module, other that I2C2 is required to be used, this can be
 *              done so by modifying the port definition is i2c_bitbang_isr.h.
 * 
 *              This is developed in accordance with In accordance with the 
 *              I2C Bus Specification and User Manual Revision 6, April 2014.
 * 
 *              The i2c_bitbang_isr.c was tested with SYSCLK running at 200 MHz
 *              and PBCLK 3 that the timer is based on running at 100 MHz 
 *
 * \author    Juan Gago
 *
 */

#include "i2c_bitbang_isr.h"

void StartI2C(void);
bool i2cBusIdle(void);
void StartI2CBusClear(void);

volatile uint16_t  I2CSWCounter;
volatile uint16_t  I2CSWData;
volatile bool      I2CNACKOut     = false;
volatile bool     s I2CACKStatus   = false;

uint16_t  I2CReadData;
volatile uint16_t i2cBaudTime;
volatile uint32_t errorTimeOut = ERROR_TIMEOUT_COUNT;

uint16_t i2cDeviceaddress;

uint8_t* i2cDevicetxBuffer;
uint16_t i2cDevicetxlen;

uint8_t* i2cDevicerxBuffer;
uint16_t i2cDevicerxlen;
bool _i2c_bit_written;

volatile BUS_STATE              i2cState        = 0;
volatile I2C_OPERATTION_TYPE    i2cOperation    = I2C_NONE;
volatile I2C_OPERATION_STATUS   i2cStatus       = I2C_OPERATION_IN_PROGRESS;
volatile uint8_t data   = 0;

bool ACKSTATUS_M                      = false;

volatile uint8_t isrcounter;

/* Function:
    I2C_Initialize (uint32_t baudrate)

  Input: I2C baud rate

  Summary:
    Initializes the TRI, LAT and Open drain enable value for the I2C pins

  Description:
    Initializes the TRI, LAT and Open drain enable value for the I2C pins
*/

void I2C_Initialize ( uint32_t baudrate, bool masterISREnable, bool busISREnable )
{
    I2C2CONbits.ON = 0;
    I2C_PINS = I2C_SCL_SDA_OUTPUT_LOW;   // Make RA2, RA3 latch values zero, retain all others.
    SCLDir  = INPUT;                    // Make 
    SDADir  = INPUT;
    SCL_OPEN_DRAIN  =  INPUT;
    SDA_OPEN_DRAIN  =  INPUT;
    
    I2C_PINS = I2C_SCL_SDA_OUTPUT_LOW;   // Make RA2, RA3 latch values zero, retain all others.
   
    i2cBaudTime = PBCLK2_MHZ/(baudrate<<2);
        
    T9CONbits.TCKPS     = 0;            //1:1 pre-scalar
    T9CONbits.TCS       = 0;            //use internal peripheral clock
    
    PR9                 = i2cBaudTime;  //load 1 BRG time into I2C period
    
    i2cStatus           = I2C_STOP_COMPLETE;
    
    _Timer_ISR_EventClear();            //clear Timer ISR flag
    _Timer_ISR_Enable();                //enable Timer ISR
    IPC10bits.T9IP      = 6;            //set Timer ISR priority to 6
    
    if (masterISREnable)
    {
        IFS4bits.I2C2MIF    = 0;
        IEC4bits.I2C2MIE    = 1;
        IPC37bits.I2C2MIP   = 5;
        IPC37bits.I2C2SIS   = 3;
    }
    
    if (busISREnable)
    {
        IFS4bits.I2C2BIF    = 0;
        IEC4bits.I2C2BIE    = 1;
        IPC37bits.I2C2BIP   = 5;
        IPC37bits.I2C2BIS   = 2;
    }
}

I2C_OPERATION_STATUS DRV_I2C_MasterBufferWrite(uint16_t deviceaddress, uint8_t* txBuffer, uint16_t txbuflen)
{
    if (i2cStatus == I2C_STOP_COMPLETE)
    {
        i2cStatus = I2C_OPERATION_IN_PROGRESS;

        i2cDeviceaddress    = deviceaddress;
        i2cDevicetxBuffer   = txBuffer;
        i2cDevicetxlen      = txbuflen;
        i2cOperation        = I2C_WRITE_BYTE;                     
        I2CSWCounter        = 9;

        StartI2C();
        
        return I2C_OPERATION_IN_PROGRESS;
    }
    else
        return I2C_OPERATION_ERROR;
}

I2C_OPERATION_STATUS DRV_I2C_MasterBufferRead(uint16_t deviceaddress, uint8_t* rxBuffer, uint16_t rxbuflen)
{
    if (i2cStatus == I2C_STOP_COMPLETE)
    {
    
        i2cStatus = I2C_OPERATION_IN_PROGRESS;
    
        i2cDeviceaddress    = (deviceaddress|0x01);
        i2cDevicerxBuffer   = rxBuffer;
        i2cDevicerxlen      = (rxbuflen-1);
        i2cOperation        = I2C_ADDRESS_BYTE_WITH_RW1;                     
        I2CSWCounter        = 9;
    
        StartI2C();
        
        return I2C_OPERATION_IN_PROGRESS;
    }
    else 
        return I2C_OPERATION_ERROR;
}

I2C_OPERATION_STATUS DRV_I2C_MasterBufferWriteRead(uint16_t deviceaddress, uint8_t* txBuffer, uint16_t txbuflen, uint8_t* rxBuffer, uint16_t rxbuflen)
{
    if (i2cStatus == I2C_STOP_COMPLETE)
    {
    
        i2cStatus = I2C_OPERATION_IN_PROGRESS;
    
        i2cDeviceaddress    = (deviceaddress);
        i2cDevicetxBuffer   = txBuffer;
        i2cDevicetxlen      = txbuflen;
        i2cDevicerxBuffer   = rxBuffer;
        i2cDevicerxlen      = (rxbuflen-1);
        i2cOperation        = I2C_WRITE_BYTE;                     
        I2CSWCounter        = 9;
    
        StartI2C();
        
        return I2C_OPERATION_IN_PROGRESS;
    }
    else 
        return I2C_OPERATION_ERROR;
}

I2C_OPERATION_STATUS DRV_I2C_MASTEROpStatus(void)
{
    return i2cStatus;
}

bool DRV_I2C_MasterACKStatus(void)
{
    return I2CACKStatus;
}

/* Function:
    bool i2cBusIdle(void)
 
  Output: returns state of bus

  Summary:
    Checks state of bus

  Description:
    Checks if SCL and SDA are high. If both SDA and SCL is high then returns 
    TRUE else returns FALSE
*/

bool i2cBusIdle(void)
{
    if ((SCL_INPUT == 1) && (SDA_INPUT == 1))
        return true;

    return false;
}


/* Function:
    void StartI2C(void)

  Summary:
    Generates START condition on I2C bus

  Description:
    Initiate a START condition on I2C bus. 

  Remarks:
    Check for BUS IDLE before setting a START condition
*/

void StartI2C(void)
{
    I2C_PINS       = I2C_SCL_SDA_OUTPUT_LOW;   // Make RA2, RA3 latch values zero, retain all others.
    if (i2cBusIdle())
    {            
        i2cState = I2C_SDA_LOW_START;
        _Timer_Count_Clear();
        _Timer_Start();
    }      
}

void StartI2CBusClear(void)
{
    I2C_PINS = I2C_SCL_SDA_OUTPUT_LOW;   // Make RA2, RA3 latch values zero, retain all others.        
    i2cStatus = I2C_OPERATION_IN_PROGRESS;

    i2cDeviceaddress    = 0;
    i2cDevicetxBuffer   = NULL;
    i2cDevicetxlen      = 0;
    i2cDevicerxBuffer   = NULL;
    i2cDevicerxlen      = 0;
    i2cOperation        = I2C_WRITE_BYTE;                     
    I2CSWCounter        = 9;
    i2cState            = I2C_SCL_LOW_DATA_CHECK;
    I2CSWData           = 0;
    _Timer_Count_Clear();
    _Timer_Start();
}


/* Function:
    bool IsByteAcked(void)
 
  Output: status of ACK or ACK after sending a byte

  Summary:
    Checks the status of ACK or NACK of a byte

  Description:
    Checks the status of ACK or NACK and return true if ACK; false if NACK
*/

inline bool IsByteAcked(void)
{
    if (I2CACKStatus == 0)
        return true;     
    else              
        return false;    
}

/*Timer interrupt ISR */ 
void __attribute__ ((nomips16,no_fpu)) __attribute__( (interrupt(ipl6SRS), vector(_TIMER_9_VECTOR))) Timer9Vector(void);
                                                    
void __attribute__ ((nomips16,no_fpu)) Timer9Vector(void)
{            
    _Timer_ISR_EventClear();
    switch (i2cState)
    {        
        case I2C_SDA_LOW_START:                     //1/2 BRG time has expired after SDA and SCL high
            SDADir  = OUTPUT;                       //pull SDA low
            i2cState = I2C_SDA_LOW_START_CHECK;     //set FSM to transition to set SCL LOW
            break;
        case I2C_SDA_LOW_START_CHECK:               //1/2 BRG time has expired after SCL is low
            i2cState = I2C_SCL_LOW_START;           //set FSM to transition to set SCL_LOW
            break;
        case I2C_SCL_LOW_START:                     //1/2 BRG time has expired after I2C_SDA_LOW_START 
            SCLDir   = OUTPUT;                      //set SCL to low
            i2cState =  I2C_SCL_LOW_START_CHECK;    //set FSM to check on SCL_LOW
            break;
        case I2C_SCL_LOW_START_CHECK:
            if ( (i2cDeviceaddress > 0) ) {
                I2CSWData       = i2cDeviceaddress;
                i2cState        = I2C_SCL_LOW_DATA_CHECK;
            }
            else {
                _Timer_Stop();                          //stop and clear timer
                _Timer_Count_Clear();
//                i2cStatus       = I2C_START_COMPLETE;   //flag that START is completed
            }
            break;
        case I2C_SDA_HIGH_RESTART:                        //1 BRG time has expired after sending data byte
            SDADir      = INPUT;                          //set SDA to high    
            i2cState    = I2C_SDA_HIGH_RESTART_CHECK;     //set FSM to set SCL high
            break;
        case I2C_SDA_HIGH_RESTART_CHECK:
            i2cState    = I2C_SCL_HIGH_RESTART;
            break;
        case I2C_SCL_HIGH_RESTART:                          //after 1 BRG time has expired after I2C_SDA_HIGH_RESTART
            SCLDir      = INPUT;                            //set SCL high
            i2cState    = I2C_SCL_HIGH_RESTART_CHECK;       //set FSM to do procedure for normal START
            break;
        case I2C_SCL_HIGH_RESTART_CHECK:
            i2cState = I2C_SDA_LOW_START;
            break;
        case I2C_SCL_LOW_DATA_CHECK:                //set LOW DATA CHECK
            SCLDir  = OUTPUT;                       //SCL pulled low immediately allowing Slave to change data here on read
            i2cState = I2C_SCL_LOW_DATA;                    
            break;
        case I2C_SCL_LOW_DATA:                      //state of low period of clock during data transfer
            if (I2CSWCounter > 1)                   //check until the low slow side of 7th clock and before rising edge of 8th clock
            {
                SDADir = (bool) (I2CSWData & 0x80);
            }
            else if (I2CSWCounter == 1)
            {
                _i2c_bit_written = 0;               //on the 9th clock clear the value of bit written
                if (i2cOperation == I2C_READ_BYTE)
                {
                    if (I2CNACKOut == 0x00)         //set SDA to high allow slave to ACK or NACK
                        SDADir = OUTPUT;            //WRITE operation; On low side of 8th clock and before rising edge 9th clock
                }
                else    
                    SDADir  = INPUT;                                                    
            }
            if (I2CSWCounter == 0)                  //if CLK count is 0, ie after all the nine clocks have been sent
            {
                if ( (i2cDevicetxlen) && (I2CACKStatus == M_ACK) && (i2cOperation == I2C_WRITE_BYTE) )
                {
                    I2CSWData   = *i2cDevicetxBuffer++;
                    i2cState    = I2C_SCL_LOW_DATA;
                    I2CSWCounter    = 9;
                    i2cDevicetxlen--;
                }
                else if ( (i2cDevicerxlen) && (i2cOperation == I2C_READ_BYTE) )
                {
                    I2CSWData   = 0xFF;
                    i2cState    = I2C_SCL_LOW_DATA;
                    I2CSWCounter    = 9;
                    i2cDevicerxlen--;
                    if (i2cDevicerxlen == 0)
                        I2CNACKOut = 0x01;
                    else
                        I2CNACKOut = 0;
                }
                else if ( (!i2cDevicetxlen) && (i2cDevicerxlen) && (i2cOperation == I2C_WRITE_BYTE) )
                {                                           //if write is complete and read is still pending
                    i2cState = I2C_SDA_HIGH_RESTART;
                    i2cOperation = I2C_ADDRESS_BYTE_WITH_RW1;
                    i2cDeviceaddress |= 0x01;     
                    I2CSWCounter        = 9;
                }
                else if ( (!i2cDevicetxlen) && (i2cDevicerxlen) && (i2cOperation == I2C_ADDRESS_BYTE_WITH_RW1) )
                {                                           //if write is complete and read is still pending
                    I2CSWData   = 0xFF;
                    i2cOperation = I2C_READ_BYTE;
                    i2cState    = I2C_SCL_LOW_DATA;
                    I2CSWCounter    = 9;
                    I2CNACKOut = 0;
                }
                else
                    i2cState    = I2C_SCL_SDA_LOW_STOP;
            }
            else
            {
                i2cState = I2C_SCL_HIGH_DATA;       //if clock count is not 0
                I2CSWCounter--;                     //decrement clock counter
                I2CSWData <<= 1;                    //shift data byte
            }
            break;    
        case I2C_SCL_HIGH_DATA:                     //1 BRG time has expired after SCL is low          
            if ( (_i2c_bit_written == 1) && (i2cOperation == I2C_WRITE_BYTE) )
            {
                if ((bool)SDA_INPUT == (bool)_i2c_bit_written)
                {
                    SCLDir  = INPUT;                    //set SCL high
                    i2cState = I2C_SCL_HIGH_DATA_CHECK; //set FSM to generate low side of clock 
                }
                else
                {
                    _Timer_Stop();                      //stop and clear Timer
                    _Timer_Count_Clear();
                    errorTimeOut = ERROR_TIMEOUT_COUNT; //reset error counter
                    SCLDir  = INPUT;                    //set SCL high after setting error condition
                }
            }
            else
            {
                SCLDir  = INPUT;                        //set SCL high
                i2cState = I2C_SCL_HIGH_DATA_CHECK;     //set FSM to generate low side of clock 
            }
            break;
        case I2C_SCL_HIGH_DATA_CHECK:               //initial state where SCL is set to LOW
            if (SCL_INPUT == HIGH)
            {
                if ((i2cOperation == I2C_READ_BYTE) && (I2CSWCounter > 0))
                {                                       //READ operation; On high side of clocks from 1 to 8
                    I2CReadData <<= 1;                  //shift previously read data
                    I2CReadData |= SDA_INPUT;           //read SDA to get the read bit                
                }
                if (I2CSWCounter == 0)                  //at SCL high on the 9th clock
                {
                    I2CACKStatus = SDA_INPUT;           //read port value of SDA to check for ACK
                    if (i2cOperation == I2C_READ_BYTE)
                        *i2cDevicerxBuffer++ = I2CReadData;
                }
                i2cState = I2C_SCL_LOW_DATA_CHECK;      //
                errorTimeOut = ERROR_TIMEOUT_COUNT;
            }
            else
            {                                           //if slave has clock stretched wait in the same state
                if  (!(errorTimeOut--))                 //decrement error counter                 
                {
                    i2cStatus = I2C_OPERATION_ERROR;    //i2c error flag
                    _Timer_Stop();                      //stop and clear Timer
                    _Timer_Count_Clear();
                    errorTimeOut = ERROR_TIMEOUT_COUNT; //reset error counter
                }
            }
            break;
        case I2C_SCL_SDA_LOW_STOP:                  
            I2C_PINS = I2C_SCL_SDA_OUTPUT_LOW;      //set I2C LATbits to 0
            SCLDir  = OUTPUT;                       //Keep SCL Low
            SDADir  = OUTPUT;                       //Keep SDA Low
            i2cState =  I2C_SCL_SDA_LOW_STOP_CHECK;
            break;
        case I2C_SCL_SDA_LOW_STOP_CHECK:
            i2cState = I2C_SCL_HIGH_STOP;
            break;
        case I2C_SCL_HIGH_STOP:                     //1 BRG time has expired after SCL and SDA low             
            SCLDir      = INPUT;                    //set SCL high
            i2cState    = I2C_SCL_HIGH_STOP_CHECK;  //set FSM state to generate HIGH SDA
            break;
        case I2C_SCL_HIGH_STOP_CHECK:
            i2cState = I2C_SDA_HIGH_STOP;
            break;
        case I2C_SDA_HIGH_STOP:                     //1 BRG time has expired after I2C_SCL_HIGH_STOP
             SDADir      = INPUT;                   //set SDA to high
             i2cState    = I2C_SDA_HIGH_STOP_CHECK;     
            break;
        case I2C_SDA_HIGH_STOP_CHECK:
            _Timer_Stop();                          //stop and clear Timer
            _Timer_Count_Clear();       
            i2cStatus = I2C_STOP_COMPLETE;          
            I2C_SW_ISR_TRIGGER    = 1;              //generate I2C s/w ISR to alert user of event complete
            break;
        default:
            break;
    }
       
}



