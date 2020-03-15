#ifndef IO__H__
#define IO__H__

#include <string.h>
#include <stdbool.h>
#include <stdio.h>

//registro que almacena las características de cada entrada o salida.
struct io{                    
  unsigned int libre;         //libre = 0 ,, ocupada.
  unsigned int remoto;        //si remoto = 1 ,, salida controlada remotamente.
  unsigned int direccion;     //variable válida sólo si remoto = 1.
};

IO_Init_Inp(struct io);
IO_Init_Out(struct io);

#endif