#include <lcd.h>
#include "menu.h"
#include "application.h"     // despierto, dormido

bool flanco;                 // s
unsigned int modo=0;
unsigned int ON = 9;         //Botón pulsado.

//***********************************************************************************
// RSI: PORT1
// Interrupt on the falling edge of pins in Port 1
// Enables WDT timer to identify mode 2 of pressing.

void Scan_Keypad (void)
{
    unsigned int columna;

    P1DIR = 0x80;      //polling de entrada.
    P1OUT = 0;

    for(columna = 0; columna<4; columna++){
        if(!bitest(P1IN, BIT6)){         //fila 0
            ON = 0 + columna;
        }
        else if (!bitest(P1IN, BIT5)){   //fila 1
            ON = 10 + columna;
        }
        else if (!bitest(P1IN, BIT4)){   //fila 2
            ON = 20 + columna;
        }
        if (columna == 0){
            P1DIR = 0x04;
        }
        else{
            P1DIR = P1DIR>>1;   //se desplaza el 1 una posición a la izq.
        }
        P1OUT = 0;
    }
    P1DIR = 0x87;         //7, 2, 1 y 0 de salida
    P1OUT = 0;

    switch(ON){  //XOR leds.
        case 2 : P6OUT ^= 0x80; break;
        case 0 : P6OUT ^= 0x20; break;
        case 10: P6OUT ^= 0x08; break;
        case 20: P6OUT ^= 0x02; break;
        case 1 : P6OUT ^= 0x10; break;
        case 11: P6OUT ^= 0x04; break;
        case 21: P6OUT ^= 0x01; break;
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void teclado (void)
{
    unsigned int fila1, fila2, fila3;

    P1IE  = 0x00;
    IE1 |= WDTIE;                         // Enable WDT interrupts
    fila1 = bitest(P1IFG,BIT4);
    fila2 = bitest(P1IFG,BIT5);
    fila3 = bitest(P1IFG,BIT6);

    if(fila1 | fila2 | fila3){
        Scan_Keypad();
    }
    P1IFG = 0;   //hay q resetear este registro vía software.
}

//***********************************************************************************
// RSI: WDTimer
// Finds out the mode a switch button is pressed
// It gets triggered once is pressed more than 16 ms (count already loaded)

#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{
    unsigned int num1, num2, num3;
    static unsigned int tmpo_modo = 0;  // mode = 1, 2

    num1 = bitest(P1IN,BIT4);
    num2 = bitest(P1IN,BIT5);
    num3 = bitest(P1IN,BIT6);
    flanco = num1 & num2 & num3;
    if(!flanco){
        tmpo_modo++;
        if(tmpo_modo > 70){
            modo = 2;
            flanco = 1;
            tmpo_modo = 0;
            IE1 &= ~WDTIE;
            P1IE  = 0x70;
            despierto;
        }
    }
    else if(tmpo_modo<71){
        modo = 1;
        tmpo_modo = 0;
        IE1 &= ~WDTIE;
        P1IE  = 0x70;
        despierto;
    }
}

//***********************************************************************************
//ENTRADAS: min(minutos que debe pintar),
//          hr(horas que debe pintar).
//SALIDAS: -
//FUNCIÓN: Muestra por pantalla la Hora(horas y minutos) introducida por valor.
void Hora(int min, int hr)
{
  char LCD[16];
  sprintf(LCD, " %d%d:%d%d ", hr/10, hr%10, min/10, min%10);
  Put_Str(LCD);
}

//***********************************************************************************
//ENTRADAS: dd(dia que debe pintar),
//          mes(mes que debe pintar),
//          aa(año que debe pintar).
//SALIDAS: -
//FUNCIÓN: Muestra por pantalla la Fecha(dia, mes y año) que le llegan como entrada.
void Fecha(unsigned int dd,//struct fecha f
           unsigned int mes,
           unsigned int aa)
{
  char LCD[16];
  sprintf(LCD, " %d%d/%d%d/%d%d", dd/10, dd%10, mes/10, mes%10, aa/10, aa%10);
  Put_Str(LCD);
}


void Poner_Fecha()
{}

void Poner_Hora()
{}

//***********************************************************************************
//ENTRADAS: linea
//SALIDAS: -
//FUNCIÓN: Pinta la línea que se corresponde con el número que le llega de entrada.
void Linea(unsigned int linea)
{
    Limpia_Linea(LINEA2);
    switch(linea)
    {
        case 1:   Put_Str("1.Hora");    break;
        case 2:   Put_Str("2.Fecha");   break;

        default:
            break;
    }
}

//***********************************************************************************
//ENTRADAS: linea
//SALIDAS: -
//FUNCIÓN: Ejecuta la línea que se corresponde con el número que le llega de entrada.
void Ejecuta(unsigned int linea)
{
    Limpia_Pantalla();
    switch(linea)
    {
        case 1: Poner_Hora();   break;
        case 2: Poner_Fecha();  break;

        default:
            break;
    }
    while(flanco == 0);       //se controla mejor la pulsación cuando queremos regresar.
    ON = 9;
    Put_Ins(Set_Display +Display_On + Cursor_Off + Blink_Off);
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Se encarga de Pintar y Ejecutar cada linea del menú, seleccinada por el
// usuario.
void Menu(void)
{
    unsigned int linea = 1;

    Limpia_Pantalla();
    Put_Ins(xy15);
    Put_Str("<MENU>");
    Put_Ins(LINEA2);
    Linea(linea);

    do
    {
        if(ON != 9){
            while (flanco == 0);
            switch (ON)
            {
                case 3: ON = 9;              // +
                       if (linea == 1){
                        linea = 9;
                       }else{ linea--;}
                       Linea(linea);
                       break;

                case 13: ON = 9;             // -
                       if (linea == 9){
                        linea = 1;
                       }else{ linea++;}
                       Linea(linea);
                       break;

                case 22: ON = 9;             //<-|
                       Ejecuta(linea);
                       Limpia_Pantalla();
                       Put_Ins(xy15);
                       Put_Str("<MENU>");
                       Put_Ins(LINEA2);
                       Linea(linea);
                       break;

                default:
                    break;
            }
        }
    } while(ON!=12);
    while(flanco == 0);
    ON = 9;
    modo = 0;
}

//*********************************************************************************//
//***********************   1.Control Manual    ***********************************//
//*********************************************************************************//
//ENTRADAS: -
//SALIDAS: -
//FUNCIÓN: Primera opción del Menú. Expande el Submenú Control Manual.
void Manual(void)
{
}

