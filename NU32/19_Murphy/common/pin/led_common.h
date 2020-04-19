#ifndef _LED_COMMON_H
#define _LED_COMMON_H

/*!\name      led_common.h
 *
 * \brief     API for LED Flashing
 *
 * \author    Juan Gago
 *
 */

// Duty cycle in multiple of 100 ms
#define MS100_MS100     {0,1,1}       // Used for led flash 100ms off 100ms on
#define MS200_MS200     {0,2,2}       // Used for led flash 200ms off 200ms on
#define MS400_MS400     {0,4,4}       // Used for led flash 500ms off 500ms on
#define NO_BLINKING     {0,0,0}       // Also valid {0,x,0} or {0,0,x}

// Same duty cycle with 90 degree (also in multiple of 100 ms)
#define MS100_MS100_90  {1,1,1}       // Used for led flash 100ms off 100ms on
#define MS200_MS200_90  {2,2,2}       // Used for led flash 200ms off 200ms on
#define MS400_MS400_90  {4,4,4}       // Used for led flash 500ms off 500ms on

typedef enum{
    LED_ACTIVE_LOW = 0,       // Powered by Vdd
    LED_ACTIVE_HIGH,          // Powered by PIC32 (current limit)
} LED_POLARITY;

// Result of a LED function
#define LED_FAILURE     -1
#define LED_SUCCESS     +1
#define LED_WAITING     0
#define LED_100MS       1

// Multiple of 100ms
typedef struct {
    uint8_t         offset;   // counter while LED is on initial standby
    uint8_t         low;      // counter while LED is on LOW state        
    uint8_t         high;     // counter while LED is on HIGH state
} LED_DUTY_CYCLE;

// LED Object
typedef struct
{
    PORTS_CHANNEL   channel;                // Pin channel
    PORTS_BIT_POS   bitPos;                 // Pin position
    LED_POLARITY    pol;                    // Led Polarity 
    LED_DUTY_CYCLE  period;                 // Duty cycle of the LED
    uint8_t         count;                  // Counter 
    uint8_t         state;                  // LEVEL_HIGH or LEVEL_LOW     
} LED_DEF;  

/* Driver API */
int led_init (const LED_DEF*);
int led_poll (void);

/* IO Functions */
LED_DEF* led_get (int);
int led_status_i (int, uint8_t*);
int led_status (LED_DEF*, uint8_t*);
int led_on (LED_DEF*);
int led_off (LED_DEF*);
int led_tasks (LED_DEF*);

#endif /* _LED_COMMON_H */

/*******************************************************************************
 End of File
 */

