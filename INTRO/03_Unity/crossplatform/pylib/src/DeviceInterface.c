/*!\name      DeviceInterface.h
 *
 * \brief     Mtrs2Device_t Definiton (dev)
 *
 */
 
#include "mrtsErrors.h"
#include "EncapsulationRouting.h"
#include "DeviceInterface.h"
#include "NetFunctions.h"

/*!\name     Mrts_Dev_OnRawReceive
 *
 * \brief    This function is called on receipt of raw data.
 *
 * \buffer
 */
int32_t Mrts_Dev_OnRawReceive(BaseObject_t* buffer, int length)
{
   int32_t retVal = merNullPointer;
   Mrts2Device_t* dev = (Mrts2Device_t* ) (buffer ? buffer->owner : NULL);

   /* Parse the contents of the buffer */
   if (dev && dev->rxRawCb)
   {
      retVal = dev->rxRawCb( buffer, length);
   }
   
   return retVal;
}


/*!\name     Mrts_Dev_OnReceiveFromRouter
 *
 * \brief    This function is when a packet is received from the router. The
 *           options are to process the packet directly or forward it.
 *
 */
int32_t Mrts_Dev_OnReceiveFromRouter(         BaseObject_t*            buffer,
                                              size_t                   packetSize,
                                              PacketType_t             type,
                                              EncAddr_t                src,
                                              EncAddr_t                dst,
                                              uint16_t                 sequence,
                                              uint32_t                 dataIndex,
                                              size_t                   dataLength,
                                              bool                     crcOk
                                             )
{
   int32_t retVal = 0;
   Mrts2Device_t* dev = (Mrts2Device_t* ) buffer->owner;

   if (dst != dev->address )
   {
      /* This packet isn't for us so let's see if it's in the routing table */
      EncapsulationRouteEntry_t* entry = EncapsulationRouting_FindEntry( dev->routingTable,  dst );
      if ( entry )
      {
         /* pass packet onto next hop unaltered */
         if (entry->addressedProcessor)
         {
            retVal = entry->addressedProcessor(buffer, packetSize, type, src, dst, sequence, dataIndex, dataLength, crcOk);
         }
         else
         {
            /* No processor assigned so discard packet */
            retVal = CircularBuffer_AdvanceReadPos( (CircularBuffer_t* ) buffer, packetSize );
         }
      }
      else
      {
         /* No entry so send error message back to source */
         retVal =  nfnSendErrorResponse( dev, dst, src, sequence, nerDestinationUnknown, NULL,0);
         /* And discard message*/
         retVal = CircularBuffer_AdvanceReadPos( (CircularBuffer_t* ) buffer, packetSize );
      }
   }
   else
   {
      /* The packet is meant for us so let's parse it...*/
      if (dev->rxAddressedPacketCb)
      {
         retVal = dev->rxAddressedPacketCb(buffer, packetSize, type, src, dst, sequence, dataIndex, dataLength, crcOk);
      }
      else
      {
         /* We don't have a callback assigned so discard the packet. */
         retVal = CircularBuffer_AdvanceReadPos( (CircularBuffer_t* ) buffer, packetSize );
      }
   }

   return retVal;
}


/*!\name     Mrts2_Dev_Init
 *
 * \brief    Initialise the device interface
 *
 */
int32_t Mrts2_Dev_Init( Mrts2Device_t*            dev,
                        BaseObject_t*             owner,
                        size_t                    rxBufferSize,
                        TxBufferEntry_t*          blankEntries,
                        int                       numTxEntries,
                        size_t                    txBufferSize)
{
   int32_t retVal = merNullPointer;

   if (owner && dev && rxBufferSize && numTxEntries && txBufferSize)
   {
      dev->base.self = (BaseObject_t*) dev;
      dev->base.owner = owner;
      //dev->rxAddressedFromRouterCb = NULL;
      dev->rxRawCb = NULL;
      dev->encProc.rxRawCb = Mrts_Dev_OnRawReceive;
      dev->encProc.rxdPacketCb = NULL;
      dev->encProc.rxdAddressedPacketCb = Mrts_Dev_OnReceiveFromRouter;
      retVal = ConfigureDuplexControl( &dev->duplexControl,  dev->base.self, false);
      retVal = CircularBuffer_Init( dev->base.self, &dev->rxBuffer, rxBufferSize);
      if (retVal>=merNoError)
      {
         retVal = EncapsulationProcessor_Init( &dev->base, &dev->encProc, &dev->rxBuffer, CircularBuffer_PeekByte );
         if (retVal>=merNoError)
         {
            dev->encProc.useRawTerminator=true;
            dev->encProc.rawTerminator='\r';
            retVal = InitialiseTxBuffers( dev->base.self, &dev->txBuffers, blankEntries, numTxEntries, txBufferSize);
         }
      }
      else
      {
         CircularBuffer_Delete( &dev->rxBuffer );
      }
   }

   return retVal;
}

