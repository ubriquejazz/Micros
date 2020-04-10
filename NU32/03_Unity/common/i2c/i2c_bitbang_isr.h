#ifndef I2C_BITBANG_ISR_H
#define	I2C_BITBANG_ISR_H

/*!\name      i2c_bitbang_isr.h
 *
 * \brief     Contains prototype for the I2C functions
 *
 * \author    Juan Gago
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define SYS_CLOCK_MHZ       200000000ul
#define PBCLK2_MHZ          100000000ul
#define I2C_BAUD_RATE       100000ul
#define I2C_BAUD_TIME       PBCLK2_MHZ/I2C_BAUD_RATE

#define SCLDir              TRISAbits.TRISA2	
#define SDADir              TRISAbits.TRISA3
#define SDA_INPUT           PORTAbits.RA3
#define SCL_INPUT           PORTAbits.RA2
#define SCL_OPEN_DRAIN      ODCAbits.ODCA2
#define SDA_OPEN_DRAIN      ODCAbits.ODCA3

#define I2C_SW_ISR_ENABLE   IEC4bits.I2C2MIE
#define I2C_SW_ISR_TRIGGER  IFS4bits.I2C2MIF

#define OUTPUT              0
#define INPUT               1
#define LOW                 0
#define HIGH                1
#define M_ACK               0
#define M_NACK              1

#define I2C_PINS                  LATACLR
#define I2C_SCL_SDA_OUTPUT_LOW    (_LATA_LATA2_MASK  | _LATA_LATA3_MASK )

#define _Timer_ISR_EventClear()     IFS1bits.T9IF   = 0     //clear Timer ISR event flag
#define _Timer_Stop()               T9CONbits.ON    = 0     //stop Timer
#define _Timer_Start()              T9CONbits.ON    = 1     //start Timer
#define _Timer_Count_Clear()        TMR9            = 0     //clear Timer Count
#define _Timer_ISR_Enable()         IEC1bits.T9IE   = 1     //enable Timer ISR
#define _Timer_ISR_Disable()        IEC1bits.T9IE   = 0;    //disable Timer ISR
#define ERROR_TIMEOUT_COUNT         200000


typedef enum
{
    I2C_NULL_STATE = 0,
    
    /* Start condition on I2C BUS with SDA low */
    I2C_SDA_LOW_START,
            
    /* I2C SDA low start check */
    I2C_SDA_LOW_START_CHECK,

    /* Start condition on I2C Bus with SCL low after SDA low */
    I2C_SCL_LOW_START,
            
    /* I2C SCL low start check */
    I2C_SCL_LOW_START_CHECK,
            
    /* setting SDA high on RESTART */
    I2C_SDA_HIGH_RESTART,
            
    /* check SDA high RESTART */
    I2C_SDA_HIGH_RESTART_CHECK,
            
    /* setting SCL high on RESTART */
    I2C_SCL_HIGH_RESTART,
            
    /* check SCL high on RESTART */
    I2C_SCL_HIGH_RESTART_CHECK,

    /* SCL High during data transfer to ensure Data is not changing */
    I2C_SCL_HIGH_DATA,
            
    /* High data check */
    I2C_SCL_HIGH_DATA_CHECK,

    /* SCL Low during data transfer where data can change */
    I2C_SCL_LOW_DATA,
            
    /* SCL low data check */
    I2C_SCL_LOW_DATA_CHECK,
            
    /* keep SCL and SDA low for 1 BRG time */
    I2C_SCL_SDA_LOW_STOP,      
            
    /* keep SCL and SDA low check */
    I2C_SCL_SDA_LOW_STOP_CHECK,

    /* SCL going high during STOP condition */            
    I2C_SCL_HIGH_STOP,
            
    /* SCL going HIGH check*/
    I2C_SCL_HIGH_STOP_CHECK,
            
    /* SDA going low during STOP condition */
    I2C_SDA_HIGH_STOP,
                
    /* SDA going high STOP check */
    I2C_SDA_HIGH_STOP_CHECK
} BUS_STATE;

typedef enum
{
    I2C_NONE = 0,
    I2C_START,
    I2C_WRITE_BYTE,
    I2C_ADDRESS_BYTE_WITH_RW1,
    I2C_RESTART,    
    I2C_READ_BYTE,
    I2C_ACK,     
    I2C_NACK, 
    I2C_STOP
} I2C_OPERATTION_TYPE;

typedef enum
{
    I2C_OPERATION_IDLE = 0,
    I2C_OPERATION_IN_PROGRESS,
    I2C_OPERATION_ERROR,
    I2C_START_COMPLETE,
    I2C_DATA_TRANSFER_COMPLETE,
    I2C_STOP_COMPLETE
} I2C_OPERATION_STATUS;

extern volatile BUS_STATE i2cState;
extern volatile I2C_OPERATTION_TYPE i2cOperation;
extern volatile I2C_OPERATION_STATUS i2cStatus;
extern volatile uint8_t data;

void I2C_Initialize ( uint32_t baudrate, bool masterISREnable, bool busExceptionEnable );

I2C_OPERATION_STATUS DRV_I2C_MasterBufferWrite(uint16_t deviceaddress, uint8_t* txBuffer, uint16_t txbuflen);

I2C_OPERATION_STATUS DRV_I2C_MasterBufferRead(uint16_t deviceaddress, uint8_t* rxBuffer, uint16_t rxbuflen);

I2C_OPERATION_STATUS DRV_I2C_MasterBufferWriteRead(uint16_t deviceaddress, uint8_t* txBuffer, uint16_t txbuflen, uint8_t* rxBuffer, uint16_t rxbuflen);

I2C_OPERATION_STATUS DRV_I2C_MASTEROpStatus(void);

void StartI2CBusClear(void);

inline bool IsByteAcked(void);
inline void _Timer_ISR_Disable(void);
inline void _Timer_ISR_Enable(void);

#endif	/* I2C_FUNCTIONS_H */

