#ifndef MENU__H__
#define MENU__H__

#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include "libreria.h"

extern bool flanco;
extern unsigned int modo, ON;
extern unsigned int salidas_ocup[];
extern unsigned int entradas_ocup[];

const char *SI    = "SI";
const char *NO    = "NO";
const char *OFF   = "OFF";
const char *OK    = "ON";
const char *PREGD = "DESACTIVAR?";
const char *PREGA = "ACTIVAR?";
const char *A     = "    ACTIVADA";
const char *D     = "  DESACTIVADA";
const char *PREGC = "    GUARDAR?";
const char *stma  =   "Sistema de Riego";
const char *menu  =   "<MENU>";

const char *cmanual    =  "1.Control Manual";
  const char *menumanual   =   "    *Manual*    ";
  const char *zonas        =   "1.Uds Riego(1-6)";
  const char *desconex     = "OFF Ud ";
  const char *manualmotor  =   "2.Motor";
  const char *manualestado =   "3.Estado Dep. %";

const char *riego     =   "2.Programa Riego";
  const char *riegox     =   "   Riego por";
  const char *tiempo     =   "Tiempo";
  const char *diariego   =   " Dias de riego";
  const char *bces       =   "Veces al dia    ";
  const char *seis       =   "  (1-6)     : 1 ";   //por defecto 1 vez
  const char *prog       =   " Programa  :";
  const char *hon        =   "1.HORA ON";
  const char *hoff       =   "2.HORA OFF";
  const char *litro      =   "2.LITROS OFF";
  const char *unidades   =   "3.Uds Riego(1-6)";

const char *estado    =   "3.Estado Alarmas";
  const char *flecha    = "->";
  const char *alarmas   = " Alarma(s)";
  const char *borrar   = " Borrar Alarmas";
  const char *borrada  = "    Borradas";

const char *config    =   "4.Configuracion ";
  const char *filtrado  = "Filtrado";
  const char *estimada  = "ESTIMADA";
  const char *caudal    = "Caudal";
  const char *mot       = "Motor";
  const char *lxmin     = " (l/min)";
  const char *frec      = " Frec(   l/min)";

const char *conf_IO   =   "5.Config. IN/OUT";
    const char *menurI_O  =   " *Menu IN/OUT* ";                            
    const char *unidad    = "   Ud Riego ";
    const char *local     = "LOCAL";
    const char *remoto    = "REMOTO"; 
    const char *sal       = "Salida "; 
    const char *sal_ocup  = "SALIDAS OCUPADAS"; 
    const char *ent_ocup  = "ENTRAD. OCUPADAS";
    const char *direc     = "Direccion ";   
    const char *salidas   =   "2.Salidas(1-8) ";
    const char *salida      = "    SALIDA "; 
    const char *configurada = "CONFIGURADA"; 
    const char *entradas  =   "3.Entradas(1-8)";
    const char *ent       = "Entrada "; 
    const char *entrada   = "   ENTRADA ";  
    const char *ver       =   "4.Ver Config I/O";  
    const char *libre     = "LIBRE";    

const char *conf_AL   =   "6.Config. Alarma";
  const char *filtro    =   "Filtro";
  const char *dep       =   "Deposito";
  const char *rebosado  =   "rebosado";
  const char *seco      =   "seco";
  const char *error     =   "Error de";
  const char *comunic   =   "Comunic.";

const char *reloj     =   "7.Config. Reloj";
  const char *menureloj =   "  *Menu Reloj*";
  const char *hora      =   "1.Hora";
  const char *fech      =   "2.Fecha";
  const char *hh        =   " HORA";
  const char *ff        =   " FECHA";

const char *deposito  =   "8.Deposito";
  const char *menudep     =   "*Menu Deposito*";
  const char *llenado     =   "1.Hora llenado";
  const char *min_llenado =   "2.% min llenado";
  const char *en_riego    =   "3.% min en riego";
  const char *porciento   =   "Porcentaje min.";
  const char *lleno       =   " llenado    %";
  const char *regando     =   "en riego    %";

const char *guarda    =   "9.Guardar Conf. ";
const char *guardada  = "    Guardada";

void Linea(unsigned int linea);
void Ejecuta(unsigned int linea);
void Menu(void);

#endif
