#ifndef LIBRERIA__H__
#define LIBRERIA__H__

#include <msp430.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "def_display.h"

#define _100us  131  //131 cycles * 6 + 13 = 799 / 799*125ns = 99,875 us
#define retdata 55  // 43 us
#define retint  50   // 39 us
#define retclear 15 // 1,53 ms 1530 us

#define LCD_OUT P2OUT
#define LCD_DIR P2DIR
#define LCD_SEL P2SEL

#define LINEA1    0x80
#define xy11      0x81
#define xy12      0x82
#define xy13      0x83
#define xy14      0x84
#define xy15      0x85
#define xy16      0x86
#define xy17      0x87
#define xy18      0x88
#define xy19      0x89
#define xy1A      0x8A
#define xy1B      0x8B
#define xy1C      0x8C
#define xy1D      0x8D
#define xy1E      0x8E
#define xy1F      0x8F

#define LINEA2    0xc0
#define xy21      0xC1
#define xy22      0xC2
#define xy23      0xC3
#define xy24      0xC4
#define xy25      0xC5
#define xy26      0xC6
#define xy27      0xC7
#define xy28      0xC8
#define xy29      0xC9
#define xy2A      0xCA
#define xy2B      0xCB
#define xy2C      0xCC
#define xy2D      0xCD
#define xy2E      0xCE
#define xy2F      0xCF

#define bitset(var,bitno) ((var) |= 1 << (bitno))
#define bitclr(var,bitno) ((var) &= ~(1 << (bitno)))
bool    bitest(unsigned char val, unsigned char bit);

void Init_Display(void);

void Delay(unsigned int);
void Delayx100us(unsigned int);

void Put_Val(unsigned char);
void Put_Ins(unsigned char);
void Put_Str(const char *);
void Put_Data(unsigned char);
void Put_Num(int);

void PutCharCGRAM(unsigned int i,
      unsigned int l1, unsigned int l2,unsigned int l3,unsigned int l4,
      unsigned int l5, unsigned int l6,unsigned int l7,unsigned int l8);

void Limpia_Pantalla (void);
void Limpia_Linea (unsigned char);

#endif
