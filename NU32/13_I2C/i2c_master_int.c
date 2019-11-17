/*!\name      i2c_master_int.c
 *
 * \brief     I2C Master utilities, using interrupts
 *            I2C Master utilities, 100 kHz, using polling rather than interrupts
 *            The functions must be callled in the correct order as per the I2C protocol
 *             - Master will use I2C1 SDA1 (D9) and SCL1 (D10)
 *             - Connect SDA1 to the SDA pin and SCL1 to the SCL pin on the slave 
 *
 * \author    Juan Gago
 *
 */
 
#include "NU32.h"          // constants, funcs for startup and UART
#include "i2c_master_int.h"

// keeps track of the current I2C state
static volatile enum {IDLE, START, WRITE, READ, RESTART, ACK, NACK, STOP, ERROR} state = IDLE;   

static buffer_t to_write = NULL;           // data to write
static buffer_t  to_read = NULL;           // data to read
static volatile unsigned char address = 0; // the 7-bit address to write to / read from
static volatile unsigned int  n_write = 0; // number of data bytes to write
static volatile unsigned int  n_read  = 0; // number of data bytes to read

bool _start_next_state ()
{
  // global n_write, address, state
  if(n_write > 0) {                 // there are bytes to write
    state = WRITE;                  // transition to write mode
    I2C1TRN = address << 1;         // send the address, with write mode set
  } else {
    state = ACK;                    // skip directly to reading
    I2C1TRN = (address << 1) & 1;   
  }
  return 0;
}

bool _write_next_state ()
{
  // global n_read, state
  if(n_read > 0) {                  // we want to read so issue a restart
    state = RESTART;         
    I2C1CONbits.RSEN = 1;           // send the restart to begin the read
  } else {                          // no data to read, issue a stop
    state = STOP;
    I2C1CONbits.PEN = 1;
  }
  return 0;
}

bool _read_next_state (unsigned int read_index)
{
  // global n_read, state
  if(read_index == n_read)          // we are done reading, so send a nack
  { 
    state = NACK;
    I2C1CONbits.ACKDT = 1;
  } else {
    state = ACK;
    I2C1CONbits.ACKDT = 0;
  }
  I2C1CONbits.ACKEN = 1;
  return 0;
}

void __ISR(_I2C_1_VECTOR, IPL1SOFT) I2C1MasterInterrupt(void) {

  static unsigned int write_index = 0, read_index = 0;  //indexes the read/write arrays

  switch(state) {
    case START:                         // start bit has been sent
      write_index = 0;                  // reset indices
      read_index = 0;
      _start_next_state();
      break;

    case WRITE:                         // a write has finished
      if(I2C1STATbits.ACKSTAT) {        // error: didn't receive an ACK from the slave
        state = ERROR;
      } else {        
        if(write_index < n_write) {         // still more data to write
          I2C1TRN = to_write[write_index];  // write the data
          ++write_index;
        } else {                            // done writing data, time to read or stop
          _write_next_state();
        }
      }
      break;

    case RESTART:   // now we want to read, send the read address
      state = ACK;  // when interrupted in ACK mode, we will initiate reading a byte
      I2C1TRN = (address << 1) | 1; // the address is sent with the read bit sent
      break;
    case READ:
      to_read[read_index] = I2C1RCV;
      _read_next_state(++read_index);
      break;
    case ACK: // just sent an ack meaning we want to read more bytes
      state = READ;
      I2C1CONbits.RCEN = 1;
      break;
    case NACK: // issue a stop
      state = STOP;
      I2C1CONbits.PEN = 1;
      break;
    case STOP:
      state = IDLE; // we have returned to idle mode, indicating that the data is ready
      break;
    default: // some error has occurred
      state = ERROR;
  }
  IFS0bits.I2C1MIF = 0;       //clear the interrupt flag
}

void i2c_master_setup() {
  int ie = __builtin_disable_interrupts();
  I2C1BRG = 90;                    // I2CBRG = [1/(2*Fsck) - PGD]*Pblck - 2
                                   // Fsck is the frequency (400 kHz here), PGD = 104ns
                                   // this is 400 khz mode
                                   // enable the i2c interrupts
  IPC6bits.I2C1IP  = 1;            // master has interrupt priority 1
  IEC0bits.I2C1MIE = 1;            // master interrupt is enabled
  IFS0bits.I2C1MIF = 0;            // clear the interrupt flag
  I2C1CONbits.ON = 1;              // turn on the I2C2 module
  
  if(ie & 1) {
    __builtin_enable_interrupts();
  }
}

// communicate with the slave at address addr.  
// first write wlen bytes to the slave, then read rlen bytes from the slave
int i2c_write_read (unsigned int addr, 
    const buffer_t write, unsigned int wlen, 
    const buffer_t read, unsigned int rlen ) 
{
  n_write = wlen;
  n_read = rlen;
  to_write = write;
  to_read = read;
  address = addr;
  state = START;
  I2C1CONbits.SEN = 1;  // initialize the start
  while(state != IDLE && state != ERROR) { ; }  // timeout?
  return state != ERROR;
}

// write a single byte to the slave
int i2c_write_byte(unsigned int addr, unsigned char byte) {
  return i2c_write_read(addr,&byte,1,NULL,0);
}
