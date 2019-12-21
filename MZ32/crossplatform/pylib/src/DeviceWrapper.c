
#include "mrtsErrors.h"
#include "Osal.h"
#include "DeviceWrapper.h"
#include "OsalCommsChannel.h"
#include "DeviceInterface.h"
#include "EncapsulationRouting.h"


/*! \name     Mtrs2DevWrap_dealloc
 *
 *  \brief    Destructor function for class/
 */
static void Mtrs2DevWrap_dealloc(Mrts2DevWrap_PyObject_t* self)
{
   Py_DECREF(self->ReadChannelCb);
   Py_DECREF(self->WriteChannelCb);
   Py_DECREF(self->InitChannelCb);
   Py_DECREF(self->CloseChannelCb);
   Py_DECREF(self->RawDataReceivedCb);
   Py_DECREF(self->PacketReceivedCb);
   Py_DECREF(self->AddressedPacketReceivedCb);

   Py_TYPE(self)->tp_free((PyObject*)self);
}

int32_t Mtrs2DevWrap_WriteChannel( MrtsCommsChannel_t* mrtsChannel,  void* bytesToSend, size_t numBytesToSend );
int32_t Mtrs2DevWrap_ReadChannel( MrtsCommsChannel_t* mrtsChannel, void* receiveBuffer, size_t receiveBufferSize);
int32_t Mtrs2DevWrap_CloseChannel( MrtsCommsChannel_t* mrtsChannel);
int32_t HandleRawReceive( BaseObject_t* buffer, int32_t length);
int32_t HandlePacket( BaseObject_t* buffer, size_t packetSize, PacketType_t type, uint32_t dataIndex, size_t dataLength, bool crcOk );
int32_t HandleAddressedPacket( BaseObject_t* buffer, size_t packetSize, PacketType_t type, EncAddr_t src, EncAddr_t dst, uint16_t sequence, uint32_t dataIndex, size_t dataLength, bool crcOk );


/*!\name      Initialise_WrappedChannel
 *
 * \brief     Used to setup the comms channel for the wrapper.
 *
 * \return    >=0 OK <0 failed.
 */
int32_t Initialise_WrappedChannel( MrtsCommsChannel_t* mrtsChannel)
{
   int32_t retVal = merNullPointer;

   if (mrtsChannel)
   {
      mrtsChannel->readChannelCB = Mtrs2DevWrap_ReadChannel;
      mrtsChannel->writeChannelCB = Mtrs2DevWrap_WriteChannel;
      mrtsChannel->closeChannelCB = Mtrs2DevWrap_CloseChannel;

      retVal = merNoError;
   }
   return retVal;
}

/*!\name      Mtrs2DevWrap_new
 *
 * \brief     "Constructor" for DeviceWraper class - called every time a new
 *            wrapper object is created.
 *
 * \return    The constructed object for success or NULL for failure.
 */
static PyObject *Mtrs2DevWrap_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
   Mrts2DevWrap_PyObject_t *self;

   self = (Mrts2DevWrap_PyObject_t *)type->tp_alloc(type, 0);
   if ( self )
   {
      int32_t retVal = merNullPointer;

      // Setup self reference so we can get back to the Python object from the device
      self->wrappingReference.base.owner = NULL;
      self->wrappingReference.base.self = (BaseObject_t* ) &self->wrappingReference;
      self->wrappingReference.pyObject = (PyObject* ) self;
      self->device.channel.base.owner = &self->device;
      self->device.base.owner = (BaseObject_t *) &self->wrappingReference;
      retVal = Mrts2_Dev_Init( &self->device,  &self->wrappingReference.base, 2048, NULL, 128, 2048);
      if (retVal<0)
      {
         DPRINTF("Mrts2_Dev_Init failed =%d\n\r", retVal);
         return NULL;
      }

      self->device.encProc.rxRawCb = HandleRawReceive;
      self->device.encProc.rxdPacketCb = HandlePacket;
      self->device.encProc.rxdAddressedPacketCb = HandleAddressedPacket;
      Initialise_WrappedChannel(&self->device.channel);

      DPRINTF("%s:%d:%s Set channel callbacks self=%p readChannelCB=%p\n\r", __FILE__, __LINE__, __PRETTY_FUNCTION__, self, self->device.channel.readChannelCB );

      self->ReadChannelCb = Py_None;
      Py_INCREF(self->ReadChannelCb);

      self->WriteChannelCb = Py_None;
      Py_INCREF(self->WriteChannelCb);

      self->InitChannelCb = Py_None;
      Py_INCREF(self->InitChannelCb);

      self->CloseChannelCb = Py_None;
      Py_INCREF(self->CloseChannelCb);

      self->RawDataReceivedCb = Py_None;
      Py_INCREF(self->RawDataReceivedCb);

      self->PacketReceivedCb = Py_None;
      Py_INCREF(self->PacketReceivedCb);

      self->AddressedPacketReceivedCb = Py_None;
      Py_INCREF(self->AddressedPacketReceivedCb);
   }

    return (PyObject *) self;
}

