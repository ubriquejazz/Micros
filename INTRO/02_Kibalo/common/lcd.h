#ifndef _LCD_H
#define _LCD_H

#define LCDPORT LATA
#define E  	LATBbits.LATB11
#define RS  LATBbits.LATB10

//EA DOG M Instructions 4 bit 3.3V
//modification for two line

#define FUNCT1 		0x29	//DL-4bits 2 lines Table select =1
#define BIAS 		0x15	// BIAS 1:5 3 line LCD
//#define BIAS 		0x14	// BIAS 1:5 2 line LCD
#define PWRCNTRL	0x55	//booster on  constrast c5 ,set c4
#define FOLLOW		0x6D	//set booster follower and gain
#define CONST		0x78	// set constrast c3,c2,c1
#define FUNCT2		0x28	//switch back to table =0
#define DISPLAY     0x0E	//display on/off --display on, cursor off, cursor blink off
#define CLEAR		0x01	//delete display --cursor at home
#define ENTRY		0x06	//cursor auto-increment

#define row1    0
#define row2	16
#define row3	32

void lcd_initialize();	
void lcd_pos_cursor(int);
void lcd_clear_display(void);
void lcd_update_display(char*, int);
void lcd_write_char(char);
void lcd_write_string(char*);
void lcd_write_array (char*, int);

#endif
