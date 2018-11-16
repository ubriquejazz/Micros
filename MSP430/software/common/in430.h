/**************************************************
 *
 * Intrinsic functions for the MSP430 IAR Systems C/EC++ Compiler
 * provided for backward compatibility with version 1 of the compiler.
 *
 * Copyright 2002-2003 IAR Systems. All rights reserved.
 *
 * $Revision: 1.5 $
 *
 **************************************************/

#ifndef __IN430_H
#define __IN430_H

#include "intrinsics.h"

#pragma language=extended

#ifdef __cplusplus
extern "C"
{
#endif

  __intrinsic unsigned short _BIS_SR(unsigned short);   //set

  __intrinsic unsigned short _BIC_SR(unsigned short);   //clear

  __intrinsic unsigned short _BIS_SR_IRQ(unsigned short);

  __intrinsic unsigned short _BIC_SR_IRQ(unsigned short);

  __intrinsic unsigned short _BIS_NMI_IE1(unsigned short);

#ifdef __cplusplus
}
#endif


/*
 * Convenience macros mapping old names to new.
 */

#define _DINT()        __disable_interrupt()
#define _EINT()        __enable_interrupt()
#define _NOP()         __no_operation()
#define _OPC(x)        __op_code(x)
#define _SWAP_BYTES(x) __swap_bytes(x)

#define monitor        __monitor
#define no_init        __no_init

#pragma language=default

#endif /* __IN430_H */