/*!\name      Mrts2_Dev_SetupRoutingTable
 *
 * \brief     This function is used to setup the routing table, swapping it
 *            for the new table if necessary. THE POINTER TO THE TABLE IS USED
 *            THIS FUNCTION DOES NOT ALLOCATE MEMORY FOR THE TABLE.
 */
int32_t Mrts2_Dev_SetupRoutingTable(Mrts2Device_t* dev , EncAddr_t myAddress, EncapsulationRoutingTable_t* newTable )
{
   if ( NULL == dev || NULL == newTable )
   {
      return merNullPointer;
   }

   dev->address = myAddress;
   dev->routingTable = newTable;

   return 0;
}

/*!\name      Mrts2_Dev_Receive
 *
 * \brief     This function is used to receive a block of bytes and store them
 *            in the device's internal storage. If there are more bytes than 
 *            space available in the buffer it loses the oldest bytes first.
 * 
 * \return    Error < 0 < Number of bytes stored.
 * 
 */
int32_t Mrts2_Dev_Receive( Mrts2Device_t* dev, void* bytes, size_t numBytes)
{
   int32_t retVal = (dev && bytes) ? merNoError : merNullPointer;
   
   if (merNoError== retVal)
   {
      retVal = CircularBuffer_AddBytes( &dev->rxBuffer, bytes, numBytes );
      if (retVal>0)
      {
         StartedReceiving( &dev->duplexControl );
      }
   }
   
   return retVal;
}


/*!\name      Mrts2_Dev_ReceiveRoger
 *
 * \brief     Sets duplex state.
 *
 * \return    Negative - an error
 */
int32_t Mrts2_Dev_ReceiveRoger( Mrts2Device_t* dev, uint16_t period )
{
   int32_t retVal = dev ? merNoError : merNullPointer;

   if (merNoError == retVal)
   {
      if (period)
      {
         retVal = EnterRadioSilenceState( &dev->duplexControl, period );
      }
      else
      {
         retVal = EnterReadyToSendState( &dev->duplexControl );
      }
   }

   return retVal;
}

/*!\name      Mrts2_Dev_AddToSendQueue
 *
 * \brief     Rather than send the data immediately this queues it up to be
 *            sent at the appropriate time.
 *
 * \return    Positive - the number of bytes added. Negative - an error code.
 */
int32_t Mrts2_Dev_AddToSendQueue (Mrts2Device_t* dev, void* bytes, size_t numBytes )
{
   int32_t retVal = (dev && bytes) ? merNoError : merNullPointer;

   if (merNoError == retVal)
   {
      TxBufferEntry_t* entry= GetNextAvailableBuffer(&dev->txBuffers);
      if (entry)
      {
         retVal = CircularBuffer_AddBytes( &entry->circular, (uint8_t*) bytes, numBytes);
      }
   }

   return retVal;
}

/*!\name      Mrts2_Dev_GetBufferIntoSendQueue
 *
 * \brief 
 */
int32_t Mrts2_Dev_GetBufferIntoSendQueue( Mrts2Device_t *dev, BaseObject_t* srcBuffer)
{
   int32_t retVal = (dev && dev->txBuffers.numEntries && srcBuffer) ? merNoError : merNullPointer;

   if (merNoError==retVal)
   {   
      int32_t remaining = CircularBuffer_GetReceivedWaiting((CircularBuffer_t *) srcBuffer);
      while ( retVal>=0 && remaining>0)
      {
         TxBufferEntry_t* entry= GetNextAvailableBuffer(&dev->txBuffers);
         if (entry)
         { 
            int32_t sent = CircularBuffer_GetIntoThisBuffer( entry->circular.base.self, srcBuffer, remaining, false );
            if (sent>=0)
            {
               remaining-=sent;
               retVal += sent;   
            }
            else
            {
               retVal=sent;
            }
         }
      }
   }
   return retVal;  
}

/*!\name      Mrts2_Dev_Transmit
 *
 * \brief     This function sends whatever is waiting in the tx buffers.
 *
 * \return    Positive, the number of bytes transmitted, negative an error.
 */
