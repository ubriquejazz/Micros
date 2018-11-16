#include <msp430x14x.h>
#include "libreria.h"
#include "functions.h"
#include "interrupts.c"
#include <stdbool.h>

const int TB_OVF  = 14;
const int TA_OVF  = 10;
const int CCI2A   = 4;     //TAIV
const unsigned long lmax   = 1000000;   //litros máximos.
const int maxcaudal  = 999;
const int maxfrec    = 999;
const int maxdir  = 246;
const int maxporc = 100;

const unsigned int tmpo_pantalla_ppal = 40;

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
  const char *fech     =   "2.Fecha";
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
const char *SI    = "SI";
const char *NO    = "NO";
const char *OFF   = "OFF";
const char *OK    = "ON";
const char *PREGD = "DESACTIVAR?";
const char *PREGA = "ACTIVAR?";
const char *A     = "    ACTIVADA";
const char *D     = "  DESACTIVADA";
const char *PREGC = "    GUARDAR?";

unsigned int cuenta;         //controla el tmpo_pantalla_ppal.
bool nuevomin;               //flag que indica si ha transcurrido un nuevo minuto.
struct date date;            //almacena la hora y fecha en tiempo real.
int dayofweek;
int minutos;                 //minutos utilizados para temporizador.                                      
int horas;                   //horas utilizados para temporizador.

bool manual = false;         //indica si se ha pulsado ese botón.
bool filtrando = false;      //indica la situación del filtro.
bool motor = false;          //indica si estña encendido o no el motor, sólo podrá activarse si conf[2].on == true.
struct hora motor_off;       //Hora de apagado del Motor.
unsigned long nivel = 0;     //Porcentaje de Nivel del Depósito.

unsigned int tmpo_modo = 0;  //si tmpo_modo > 5, consideramos pulsación en modo 2.   

bool flanco;
unsigned int modo=0;
unsigned int ON = 9;         //Botón pulsado.

struct programa programa[6];                     //array de registros tipo programa.
unsigned int tmpo_caudal = 0;                    //Por defecto, riego por tiempo 
unsigned int dias[7] = {0, 0, 0, 0, 0, 0, 0};    //dias marcados en Programación de Riego.
unsigned int veces = 1;                          //número de programas al día.
unsigned char cursor = xy21;                     //Posición del cursor en el LCD.

struct conf conf[3];         //array de registros tipo conf.
struct ud zona[6];           //array de registros tipo ud.
bool uni[6] = {0, 0, 0, 0, 0, 0};

struct io out[8];            //array de registros tipo io. Salidas
struct io in[8];             //array de registros tipo io. Entradas
unsigned int salidas_ocup[9]={1, 1, 1, 1, 1, 1, 1, 1, 1};   //Todas las salidas libres. 
unsigned int entradas_ocup[9]={1, 1, 1, 1, 1, 1, 1, 1, 1};  //Todas las salidas libres. 

struct alarma alarma[4];     //array de registros tipo alarma.

struct hora hora_llenado;    //Hora de llenado del depósito.
unsigned int porcentaje_llenado = 100;           //Porcentaje mínimo del llenado del depósito.
unsigned int porcentaje_riego = 100;             //Porcentaje mínimo del llenado del depósito en riego.

unsigned int litros[6] = {0, 0, 0, 0, 0, 0};
unsigned int l = 0;                 //litros/min correspondientes a la frecuencia medida. 
float periodo = 0;
unsigned int frec_user = 40; //Frecuencia que introduce el usuario, correspondiente al caudal ya insertado. 
unsigned int vez = 0;
bool esta_regando[6] = {0, 0, 0, 0, 0, 0};

bool bat = false;            //variable que indica la situación de la batería.

int main (void)
{
 Configuracion();   //configura puertos e inicializa variables.
 Init_Osc();        // inicialización osciladores
 Init_Display();    // inicialización LCD
 
 PutCharCGRAM(0,31,23,23,23,23,23,17,31);   //se redefinen 7 caracteres para Dias de Riego.
 PutCharCGRAM(1,31,21,17,17,21,21,21,31);
 PutCharCGRAM(2,31,21,21,27,27,21,21,31);
 PutCharCGRAM(3,31,17,27,27,27,11,23,31);
 PutCharCGRAM(4,31,21,21,21,21,21,27,31); 
 PutCharCGRAM(5,31,25,23,27,29,29,19,31);  
 PutCharCGRAM(6,31,19,21,21,21,21,19,31);  
 
 Limpia_Pantalla();
 Put_Str(stma);
 Delayx100us(10000);//espera 1seg.
 Put_Ins(LINEA1);
   
 Init_COMP();      // inicialización Comparador
 Init_CAD();       // inicialización Convertidor AD
 Read_Flash();     // carga los datos de Memoria Flash a RAM 
 Init_TA();        // inicialización TB1
 Init_TB();        // inicialización TB0

 while(1){ 
   Limpia_Pantalla();
   Put_Ins(Set_Display + Display_On + Cursor_Off + Blink_Off);
   nuevomin = 0;
   Hora(date.hora.min, date.hora.hr);
   Fecha(date.fecha.dd, date.fecha.mes, date.fecha.aa); 
   Put_Ins(xy2A);
   Put_Str(menu); 
   dormido;
   
   ///Si se pulsa <MENU>
   if(ON == 22){         //Intro <-|
    ON = 9;
    Menu();
   }
 }
}

