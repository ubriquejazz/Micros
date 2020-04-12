#include "system.h"
#include "see.h"

#define SCK2   _TRISB15    // tris control for SCK2 pin26
                           // note CSEE define and set in SEE.C as RBP11

int main()
{
    ANSELA =0; //make sure analog is cleared
    ANSELB =0;
    PPSInput(3,SDI2,RPB13);     // Assign SDI2 to RBP13
    PPSOutput(3,RPB2,SDO2);     // Assign SD02 to pin RPB2
    SCK2 = 0;                   //set as output

    see_init();     // initialize the SPI2 port and CS to access the 25LC256
    int data;
    while (1) {
        
        data = see_read( 32);       // read current content of memory location
        data++;                     // <-set breakpoint here
        see_write( 32, data);       // write back the new value
        //address++;
    } // main loop
    return 0;
} //main