int32_t Mrts2_Dev_Transmit( Mrts2Device_t* dev )
{
   int32_t retVal = merNoError;

   if (NULL==dev)
   {
      return merNullPointer;
   }

   //if (!dev->duplexControl.isHalfDuplex || dev->duplexControl.state == hdpxTransmitting)
   if (!(dev->duplexControl.isHalfDuplex && dev->duplexControl.state==hdpxRadioSilence))
   {
      TxBufferEntry_t* entry =dev->txBuffers.transmittingEntry;
      if (!entry)
      {
         entry = GetNextToSend( &dev->txBuffers );
         dev->txBuffers.transmittingEntry = entry;            
      }
      
      if (entry)
      {
         uint8_t tempBuffer[DEFAULT_DEVICE_TX_BUFFER_SIZE];
         int32_t bytesToCopy = CircularBuffer_GetReceivedWaiting( &entry->circular );
         if (bytesToCopy>0)
         {
            EnterTransmittingState(&dev->duplexControl, 200);
            if (bytesToCopy>DEFAULT_DEVICE_TX_BUFFER_SIZE)
            {
               bytesToCopy = DEFAULT_DEVICE_TX_BUFFER_SIZE;
            }   
            retVal = CircularBuffer_CopyBytes(entry->circular.base.self, 0, tempBuffer, bytesToCopy );
            if (retVal>0)
            {
               retVal = MrtsWriteChannel( &dev->channel, tempBuffer, retVal );
               if (retVal>0)
               {
                  //retVal = CircularBuffer_AdvanceReadPos( &entry->circular, retVal);
                  retVal = CircularBuffer_GetBytes(&entry->circular, tempBuffer, retVal );
               }
            }
            bytesToCopy = CircularBuffer_GetReceivedWaiting( &entry->circular );
         }
         
         if (bytesToCopy<=0)
         {
            /* Now sent it */
            dev->txBuffers.transmittingEntry = 0;
            if (entry->txToGo>0)
            {
               entry->txToGo=0;
            }
            EnterRadioSilenceState(&dev->duplexControl, 200);   
         }         
      }
      else
      {
      }
   }


   return retVal;
}

/*!\name      Mrts2_Dev_SetTxEnable
 *
 * \brief     Callback from HalfDuplex to enable output on state changes.
 *
 * \param     target         Pointer to this device which contains the channel
 *                           to have it's output enabled or disabled.
 *
 * \param     enable         New enabled or disabled state of output.
 *
 * \return    Negative false
 */
bool Mrts2_Dev_SetTxEnable( BaseObject_t* target, bool enable)
{
   int32_t retVal = target ? merNoError : merNullPointer;

   if (merNoError==retVal)
   {
      Mrts2Device_t* dev = (Mrts2Device_t* ) target;
      retVal = MrtsEnableWriteChannel(&dev->channel, enable);
   }

   return retVal;
}

/*!\name      Mrts2_Dev_SetRxEnable
 *
 * \brief     Callback from HalfDuplex to enable input on state changes.
 *
 * \param     target         Pointer to this device which contains the channel
 *                           to have it's input enabled or disabled.
 *
 * \param     enable         New enabled or disabled state of input.
 *
 * \return    Negative false
 */
bool Mrts2_Dev_SetRxEnable( BaseObject_t* target, bool enable)
{
   int32_t retVal = target ? merNoError : merNullPointer;

   if (merNoError==retVal)
   {
      Mrts2Device_t* dev = (Mrts2Device_t* ) target;
      retVal =  MrtsEnableReadChannel(&dev->channel, enable);
   }

   return retVal;
}


/*!\name      Mrts2_Dev_Poll
 *
 * \brief     This function is used to poll the input of the device.
 * 
 * \param     target         Pointer to the device to poll.
 *
 * \return    Error < 0 < Number of bytes read.
 */
int32_t Mrts2_Dev_Poll( Mrts2Device_t* dev )
{
   int32_t retVal = merNullPointer;
      char bytes[100];
   if ( dev )
   {
      if (dev->duplexControl.isHalfDuplex)
      {
         if (hdpxRadioSilence==dev->duplexControl.state || hdpxReadyToSend==dev->duplexControl.state)
         {
            /* Receive only right now */
            retVal = MrtsReadChannel( &dev->channel, bytes, sizeof(bytes) );
            if (retVal>0)
            {
               EnterRadioSilenceState( &dev->duplexControl, 50);
               Mrts2_Dev_Receive( dev, bytes, retVal );
            }
            else
            {               
               EnterReadyToSendState( &dev->duplexControl);
            }
         }

         EncapsulationProcessor_Machine(&dev->encProc);
         
         if (hdpxTransmitting==dev->duplexControl.state || hdpxReadyToSend==dev->duplexControl.state)
         {
            retVal = Mrts2_Dev_Transmit( dev );           
         }
         
         PollStates(&dev->duplexControl);
      }
      else
      {
         //DPRINTF("%s:%d: Ready to read dev=%p channel=%p cb=%p\n\r", __PRETTY_FUNCTION__, __LINE__, dev, &(dev->channel), dev->channel.readChannelCB);
         retVal = MrtsReadChannel( &dev->channel, bytes, sizeof(bytes) );
         if (retVal>0)
         {
            Mrts2_Dev_Receive( dev, bytes, retVal );
            //DPRINTF("%s:%d: received=%d\n\r", __PRETTY_FUNCTION__, __LINE__, retVal);
         }
         else
         {
             //DPRINTF("%s:%d: MrtsReadChannel returned %d\n\r", __PRETTY_FUNCTION__, __LINE__, retVal);
         }
         EncapsulationProcessor_Machine(&dev->encProc);
         
         /* Whatever we have in our buffers it's time to send it */
         retVal = Mrts2_Dev_Transmit( dev );       
      }
   }

   return retVal;
}


