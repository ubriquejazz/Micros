#ifndef _CALLBACK_H    /* Guard against multiple inclusion */
#define _CALLBACK_H

/*!\name      callback.h
 *
 * \brief     IRDA Pic and DUMMY Pic
 *
 * \author    Juan Gago
 *
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "system_config.h"
#include "system_definitions.h"
#include "Global.h"
#include "Uart.h"

bool DisableUsartModule(uint32_t);

#ifdef DUMMY 
	void    DUMMY_Callback_Helper	(UART_DATA*, uint8_t);
#else
	void    IRDA_Callback_Helper	(UART_DATA*, uint8_t);
#endif

#endif /* _CALLBACK_H */

/* *****************************************************************************
 End of File
 */
