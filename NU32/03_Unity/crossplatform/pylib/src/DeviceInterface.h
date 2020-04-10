#ifndef __DEVICE_INTERFACE
#define __DEVICE_INTERFACE

/*!\name      DeviceInterface.h
 *
 * \brief     Mtrs2Device_t* Definition (dev)
 *            CircularBuffer_t   dev.rxBuffer
 *            TxBuffers_t        dev.txBuffers
 *            DuplexControl_t    dev.DuplexControl
 *            MrtsCommsChannel_t dev.channel
 *
 */

#include "EncapsulationRouting.h"
#include "CircularBuffer.h"
#include "OsalCommsChannel.h"
#include "HalfDuplex.h"
#include "TransmitBuffers.h"

#ifndef DEFAULT_DEVICE_TX_BUFFER_SIZE
#define DEFAULT_DEVICE_TX_BUFFER_SIZE 1024
#endif

typedef struct Mtrs2Device_t
{
   BaseObject_t                    base;
   CircularBuffer_t                rxBuffer;
   EncapsulationProcessor_t        encProc;
   MrtsCommsChannel_t              channel;
   EncapsulationRoutingTable_t*    routingTable;
   DuplexControl_t                 duplexControl;
   TxBuffers_t                     txBuffers;
   RxRawCb_t*                      rxRawCb;
   RxdAddressedPacketCb_t*         rxAddressedPacketCb;
   //RxdAddressedPacketCb_t*         rxAddressedFromRouterCb;
   EncAddr_t                       address;
} Mrts2Device_t;

int32_t Mrts2_Dev_Init(Mrts2Device_t* dev, BaseObject_t* owner, size_t rxBufferSize, TxBufferEntry_t* blankEntries, int numTxEntries, size_t txBufferSize);

int32_t Mrts2_Dev_Receive( Mrts2Device_t* dev, void* bytes, size_t numBytes);

int32_t Mrts2_Dev_ReceiveRoger( Mrts2Device_t* dev, uint16_t period );

int32_t Mrts2_Dev_Transmit( Mrts2Device_t* dev );

int32_t Mrts2_Dev_AddToSendQueue (Mrts2Device_t* dev, void* bytes, size_t numBytes );

int32_t Mrts2_Dev_GetBufferIntoSendQueue( Mrts2Device_t *dev, BaseObject_t* srcBuffer);

int32_t Mrts2_Dev_Poll( Mrts2Device_t* dev );

int32_t Mrts2_Dev_SetupRoutingTable(Mrts2Device_t* dev, EncAddr_t myAddress, EncapsulationRoutingTable_t* newTable );

#endif

