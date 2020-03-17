#include <lcd.h>
#include "application.h"
#include "menu.h"
#include "nvm.h"

APP_DATA      appData;

// ---------------------------------------------------------------------------
// RSI: COMPARATOR-A
//
// Voltage of pin 3  port 2 (BAT SUP) to internal reference of 1.65V (0.5*Vcc)
// - CAOUT = 0, battery on
// - CAOUT = 1, power supply (12V)

#pragma vector=COMPARATORA_VECTOR
__interrupt void BAT (void)
{
    if (!bitest(CACTL2, BIT0)){       // BAT SUP < 0.5*Vcc
        appData.bat = 1;
        P1IE  = 0x00;                //Inhabilito interrupción de pulsadores.
        RELE_OUT = 0;                //Se apagarán todas las salidas.

        ADC12CTL0 &= ~ADC12ON;       //Inhabilito ADC
        CACTL1 &= ~CAIES;            //siguiente interrupcion con flanco de subida de CAOUT
        IE1 &= ~URXIE0;              // Disable USART0 RX/TX interrupts
        IE1 &= ~UTXIE0;
        IE1 &= ~WDTIE;               //Inhabilita el timer del WatchDog.
        dormido;                     //Entra en LPM3
    }
    else if (appData.bat){           // If 12V are back, wake-up
        appData.bat = 0;
        P1IE  = 0x70;                //Habilitación de interrupción de pulsadores.

        CACTL1 |= CAIES;             // siguiente interrupcion con flanco de bajada de CAOUT
        IE1 |= URXIE0 + UTXIE0;      // Enable USART0 RX/TX interrupt
        IE1 |= WDTIE;                // Habilita el timer del WatchDog.
        Init_Display();              // inicialización LCD
        despierto;
    }
}

// ------------------------------------------------------------
// RSI: TIMER-B
//
// Capture of the CCR0 register to interrupt every second (RTC)
// If the MCU is not powered, the ADC is disabled.

void new_minute()
{
    if (appData.dia.hora.min == 59) {  // new hour
        appData.dia.hora.min = 00;
        if (appData.dia.hora.hr == 23) {    // new day
            appData.dia.hora.hr = 00;
            int DiasEnMes;
            bool mes_impar = (  (appData.dia.fecha.mes==1)|(appData.dia.fecha.mes==3)|(appData.dia.fecha.mes==5)|
                                (appData.dia.fecha.mes==7)|(appData.dia.fecha.mes==8)|(appData.dia.fecha.mes==10));
            bool mes_par   = (  (appData.dia.fecha.mes==4)|(appData.dia.fecha.mes==6)|(appData.dia.fecha.mes==9)|
                                (appData.dia.fecha.mes==11));

            if (mes_impar & (appData.dia.fecha.dd == 31)){
                DiasEnMes = 31;
            }
            else if (mes_par & (appData.dia.fecha.dd == 30)){
                DiasEnMes = 30;
            }
            else if ((appData.dia.fecha.mes==2)&(appData.dia.fecha.aa%4 != 0)){
                DiasEnMes = 28;
            }
            else if ((appData.dia.fecha.mes==2)&(appData.dia.fecha.aa%4 == 0)){
                DiasEnMes = 29;
            }

            if (appData.dia.fecha.dd == DiasEnMes) {   // new month
                appData.dia.fecha.dd = 01;
                if (appData.dia.fecha.mes==12)         // new year
                {
                    appData.dia.fecha.mes = 01;
                    if (appData.dia.fecha.aa == 99) {  // new century
                        appData.dia.fecha.aa = 00;
                    }
                    else {
                        appData.dia.fecha.aa++;
                    }
                }
                else {
                    appData.dia.fecha.mes++;
                }
            }
            else {
                appData.dia.fecha.dd++;
            }
        }
        else{
            appData.dia.hora.hr++;
        }
    }
    else{
        appData.dia.hora.min++;
    }
    return;
}

#pragma vector=TIMERB0_VECTOR
__interrupt void CLOCK(void)
{
    if (appData.dia.hora.seg == 59){
        appData.dia.hora.seg = 00;
        new_minute();
    }
    else {
        appData.dia.hora.seg++;
    }
    if (!appData.bat) {
        ADC12CTL0 |= ADC12SC;    //Convierte cada segundo.
    }
}

// ------------------------------------------------------------
// RSI: Analog-Digital Converter
// It interrupts every second with the main 12V.
// Reading ADC12MEM0 register (level sensor)

#define ADC_MIN_VAL 744     // 0x123 corresponds to 0.6V or 0%
#define ADC_MAX_VAL 3723    // 0xE8B corresponds to 3V or 100%
#define ADC_FACTOR  2978    // 0xBA2 corresponds to 2,4V.

unsigned long nivel = 0;    // porcentag of the deposit level

