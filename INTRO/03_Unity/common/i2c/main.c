/*!\name      main.h
 *
 * \brief     Test program for performance study of i2c bit banging solution
 *
 * \author    Juan Gago
 *
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <xc.h>
#include "i2c_bitbang_isr.h"
#include "i2c_queue.h"
#include "main.h"

uint8_t txmsg1[] = "\x45""Master";
uint8_t txmsg2[] = "\x45";
uint8_t rxmsg1[10];
uint8_t rxmsg2[10];

volatile uint16_t   MessageQueueIndex   = 0;

bool        QueueAddStatus = false;
uint32_t    qindex = 0;

BUFFER_TRANSFER_QUEUE BTQ1 = {
    .address = RTCC_address, 
    .wrBuf= txmsg1, 
    .rdBuf = NULL, 
    .wrlen = (sizeof(txmsg1)-1), 
    .rdlen = 0} ;
                          
LED_TOGGLE_STATES LED_STATE;

uint8_t readbyte[10];

uint32_t    avgPerformance;
uint32_t    cumulatePerformance;
uint32_t    timer23Reading;
uint32_t    timer45Reading;
uint32_t    timer67Reading;
uint32_t    timer4567Reading;
uint32_t    timer23difference;

/******************************************************************************
 Config Bit Settings
 ******************************************************************************/

/*** DEVCFG0 ***/
#pragma config DEBUG =      ON
#pragma config JTAGEN =     OFF
#pragma config ICESEL =     ICS_PGx2
#pragma config TRCEN =      OFF
#pragma config BOOTISA =    MIPS32
#pragma config FECCCON =    OFF_UNLOCKED
#pragma config FSLEEP =     OFF
#pragma config DBGPER =     ALLOW_PG2
#pragma config SMCLR =      MCLR_NORM
#pragma config SOSCGAIN =   GAIN_2X
#pragma config SOSCBOOST =  ON
#pragma config POSCGAIN =   GAIN_2X
#pragma config POSCBOOST =  ON
#pragma config EJTAGBEN =   NORMAL
#pragma config CP =         OFF

/*** DEVCFG1 ***/
#pragma config FNOSC =      SPLL
#pragma config DMTINTV =    WIN_127_128
#pragma config FSOSCEN =    OFF
#pragma config IESO =       OFF
#pragma config POSCMOD =    EC
#pragma config OSCIOFNC =   OFF
#pragma config FCKSM =      CSDCMD
#pragma config WDTPS =      PS1048576
#pragma config WDTSPGM =    STOP
#pragma config FWDTEN =     OFF
#pragma config WINDIS =     NORMAL
#pragma config FWDTWINSZ =  WINSZ_25
#pragma config DMTCNT =     DMT9
#pragma config FDMTEN =     OFF

/*** DEVCFG2 ***/

#pragma config FPLLIDIV =   DIV_3
#pragma config FPLLRNG =    RANGE_5_10_MHZ
#pragma config FPLLICLK =   PLL_POSC
#pragma config FPLLMULT =   MUL_50
#pragma config FPLLODIV =   DIV_2
#pragma config UPLLFSEL =   FREQ_24MHZ

/*** DEVCFG3 ***/
#pragma config USERID =     0xffff
#pragma config FMIIEN =     OFF
#pragma config FETHIO =     OFF
#pragma config PGL1WAY =    OFF
#pragma config PMDL1WAY =   OFF
#pragma config IOL1WAY =    OFF
#pragma config FUSBIDIO =   OFF

/*** BF1SEQ0 ***/
#pragma config TSEQ =       0xffff
#pragma config CSEQ =       0xffff

// *****************************************************************************
/* Function:
    void SYS_DEVCON_PerformanceConfig( unsigned int sysclk )

  Summary:
    Configures the PFM wait states and prefetch (cache) module for maximum 
    performance.

  Description:
    This function configures the PFM wait states and prefetch (cache) module 
    for maximum performance.

  Remarks:
    None.
*/

