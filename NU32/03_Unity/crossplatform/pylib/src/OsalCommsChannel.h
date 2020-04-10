
#ifndef __OsalCommsChannel_h
#define __OsalCommsChannel_h

/*!\name      OsalCommsChannel.h
 *
 * \brief     This file contains the definitions for an abstracted
 *            communications interface. MrtsCommsChannel_t dev.channel
 *            (see DeviceInterface)
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "mrtsTypes.h"

#ifdef LINUX_BUILD

#endif
#ifdef PIC32_BUILD
   #include "driver/driver_common.h"
#endif


struct MrtsCommsChannel_t;

typedef struct MrtsCommsSettings_t
{
   BaseObject_t                    base;
   void*                           rawSettings;
   bool                            outputEnabled;
   bool                            inputEnabled;
#ifdef LINUX_BUILD
   int                             fileDescriptor;
#endif 
#ifdef PIC32_BUILD
   DRV_HANDLE                      serialHandle;
#endif
   
} MrtsCommsSettings_t;

/*\name    MrtsCommsWriteChannelCB_t
 *
 *\brief   Definition of callback used to write to a comms channel.
 *
 */
typedef int32_t (MrtsCommsWriteChannelCB_t ) (struct MrtsCommsChannel_t* mrtsChannel, void* bytesToSend, size_t numBytesToSend);
typedef int32_t (MrtsCommsReadChannelCB_t )  (struct MrtsCommsChannel_t* mrtsChannel, void* receiveBuffer, size_t receiveBufferSize);
typedef int32_t (MrtsCommsCloseChannelCB_t ) (struct MrtsCommsChannel_t* mrtsChannel);
typedef int32_t (MrtsCommsEnableDisableCB_t ) (struct MrtsCommsChannel_t* mrtsChannel, bool enable);

typedef struct MrtsCommsChannel_t
{
   BaseObject_t                    base;
   MrtsCommsSettings_t             settings;
   MrtsCommsWriteChannelCB_t*      writeChannelCB;
   MrtsCommsReadChannelCB_t*       readChannelCB;
   MrtsCommsCloseChannelCB_t*      closeChannelCB;
   MrtsCommsEnableDisableCB_t*     enableReadCB;
   MrtsCommsEnableDisableCB_t*     enableWriteCB;
   
} MrtsCommsChannel_t;


int32_t MrtsWriteChannel( MrtsCommsChannel_t* mrtsChannel,  void* bytesToSend, size_t numBytesToSend );
int32_t MrtsReadChannel( MrtsCommsChannel_t* mrtsChannel, void* receiveBuffer, size_t receiveBufferSize);
int32_t MrtsCloseChannel( MrtsCommsChannel_t* mrtsChannel);
int32_t MrtsEnableReadChannel( MrtsCommsChannel_t* mrtsChannel, bool enable);
int32_t MrtsEnableWriteChannel( MrtsCommsChannel_t* mrtsChannel, bool enable);

#endif /* _OsalCommsChannel_H_ */
