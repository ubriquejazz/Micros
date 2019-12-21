#ifndef __mrtsErrors_h
#define __mrtsErrors_h

/*!\name      mrtsErrors.h
 *
 * \brief     This file contains the enumeration for errors.
 *            ALL ERRORS MUST BE NEGATIVE.
 *
 */

typedef enum MrtsErrors_e
{
   merNoError                =   0,
   merNullPointer            =  -1,
   merRangeExceeded          =  -2,
   merIOError                =  -3,
   merBufferTooSmall         =  -4,
   merBufferOverflow         =  -5,
   merInvalidCrcLen          =  -6,
   merCrcFail                =  -7,
   merInvalidPacketType      =  -8,
   merMutexInitFailed        =  -9,
   merMutexOperationFailed   = -10,
   merInvalidSequenceNo      = -11,
   merInvalidAddress         = -12,
   merAddressMismatch        = -13,
   merNotANumber             = -14,
   merInvalidSubnet          = -15,
   merMallocFailed           = -16,
   merBusy                   = -17,
   merMutexOwnedByOtherThread= -18,
   merDeadlock               = -19,
   merMutexMaxLocksExceeded  = -20,
   merInvalidArgument        = -21,
   merI2cLoopError           = -22,
   merIgnored                = -23,
   merOneWireError           = -24, 
   merNotImplemented         = -25,

   merUnknownError           = -32768

} MrtsErrors_e;

#endif	/* __MRTSERRORS */