/*! \name     Mtrs2_SetPythonWriteCb
 *
 *  \brief    Set callback function for the write function.
 *
 *  \returns  -1 on error 0 on success
 */
static int Mtrs2DevWrap_SetPythonWriteCb(Mrts2DevWrap_PyObject_t* self, PyObject* newCb, void* closure)
{
   if ( NULL == newCb )
   {
      PyErr_SetString(PyExc_TypeError, "Can not delete WriteCallback");
      return -1;
   }

   if (!PyCallable_Check(newCb))
   {
      PyErr_SetString(PyExc_TypeError, "WriteCallback must be callable.");
      return -1;
   }

   Py_DECREF(self->WriteChannelCb);
   Py_INCREF(newCb);
   self->WriteChannelCb = newCb;

   return 0;
}

static PyObject* Mtrs2DevWrap_GetPythonWriteCb(Mrts2DevWrap_PyObject_t* self, void* closure)
{
   Py_INCREF(self->WriteChannelCb);
   return self->WriteChannelCb;
}

/*! \name     Mtrs2DevWrap_SetPythonReadCb
 *
 *  \brief    Set callback function for the read function.
 *
 *  \returns  -1 on error 0 on success
 */
static int Mtrs2DevWrap_SetPythonReadCb(Mrts2DevWrap_PyObject_t* self, PyObject* newCb, void* closure)
{
   if ( NULL == newCb )
   {
      PyErr_SetString(PyExc_TypeError, "Can not delete ReadCallback");
      return -1;
   }

   if (!PyCallable_Check(newCb))
   {
      PyErr_SetString(PyExc_TypeError, "ReadCallback must be callable.");
      return -1;
   }

   DPRINTF("About to decrement callback\n\r");
   Py_DECREF(self->ReadChannelCb);
   Py_INCREF(newCb);
   DPRINTF("And to increment new callback\n\r");
   self->ReadChannelCb = newCb;

   return 0;
}

static PyObject* Mtrs2DevWrap_GetPythonReadCb(Mrts2DevWrap_PyObject_t* self, void* closure)
{
   Py_INCREF(self->ReadChannelCb);
   return self->ReadChannelCb;
}

/*! \name     Mtrs2DevWrap_SetPythonInitChannelCb
 *
 *  \brief    Set callback function for the init channel function.
 *
 *  \returns  -1 on error 0 on success
 */
static int Mtrs2DevWrap_SetPythonInitChannelCb(Mrts2DevWrap_PyObject_t* self, PyObject* newCb, void* closure)
{
   if ( NULL == newCb )
   {
      PyErr_SetString(PyExc_TypeError, "Can not delete InitChannelCallback");
      return -1;
   }

   if (!PyCallable_Check(newCb))
   {
      PyErr_SetString(PyExc_TypeError, "InitChannelCallback must be callable.");
      return -1;
   }

   DPRINTF("About to decrement callback\n\r");
   Py_DECREF(self->InitChannelCb);
   Py_INCREF(newCb);
   DPRINTF("And to increment new callback\n\r");
   self->InitChannelCb = newCb;

   return 0;
}

static PyObject* Mtrs2DevWrap_GetPythonInitChannelCb(Mrts2DevWrap_PyObject_t* self, void* closure)
{
   Py_INCREF(self->InitChannelCb);
   return self->InitChannelCb;
}



/*! \name     Mtrs2DevWrap_SetPythonCloseChannelCb
 *
 *  \brief    Set callback function for the close channel function.
 *
 *  \returns  -1 on error 0 on success
 */
static int Mtrs2DevWrap_SetPythonCloseChannelCb(Mrts2DevWrap_PyObject_t* self, PyObject* newCb, void* closure)
{
   if ( NULL == newCb )
   {
      PyErr_SetString(PyExc_TypeError, "Can not delete CloseChannelCb");
      return -1;
   }

   if (!PyCallable_Check(newCb))
   {
      PyErr_SetString(PyExc_TypeError, "CloseChannelCb must be callable.");
      return -1;
   }

   DPRINTF("About to decrement callback\n\r");
   Py_DECREF(self->CloseChannelCb);
   Py_INCREF(newCb);
   DPRINTF("And to increment new callback\n\r");
   self->CloseChannelCb = newCb;

   return 0;
}

