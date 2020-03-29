#ifndef _SYSTEM_H
    #define _SYSTEM_H
//major compiler device/libraries/configuration settings
//thk 2/5/2012
//this leave system and perpherial clocks @ 8Mhz using internal fast oscillator
#include <plib.h>
// DEVCFG3
// USERID = No Setting
#pragma config IOL1WAY = ON             // Peripheral Pin Select Configuration (Allow only one reconfiguration)
// DEVCFG2
#pragma config UPLLEN = OFF             // USB PLL Enable (Disabled and Bypassed)
// DEVCFG1
#pragma config FNOSC = FRC     // Oscillator Selection Bits Fast RC osc 8MHZ
#pragma config FSOSCEN = ON             // Secondary Oscillator Enable (Enabled)
#pragma config IESO = OFF                // Internal/External Switch Over (Disabled)
#pragma config POSCMOD = OFF            // Primary Oscillator Configuration (Primary osc disabled)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_1           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/1)8MHZ
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (WDT Disabled (SWDTEN Bit Controls))

// DEVCFG0
#pragma config JTAGEN = OFF             // JTAG Enable (JTAG Disabled)
#pragma config ICESEL = ICS_PGx1        // ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF
#endif // _SYSTEM_H