
#ifndef __DeviceWapper_h
#define __DeviceWapper_h

#include <Python.h>

#include "DeviceInterface.h"

typedef struct Mrts2WrapperReference_t
{
   BaseObject_t     base;
   PyObject*        pyObject;
} Mrts2WrapperReference_t;


typedef struct Mrts2DevWrap_PyObject_t
{
    PyObject_HEAD
    PyObject* InitChannelCb;
    PyObject* WriteChannelCb;
    PyObject* ReadChannelCb;
    PyObject* CloseChannelCb;
    PyObject* RawDataReceivedCb;
    PyObject* PacketReceivedCb;
    PyObject* AddressedPacketReceivedCb;
    Mrts2WrapperReference_t wrappingReference;
    Mrts2Device_t device;
    
} Mrts2DevWrap_PyObject_t;


#endif