static PyObject* Mtrs2DevWrap_GetPythonCloseChannelCb(Mrts2DevWrap_PyObject_t* self, void* closure)
{
   Py_INCREF(self->CloseChannelCb);
   return self->CloseChannelCb;
}

static int Mtrs2DevWrap_SetPythonRawDataReceivedCb(Mrts2DevWrap_PyObject_t* self, PyObject* newCb, void* closure)
{
   if ( NULL == newCb )
   {
      PyErr_SetString(PyExc_TypeError, "Can not delete RawDataReceivedCb");
      return -1;
   }

   if (!PyCallable_Check(newCb))
   {
      PyErr_SetString(PyExc_TypeError, "RawDataReceivedCb must be callable.");
      return -1;
   }

   DPRINTF("About to decrement callback\n\r");
   Py_DECREF(self->RawDataReceivedCb);
   Py_INCREF(newCb);
   DPRINTF("And to increment new callback\n\r");
   self->RawDataReceivedCb = newCb;

   return 0;
}

static PyObject* Mtrs2DevWrap_GetPythonRawDataReceivedCb(Mrts2DevWrap_PyObject_t* self, void* closure)
{
   Py_INCREF(self->RawDataReceivedCb);
   return self->RawDataReceivedCb;
}

static int Mtrs2DevWrap_SetPythonPacketReceivedCb(Mrts2DevWrap_PyObject_t* self, PyObject* newCb, void* closure)
{
   if ( NULL == newCb )
   {
      PyErr_SetString(PyExc_TypeError, "Can not delete PacketReceivedCb");
      return -1;
   }

   if (!PyCallable_Check(newCb))
   {
      PyErr_SetString(PyExc_TypeError, "PacketReceivedCb must be callable.");
      return -1;
   }

   DPRINTF("About to decrement callback\n\r");
   Py_DECREF(self->PacketReceivedCb);
   Py_INCREF(newCb);
   DPRINTF("And to increment new callback\n\r");
   self->PacketReceivedCb = newCb;

   return 0;
}

static PyObject* Mtrs2DevWrap_GetPythonPacketReceivedCb(Mrts2DevWrap_PyObject_t* self, void* closure)
{
   Py_INCREF(self->PacketReceivedCb);
   return self->PacketReceivedCb;
}

static int Mtrs2DevWrap_SetPythonAddressedPacketReceivedCb(Mrts2DevWrap_PyObject_t* self, PyObject* newCb, void* closure)
{
   if ( NULL == newCb )
   {
      PyErr_SetString(PyExc_TypeError, "Can not delete AddressedPacketReceivedCb");
      return -1;
   }

   if (!PyCallable_Check(newCb))
   {
      PyErr_SetString(PyExc_TypeError, "AddressedPacketReceivedCb must be callable.");
      return -1;
   }

   DPRINTF("About to decrement callback\n\r");
   Py_DECREF(self->AddressedPacketReceivedCb);
   Py_INCREF(newCb);
   DPRINTF("And to increment new callback\n\r");
   self->AddressedPacketReceivedCb = newCb;

   return 0;
}

static PyObject* Mtrs2DevWrap_GetPythonAddressedPacketReceivedCb(Mrts2DevWrap_PyObject_t* self, void* closure)
{
   Py_INCREF(self->AddressedPacketReceivedCb);
   return self->AddressedPacketReceivedCb;
}


/*! \name     Mtrs2DevWrap_SetPythonUseRawTerminator
 *
 *  \brief    This function is called to enable or disable the terminator used
 *            for handling raw strings. It's normally meant for use on command
 *            lines.
 */
static int Mtrs2DevWrap_SetPythonUseRawTerminator(Mrts2DevWrap_PyObject_t* self, PyObject* useRawTerminator, void* closure)
{
   if ( NULL == useRawTerminator )
   {
      PyErr_SetString(PyExc_TypeError, "Can not enable or disable raw terminator.");
      return -1;
   }

   if (!PyBool_Check(useRawTerminator))
   {
      PyErr_SetString(PyExc_TypeError, "UseRawTerminator must be Boolean.");
      return -1;
   }

   self->device.encProc.useRawTerminator = PyObject_IsTrue(useRawTerminator);

   return 0;
}

/*! \name     Mtrs2DevWrap_GetPythonUseRawTerminator
 *
 *  \brief    This function is called find out whether the terminator has been
 *            enabled or disabled.
 */
