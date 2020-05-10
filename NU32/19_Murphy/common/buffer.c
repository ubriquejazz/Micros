#include "buffer.h"

static volatile uint8_t read[BUFFER_INST];    // circular buf index
static volatile uint8_t write[BUFFER_INST];   // circular buf index
static volatile Base    buffer[BUFFER_INST][BUFFER_SIZE];  // data

int buffer_empty(int i) {    // return true if the buffer is empty (read = write)
  return read[i] == write[i]; 
}

int buffer_full(int i) {     // return true if the buffer is full.  
  return (write[i] + 1) % BUFFER_SIZE == read[i]; 
}

/*******************************************************************************
 End of File
 */
