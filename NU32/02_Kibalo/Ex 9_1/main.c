/*!\name      main.c
 *
 * \brief	  Console client
 *
 * \author    Juan Gago
 *
 */							
#include "system.h"
#include "conu2.h"

static char	rxchar='0'; 		//received character

int main ( void )
{	

	ANSELA =0; //make sure analog is cleared
	ANSELB =0;

	PPSInput(2, U2RX, RPB11);          //Assign U2RX to pin RPB11
	PPSOutput(4,RPB10, U2TX);          //Assign U2TX to pin RPB10
	uart_init();
	rxchar =0;
	
	// Hyperterminal or Tera Term Startup Inro Screen
	clrscr();  //clear hyper terminal screen
    home();
    uart_putchar("The MicroStick UART2 Experiment");
    uart_putchar(0x0a);  //carriage return /line feed
    uart_putchar(0x0d);
    uart_putchar("Type a character and watch the return");
    uart_putchar(0x0a);  //carriage return /line feed
    uart_putchar(0x0d);
    
	while (1) {
		rxchar = uart_getchar();		
		uart_putchar(rxchar);	
	}
	return 0;
}