static PyObject* Mtrs2DevWrap_GetPythonUseRawTerminator(Mrts2DevWrap_PyObject_t* self, void* closure)
{
   return PyBool_FromLong(self->device.encProc.useRawTerminator);
}

/*! \name     Mtrs2DevWrap_WriteChannel
 *
 *  \brief    This function is called to write to the channel using a callback
 *            into the Python host.a.WriteCallback=Wibble
 */
int32_t Mtrs2DevWrap_WriteChannel( MrtsCommsChannel_t* mrtsChannel,  void* bytesToSend, size_t numBytesToSend )
{
   int32_t retVal = merNullPointer;

   if (mrtsChannel && bytesToSend && numBytesToSend)
   {
      Mrts2WrapperReference_t* wrapperRef = (Mrts2WrapperReference_t* ) (mrtsChannel->base.owner)->owner;
      Mrts2DevWrap_PyObject_t* wrap = (Mrts2DevWrap_PyObject_t *) wrapperRef->pyObject;

      if (PyCallable_Check(wrap->WriteChannelCb))
      {
         PyObject* arglist = Py_BuildValue("y#", bytesToSend, numBytesToSend);
         if (arglist)
         {
            PyObject *result = PyObject_CallObject(wrap->WriteChannelCb, arglist);
            if (result)
            {
               /* Now we need to work out what the result is...*/
               retVal =0;
               Py_DECREF(result);
            }
            Py_DECREF(arglist);
         }
      }
   }

   return retVal;
}

/*!\name      Mtrs2DevWrap_ReadChannel
 *
 * \brief     This function requests data from the python application.
 *
 * \return    Positive/Zero OK, negative error
 */
int32_t Mtrs2DevWrap_ReadChannel( MrtsCommsChannel_t* mrtsChannel, void* receiveBuffer, size_t receiveBufferSize)
{
   int32_t retVal = merNullPointer;

   DPRINTF("Mtrs2DevWrap_ReadChannel\n\r");

   if (mrtsChannel && receiveBuffer && receiveBufferSize && mrtsChannel->base.owner && mrtsChannel->base.owner->owner)
   {
      DPRINTF("Going to wrap %p %p\n\r", &mrtsChannel->base, mrtsChannel->base.owner);
      Mrts2WrapperReference_t* wrapperRef = (Mrts2WrapperReference_t* ) (mrtsChannel->base.owner)->owner;
      Mrts2DevWrap_PyObject_t* wrap = (Mrts2DevWrap_PyObject_t *) wrapperRef->pyObject;

      DPRINTF("wrap=%p ReadChannelCB=%p\n\r", wrap, wrap->ReadChannelCb);
      if (PyCallable_Check(wrap->ReadChannelCb))
      {
         PyObject* arglist = Py_BuildValue("(i)", receiveBufferSize);
         if (arglist)
         {
            PyObject *result = PyObject_CallObject(wrap->ReadChannelCb, arglist);
            if (result)
            {
               /* Now we need to work out what the result is...*/
               char* rxBytes=0;
               Py_ssize_t length;
               DPRINTF("About to parse\n\r");
               if (PyArg_ParseTuple(result, "|z#", &rxBytes, &length))
               {
                  /* The Py_ssize_t is extremely big and even using it as the
                   * type for the loop counter causes it to overflow.
                   */
                  retVal = (int) length;
                  memcpy(receiveBuffer, rxBytes, retVal);
                  DPRINTF("Tuple result %d = ", retVal);
                  #ifdef DPRINTF_ENABLED
                  if (rxBytes)
                  {
                     for (int i=0; i<retVal; i++)
                     {
                        uint8_t theByte = ((uint8_t*) receiveBuffer)[i];
                        DPRINTF("%02x ", theByte);
                     }
                  }
                  #endif
               }
               else
               {
                  PyErr_Clear();
                  DPRINTF("Failed to parse\n\r");
               }
               Py_DECREF(result);
               DPRINTF("Py_DECREF(result);\n\r");
            }
            Py_DECREF(arglist);
         }
         else
         {
            DPRINTF("Failed to parse\n\r");
         }
      }
      else
      {
         retVal = merIOError;
      }
   }
   DPRINTF("Leaving %d\n\r", retVal);

   return retVal;
}

int32_t Mtrs2DevWrap_CloseChannel( MrtsCommsChannel_t* mrtsChannel)
{
   int32_t retVal = merNoError;

   close(mrtsChannel->settings.fileDescriptor);
   mrtsChannel->settings.fileDescriptor=-1;

   return retVal;
}


