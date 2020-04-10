/*!\name      i2c_queue.c
 *
 * \brief     This file implements queueing of I2C transactions. If an I2C
 *            I2C transaction is currently in progress and if the user calls
 *            an I2C data transfer function again before the previous transfer
 *            is not completed, then an error condition is generated. Thus 
 *            instead of polling for the completion of a transfer, the user 
 *            can queue up all the transfers. Each item in the queue is 
 *            transferred onto the I2C bus once the prior transaction is complete.
 *
 * \author    Juan Gago
 *
 */


#include "i2c_queue.h"

volatile uint16_t QueueIn = 0;
volatile uint16_t QueueOut = 0;
volatile uint16_t QueueDelta = 0;

BUFFER_TRANSFER_QUEUE BTQ_Message[MAX_NUMBER_OF_QUEUE_ELEMENTS] ;
BUFFER_TRANSFER_QUEUE* queue[MAX_NUMBER_OF_QUEUE_ELEMENTS];

void QueueInitialize(void)
{
    QueueIn =  0;
    QueueOut = 0;
}

/* Add a new item to the queue. Before adding a new item to the queue,
 * the queue is checked if there are any free slots available. Only if the 
 * returned value indicates that there are free slots available, that a new
 * message is added to the queue */

bool AddNewQueueItem(BUFFER_TRANSFER_QUEUE* BTQ_IN)
{
    uint32_t    QueueSlotNumber;   
    
    QueueSlotNumber = GetNumberOfFreeSlots();
    
    /* if the number of free slots available is greater than 0, then advance
     * the index and add the next item to the queue */
         
    if (QueueSlotNumber > 0)
    {
        if (BTQ_IN != NULL)
        {
            queue[QueueIn] = BTQ_IN;
            QueueIn++;
            if (QueueIn >= MAX_NUMBER_OF_QUEUE_ELEMENTS)
                QueueIn = 0;
            if (DRV_I2C_MASTEROpStatus() == I2C_STOP_COMPLETE)
            {
                Start_I2C_Transaction();
            }
            return true;
        }
    }
    else
        return false;
    
    return false;
}

/* Returns the number of free elements in the queue*/

uint32_t  GetNumberOfFreeSlots(void)
{
    
    /* if position of item populated in the queue (Queue-In pointer) is greater 
     * than the position where the item is to be taken out of the queue,
     * (Queue-Out pointer) then the number of free slots is the remainder of the 
     * slots excluding the slot index where Queue Out pointer resides to the 
     * slot index where Queue-In pointer resides.
     * Ex: # of Queue Slots = 6, Queue-In = 5 and Queue-Out = 2
     * Number of free queue available  = 6 -(5-2) - 1 = 2 (free slots - 6 & 1)
     * 
     * if Queue-Out pointer is greater than Queue-In pointer but the Queue Out 
     * pointer is adjacent to Queue-In pointer (Queue-Out - Queue-In == 1) 
     * then buffer is full. Return 0 slots available if it's the case.      
     * If Queue-Out pointer is greater than Queue-In pointer then slots starting
     * from Queue-In pointer to Queue-Out pointer is number of available queue
     * slots 
     * Ex: # of Queue Slots = 6, Queue-Out = 4 and Queue-In = 2
     * Number of free queue slots available  = 4-2-1 (free slots - 3)
     */
    
    if (QueueIn >= QueueOut) {
        return (MAX_NUMBER_OF_QUEUE_ELEMENTS-(QueueIn-QueueOut) -1);
    }
    else {
        return ((QueueOut-QueueIn) -1);        
    }
    return 0;
}

uint32_t  GetNumberofQueueEntires(void)
{    
    /* if position of item to be populated in the queue (Queue-In) is greater 
     * than the position where the item is to be taken out of the queue,
     * (Queue-Out), the number of elements in the queue is the difference 
     * between Queue-In and Queue-Out.
     * Ex: # of Queue Slots = 6, Queue-In = 5 and Queue-Out = 2
     * Number of queue entries after adding the latest one (5-2)
     * Slots having data = 2,3 and 4
     * 
     * if Queue-Out pointer is greater than Queue-In pointer, the number of free
     * spaces is between the Queue-In pointer and Queue-Out pointer, so the 
     * number of Queue entries difference of this from the total number of 
     * queue slots
     * Ex: # of Queue Slots = 6, Queue-In = 1 and Queue-Out = 3
     * Number of queue entries = 6 - (3-1)
     * slots having data == 3,4,5,0 */
    
    if (QueueIn >= QueueOut) {
        return ((QueueIn - QueueOut));
    }
    else {    
        return (MAX_NUMBER_OF_QUEUE_ELEMENTS-(QueueOut-QueueIn));
    }
    return 0;
}

/* Checks if Queue is empty, returns true if Queue is empty else returns false*/
bool IsQueueEmpty(void)
{
    if ( (QueueOut == QueueIn) )
        return true;
    return false;
}

/* returns the next item from the queue*/
BUFFER_TRANSFER_QUEUE* GetNextQueueItem(void)
{
    uint8_t temp;
    
    /* if queue is empty then return NULL */
    if (IsQueueEmpty()) {
        Nop();
        return (BUFFER_TRANSFER_QUEUE*)NULL;
    }
    
    temp = QueueOut;
    if ( QueueOut >= (MAX_NUMBER_OF_QUEUE_ELEMENTS-1) )
        QueueOut = 0;
    else
        QueueOut++;

    return (queue[temp]);
}



/*******************************************************************************
 End of File
*/
