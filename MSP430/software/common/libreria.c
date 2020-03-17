#include "libreria.h"

//***********************************************************************************
//ENTRADAS: val(valiable cuyo bit queremos testear),
//          bit(valor hexadecimal indicando con un '1 en la posici�n del bit que 
//              queremos testear).
//SALIDAS: boleana
//FUNCTION: Testea el 'bit' indicado a la entrada de la variable 'val'. Resultar� falso
// si el bit es '0', y ser� true cuando bit sea '1'.

bool bitest(unsigned char val, unsigned char bit)
{
    return(val & bit);
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCTION: Inicializa todos los puertos del Micro.
void Init_Ports(void)
{
  P1SEL = 0x08;         //P1.3 en modo TA2 para CAUDAL.
  P1DIR = 0x87;         //P1.7, P1.2, P1.1 and P1.0 de salida
  P1OUT = 0;
  P1IE  = 0x70;         //Habilitaci�n de interrupci�n.
  P1IES = 0x70;         //Interrupci�n por flanco de bajada
  
  LCD_SEL = 0x08;      
  LCD_OUT = 0x02;   //R/W en modo lectura.
  LCD_DIR = 0xF7;   //P2.3 de entrada BAT_SUP 
  
  P3SEL = 0xF0;
  P3DIR = 0x5D;      //Entradas P3.1, P3.5 y P3.7. Resto salidas
  bitset(P3OUT, 2);  // ERROR apagado
  
  OPTO_SEL = 0;            //Optos
  OPTO_DIR = 0;            //Entradas

  RELE_SEL = 0;            //Rel�s
  RELE_DIR = 0xFF;         //Salidas
  RELE_OUT = 0; 
    
  P6SEL = 0x40;  
  P6DIR |= 0xBF;       
  P6OUT = 0xFF;         //Se apagar�n los leds de teclado
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCTION: Para el Watchdog y activa los dos ociladores del sistema, configurando
// ACLK = LFXT1 = 32768 y MCLK = XT2 = 8MHz, y si �ste fallara, autom�ticamente 
// MCLK = DCO = 8MHz. 
void Init_Osc(void)
{
  WDTCTL = WDT_ADLY_16; 	// interrumpir� al pulsar una tecla, y despu�s cada 16mseg.
  BCSCTL1 &= ~XT2OFF; 		// XT2 = 8MHz on.
  _BIC_SR(OSCOFF); 		// turn on LFXT1 oscillator = ACLK.

  BCSCTL2 = SELM1;              // MCLK = XT2 = 8 MHz.
  DCOCTL |= DCO2 | DCO1 | DCO0;
  BCSCTL1 |= RSEL2 | RSEL1 | RSEL0; // MCLK = DCO, 8 MHz  
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCTION: Inicializaci�n del Comparador A para el testeo del pin BAT SUP. Comparamos
// con Vref = 0.5*Vcc = 1.65. Habilita el Filtro para aumentar la exactitud y resoluci�n 
// del resultado de la comparaci�n.
void Init_COMP(void)
{
 CACTL1 = CAREF_2 + CARSEL + CAON + CAIES + CAIE;  //Vref = 0.5*Vcc, interrupci�n 
                                                   //cuando flanco de bajada de CAOUT
 CACTL2 = P2CA0 +CAF;        //selecci�n de CA0(P2.3), habilita el filtro de salida.
 CAPD = 0xF7;                //Todos los pines deshabilitados, menos CA0 = P2.3
 _EINT();
}

//*********************************************************************************//
//****************************     NIVEL      *************************************//
//*********************************************************************************//
//ENTRADAS: -
//SALIDAS: -
//FUNCTION: Configuraci�n del Convertidor Anal�gico Digital para decodificar el valor
// suministrado por el sensor de Nivel. Usando el reloj ADC12OSC = 5MHz y 
// habilitando la conversi�n de tal manera que Ts = 1muestra/seg.
void Init_CAD(void)
{
 ADC12MCTL0 = SREF_0 + INCH_6;  // Vref+ = AVcc y Vref- = AVss. A6
 ADC12CTL0 = ADC12ON + SHT0_15; // configuraci�n del sampling time a 1024*ADC12OSC
 ADC12CTL1 |= SHP;              // usa el sampling timer. Modo de Pulso(SHP = 1)
 ADC12IE = 0x01;                // habilita ADC12IFG.0. Se guarda en ADC12MEM0
 ADC12CTL0 |= ENC;              // Enable conversions
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCTION: Configura el Timer B para generar un RTC, interrumpiendo cada segundo,
// utilizando el registro de comparaci�n CCR0.
void Init_TB(void)
{
 TBCCTL0 = CCIE;             // habilita la interrupci�n para CCR0
 TBCCR0 = cargaTB;           // cuenta desde 0000h hasta 8000h(1seg)
 TBCTL = TBSSEL0 + MC_1;     // usa ACLK, modo up to TBCL0 
}

//***********************************************************************************
//ENTRADAS: a(valor m�ximo de la cuenta).
//SALIDAS: -
//FUNCTION: Cuenta retrasos de (15 + a*11) ciclos de reloj.
void Delay (unsigned int a)
{
 unsigned int k;
 for (k=0 ; k != a; ++k);    //15 + a*11 ciclos.
}

//***********************************************************************************
//ENTRADAS: b(valor m�ximo de la cuenta en usegundos).
//SALIDAS: -
//FUNCTION: Llama a la funci�n Delay, para contar en una escala mayor.
void Delayx100us(unsigned int b)
{
 unsigned int j;

 for (j=0; j!=b; ++j) Delay (_100us);
}

//***********************************************************************************
//ENTRADAS: val(caracter enviado al LCD).
//SALIDAS: -
//FUNCTION: Al configurarse el LCD en modo 4 bits, es necesario dividir el caracter
// 'val' en dos partes, as�, hacemos una doble transferencia.
void Put_Val(unsigned char val)
{
  E_ON;
  LCD_OUT = (LCD_OUT & 0x0F)|( val & 0xF0);
  Delay(1); 			      // tw ancho del pulso E. min de 400ns
  E_OFF;
  Delay(1); 			      // tiempo de ciclo de E. min de 1400 ns
  E_ON;
   
  LCD_OUT = (LCD_OUT & 0x0F)|((val<<4)& 0xF0);   
  Delay(1); 			      //2,375 us tw
  E_OFF;
  Delay(1); 			      //th1
  RS_OFF;
}

//***********************************************************************************
//ENTRADAS: Ins(instruccion enviada al LCD).
//SALIDAS: -
//FUNCTION: Se encarga de configurar el LCD para recibir instrucciones.
void Put_Ins(unsigned char Ins)
{
 RS_OFF;
 Delay(1);
 Put_Val(Ins);
 Delay(retint); 	// 39us
}

//***********************************************************************************
//ENTRADAS: Ch(car�cter enviada al LCD).
//SALIDAS: -
//FUNCTION: Se usa para escribir caracteres en el LCD
void Put_Data(unsigned char Ch)
{
 RS_ON;
 Delay(1);
 Put_Val(Ch);
 Delay(retdata);
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCTION: Secuencia de instrucciones que hacen Inicializar al Display.
void Init_Display(void)
{
 Delayx100us(700); //esperar 70 ms. Queda en modo 8 bits.
 E_ON; 
 LCD_OUT = Set_Function+Data_Length_4+Enable_High;
 Delay(1);
 E_OFF;
 Delay(retint);//esperar cambio a modo 4 bits
 Put_Ins(Set_Function + Data_Length_4 + Two_Display_Lines + Font_5x7);
 //no basta una
 Put_Ins(Set_Function + Data_Length_4 + Two_Display_Lines + Font_5x7);
 //la segunda es correctamente aceptada
 Put_Ins(Set_Display + Display_On + Cursor_Off + Blink_Off);
 Put_Ins(Clear_Display);Delayx100us(retclear);
 Put_Ins(Set_Entry_Mode + Increment_Address + Shift_Display_Off);
}

//***********************************************************************************
//ENTRADAS: *Str(Cadena de caracteres).
//SALIDAS: -
//FUNCTION: Funci�n que escribe en el LCD llamando a la funci�n Put_Data, teniendo en
// cuenta que si termina de escribir en la primera l�nea, pasar� a la segunda.
void Put_Str(const char *Str)
{
 unsigned int i, j;

 j = strlen(Str);
 for (i=0; i<j; i++){
   Put_Data(*(Str + i));
   if (i==15) Put_Ins(LINEA2);
 }
}

//***********************************************************************************
//ENTRADAS: direccion(valor donde se colocar� el cursor).
//SALIDAS: -
//FUNCTION: Funci�n que se encarga de limpiar la l�nea seleccinada en la entrada. El
// cursor volver� a la 'direccion' indicada.
void Limpia_Linea(unsigned char direccion)
{
  Put_Ins(direccion);
  Put_Str("                ");
  Put_Ins(direccion);
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCTION: Posiciona el cursor al Inicio de la pantalla de la l�nea 1 y escribe 32
// caracteres en blanco, para luego volver a posicionar el cursor el la l�nea 1.
void Limpia_Pantalla(void)
{
  Put_Ins(LINEA1);
  Put_Str("                                ");
  Put_Ins(LINEA1);
}

//***********************************************************************************
//ENTRADAS: num(entero).
//SALIDAS: -
//FUNCTION: Funci�n encargada de pintar el 'num' en el LCD
void Put_Num(int num)
{
  char LCD[16];
  sprintf(LCD, "%d",num);         
  Put_Str(LCD);
}  

//***********************************************************************************
//ENTRADAS: i(i�simo caracter dela CGRAM donde ser� redefinido el car�cter),
//          l1(),
//          ...
//          l8(). Corresponden a las 8 filas de puntos de cada car�cter.
//SALIDAS: -
//FUNCTION: Rutina que escribe el car�cter i en la CGRAM.
//Los puntos se pasan por l�neas, asegurando que el n�mero sea menor que 32, para no 
// exceder los 5 puntos horizontales por l�nea. 
void PutCharCGRAM(unsigned int i, 
                  unsigned int l1, unsigned int l2,unsigned int l3,unsigned int l4,
                  unsigned int l5, unsigned int l6,unsigned int l7,unsigned int l8)                  
{ 
  if (i<16) { 
    l1%=32; l2%=32; l3%=32; l4%=32; l5%=32; l6%=32; l7%=32; l8%=32;
    Put_Ins(Set_CGRAM_Address + i*8); // Set CGRAM address to ith-character 
                                      // (automatically incremented after write) 
    //                      BitPattern DataByte 
    // Character i // ================ ===== 
    Put_Data( l1 ); // x x x . . . . * 00001 
    Put_Data( l2 ); // x x x . . . * . 00010 
    Put_Data( l3 ); // x x x . . * . . 00100 
    Put_Data( l4 ); // x x x . * . . . 01000 
    Put_Data( l5 ); // x x x * . . . . 10000 
    Put_Data( l6 ); // x x x * . . . * 10001 
    Put_Data( l7 ); // x x x . . * . * 00101 
    Put_Data( l8 ); // x x x * * * * * 11111 
    Put_Ins(Set_DDRAM_Address + 0);
  }
}



