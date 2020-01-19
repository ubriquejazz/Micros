#ifndef _TMR__H__
#define _TMR__H__

/*!\name      tmr.h
 *
 * \brief     Timer Class
 *
 * \author    Juan Gago
 *
 */

typedef uint32_t object_t;
typedef struct NODE node_t;

struct NODE {
	node_t*		self;
	node_t*		owner;
	object_t 	data;
};

typedef enum
{
    TMR_DRV_INIT=0,
    TMR_DRV_DONE,
} TMR_DRV_STATE;

typedef enum {
	TMR_CLIENT_NONE=0, 
	TMR_CLIENT_REQ, 
	TMR_CLIENT_COMPLETE,	// Complete
	TMR_CLIENT_USED, 		// Processed
	TMR_CLIENT_ERROR, 		// Error
	TMR_CLIENT_TIMEOUT		// Pending
} TMR_CLIENT_STATE;

/* Driver & Clients */

typedef struct {
	node_t				base;
    TMR_CLIENT_STATE	state;
    uint32_t       		alarm, now;  
} TMR_CLIENT;

typedef struct {
	node_t			base;			
	TMR_DRV_STATE	state;
    uint8_t         index; 
  	uint32_t        error_count;  
} TMR_DRIVER;

typedef struct {
	TMR_CLIENT		user; /* Only one client per driver */
	TMR_DRIVER		drv;
} TMR_OBJECT;


/* Application Initialization and State Machine Functions */

void TMR_Initialize (TMR_OBJECT*, uint8_t);
void TMR_Tasks (TMR_CLIENT*, uint16_t);

#endif
