#ifndef __pwm_h
#define __pwm_h

#include "enum.h"

#define PWM_SW_VERSION          3
#define MAX_FAN_MUX_PORTS       7
#define MASTER_SYNCHRO_MS       50      // 5, 50(*), 500
#define IRDA_CHANNEL_MAX        6       // 5, 6(*), 7

/*!\brief     IRDA Constants. */

#define DISCOVER_START_TIME     2
#define DISCOVER_END_TIME       28
#define DEFAULT_TX_LEVEL_PWM    10

/*!\brief     FAN Constants. */

#define EnA_On()                PIC_PIC_IO_9On()
#define EnA_Off()               PIC_PIC_IO_9Off()
#define Clk_Toggle()            PIC_PIC_IO_0Toggle()
#define EnB_On()                PIC_PIC_IO_15On()
#define EnB_Off()               PIC_PIC_IO_15Off()


/*!\name      pwm.h
 *
 * \brief     IrDA and FAN Pics
 *
 * \author    Juan Gago
 *
 */

typedef enum
{
	/* Application's state machine's initial state. */
	APP_STATE_INIT=0,
    APP_STATE_IDLE,
    APP_STATE_TIMER_1HZ,
    APP_STATE_TIMER_1MS,
    APP_STATE_ERROR

} APP_STATES;

typedef struct
{
    APP_STATES      state;
    uint32_t        seconds, milliseconds;
    uint32_t        current_tick, prev_tick;
    uint32_t        error_count;   
    char            pic_address;      
    char            irMessage;      
    int             irChannel;
    
    /* IRDA Pic's */
    uint16_t        adc_buffer[5];          // ADC channels 7:8 and 40:42 
    uint16_t        edge_count;             
    
    /* FAN Pic*/
    uint16_t        fan_rpm[21];            // ADC channels 0:20 
    uint8_t         edge_cnt0, edge_cnt1, edge_cnt2;
    
} APP_DATA;
  
/* Application Functions */

bool APP_isDowntime ( void );
bool APP_isUptime ( void );
void APP_ChangeNotice_Callback (void);

/* Application Initialization and State Machine Functions */

void APP_Initialize ( void );
void APP_Tasks( void );

#endif /* __pwm_h */

/*******************************************************************************
 End of File
 */

