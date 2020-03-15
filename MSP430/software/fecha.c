#include "fecha.h"

//***********************************************************************************
//ENTRADAS: struct hora h
//SALIDAS: struct hora h
//FUNCIÓN: Inicializa el registro hora.
struct hora Limpia_Hora(struct hora h)
{
 h.seg = 0;
 h.min = 0;
 h.hr  = 0; 
 return(h);
}

//***********************************************************************************
//ENTRADAS: hora1(registro tipo hora),
//          hora2(registro tipo hora).
//SALIDAS: booleana
//FUNCIÓN: Compara dos registros tipo hora, sacando a la salida un '0' si no son 
// iguales, y un '1' si lo son.
bool Iguales(struct hora hora1, struct hora hora2)
{
 return((hora1.hr == hora2.hr) & (hora1.min == hora2.min) & (hora1.seg == hora2.seg));
} 

//***********************************************************************************
//ENTRADAS: struct fecha f
//SALIDAS: struct fecha f
//FUNCIÓN: Inicializa el registro fecha.
struct fecha Limpia_Fecha(struct fecha f)
{
 f.dd  = 0;
 f.mes = 0;
 f.aa  = 0;
 return(f);
}

//***********************************************************************************
//ENTRADAS: struct date d
//SALIDAS: 
//FUNCIÓN:  inicialización del registro date
void Init_Fecha_Hora(struct date d)
{
  d.hora.seg  = 00;      
  d.hora.min  = 26;
  d.hora.hr   = 10;
  d.fecha.dd  = 15;
  d.fecha.mes = 5;
  d.fecha.aa  = 05; 
  return;
}

//***********************************************************************************
//ENTRADAS: struct date d
//SALIDAS: 
//FUNCIÓN: inicialización del array de registros alarma
void Init_Alarma(struct alarma) 
{ 
	alarma.on          = 0;
	alarma.num_alarmas = 0;
	alarma.hora = Limpia_Hora(alarma.hora);
	alarma.fecha = Limpia_Fecha(alarma.fecha);
	return
}


//***********************************************************************************
//ENTRADAS: struct fecha 
//SALIDAS: 
int Dia_Semana(struct fecha f)
{
  int century, yr, dw;
  if (f.mes < 3){
   f.mes = f.mes + 10;
   f.aa--;
  }
  else{
     f.mes = f.mes - 2;
  }   
  century = f.aa / 100;
  yr = f.aa % 100;
  dw = (((26 * f.mes - 2) / 10) + f.dd + yr + (yr / 4) + (century / 4) - (2 * century)) % 7;

  if (dw < 0){  
    return(5);
  }
  else if(dw==0){
    return(6);
  }
  else{ 
    return(dw - 1); 
  }   
}