void __attribute__((nomips16)) SYS_DEVCON_PerformanceConfig( unsigned int sysclk )
{
    bool ecc;

    /* Is ECC enabled? */
    ecc = (((CFGCON & 0x00000030) >> 4) < 2) ? true : false;

    /* Set the PFM wait states based on the system clock and ECC setting */
    if (sysclk <= (ecc ? 66000000 : 83000000))
        PRECONbits.PFMWS = 0;   // Wait States = 0
    else if (sysclk <= (ecc ? 133000000 : 166000000))
        PRECONbits.PFMWS = 1;   // Wait States = 1
    else
        PRECONbits.PFMWS = 2;   // Wait States = 2

    /* Enable Prefetch Cache Module */
    PRECONbits.PREFEN = 0x3;    // Enable All
}

void initializeTestPorts(void)
{
    /* Test pin - Pin 12 on J2 MEB-2*/
    TRISDbits.TRISD10   = 0;
    LATDbits.LATD10     = 0;
    LATDbits.LATD10     = 1;
    LATDbits.LATD10     = 0;
    Nop();
}

// *****************************************************************************
/* Function:
    void I2C_Initialize ( void )

  Summary:
    Initializes and enables the I2C using the given BRG value.
*/

static inline void ISR_Initialize(void)
{
       
    INTCONSET = 0x00001000;     //Enable Multi Vector Interrupt mod
    
    __builtin_mtc0(12,0,(__builtin_mfc0(12,0) | 0x0001));      //Enable ISR 
}

static inline void LED_On (void)
{
    LATHbits.LATH0      = 1;
}

void APP_INITIALIZE(void)
{
    APP_LED_Initialize();
    initializeTestPorts();           
    LED_On();
       
    if (SDA_INPUT == LOW)
    {        
//        QueueInitialize();
//        APP_I2C_POPULATE_QUEUE_DATA(&BTQ_Message[MessageQueueIndex++], RTCC_address, 0 , NULL, 0, 0);
//        AddNewQueueItem(&BTQ_Message[qindex]);
//        MessageQueueIndex = 0;
//        qindex = 0;
    }
}


void APP_LED_Initialize(void)
{
    ANSELHbits.ANSH0    = 0;
    TRISHbits.TRISH0    = 0;
    LATHbits.LATH0      = 0;
    
    ANSELHbits.ANSH1    = 0;
    TRISHbits.TRISH1    = 0;
    LATHbits.LATH1      = 1;
    
    TRISHbits.TRISH2    = 0;
    LATHbits.LATH2      = 0;
    
    LED_STATE = LED_H0_D3_TOGGLE;
}

// *****************************************************************************
/* Main entry point.
 *
 */
int main(int argc, char** argv)
{
           
    PERFORMANCE_VARS_INITIALIZE();
    
    /* Set wait states and enable pre-fetch. */
    SYS_DEVCON_PerformanceConfig(SYS_CLOCK_MHZ);
    
    PRISS = 0x76543210;  

    ISR_Initialize();
    
    /* Initialize I2C */
    I2C_Initialize(400000, true, true);
    QueueInitialize();
    
    /* initialize application variables */
    APP_INITIALIZE();
    
    StartI2CBusClear();
        
    APP_I2C_POPULATE_QUEUE_DATA(&BTQ_Message[MessageQueueIndex++], RTCC_address, txmsg1 , NULL, (sizeof(txmsg1)-1), 0);
    APP_I2C_POPULATE_QUEUE_DATA(&BTQ_Message[MessageQueueIndex++], RTCC_address, txmsg2, rxmsg1, (sizeof(txmsg2)-1), 6);
    APP_I2C_POPULATE_QUEUE_DATA(&BTQ_Message[MessageQueueIndex++], RTCC_address, NULL , rxmsg2, 0, 6);
    APP_I2C_POPULATE_QUEUE_DATA(&BTQ_Message[MessageQueueIndex++], RTCC_address, txmsg1 , NULL, (sizeof(txmsg1)-1), 0);
                   
    while (1) {        
        APP_LED_STATE();   
        QueueAddStatus = AddNewQueueItem(&BTQ_Message[qindex]);
        if(QueueAddStatus) {
            qindex++;
            if(qindex > 3) {
                qindex = 0;
            }
        }       
    }
    return (EXIT_SUCCESS);
}

