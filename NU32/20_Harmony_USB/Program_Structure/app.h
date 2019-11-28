#ifndef APP__H__
#define APP__H__

/*!\name      app.h
 *
 * \brief     demonstrates the timer driver
 *
 * \author    Juan Gago
 *
 */

// The application states. 
typedef enum { 
	APP_STATE_INIT, 	// initial state, used to perform app-specific setup
	APP_STATE_WAIT,		// during program operation, we enter as the timer takes over
	APP_STATE_ERROR
} APP_STATES; 

// Harmony suggests that you place your application-specific data in a struct.
typedef struct {
  APP_STATES state;
  DRV_HANDLE handleTmr;
} APP_DATA;

void APP_Initialize(void);
void APP_Tasks(void);

#endif
