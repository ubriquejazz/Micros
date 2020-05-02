#include "buffer.h"

static volatile int data_buf[BUFFER_LEN]; // array that stores the data
static volatile uint8_t read, write;      // circular buf indexes

int buffer_init() {
  write = 0;
  read = 0;
  return BUFFER_OK;
}

int buffer_empty() {    // return true if the buffer is empty (read = write)
  return read == write; 
}

int buffer_full() {     // return true if the buffer is full.  
  return (write + 1) % BUFFER_LEN == read; 
}

int buffer_read(int* val) {     // reads from current buffer location; assumes buffer not empty
  *val = data_buf[read];
  ++read;               // increments read index
  if(read >= BUFFER_LEN) {  // wraps the read index around if necessary
    read = 0;
  }
  return BUFFER_OK;
}

int buffer_write(int data) { // add an element to the buffer.  
  if(!buffer_full()) {        // if the buffer is full the data is lost
    data_buf[write] = data;
    ++write;                  // increment the write index and wrap around if necessary
    if(write >= BUFFER_LEN) {
      write = 0;
    }
  }
  return BUFFER_OK;
}

/*******************************************************************************
 End of File
 */
