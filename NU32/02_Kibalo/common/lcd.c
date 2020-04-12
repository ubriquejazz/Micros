//************************************************************************
// Simple library calls 
// modified for PIC32MX220F032B microstick II 2/10/2012
//
//************************************************************************
#include "delay.h"
#include "lcd.h"

const char TopMessage[] = "PIC3MX250F128B ";
const char SecMessage[] = " LCD Test";

void lcd_initialize(void) {  //one call initialize for LCD
	initLCDIo();	//initializes PIC24F I/O
	LCDInit();		//initializes EADOG 16x3
}

void lcd_pos_cursor( int position) { // position 0-47
	int address = position+ 0x80;
	LCDWrite(address,0); //move curror to display
}

void lcd_write_char(char mychar) {
	char * pt;       //need to make a pointer
	pt = &mychar;
	lcd_update_display(pt,1);  //display single character using pt and numofchar =1
}

void lcd_write_string(char* chpointer) {
	while (*chpointer != '\0') {
		LCDWrite(*chpointer,1);
		chpointer++;
	}
}

void lcd_write_array(char* chpointer, int numofchar ) {
	int i=0;
	//LCDWrite(0b00000001,0); //Clear LCD
	for (i =0; i<numofchar; i++) {
		LCDWrite(*chpointer,1);
		chpointer++;
	}
}

/*=============================================================================
I/O INITIALISATION ROUTINE
=============================================================================*/
void initLCDIo(void)
{ 
	ANSELA =0; //make sure analog is cleared
    ANSELB =0;
//configure port A to drive for output
	LATAbits.LATA0 =0;
	LATAbits.LATA1 =0;
	LATAbits.LATA2 =0; //because of CLKI don't use
	LATBbits.LATB2 =0;	// use RB2 instead
	LATAbits.LATA3 =0;
	TRISAbits.TRISA0=0;
	TRISAbits.TRISA1=0;
	TRISAbits.TRISA2=1; //use RB2 instead of A2
	TRISBbits.TRISB2 =0;
	TRISAbits.TRISA3=0;
//configure port b
	LATBbits.LATB10 =0;
	LATBbits.LATB11=0;
	TRISBbits.TRISB10 =0;
	TRISBbits.TRISB11 =0;;
	return;
}

void toggle_E() {
    E =0;
    delay_1us();
	E =1;
	delay_1us();
	E=0;
}

//low level driver derived from Predko Evil Genius p 127
void LCDWrite(int LCDData, int RSValue){
	LCDPORT = (LCDData >> 4)&0x0f; //get high 4 bits for output
	//fix for RB2 versus RA2
	if ((LCDPORT & 0x0004) !=0)
		LATBbits.LATB2 =1;
	else
		LATBbits.LATB2 =0;
	RS =RSValue;
	toggle_E();
	LCDPORT = LCDData &0x0f; //get low 4 bits for output
	//fix for RB2 versus RA2
	if ((LCDPORT & 0x0004)!=0)
		LATBbits.LATB2 =1;
	else
		LATBbits.LATB2 =0;
	RS =RSValue;
	toggle_E();
	Delayms(5);
}

// 
void LCDInit() 
{
	int i =0;

	//fix for RB2
	LATBbits.LATB2 =0;
	LCDPORT =0; //start with everything low

	//power up wait
	Delayms(40);
	//fix for RB2
	LATBbits.LATB2 =0;
	LCDPORT =3; //start initialization process
	toggle_E(); //send reset1 command 03
	Delayms(5);
	toggle_E(); //repeat reset2 command 03
	Delay10us(3);
	toggle_E();	//send reset3 command third time 03
	Delay10us(3);
	//fix for RB2
	LATBbits.LATB2 =0;
	LCDPORT =2 ; 	//initialize 4 LCD 4 bit mode 02
	toggle_E();
    Delay10us(3);

	// FUNCT1 		0x29	//DL-4bits 2 lines Table select =1
	LCDWrite(FUNCT1,0);
	// BIAS 		0x14	// BIAS 1:5 2 line LCD
	LCDWrite(BIAS,0);
	// PWRCNTRL	0x55		//booster on  constrast c5 ,set c4
	LCDWrite(PWRCNTRL,0);
	// FOLLOW		0x6D	//set booster follower and gain
	LCDWrite(FOLLOW,0);
	// CONST		0x78	// set constrast c3,c2,c1
	LCDWrite(CONST,0);
	// FUNCT2		0x28	//switch back to table =0
	LCDWrite(FUNCT2,0);
	// DISPLAY     0x0F		//display on/off --display on, cursor on, cursor blink
	LCDWrite(DISPLAY,0);
	// CLEAR		0x01	//delete display --cursor at home
	LCDWrite(CLEAR,0);
	// ENTRY		0x06	//cursor auto-increment
	LCDWrite(ENTRY,0);

	move_FirstLine();
	for (i=0; TopMessage[i] !=0; i++)
		LCDWrite(TopMessage[i],1);
	move_SecondLine();
	for ( i=0; SecMessage[i] !=0; i++)
		LCDWrite(SecMessage[i],1);
	return;
}

void write_char(char*chpointer) {
	LCDWrite(*chpointer,1);
}

void lcd_update_display(char*chpointer, int numofchar) {
	int i=0;
	//LCDWrite(0b00000001,0); //Clear LCD
	for (i =0; i<numofchar; i++) {
		LCDWrite(*chpointer,1);
		chpointer++;
	}
	return;
}

void lcd_clear_display() {
	LCDWrite(0b00000001,0); //clear LCD
}
void move_FirstLine() {
    unsigned char address =0x80+0x00; // line 1
	LCDWrite(address,0); //move curror to display
}
void move_SecondLine() {
  	unsigned char address =0x80+16; // line 2--3 line idsplay
  //unsigned char address =0x80+0x40; // line 2 of 2 line
	LCDWrite(address,0); //move curror to display
}
void move_ThirdLine() {
  	unsigned char address =0x80+32; // line 3
	LCDWrite(address,0); //move curror to display
}
void move_FourthLine() {
  	unsigned char address =0x80+0x54; // line 4
	LCDWrite(address,0); //move curror to display
}