/*!\name      mrts2Dev_getseters
 *
 * \brief     Array containing definitions for properties of mrts device
 *            including name + get/set function pointers.
 */
static PyGetSetDef mrts2Dev_getseters[] =
{
   {
      (char* ) "WriteCallback",
      (getter)Mtrs2DevWrap_GetPythonWriteCb, (setter)Mtrs2DevWrap_SetPythonWriteCb,
      (char* ) "Callback used for the client application to send raw data.",
      NULL
   },
   {
      (char* ) "ReadCallback",
      (getter)Mtrs2DevWrap_GetPythonReadCb, (setter)Mtrs2DevWrap_SetPythonReadCb,
      (char* ) "Callback used for the client application to read raw data.",
      NULL
   },
   {
      (char* ) "InitChannelCallback",
      (getter)Mtrs2DevWrap_GetPythonInitChannelCb, (setter)Mtrs2DevWrap_SetPythonInitChannelCb,
      (char* ) "Callback used for the client application to read raw data.",
      NULL
   },
   {
      (char* ) "CloseChannelCallback",
      (getter)Mtrs2DevWrap_GetPythonCloseChannelCb, (setter)Mtrs2DevWrap_SetPythonCloseChannelCb,
      (char* ) "Callback used for the client application to read raw data.",
      NULL
   },
   {
      (char* ) "ReceivedRawDataCallback",
      (getter)Mtrs2DevWrap_GetPythonRawDataReceivedCb, (setter)Mtrs2DevWrap_SetPythonRawDataReceivedCb,
      (char* ) "Callback used to pass non-encapsulated data as it was received.",
      NULL
   },
   {
      (char* ) "ReceivedPacketCallback",
      (getter)Mtrs2DevWrap_GetPythonPacketReceivedCb, (setter)Mtrs2DevWrap_SetPythonPacketReceivedCb,
      (char* ) "Callback when an encapsulated packet has been received.",
      NULL
   },
   {
      (char* ) "ReceivedAddressedPacketCallback",
      (getter)Mtrs2DevWrap_GetPythonAddressedPacketReceivedCb, (setter)Mtrs2DevWrap_SetPythonAddressedPacketReceivedCb,
      (char* ) "Callback when an encapsulated packet for this address has been received.",
      NULL
   },
   {
      (char* ) "UseRawTerminator",
      (getter) Mtrs2DevWrap_GetPythonUseRawTerminator, (setter) Mtrs2DevWrap_SetPythonUseRawTerminator,
      (char* ) "Enable the use the raw terminator, typically for non encapsulated command lines.",
      NULL
   },
   {NULL}  /* Sentinel */
};

int32_t HandleRawReceive ( BaseObject_t* buffer, int32_t length)
{
   int32_t retVal = merNullPointer;
   if (buffer)
   {
      DPRINTF("HandleRawReceive, length=%d waiting=%d\n\r", length, CircularBuffer_GetReceivedWaiting((CircularBuffer_t*) buffer));
      Mrts2Device_t* device = (Mrts2Device_t* ) buffer->owner;
      if (device->base.owner)
      {
         Mrts2WrapperReference_t* wrapperRef = (Mrts2WrapperReference_t* ) device->base.owner;
         Mrts2DevWrap_PyObject_t* wrap = (Mrts2DevWrap_PyObject_t *) wrapperRef->pyObject;
         if (wrap && PyCallable_Check(wrap->RawDataReceivedCb))
         {
            uint8_t* extracted = malloc(length);
            if (extracted)
            {
               int32_t numExtracted = CircularBuffer_GetBytes( (CircularBuffer_t*) buffer, extracted, length);
               if ( numExtracted > 0 )
               {
                  int i;
                  DPRINTF("numExtracted %d extracted\n\r", numExtracted);
                  for (i=0; i<numExtracted; i++)
                  {
                     DPRINTF("%02x ", extracted[i]);
                  }
                  DPRINTF(" done\n\r");
                  PyObject* arglist = Py_BuildValue("(y#)", extracted, numExtracted);
                  if (arglist)
                  {
                     PyObject *result = PyObject_CallObject(wrap->RawDataReceivedCb, arglist);
                     if (result)
                     {
                        /* Do we care about the result? */
                        DPRINTF("We have a result\n\r");
                     }
                     else
                     {
                        DPRINTF("We don't have a result\n\r");
                     }
                  }
                  else
                  {
                     DPRINTF("It didn't like the args\n\r");
                  }
               }

               free(extracted);
            }
         }
         else
         {
            DPRINTF("Not callable\n\r");
         }
      }
   }

   return retVal;
}


