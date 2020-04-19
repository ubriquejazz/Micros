#include "pin_common.h"
#include "test_common.h"

// Private variables
static PIN_DEF PinDefine[PIN_COUNT]; // Copy of the static array

// <editor-fold defaultstate="collapsed" desc="IO Functions">

/*! \fn         pin_get(index)
 * 
 *  \brief      Returns the led indicated by the index
 * 
 *  \param      index       index in the array LedState[]
 * 
 *  \return     Pointer to the specified led
 */
PIN_DEF* pin_get (int index) 
{
    return &PinDefine[index];
}

/*! \fn         pin_read (index, state)
 * 
 *  \brief      Use PORTx register
 * 
 *  \param      index       index in the array PinDefine[]
 *  \param      state       status of the led (pointer)
 * 
 *  \return     SUCCESS if some conditions
 */
int pin_read (PIN_DEF pin, uint8_t* state) 
{
    if ( state != NULL ) {
        *state = SYS_PORTS_PinRead(PORTS_ID_0, pin.channel, pin.bitPos);
        return PIN_SUCCESS;
    }
    else
        return PIN_FAILURE;
}

int pin_read_i (int index, uint8_t* state) 
{
    if (index < PIN_COUNT) {
        PIN_DEF pin = PinDefine[index];
        return pin_read (pin, state);  
    }
    else
        return PIN_FAILURE;
}

/*! \fn         pin_drive (index, value) 
 *  
 *  \brief      Use LATx, TRISx registers
 *  
 *  \param      index       index in the array PinDefine[]
 *  \param      value		{0,1}
 * 
 *  \return     SUCCESS
 */
int pin_drive (PIN_DEF pin, uint8_t value)
{
	(value)?
		SYS_PORTS_PinSet(PORTS_ID_0, pin.channel, pin.bitPos):
		SYS_PORTS_PinClear(PORTS_ID_0, pin.channel, pin.bitPos);
	// setup the direction
    SYS_PORTS_PinDirectionSelect (PORTS_ID_0, SYS_PORTS_DIRECTION_OUTPUT,
            pin.channel, pin.bitPos);
    return PIN_SUCCESS;    
}

int pin_drive_i (int index, uint8_t value)
{
    if (index < PIN_COUNT) {
        PIN_DEF pin = PinDefine[index];
        return pin_drive (pin, value);  
    }
    else
        return PIN_FAILURE;
}

/*! \fn         pin_release (index)
 * 
 *  \brief      Use TRISx register
 * 
 *  \param      index       index in the array PinDefine[]
 * 
 *  \return     SUCCESS
 */
int pin_release (PIN_DEF pin)
{
    SYS_PORTS_PinDirectionSelect (PORTS_ID_0, SYS_PORTS_DIRECTION_INPUT,
        pin.channel, pin.bitPos);
    return PIN_SUCCESS;
}

int pin_release_i (int index)
{   
    if (index < PIN_COUNT) {
        PIN_DEF pin = PinDefine[index];
        return pin_release (pin);  
    }
    else
        return PIN_FAILURE;
}

// </editor-fold>
 
/*! \fn         pin_pull (index) 
 *  
 *  \brief      Use CNPUE and ODCB registers
 * 
 *  \param      index       index in the static PIN_DEF[] array
 * 
 *  \return     SUCCESS if some conditions
 */
int pin_pull (PIN_DEF pin)
{    
    int retVal = PIN_SUCCESS;
    switch(pin.pinPull)
    {
        case PULL_OD:
            // Port -> PLIB_PORTS_OpenDrainEnable(PORTS_ID_0, PORT_CHANNEL_A, ...);
            PLIB_PORTS_PinOpenDrainEnable (PORTS_ID_0, 
                    pin.channel, pin.bitPos);
            break;
            
        default:
            retVal = PIN_FAILURE;
            break;
    }          
    return retVal;
}

/*! \fn         pin_default (pin) 
 *  
 *  \brief      Configure the default state (PIN_DEFAULT)
 *  
 *  \param      index       index in the array PinDefine[]
 * 
 *  \return     SUCCESS if some conditions
 */
 int pin_default (PIN_DEF pin)
{    
    int retVal = PIN_SUCCESS;
    switch(pin.pinDefault)
    {            
        case PIN_INPUT:
            retVal = pin_release (pin);
            break;
        case PIN_OUTPUT_HIGH:
            retVal = pin_drive (pin, 1);
            break;
        case PIN_OUTPUT_LOW:
            retVal = pin_drive (pin, 0);
            break;           
        case PIN_IGNORE:
            break;            
        default:
            retVal = PIN_FAILURE;
            break;        
    }                  
    return retVal;
}

/*! \fn         led_int(PinDefineFlash)
 * 
 *  \brief      Initilize all PIN's after running a simple test
 *
 *	\param		Static array pointer
 * 
 *  \return     SUCCESS if the test passed
 */
int pin_init (const PIN_DEF* pin)
{
    int retVal = 0;
    int i = 0;
    test_init();                            // initialize the error array
    for (i=0; i<PIN_COUNT; i++) {
        PinDefine[i] = pin[i];
        retVal += pin_pull(pin[i]);         // first, pull ups
        retVal += test_run(i);              // run the test
        retVal += pin_default (pin[i]);     // and configure the pin
    }  
    if (retVal < 3) {
        retVal = PIN_FAILURE; // BP to watch the global TestError[]
    }
    else {
        retVal = PIN_SUCCESS;
    }
    return retVal;
}

// *****************************************************************************
// End of File
//