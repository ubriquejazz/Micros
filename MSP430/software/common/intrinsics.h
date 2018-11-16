/**************************************************
 *
 * Intrinsic functions for the MSP430 IAR Systems C/EC++ Compiler.
 *
 * Copyright 2002 IAR Systems. All rights reserved.
 *
 * $Revision: 1.11 $
 *
 **************************************************/

#ifndef __INTRINSICS_H
#define __INTRINSICS_H

#pragma language=extended


/*
 * The return value of "__get_interrupt_state".
 */

typedef unsigned short istate_t;

#ifdef __cplusplus
extern "C"
{
#endif

  __intrinsic void __no_operation(void);
  __intrinsic void __enable_interrupt(void);
  __intrinsic void __disable_interrupt(void);

  __intrinsic istate_t __get_interrupt_state(void);
  __intrinsic void     __set_interrupt_state(istate_t);

  __intrinsic void __op_code(unsigned short);

  __intrinsic unsigned short __swap_bytes(unsigned short);

  __intrinsic void           __bic_SR_register(unsigned short);
  __intrinsic void           __bis_SR_register(unsigned short);
  __intrinsic unsigned short __get_SR_register(void);
  __intrinsic void           __bic_SR_register_on_exit(unsigned short);
  __intrinsic void           __bis_SR_register_on_exit(unsigned short);
  __intrinsic unsigned short __get_SR_register_on_exit(void);

  __intrinsic unsigned short __bcd_add_short(unsigned short, unsigned short);
  __intrinsic unsigned long  __bcd_add_long (unsigned long , unsigned long);
  __intrinsic unsigned long long __bcd_add_long_long(
    unsigned long long, unsigned long long);

  /* Support for efficient switch:es. E.g. switch(__even_in_range(TAIV, 10)) */
  __intrinsic unsigned short __even_in_range(unsigned short, unsigned short);

  /*
   * The following intrinsic functions are only available when the
   * corresponding register is locked.
   */

  __intrinsic unsigned short __get_R4_register(void);
  __intrinsic void           __set_R4_register(unsigned short);

  __intrinsic unsigned short __get_R5_register(void);
  __intrinsic void           __set_R5_register(unsigned short);

  __intrinsic unsigned short __get_SP_register(void);
  __intrinsic void           __set_SP_register(unsigned short);

#ifdef __cplusplus
}
#endif


/*
 * Alias for locations used for global register variables.  For example,
 * "__no_init __regvar int x @ __R4;".
 */

#define __R4 4
#define __R5 5

/*
 * Control bits in the processor status register, SR.
 */

#define __SR_GIE     (1<<3)
#define __SR_CPU_OFF (1<<4)
#define __SR_OSC_OFF (1<<5)
#define __SR_SCG0    (1<<6)
#define __SR_SCG1    (1<<7)


/*
 * Functions for controlling the processor operation modes.
 */

#define __low_power_mode_0() (__bis_SR_register(  __SR_GIE      \
                                                | __SR_CPU_OFF))

#define __low_power_mode_1() (__bis_SR_register(  __SR_GIE      \
                                                | __SR_CPU_OFF  \
                                                | __SR_SCG0))

#define __low_power_mode_2() (__bis_SR_register(  __SR_GIE      \
                                                | __SR_CPU_OFF  \
                                                | __SR_SCG1))

#define __low_power_mode_3()                    \
  (__bis_SR_register(  __SR_GIE                 \
                     | __SR_CPU_OFF             \
                     | __SR_SCG0                \
                     | __SR_SCG1))

#define __low_power_mode_4()                    \
  (__bis_SR_register(  __SR_GIE                 \
                     | __SR_CPU_OFF             \
                     | __SR_SCG0                \
                     | __SR_SCG1                \
                     | __SR_OSC_OFF))

#define __low_power_mode_off_on_exit()          \
  (__bic_SR_register_on_exit(  __SR_CPU_OFF     \
                             | __SR_SCG0        \
                             | __SR_SCG1        \
                             | __SR_OSC_OFF))


#pragma language=default

#endif /* __INTRINSICS_H */