/*!\name      HandlePacket
 *
 * \brief     This is the handler callback function that is called when the
 *            encapsulation processor encounters a packet that doesn't contain
 *            address information. In turn the Python callback function is
 *            called with the following parameters:
 *
 *            payload
 *            type
 *            crcOK
 *
 * \param     buffer         Pointer to base object that needs to be cast to
 *                           the circular buffer object that contains the
 *                           entire input including this packet.
 *
 * \param     packetSize     Size of whole packet.
 *
 * \param     dataIndex      Start postion of data within the packet.
 *
 * \param     dataLength     Length of data within the packet.
 *
 * \param     crcOK          Indicates whether the CRC computation passed.
 *
 * \return                   >=0 Success and size of packet
 *                           <0  Error
 *
 */
int32_t HandlePacket( BaseObject_t* buffer, size_t packetSize, PacketType_t type, uint32_t dataIndex, size_t dataLength, bool crcOk )
{
   int32_t retVal= merNullPointer;
   if (buffer)
   {
      DPRINTF("HandlePacket, packetSize=%d waiting=%d\n\r", packetSize, CircularBuffer_GetReceivedWaiting((CircularBuffer_t*) buffer));
      Mrts2Device_t* device = (Mrts2Device_t* ) buffer->owner;
      if (device->base.owner)
      {
         Mrts2WrapperReference_t* wrapperRef = (Mrts2WrapperReference_t* ) device->base.owner;
         Mrts2DevWrap_PyObject_t* wrap = (Mrts2DevWrap_PyObject_t *) wrapperRef->pyObject;
         if (wrap && PyCallable_Check(wrap->PacketReceivedCb))
         {
            uint8_t* extracted = malloc(packetSize);
            if (extracted)
            {
               int32_t numExtracted = CircularBuffer_GetBytes( (CircularBuffer_t*) buffer, extracted, packetSize);
               if ( packetSize == (size_t) numExtracted )
               {
                  int i;
                  DPRINTF("numExtracted %d extracted\n\r", numExtracted);
                  for (i=0; i<numExtracted; i++)
                  {
                     DPRINTF("%02x ", extracted[i]);
                  }
                  DPRINTF(" done\n\r");
                  PyObject* arglist = Py_BuildValue("(y#)ii", extracted+dataIndex, dataLength, (int) type, (int) crcOk);
                  if (arglist)
                  {
                     PyObject *result = PyObject_CallObject(wrap->PacketReceivedCb, arglist);
                     if (result)
                     {
                        /* Do we care about the result? */
                        DPRINTF("We have a result\n\r");
                     }
                     else
                     {
                        DPRINTF("We don't have a result\n\r");
                     }
                  }
                  else
                  {
                     DPRINTF("It didn't like the args\n\r");
                  }
               }

               free(extracted);
            }
         }
         else
         {
            DPRINTF("Packet handler function not callable\n\r");
         }
      }
   }

   return retVal;
}


/*!\name      HandleAddressedPacket
 *
 * \brief     This is the handler callback function that is called when the
 *            encapsulation processor encounters a packet that contains
 *            address information. In turn the Python callback function is
 *            called with the following parameters:
 *
 *            payload
 *            source address
 *            dest address
 *            sequence number
 *            type
 *            crcOK
 *
 * \param     buffer         Pointer to base object that needs to be cast to
 *                           the circular buffer object that contains the
 *                           entire input including this packet.
 *
 * \param     packetSize     Size of whole packet.
 *
 * \param     dataIndex      Start postion of data within the packet.
 *
 * \param     dataLength     Length of data within the packet.
 *
 * \param     crcOK          Indicates whether the CRC computation passed.
 *
 * \return                   >=0 Success and size of packet
 *                           <0  Error
 *
 */
