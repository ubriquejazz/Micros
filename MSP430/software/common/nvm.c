#include "nvm.h"
#include "application.h"

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Función llamada cuando el usuario eligió Guardar la configuración en Memoria
// Flash.
void NVM_Write_Flash(void)
{
    char *Flash_ptr;  // Flash pointer
    Flash_ptr = (char *) 0x1000; //Se posiciona el puntero en el inicio del Segmento B.

    _DINT();
    FCTL2 = FWKEY + FSSEL_2 + FN5;   // SMCLK/32 = 250kHz para Flash Timing Generator
    FCTL3 = FWKEY;                   // Limpia el bit Lock.
    FCTL1 = FWKEY + ERASE;           // Ponemos a 1 el bit de borrado
    *Flash_ptr = 0;                  // escritura Dummy para borrar el segmento de la Flach.
    FCTL1 = FWKEY + WRT;             // operación de escritura.

    *Flash_ptr++ = appData.dia.hora.seg;
    *Flash_ptr++ = appData.dia.hora.min;
    *Flash_ptr++ = appData.dia.hora.hr;
    *Flash_ptr++ = appData.dia.fecha.dd;
    *Flash_ptr++ = appData.dia.fecha.mes;
    *Flash_ptr++ = appData.dia.fecha.aa;

    FCTL1 = FWKEY;                  // Clear WRT bit
    FCTL3 = FWKEY + LOCK;           // Reset LOCK bit
    _EINT();
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Cada vez que se vaya la alimentación principal y secundaria(batería), al
// aparecer de nuevo, se cargaran todos los datos guardados en Flash el memoria RAM.
// La primera vez que se ejecuta el programa, la Flash está vacía, por lo que no
// deberá leer de Flash.
void NVM_Read_Flash(void)
{
    char *Flash_ptr;  // Flash pointer
    Flash_ptr = (char *) 0x1000; //Se posiciona el puntero en el inicio del Segmento B.

    if(*Flash_ptr != 0xFF){      //podrá leer si hay algo escrito.
        appData.dia.hora.seg = *Flash_ptr++;
        appData.dia.hora.min = *Flash_ptr++;
        appData.dia.hora.hr  = *Flash_ptr++;
        appData.dia.fecha.dd = *Flash_ptr++;
        appData.dia.fecha.mes= *Flash_ptr++;
        appData.dia.fecha.aa = *Flash_ptr++;
    }
    return;
}
