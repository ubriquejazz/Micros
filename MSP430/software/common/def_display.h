//hd44780.h
// HD44780 Instructions and Parameter definitions
//
// Copyright (C) 1997, Ian Harries & Imperial College, London, UK
//
// DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
// ======================= === === === === === === === ===
#define Clear_Display 0x01 // 0 0 0 0 0 0 0 1
#define Return_Home 0x02 // 0 0 0 0 0 0 1 *
#define Set_Entry_Mode 0x04 // 0 0 0 0 0 1 I/D SH
#define Set_Display 0x08 // 0 0 0 0 1 D C B
#define Set_Cursor_and_Display_Shift 0x10 // 0 0 0 1 S/C R/L * *
#define Set_Function 0x20 // 0 0 1 DL N F * *
#define Set_CGRAM_Address 0x40 // 0 1 A5 A4 A3 A2 A1 A0
#define Set_DDRAM_Address 0x80 // 1 A6 A5 A4 A3 A2 A1 A0

// HD44780 Parameters
// ================
// Los parámetros deben pasarse cada vez que se emplea alguna de las instrucciones
// Set_Entry_Mode
// ==============
#define Decrement_Address 0x00 // . . . . . . 0 .
#define Increment_Address 0x02 // . . . . . . 1 .
#define Shift_Display_Off 0x00 // . . . . . . . 0
#define Shift_Display_On 0x01 // . . . . . . . 1
// Set_Display
// ===========
#define Display_Off 0x00 // . . . . . 0 . .
#define Display_On 0x04 // . . . . . 1 . .
#define Cursor_Off 0x00 // . . . . . . 0 .
#define Cursor_On 0x02 // . . . . . . 1 .
#define Blink_Off 0x00 // . . . . . . . 0
#define Blink_On 0x01 // . . . . . . . 1
// Set_Cursor_and_Display_Shift
// ============================
#define Cursor 0x00 // . . . . 0 . . .
#define Display_and_Cursor 0x08 // . . . . 1 . . .
#define Left 0x00 // . . . . . 0 . .
#define Right 0x04 // . . . . . 1 . .
// Set_Function
// ============
#define Data_Length_4 0x00 // . . . 0 . . . .
#define Data_Length_8 0x10 // . . . 1 . . . .
#define One_Display_Line 0x00 // . . . . 0 . . .
#define Two_Display_Lines 0x08 // . . . . 1 . . .
#define Font_5x7 0x00 // . . . . . 0 . .
#define Font_5x10 0x04 // . . . . . 1 . .
#define Line2_Offset 0x40

// mspdefs.h - msp430 Pinout definitions
// MSP430 Port
// ===========
// 7 6 5 4 3 2 1 0 I/O Port
// +---+---+---+---+---+---+---+---+
// |DB7|DB6|DB5|DB4| E | RS| - | - | 2
// +---+---+---+---+---+---+---+---+
//
//If timing is non-critical (no need to check Busy Flag DB7)
//then the R/W line - LCD Pin 5 - can be tied to Ground LOW).
// Especificación de pines de salida
// =========================================
// Tipo de intrucción 7 6 5 4 3 2 1 0
// =============== === === === === === === === ===
#define Data_Register 0x01 // . . . . . 1 . .
#define Ins_Register 0x00 // . . . . . 0 . .
// Enable 7 6 5 4 3 2 1 0
// ====== === === === === === === === ===
#define Enable_Low 0x00 // . . . . 0 . . .
#define Enable_High 0x04 // . . . . 1 . . .
//Alternativamente pueden especificarse las señales de control mediante operaciones al bit.
#define E_OFF P2OUT &= ~BIT2 // P2.2 = 0
#define E_ON P2OUT |= BIT2 // P2.2 = 1
#define RS_OFF P2OUT &= ~BIT0 // P2.0 = 0
#define RS_ON P2OUT |= BIT0 // P2.0 = 1
#define RW P2OUT |= BIT1; // P2.1 = 1