void APP_LED_STATE(void)
{
    static uint32_t wait_time;
    switch (LED_STATE)
    {
        case LED_H0_D3_TOGGLE:
            wait_time = 0;
            LATHINV = 0x00001;
            LED_STATE = LED_H0_D3_IDLE;
            break;
        case LED_H0_D3_IDLE:            
            if (wait_time++ > 100000)
                LED_STATE = LED_H2_D4_TOGGLE;
            else
                LED_STATE = LED_H0_D3_IDLE;
            break;
        case LED_H2_D4_TOGGLE:
            wait_time = 0;
            LATHINV = 0x00004;
            LED_STATE = LED_H2_D4_IDLE;
            break;
        case LED_H2_D4_IDLE:
            if (wait_time++ > 100000)
                LED_STATE = LED_H2_D4_IDLE;
            else
                LED_STATE = LED_H1_D5_TOGGLE;
            break;    
        case LED_H1_D5_TOGGLE:
            wait_time = 0;
            LATHINV = 0x00002;
            LED_STATE = LED_H1_D5_IDLE;
            break;
        case LED_H1_D5_IDLE:
            if (wait_time++ > 100000)
                LED_STATE = LED_H1_D5_IDLE;
            else
                LED_STATE = LED_H0_D3_TOGGLE;
            break;
        default:
            break;
    }
}

void PERFORMANCE_VARS_INITIALIZE(void)
{
    timer23Reading = 0;
    timer45Reading = 0;
    timer67Reading = 0;
    timer4567Reading = 0;
}

void APP_I2C_BUFFER_TRANSFER_QUEUE(BUFFER_TRANSFER_QUEUE* MessageQueue)
{       
    if ( (MessageQueue->wrlen) && (MessageQueue->rdlen) )
        DRV_I2C_MasterBufferWriteRead(MessageQueue->address, MessageQueue->wrBuf, MessageQueue->wrlen, MessageQueue->rdBuf, MessageQueue->rdlen);
    else if (MessageQueue->wrlen)
        DRV_I2C_MasterBufferWrite(MessageQueue->address, MessageQueue->wrBuf, MessageQueue->wrlen);
    else 
        DRV_I2C_MasterBufferRead(MessageQueue->address, MessageQueue->rdBuf, MessageQueue->rdlen);
}

void APP_I2C_POPULATE_QUEUE_DATA(BUFFER_TRANSFER_QUEUE* BTQNew ,uint16_t slavedeviceaddress, uint8_t* i2ctxdata, uint8_t* i2crxdata, uint8_t writelength, uint8_t readlength)
{
    BTQNew->address      = slavedeviceaddress;
    BTQNew->wrBuf        = i2ctxdata;
    BTQNew->rdBuf        = i2crxdata;
    BTQNew->wrlen        = writelength;
    BTQNew->rdlen        = readlength;     
}

/*I2C interrupt ISR */ 
void __attribute__ ((nomips16,no_fpu)) __attribute__( (interrupt(ipl5AUTO), vector(_I2C2_MASTER_VECTOR))) I2C2MasterVector(void);
                                                    
void __attribute__ ((nomips16,no_fpu)) I2C2MasterVector(void)
{
    BUFFER_TRANSFER_QUEUE* BTQ_TX;
    
    IFS4bits.I2C2MIF = 0;
    BTQ_TX = GetNextQueueItem();
    if (BTQ_TX != NULL) {
        APP_I2C_BUFFER_TRANSFER_QUEUE(BTQ_TX);
    }
}


/*******************************************************************************
 End of File
*/