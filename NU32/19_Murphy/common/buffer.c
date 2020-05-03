#include "buffer.h"

static volatile uint8_t read=0, write=0;  // circular buf indexes
static volatile Base buffer[BUFFER_LEN];  // buffer of pointers

int buffer_empty() {    // return true if the buffer is empty (read = write)
  return read == write; 
}

int buffer_full() {     // return true if the buffer is full.  
  return (write + 1) % BUFFER_LEN == read; 
}

int buffer_write(Base data) { // add an element to the buffer.  
  if(!buffer_full()) {        // if the buffer is full the data is lost
    buffer[write] = data;
    buffer[write].data = 1;
    ++write;                  // increment the write index and wrap around if necessary
    if(write >= BUFFER_LEN) {
      write = 0;
    }
  }
  return BUFFER_OK;
}

int buffer_read(Base* val) {     // reads from current buffer location; assumes buffer not empty
  buffer[read].data = 0;
  *val = buffer[read];
  ++read;               // increments read index
  if(read >= BUFFER_LEN) {  // wraps the read index around if necessary
    read = 0;
  }
  return BUFFER_OK;
}

/*******************************************************************************
 End of File
 */
