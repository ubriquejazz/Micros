#include "functions.h"
#include "fecha.h"
#include "io.h"

struct date     date;          // hora y fecha en tiempo real.
struct alarma   alarma[4];     // registros tipo alarma.
struct io       out[8];        // registros tipo io. Salidas
struct io       input[8];      // registros tipo io. Entradas

unsigned int    cuenta;        // controla el tmpo_pantalla_ppal.

struct hora   hora_llenado;               //Hora de llenado del depósito.
unsigned int  porcentaje_llenado = 100;   //Porcentaje mínimo del llenado del depósito.
unsigned int  porcentaje_riego = 100;     //Porcentaje mínimo del llenado del depósito en riego.


//ENTRADAS: -
//SALIDAS:  -
//FUNCIÓN: Se encarga de inicializar todas las variables globales.
void Configuracion(void)
{
  unsigned int i;
  
  Init_Ports();       // configura los puertos
  Init_Fecha_Hora(date);

  for(i=0; i<5; i++){ 
    Init_Alarm(alarma[i]);
  }

  for(i = 0; i<8; i++){
      IO_Init_Out(out[i]);
      IO_Init_Inp(input[i]);
  } 

  cuenta = tmpo_pantalla_ppal;  // init la cuenta.
  hora_llenado = Limpia_Hora(hora_llenado); 
  return;
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Función llamada cuando el usuario eligió Guardar la configuración en Memoria
// Flash.
void Write_Flash(void)
{
 char *Flash_ptr;  // Flash pointer
 unsigned int i, j;

 Flash_ptr = (char *) 0x1000; //Se posiciona el puntero en el inicio del Segmento B.

 _DINT();
 FCTL2 = FWKEY + FSSEL_2 + FN5;   // SMCLK/32 = 250kHz para Flash Timing Generator
 FCTL3 = FWKEY;                   // Limpia el bit Lock.
 FCTL1 = FWKEY + ERASE;           // Ponemos a 1 el bit de borrado
 *Flash_ptr = 0;                  // escritura Dummy para borrar el segmento de la Flach.
 
 FCTL1 = FWKEY + WRT;             // operación de escritura.
 
 //Escribir in[], out[], alarma[],...
  for (i=0; i<8; i++){
    *Flash_ptr++ = input[i].libre;
    *Flash_ptr++ = input[i].remoto;
    *Flash_ptr++ = input[i].direccion;
    *Flash_ptr++;
  }
  for (i=0; i<8; i++){
    *Flash_ptr++ = out[i].libre;
    *Flash_ptr++ = out[i].remoto;
    *Flash_ptr++ = out[i].direccion;
    *Flash_ptr++;
  }
  for (i=0; i<4; i++){
    *Flash_ptr++ = alarma[i].on;
    *Flash_ptr++ = alarma[i].num_alarmas;
    *Flash_ptr++ = alarma[i].hora.hr;
    *Flash_ptr++ = alarma[i].hora.min;
    *Flash_ptr++ = alarma[i].fecha.dd;
    *Flash_ptr++ = alarma[i].fecha.mes;  
  } 
 
  *Flash_ptr++ = hora_llenado.hr;
  *Flash_ptr++ = hora_llenado.min;

  *Flash_ptr++ = porcentaje_llenado;
  *Flash_ptr++ = porcentaje_riego;  

  *Flash_ptr++ = date.hora.seg;         
  *Flash_ptr++ = date.hora.min;         
  *Flash_ptr++ = date.hora.hr;         
  *Flash_ptr++ = date.fecha.dd;         
  *Flash_ptr++ = date.fecha.mes;         
  *Flash_ptr++ = date.fecha.aa;          

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
void Read_Flash(void)
{
 char *Flash_ptr;  // Flash pointer
 unsigned int i, j; 

 Flash_ptr = (char *) 0x1000; //Se posiciona el puntero en el inicio del Segmento B.

 if(*Flash_ptr != 0xFF){      //podrá leer si hay algo escrito.
  
   for (i=0; i<8; i++){
    input[i].libre = *Flash_ptr++;
    input[i].remoto = *Flash_ptr++;
    input[i].direccion = *Flash_ptr++;
    *Flash_ptr++;
   }
   for (i=0; i<8; i++){
    out[i].libre = *Flash_ptr++;
    out[i].remoto = *Flash_ptr++;
    out[i].direccion = *Flash_ptr++;
    *Flash_ptr++;
   }
   for (i=0; i<4; i++){
    alarma[i].on = *Flash_ptr++;
    alarma[i].num_alarmas = *Flash_ptr++;
    alarma[i].hora.hr = *Flash_ptr++;
    alarma[i].hora.min = *Flash_ptr++;
    alarma[i].fecha.dd = *Flash_ptr++;
    alarma[i].fecha.mes = *Flash_ptr++;  
   }  
   
   hora_llenado.hr = *Flash_ptr++;  
   hora_llenado.min = *Flash_ptr++;
   
   porcentaje_llenado = *Flash_ptr++;
   porcentaje_riego = *Flash_ptr++; 
    
   date.hora.seg = *Flash_ptr++;         
   date.hora.min = *Flash_ptr++;            
   date.hora.hr = *Flash_ptr++;            
   date.fecha.dd = *Flash_ptr++;            
   date.fecha.mes = *Flash_ptr++;            
   date.fecha.aa = *Flash_ptr++; 
 }  
 return;  
}
