#ifndef DATE__H__
#define DATE__H__

#include <msp430.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

struct hora{
  unsigned int seg;
  unsigned int min;
  unsigned int hr;
}; 

struct fecha{
  unsigned int dd;
  unsigned int mes;
  unsigned int aa;
}; 

typedef struct {
  struct hora hora;
  struct fecha fecha;
} date_t;

int  Dia_Semana  (struct fecha f);
bool Hora_Iguales(struct hora hora1, struct hora hora2);

struct hora Init_Hora(struct hora h);
struct fecha Init_Fecha(struct fecha f);
date_t Init_Fecha_Hora(date_t d);

#endif
