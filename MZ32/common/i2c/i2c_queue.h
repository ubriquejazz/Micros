#ifndef I2C_QUEUE_H
#define	I2C_QUEUE_H

/*!\name      i2c_queue.h
 *
 * \brief     Contains prototype for main
 *
 * \author    Juan Gago
 *
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "main.h"
#include "i2c_bitbang_isr.h"

#define MAX_NUMBER_OF_QUEUE_ELEMENTS 6

extern BUFFER_TRANSFER_QUEUE BTQ_Message[MAX_NUMBER_OF_QUEUE_ELEMENTS] ;
extern BUFFER_TRANSFER_QUEUE* queue[MAX_NUMBER_OF_QUEUE_ELEMENTS];

extern volatile uint16_t QueueIn;
extern volatile uint16_t QueueOut;
extern volatile uint16_t QueueDelta;

void QueueInitialize(void);

BUFFER_TRANSFER_QUEUE* GetNextQueueItem(void);

uint32_t  GetNumberOfFreeSlots(void);

bool AddNewQueueItem(BUFFER_TRANSFER_QUEUE* BTQ_IN);

uint32_t  GetNumberofQueueEntires(void);

bool IsQueueEmpty(void);

#endif	/* I2C_QUEUE_H */
