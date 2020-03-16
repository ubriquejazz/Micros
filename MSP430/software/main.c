#include <msp430x14x.h>
#include "libreria.h"
#include "functions.h"
#include "interrupts.c"
#include <stdbool.h>


#define dormido       _BIS_SR(LPM3_bits + GIE)
#define despierto     _BIC_SR_IRQ(LPM3_bits)

#define TB_OVF                14
#define TA_OVF                10
#define CCI2A                 4     //TAIV

bool manual = false;         //indica si se ha pulsado ese botón.
bool filtrando = false;      //indica la situación del filtro.
bool motor = false;          //indica si estña encendido o no el motor, sólo podrá activarse si conf[2].on == true.
struct hora motor_off;       //Hora de apagado del Motor.
unsigned long nivel = 0;     //Porcentaje de Nivel del Depósito.

unsigned int tmpo_modo = 0;  //si tmpo_modo > 5, consideramos pulsación en modo 2.   

bool flanco;                 // s
bool bat = false;            //variable que indica la situación de la batería.

unsigned int modo=0;
unsigned int ON = 9;         //Botón pulsado.

/*
struct conf conf[3];         //array de registros tipo conf.
struct ud zona[6];           //array de registros tipo ud.
bool uni[6] = {0, 0, 0, 0, 0, 0};
struct programa programa[6];                     //array de registros tipo programa.
unsigned int tmpo_caudal = 0;                    //Por defecto, riego por tiempo 
unsigned int dias[7] = {0, 0, 0, 0, 0, 0, 0};    //dias marcados en Programación de Riego.
unsigned int veces = 1;                          //número de programas al día.
unsigned char cursor = xy21;                     //Posición del cursor en el LCD.
unsigned int litros[6] = {0, 0, 0, 0, 0, 0};
unsigned int l = 0;                 //litros/min correspondientes a la frecuencia medida. 
float periodo = 0;
unsigned int frec_user = 40; //Frecuencia que introduce el usuario, correspondiente al caudal ya insertado. 
unsigned int vez = 0;
bool esta_regando[6] = {0, 0, 0, 0, 0, 0};
*/

bool nuevomin;               //flag que indica si ha transcurrido un nuevo minuto.
int dayofweek;
int minutos;                 //minutos utilizados para temporizador.                                      
int horas;                   //horas utilizados para temporizador.




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

