#ifndef __CONU2_H__
#define __CONU2_H__

/*!\name      conu2.h
 *
 * \brief	  Console for Microstick II using UART 2 module
 *
 * \author    Juan Gago
 *
 */

// useful macros
#define clrscr() 	uart_putchar( "\x1b[2J") 
#define home()   	uart_putchar( "\x1b[H") 

#define BACKSPACE 	0x8   // ASCII backspace character code            

// init the serial port (UART2, 9600, 8, N, 1, CTS/RTS )
void uart_init( void);

// send a character to the serial port
int uart_putchar( int c);

// wait for a new character to arrive to the serial port
char uart_getchar( void);

// send a null terminated string to the serial port
void uart_printf( char *s);

// receive a null terminated string in a buffer of len char
char * uart_scanf( char *s, int n);

#endif