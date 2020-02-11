void APP_Tasks ( void )
{
  switch(state)
  {
    case APP_STATE_INIT: /* opens the SPI channel.
                         If the SPI channels is successfully opened
                         the CS line is asserted and the State is set
                        to APP_SEND_READ_CMD*/.
         
            SPIHandle = DRV_SPI_Open(DRV_SPI_INDEX_0, DRV_IO_INTENT_READWRITE );
  
            if(SPIHandle != (uintptr_t)NULL)
            {
               APP_SPI_CS_SELECT();
               state = APP_SEND_READ_CMD;
            }
            break;
         
  
      case APP_SEND_READ_CMD: /* Loads the transmit buffer with the Read command
                              (0x03), the 16-bit address to read ( 0x0000),
                              and one byte of don't cares. ( 0x00).
                              The data buffer is sent to the SPI peripheral
                              using DRV_SPI_BufferAddWrite. The state
                              is incremented after the call to the transmit function.*/
         {
           TXbuffer[0] = 3; /* EEPROM Read Opcode */
           TXbuffer[1] = 0; /* Address - LSB */
           TXbuffer[2] = 0; /* Address - MSB */
           TXbuffer[3] = 0; /* Dummy byte */
 
           Write_Buffer_Handle = DRV_SPI_BufferAddWrite(SPIHandle,
                               (SPI_DATA_TYPE *)&TXbuffer[0], 4, 0, 0);
           state = APP_WAIT_FOR_REPLY;
           break;
         }
 
      case APP_WAIT_FOR_REPLY: /* Verifies the transmit function has completed before
                                                              changing the state to APP_GET_DATA */
         {
           if(DRV_SPI_BUFFER_EVENT_COMPLETE &
                               DRV_SPI_BufferStatus(Write_Buffer_Handle))
           state = APP_GET_DATA;
           break;
         }
 
      case APP_GET_DATA: /* Sends out 16 clock signals (4 bytes of don't care data) to
                                               receive the contents of the address 0x00, and 0x01 from the EEPROM.
                                               Increments the state upon return from DRV_SPI_BufferAddRead */
         {
           Read_Buffer_Handle = DRV_SPI_BufferAddRead( SPIHandle,
                               (SPI_DATA_TYPE *)&RXbuffer[0], 4, 0, 0);
           state = APP_WAIT_FOR_DATA;
           break;
         }
 
      case APP_WAIT_FOR_DATA: /* Checks to see if the SPI peripheral has finished receiving the data.
                                                       Upon successful completion, the data will be in the receive buffer. The CS line
                                                       will be de-selected.*/
         { if(DRV_SPI_BUFFER_EVENT_COMPLETE &
                               DRV_SPI_BufferStatus (Read_Buffer_Handle))
            {
              APP_SPI_CS_DESELECT();
              state = APP_READ_COMPLETE;
            }
           break;
         }
 
      case APP_READ_COMPLETE:
         break; /* end of function */
   }