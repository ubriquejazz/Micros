#include "test_common.h"
#include "pin_common.h"

bool TestError[PIN_COUNT];

/*! \fn         test_init () 
 *  
 *  \brief      initialize the error array
 * 
 *  \return     SUCCESS if some conditions
 */
int test_init() 
{
    int i;
    for (i=0; i<PIN_COUNT; i++)
        TestError[i] = false;
    return PIN_SUCCESS;
}

/*! \fn         test_run (index) 
 *  
 *  \brief      Run the test specified for the pin (PIN_TEST)  
 *  
 *  \param      index       index in the array PinDefine[]
 * 
 *  \return     SUCCESS if some conditions
 */
int test_run (int index)
{    
    int retVal = PIN_SUCCESS;
    PIN_DEF* pinPtr = pin_get(index);
    switch(pinPtr->pinTest)
    {
        case TEST_INPUT_LOW:
            retVal = test_inputLow (index);
            break;
        case TEST_INPUT_HIGH:
            retVal = test_inputHigh (index);
            break;
         case TEST_DRIVE_LOW:
            retVal = test_driveLow (index);
            break;
        case TEST_DRIVE_HIGH:
            retVal = test_driveHigh (index);
            break;
        case TEST_PULL_LOW:
            retVal = test_pullLow (index);
            break;
        case TEST_PULL_HIGH:
            retVal = test_pullHigh (index);
            break;
        case TEST_NONE:
            break;
        default:
            retVal = PIN_FAILURE;
            break;        
    }                  
    return retVal;
}
 
// *****************************************************************************
// End of File