int32_t HandleAddressedPacket( BaseObject_t* buffer, size_t packetSize, PacketType_t type, EncAddr_t src, EncAddr_t dst, uint16_t sequence, uint32_t dataIndex, size_t dataLength, bool crcOk )
{
   int32_t retVal= merNullPointer;
   if (buffer)
   {
      DPRINTF("HandleAddressedPacket, packetSize=%d waiting=%d\n\r", packetSize, CircularBuffer_GetReceivedWaiting((CircularBuffer_t*) buffer));
      Mrts2Device_t* device = (Mrts2Device_t* ) buffer->owner;
      if (device->base.owner)
      {
         Mrts2WrapperReference_t* wrapperRef = (Mrts2WrapperReference_t* ) device->base.owner;
         Mrts2DevWrap_PyObject_t* wrap = (Mrts2DevWrap_PyObject_t *) wrapperRef->pyObject;
         if (wrap && PyCallable_Check(wrap->AddressedPacketReceivedCb))
         {
            uint8_t* extracted = malloc(packetSize);
            if (extracted)
            {
               int32_t numExtracted = CircularBuffer_GetBytes( (CircularBuffer_t*) buffer, extracted, packetSize);
               if ( packetSize == (size_t) numExtracted )
               {
                  int i;
                  DPRINTF("numExtracted %d extracted\n\r", numExtracted);
                  for (i=0; i<numExtracted; i++)
                  {
                     DPRINTF("%02x ", extracted[i]);
                  }
                  DPRINTF(" done\n\r");
                  PyObject* arglist = Py_BuildValue("(y#)iiiii", extracted+dataIndex, dataLength, (int) src, (int) dst, (int) sequence, (int) type, (int) crcOk);
                  if (arglist)
                  {
                     PyObject *result = PyObject_CallObject(wrap->AddressedPacketReceivedCb, arglist);
                     if (result)
                     {
                        /* Do we care about the result? */
                        DPRINTF("We have a result\n\r");
                     }
                     else
                     {
                        DPRINTF("We don't have a result\n\r");
                     }
                  }
                  else
                  {
                     DPRINTF("It didn't like the args\n\r");
                  }
               }

               free(extracted);
            }
         }
         else
         {
            DPRINTF("Addressed packet handler function callable\n\r");
         }
      }
   }

   return retVal;
}

/*!\name      Mrts2DevWrap_Poll
 *
 * \brief     This function polls (or prods?) the device to look at the input
 *            process the state machine and if necessary send any callbacks
 *
 * \return    None (effectively void)
 */
static PyObject* Mrts2DevWrap_Poll( Mrts2DevWrap_PyObject_t* self, PyObject* args )
{
   if (self)
   {
      Mrts2_Dev_Poll( &self->device );
   }
   Py_INCREF(Py_None);
   return Py_None;
}


/*!\name      Mrts2DevWrap_EncapsulateAddressed
 *
 * \brief     This function is used to encapsulate a block of data in an
 *            addressed packet.
 *
 * \return    Bytes object containing encapsulated data
 */
static PyObject* Mrts2DevWrap_EncapsulateAddressed( Mrts2DevWrap_PyObject_t* self, PyObject* args )
{
   PyObject* retObj = 0;

   if (self && args)
   {
      unsigned int type, src, dst, crcLen, sequence;
      uint8_t* payload;
      Py_ssize_t payloadLen=sizeof(payload);

      if (PyArg_ParseTuple(args, "z#IIIII", &payload, &payloadLen, &type, &src, &dst, &sequence, &crcLen))
      {
         int encodedLen = crcLen + ENC_ADDRESSED_PACKET_OVERHEAD_NO_CRC + (int) payloadLen;
         uint8_t* destBuffer = malloc( (int) encodedLen);
         if (destBuffer)
         {
            int32_t result = EncapsulationProcessor_EncodeAddressedPacket( destBuffer, encodedLen, type, src, dst, (uint16_t) sequence, payload, (int) payloadLen, crcLen);
            if (result>=0)
            {
               int i;
               DPRINTF("%s Building new value (%d) :", __PRETTY_FUNCTION__, encodedLen);
               for (i=0; i<encodedLen; i++)
               {
                  DPRINTF(" %02x", destBuffer[i]);
               }
               DPRINTF("\n\r");
               retObj = Py_BuildValue("y#", destBuffer, encodedLen );
            }
            else
            {
               PyErr_SetString(PyExc_ValueError, "Incorrect parameters supplied.");
            }
         }
         free(destBuffer);
      }
      else
      {
         PyErr_SetString(PyExc_TypeError, "Incorrect parameters supplied.");
         DPRINTF("Failed to parse\n\r");
      }
   }

   return retObj;
}

/*!\name      Mrts2DevWrap_Encapsulate
 *
 * \brief     This function is used to encapsulate a block of data in a non
 *            addressed packet.
 *
 * \return    Bytes object containing encapsulated data
 */
