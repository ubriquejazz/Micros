#ifndef FECHA__H__
#define FECHA__H__

#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define mes_impar ((date.fecha.mes==1)|(date.fecha.mes==3)|(date.fecha.mes==5)|(date.fecha.mes==7)|(date.fecha.mes==8)|(date.fecha.mes==10))
#define mes_par ((date.fecha.mes==4)|(date.fecha.mes==6)|(date.fecha.mes==9)|(date.fecha.mes==11))

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

struct date{
  struct hora hora;
  struct fecha fecha;
};  

struct alarma{
  bool on;
  unsigned int num_alarmas;   //Estos campos los actualizo cuando sensor Nivel avise de algún
  struct hora hora;           //problema con depósito o entrada del Filtro o Error bus.
  struct fecha fecha;
}; 

struct hora Limpia_Hora(struct hora h);
struct fecha Limpia_Fecha(struct fecha f);

int  Dia_Semana (struct fecha f);
bool Iguales(struct hora hora1, struct hora hora2);
void Init_Fecha_Hora(struct date d);
void Init_Alarm (struct alarma);

#endif