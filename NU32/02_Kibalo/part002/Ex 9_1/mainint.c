/*!\name      main.c
 *
 * \brief     Console client
 *
 * \author    Juan Gago
 *
 */     
#include "system.h"
#include "conu2.h"

static char rxchar='0'; 		//received character
unsigned char got_rx =0;		//rx char flag

void __ISR(_UART_2_VECTOR, ipl2) UART2BHandler(void)
{
    rxchar =  (char) (U2RXREG & 0x00ff);
    got_rx =1;
    IFS1bits.U2RXIF = 0; // Clear the interrupt flag bit
}

int main ( void )
{
    ANSELA =0; //make sure analog is cleared
    ANSELB =0;

    PPSInput(2, U2RX, RPB11);          //Assign U2RX to pin RPB11
    PPSOutput(4,RPB10, U2TX);          //Assign U2TX to pin RPB10
    uart_init();
    rxchar =0;
    got_rx= 0;

    INTEnableSystemMultiVectoredInt(); // interrupt setting
    IEC1bits.U2RXIE =1;
    IPC9bits.U2IP   =2;                //set priority to 2
    IFS1bits.U2RXIF =0;

    // Hyperterminal Startup Inro Screen
    clrscr();  //clear hyper terminal screen
    home();
    uart_putchar("The MicroStick UART2 Experiment");
    uart_putchar(0x0a);  //carriage return /line feed
    uart_putchar(0x0d);
    uart_putchar("Type a character and watch the return");
    uart_putchar(0x0a);  //carriage return /line feed
    uart_putchar(0x0d);

    while (1) 
    {
        if (got_rx==1) {
            got_rx =0;
            uart_putchar(rxchar);
        }
    }
    return 0;
}