static PyObject* Mrts2DevWrap_Encapsulate( Mrts2DevWrap_PyObject_t* self, PyObject* args )
{
   PyObject* retObj = 0;

   if (self && args)
   {
      unsigned int type, crcLen;
      uint8_t* payload;
      Py_ssize_t payloadLen=sizeof(payload);

      if (PyArg_ParseTuple(args, "z#II", &payload, &payloadLen, &type, &crcLen))
      {
         int encodedLen = crcLen + ENC_ADDRESSED_PACKET_OVERHEAD_NO_CRC + (int) payloadLen;
         uint8_t* destBuffer = malloc( (int) encodedLen);
         if (destBuffer)
         {
            int32_t result = EncapsulationProcessor_EncodePacket( destBuffer, encodedLen, type, payload, (int) payloadLen, crcLen);
            if (result>=0)
            {
               int i;
               DPRINTF("%s Building new value (%d) :", __PRETTY_FUNCTION__, encodedLen);
               for (i=0; i<encodedLen; i++)
               {
                  DPRINTF(" %02x", destBuffer[i]);
               }
               DPRINTF("\n\r");
               retObj = Py_BuildValue("y#", destBuffer, encodedLen );
            }
         }
         free(destBuffer);
      }
      else
      {
         DPRINTF("Failed to parse\n\r");
      }
   }

   return retObj;
}

/*!\name      mrts2_methods
 *
 * \brief     Table containing methods supported by Mrts device class
 *
 */
static PyMethodDef mrts2_methods[] = {
    {"poll", (PyCFunction) Mrts2DevWrap_Poll, METH_NOARGS,
     "Poll the wrapped device."
    },
    {"encapsulate", (PyCFunction) Mrts2DevWrap_Encapsulate, METH_VARARGS,
     "Encapsulate data in a packet."
    },
    {"encapsulateAddressed", (PyCFunction) Mrts2DevWrap_EncapsulateAddressed, METH_VARARGS,
     "Encapsulate data in an addressed packet."
    },
    {NULL}  /* Sentinel */
};

/*!\name      MrtsDevWrapperType
 *
 * \brief     Type definition
 */
static PyTypeObject MrtsDevWrapperType = {
   PyVarObject_HEAD_INIT(NULL, 0)
   "deviceWrapper.DeviceWrapper", /* tp_name */
   sizeof(Mrts2DevWrap_PyObject_t),             /* tp_basicsize */
   0,                         /* tp_itemsize */
   (destructor)Mtrs2DevWrap_dealloc, /* tp_dealloc */
   0,                         /* tp_print */
   0,                         /* tp_getattr */
   0,                         /* tp_setattr */
   0,                         /* tp_reserved */
   0,                         /* tp_repr */
   0,                         /* tp_as_number */
   0,                         /* tp_as_sequence */
   0,                         /* tp_as_mapping */
   0,                         /* tp_hash  */
   0,                         /* tp_call */
   0,                         /* tp_str */
   0,                         /* tp_getattro */
   0,                         /* tp_setattro */
   0,                         /* tp_as_buffer */
   Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,   /* tp_flags */
   "DeviceWrapper objects",           /* tp_doc */
   0,                         /* tp_traverse */
   0,                         /* tp_clear */
   0,                         /* tp_richcompare */
   0,                         /* tp_weaklistoffset */
   0,                         /* tp_iter */
   0,                         /* tp_iternext */
   mrts2_methods,             /* tp_methods */
   NULL/*Noddy_members*/,             /* tp_members */
   mrts2Dev_getseters,           /* tp_getset */
   0,                         /* tp_base */
   0,                         /* tp_dict */
   0,                         /* tp_descr_get */
   0,                         /* tp_descr_set */
   0,                         /* tp_dictoffset */
   (initproc)NULL/*Noddy_init*/,      /* tp_init */
   0,                         /* tp_alloc */
   Mtrs2DevWrap_new,                 /* tp_new */
};

static PyModuleDef deviceWrapperModule =
{
   PyModuleDef_HEAD_INIT,
   "deviceWrapper",
   "Device wrapper for mrts2 device.",
   -1,
   NULL, NULL, NULL, NULL, NULL
};

/*!\name      PyInit_DeviceWrapper
 *
 * \brief     The initialisation function called when the library is loaded -
 *            IT IS VERY IMPORTANT THE NAME FOLLOWING PyInit_ matches that of
 *            the shared object or Python will not load it.
 *
 */
PyMODINIT_FUNC PyInit_DeviceWrapper(void)
{
   PyObject* module = NULL;

   if (PyType_Ready(&MrtsDevWrapperType)>=0)
   {
      module = PyModule_Create(&deviceWrapperModule);
      if (module)
      {
         Py_INCREF(&MrtsDevWrapperType);
         PyModule_AddObject( module, "DeviceWrapper", (PyObject* ) &MrtsDevWrapperType);
      }
   }

   return module;
}
