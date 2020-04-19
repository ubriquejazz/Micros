#ifndef _PIN_COMMON_H
#define _PIN_COMMON_H

/*!\name      pin_common.h
 *
 * \brief     API for PIN controlling 
 *
 * \author    Juan Gago
 *
 */ 

// Result of a PIN function
#define PIN_FAILURE     -1
#define PIN_SUCCESS     +1

// See test_common.h
typedef enum
{
    TEST_NONE=0,
    TEST_INPUT_LOW,     // Release; PORTx == 0?
    TEST_INPUT_HIGH,    // Release; PORTx == 1?
    TEST_DRIVE_LOW,     // LATx = 0; PORTx == 0? 
    TEST_DRIVE_HIGH,    // LATx = 1; PORTx == 1?
    TEST_PULL_LOW,      // LATx = 1; Release; PORTx == falling edge?
    TEST_PULL_HIGH,     // LATx = 0; Release; PORTx == rising edge?
} PIN_TEST;
        
// Initial setup of the port after test
typedef enum
{
    PIN_INPUT=0,        // TRISx = 1   
    PIN_OUTPUT_HIGH,    // TRISx = 0, LATx = 1
    PIN_OUTPUT_LOW,     // TRISx = 0, LATx = 0
    PIN_IGNORE,
} PIN_DEFAULT;

// Setup of CNPUEx register (pull-pus) for the test
typedef enum
{
    PULL_HIGH=0,
    PULL_LOW,
    PULL_OD,            // Open-drain (ODCx = 1)
    PULL_NONE,
} PIN_PULL;

// PIN Object
typedef struct
{
    PORTS_CHANNEL   channel;
    PORTS_BIT_POS   bitPos;
    PIN_PULL		pinPull;
    PIN_TEST        pinTest;
    PIN_DEFAULT     pinDefault;
    int             optional;
} PIN_DEF;

PIN_DEF* pin_get (int);
int pin_pull(PIN_DEF);
int pin_default(PIN_DEF);
int pin_release (PIN_DEF);
int pin_read (PIN_DEF, uint8_t*);
int pin_drive (PIN_DEF, uint8_t);

/* Functions by index */
int pin_release_i (int);        // high impedance
int pin_read_i (int, uint8_t*); // input
int pin_drive_i (int, uint8_t); // output (low or high)

/* Driver API */
int pin_init(const PIN_DEF*);   // pull, test and default

#endif /* _PIN_COMMON_H */

// *****************************************************************************
// End of File