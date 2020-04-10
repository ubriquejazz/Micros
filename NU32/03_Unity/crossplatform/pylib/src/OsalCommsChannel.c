/*!\name      OsalCommsChannel.c
 *
 * \brief     Provides an abstraction to the communications channel and allows
 *            channels of different types across the system, hence all the
 *            callbacks. MrtsCommsChannel_t dev.channel (see DeviceInterface)
 *
 */
#include "Osal.h"
#include "mrtsErrors.h"
#include "mrtsTypes.h"
#include "OsalCommsChannel.h"

int32_t MrtsWriteChannel(MrtsCommsChannel_t* mrtsChannel,  void* bytesToSend, size_t numBytesToSend )
{
   int32_t retVal = merNullPointer;

   if (mrtsChannel && bytesToSend && mrtsChannel->writeChannelCB)
   {
      retVal = mrtsChannel->writeChannelCB(mrtsChannel, bytesToSend, numBytesToSend);
   }

   return retVal;
}

int32_t MrtsReadChannel(struct MrtsCommsChannel_t* mrtsChannel, void* receiveBuffer, size_t receiveBufferSize)
{
   int32_t retVal = merNullPointer;
   DPRINTF("%s:%d:%s mrtsChannel=%p mrtsChannel->readChannelCB=%p\n\r", __FILE__, __LINE__, __PRETTY_FUNCTION__, mrtsChannel, mrtsChannel->readChannelCB);
   if (!mrtsChannel) DPRINTF("%s:%d:%s mrtsChannel=0\n\r", __FILE__, __LINE__, __PRETTY_FUNCTION__);
   if (!receiveBuffer) DPRINTF("%s:%d:%s receiveBuffer=0\n\r", __FILE__, __LINE__, __PRETTY_FUNCTION__);
   if (!mrtsChannel->readChannelCB) DPRINTF("%s:%d:%s mrtsChannel->readChannelCB=0\n\r", __FILE__, __LINE__, __PRETTY_FUNCTION__);
   if (mrtsChannel && receiveBuffer && mrtsChannel->readChannelCB)
   {
      retVal = mrtsChannel->readChannelCB(mrtsChannel, receiveBuffer, receiveBufferSize);
   }

   return retVal;
}

int32_t MrtsCloseChannel(struct MrtsCommsChannel_t* mrtsChannel)
{
   int32_t retVal = merNullPointer;

   if (mrtsChannel && mrtsChannel->closeChannelCB)
   {
      mrtsChannel->closeChannelCB(mrtsChannel);
      retVal = merNoError;
   }

   return retVal;
}

int32_t MrtsEnableReadChannel( MrtsCommsChannel_t* mrtsChannel, bool enable)
{
   int32_t retVal = merNullPointer;

   if (mrtsChannel && mrtsChannel->enableReadCB)
   {
      mrtsChannel->enableReadCB(mrtsChannel, enable);
      retVal = merNoError;
   }

   return retVal;
}

int32_t MrtsEnableWriteChannel( MrtsCommsChannel_t* mrtsChannel, bool enable)
{
   int32_t retVal = merNullPointer;

   if (mrtsChannel && mrtsChannel->enableWriteCB)
   {
      mrtsChannel->enableWriteCB(mrtsChannel, enable);
      retVal = merNoError;
   }

   return retVal;
}