#pragma vector=ADC_VECTOR
__interrupt void NIVEL (void)
{
    unsigned long result = ADC12MEM0;       // IFG is set to '0'

    if((result < 3723) & (result > 744)){   // 0.6V < result < 3V
        result = result - 744;              //ahora el rango será de 0-2.4V
        nivel = (100*result)/2978;          //calcula el nivel en %
    }
    else if(result >= 3723){    //si result >= 3.0
        nivel = 101;
    }
    else if(result <= 744){     //si result <= 0.6
        nivel = 0;
    }
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCTION:

void Init_Ports(void)
{
    P1SEL = 0x08;         //P1.3 en modo TA2 para CAUDAL.
    P1DIR = 0x87;         //P1.7, P1.2, P1.1 and P1.0 de salida
    P1OUT = 0;
    P1IE  = 0x70;         //Habilitaci�n de interrupci�n.
    P1IES = 0x70;         //Interrupci�n por flanco de bajada

    LCD_SEL = 0x08;
    LCD_OUT = 0x02;       //R/W en modo lectura.
    LCD_DIR = 0xF7;       //P2.3 de entrada BAT_SUP

    P3SEL = 0xF0;
    P3DIR = 0x5D;         //Entradas P3.1, P3.5 y P3.7. Resto salidas
    bitset(P3OUT, 2);     // ERROR apagado

    OPTO_SEL = 0;            //Optos
    OPTO_DIR = 0;            //Entradas

    RELE_SEL = 0;            //Rel�s
    RELE_DIR = 0xFF;         //Salidas
    RELE_OUT = 0;

    P6SEL = 0x40;
    P6DIR |= 0xBF;
    P6OUT = 0xFF;         //Se apagan los leds de teclado
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCTION: Activa los dos ociladores del sistema, configurando
//          ACLK = LFXT1 = 32768 y MCLK = XT2 = 8MHz, y si fallara, autom�ticamente
//          MCLK = DCO = 8MHz.

void Init_Osc(void)
{
    WDTCTL = WDT_ADLY_16; // interrumpe cada 16ms
    BCSCTL1 &= ~XT2OFF;   // XT2 = 8MHz on.
    _BIC_SR(OSCOFF);      // turn on LFXT1 oscillator = ACLK.

    BCSCTL2 = SELM1;                  // MCLK = XT2 = 8 MHz.
    DCOCTL |= DCO2 | DCO1 | DCO0;
    BCSCTL1 |= RSEL2 | RSEL1 | RSEL0; // MCLK = DCO, 8 MHz
}

//***********************************************************************************
//ENTRADAS: -
//SALIDAS: -
//FUNCTION: Comparador A para el testeo del pin BAT SUP. Comparamos con Vref = 0.5*Vcc
//          Habilita el Filtro para aumentar la exactitud y resolucion

void Init_COMP(void)
{
    CACTL1 = CAREF_2 + CARSEL + CAON + CAIES + CAIE;  //Vref = 0.5*Vcc, interrupci�n
                                                   //cuando flanco de bajada de CAOUT
    CACTL2 = P2CA0 +CAF;        //selecci�n de CA0(P2.3), habilita el filtro de salida.
    CAPD = 0xF7;                //Todos los pines deshabilitados, menos CA0 = P2.3
    _EINT();
}

//*********************************************************************************//
//ENTRADAS: -
//SALIDAS: -
//FUNCTION: Convertidor ADC para decodificar el valor suministrado por
//          el sensor de Nivel. Usando el reloj ADC12OSC = 5MHz y
//          habilitando la conversion @ Ts = 1muestra/seg.

void Init_ADC(void)
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
//FUNCTION:     Timer B para generar un RTC, interrumpiendo cada segundo,
//              utilizando el registro de comparaci�n CCR0.

void Init_TB(void)
{
    TBCCTL0 = CCIE;             // habilita la interrupci�n para CCR0
    TBCCR0 = cargaTB;           // cuenta desde 0000h hasta 8000h(1seg)
    TBCTL = TBSSEL0 + MC_1;     // usa ACLK, modo up to TBCL0
}

/********************************************************/

void APP_Initialize ()
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
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
    Put_Ins(Set_Display + Display_On + Cursor_Off + Blink_Off);

    Init_Ports();     // configura los puertos
    Init_COMP();      // inicialización Comparador
    Init_TB();        // inicialización TB0
    Init_ADC();       // inicialización Convertidor AD

    Init_Fecha_Hora (appData.dia);
    NVM_Read_Flash(); // carga los datos de Memoria Flash a RAM
	appData.state = APP_STATE_INIT;
}

void APP_Tasks ()
{
	switch(appData.state)
	{
		case APP_STATE_INIT:
            Put_Str("Sistema de Riego");
            Delayx100us(10000);//espera 1seg.
            Put_Ins(LINEA1);
			appData.state = APP_STATE_IDLE;
			break;

		case APP_STATE_IDLE:
		   Menu();
		   break;

		default:
			break;
	}
}
