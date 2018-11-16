		list      p=16f876            ; list directive to define processor
		#include <p16f876.inc>        ; processor specific variable definitions

		__CONFIG _CP_OFF & _WDT_OFF & _BODEN_ON & _PWRTE_ON & _XT_OSC & _WRT_ENABLE_ON & _LVP_OFF & _DEBUG_OFF & _CPD_OFF 

;***************************** Interfaz PIC-SLE4442 ***********************************

IO		EQU	4		; Línea serie de datos
CLK		EQU	3		; Línea de reloj
RST		EQU	2		; Reset
CARD		EQU	1		; Presencia de tarjeta

;******************************** Interfaz PIC-Periféricos ****************************

RELE		EQU	7		; Relé
LEDR		EQU	0		; LED rojo
ZUMB		EQU	6		; Zumbador
OK		EQU	7		; Tecla que detiene la secuencia Acerca de..

;********************************* Interfaz PIC-LCD ***********************************

LCDModo		EQU	0		;Selecciona Registro LCD
LCDRW		EQU	1		;Lectura / Escritura LCD
LCDE		EQU	2		;Habilita LCD

;********************************* Comandos LCD ***************************************

LCDLinea1	EQU	0x80		;Dirección de comienzo Linea1
LCDLinea2	EQU	0xC0		;Dirección de comienzo Linea2
LCDCLR		EQU	0x01		;Borra pantalla, cursor a casa
LCDInc		EQU	0x06		;Modo Incrementa cursor
LCDOn		EQU	0x0C		;Pantalla On
CursOn		EQU	0x0E		;Pantalla On y cursor On
CursOff		EQU	0x0C		;Pantalla On y cursor Off
CursBlink	EQU	0x0F		;Pantalla On y cursor parpadeo
LCDIzda		EQU	0x18		;Desplaza pantalla a la Izda.
LCDFuncion	EQU	0x38		;Inicialización Registro Función

;********************************* Registros Globales *********************************

hh		EQU	0x70
mm		EQU	0x71		; registros 0x70 - 0x7F
ss		EQU	0x72		;
AA		EQU	0x73		; comunes para todos
MM		EQU	0x74		; los bancos
DD		EQU	0x75

buffer		EQU	0x76		; buffer  de  bits:
di		EQU	7		; bit leído de la tarjeta
do		EQU	6		; bit a escribir en la tarjeta
PAGE1		EQU	4		; copia del bit 4 de PCLATH  
PAGE0		EQU	3		; copia del bit 3 de PCLATH
TU		EQU	2		; Tipo de Usuario de Tarjetas
Cobrar		EQU	1		; indica si hay que cobrar o no
NuevoDia	EQU	0		; indica un nuevo día

MASTER_PIN1	EQU	0x77		; PIN que activa:
MASTER_PIN2	EQU	0x78		;  * el menú de configuración
MASTER_PIN3	EQU	0x79		;  * la recarga de la tarjeta

;************************* Registro Escanea3s, EscaneaCodigo **************************

max		equ	0x7A		; valor máximo al que puede llegar ss en
					; Escanea3s, CodigoConfig y CodigoRecarga

;********************************** Registros RSI *************************************

Contador	EQU	0x7B		; Contador para temporización
w_temp		EQU	0x7C
status_temp	EQU	0x7D

;******************************** Registros EEPROM.inc *********************************

ADDR		EQU	0x7E		; Dirección de la EE donde escribir
VALUE		EQU	0x7F		; Valor a escribir en EE(ADDR) 

;******************************** Registros PonerFecha ********************************

M1		EQU	0x60		; 1º dígito de MM 
D1		EQU	0x61		; 1º dígito de DD 

;******************************** Registros AnalizaDM *********************************

Dir1		EQU	0x62		; Dirección byte bajo y alto de la palabra  a
Dir2		EQU	0x63		; escribir en la EE (RecargaSaldo también las usa)

;****************************** Contadores para Pausas ********************************

Contador1	EQU	0x64		; También usado como contador de bits 
Contador2	EQU	0x65		; Contador para pausas

;****************************** Registros TRADUCIR.inc **************************************

Decenas		EQU	0x66		; dígito para las decenas
Unidades	EQU	0x67		; dígito para las unidades

R0		EQU	0x68		; Nibble Bajo = Decenas de Millar
R1		EQU	0x69		; Nibble Bajo = Centenas; Nibble Alto = Unides de Mill
R2		EQU	0x6A		; Nibble Bajo = Unidades; Nibble Alto = Decenas; 	 

;******************************* Registros TARJETA.inc ***************************************

txbuf		EQU	0x6B		; buffer de transmisión
datai		EQU	0x6C		; byte leído
datao		EQU	0x6D		; byte a escribir

EC		EQU	0x6F		; Contador de Errores
PIN1		EQU	0x50		; 1º dígito del PIN
PIN2		EQU	0x51		; 2º dígito del PIN
PIN3		EQU	0x52		; 3º dígito del PIN

D_ini		EQU	0x53		; Fecha 
M_ini		EQU	0x54		; Inicio de
A_ini		EQU	0x55		; la Validez

D_fin		EQU	0x56		; Fecha 	RecargaSaldo
M_fin		EQU	0x57		; Final de	  también
A_fin		EQU	0x58		; la Validez	  las usa

D_ult		EQU	0x59		; Fecha
M_ult		EQU	0x5A		; Última
A_ult		EQU	0x5B		; Entrada

Saldo		EQU	0x5C		; Saldo

;*************************** Registros de propósito general ***************************

Tecla		EQU	0x5D		; índice de la tecla pulsada
Linea		EQU	0x5E		; índice en el menú de configuración
Pareja		EQU	0x5F		; índice en el menú de usuario

L_byte		EQU	0x4F		; Palabra de 16 bits 
H_byte		EQU	0x4E		; a convertir en BCD  

L_temp  	equ     0x4D		; Palabra de 16 
H_temp  	equ     0x4C		; bits temporal
temp  		equ     0x4B		; byte temporal (AnalizaDM también lo usa)

Contador3	EQU	0x4A		; Contador de propósito general
Contador4	EQU	0x49		; Contador de propósito general

;**************************** Sección Código de Reset *********************************

		ORG	0x000		; Comienza el programa en Vector de Reset
		CLRF	PCLATH		; selecciona página 0 (000h - 7FFh)
		GOTO 	Principal	

;********************** Rutina de Servicio a la Interrupción **************************

		ORG     0x004        	; interrupt vector location

		BCF 	buffer,PAGE0	
		BTFSC	PCLATH,3	; buffer(3) = PCLATH(3)
		BSF	buffer,PAGE0

		BCF	PCLATH,3
		GOTO	RSI

;*************************** Librería con tablas de datos *****************************

		#include <PRAL_TAB.inc>
		#include <CNFG_TAB.inc>

;**************************************************************************************
;******************************** Programa Principal **********************************
;**************************************************************************************

Principal	CLRF	INTCON

		BSF	STATUS,RP0
		MOVLW	0x06		; Configuración de puertos:
		MOVWF	ADCON1
		CLRF	TRISA		; Puerto A como salida
		CLRF	TRISB		; Puerto B como salida
		MOVLW	b'00000010'	; Puerto C como salida excepto pin CARD
		MOVWF	TRISC
		BCF	STATUS,RP0
		
		BCF	PORTC,RST	; RST  = 0
		BCF	PORTC,CLK	; CLK  = 0
		BCF	PORTC,RELE	; RELE = 0
		BSF	PORTC,LEDR	; LEDR = 1
		BSF	PORTC,ZUMB	; ZUMB = 1
		CLRF	buffer		; buffer = 0
		CLRF	PORTA		; PORTA  = 0
		CLRF	PORTB		; PORTB  = 0
		CLRF	hh		; hh = 0
		CLRF	mm		; mm = 0
		CLRF	ss		; ss = 0

		MOVLW	0x04
		CALL	Leer_EE		; AA = EE(4)
		MOVWF	AA
		MOVLW	0x05
		CALL	Leer_EE		; MM = EE(5)
		MOVWF	MM
		MOVLW	0x06
		CALL	Leer_EE		; DD = EE(6)
		MOVWF	DD

		MOVLW	0x07
		CALL	Leer_EE		; MASTER_PIN1 = EE(7)
		MOVWF	MASTER_PIN1
		MOVLW	0x08
		CALL	Leer_EE		; MASTER_PIN2 = EE(8)
		MOVWF	MASTER_PIN2
		MOVLW	0x08
		CALL	Leer_EE		; MASTER_PIN3 = EE(9)
		MOVWF	MASTER_PIN3

		CALL	Pausa_500ms	; Para que se inicialice el LCD
		CALL	LCD_Inic_P0	; Inicializa LCD
		CALL	Config		; Menú de Configuración
		CALL	Mostrar_Hora
		CALL	Mostrar_Fecha	; Muestra la fecha y el número de visita

		MOVLW	10h
		MOVWF	Contador	; carga 16 a Contador
		MOVLW	0x0C
		MOVWF	TMR0		; carga TMR0
		BSF	STATUS,RP0
		MOVLW	b'10000111'
		MOVWF	OPTION_REG	; Predivisor 256
		BCF	STATUS,RP0
		MOVLW	b'10100000'
		MOVWF	INTCON		; Activa GIE y T0IE
		MOVLW	0x01
		MOVWF	Contador4	; i = 1

Siempre		CALL 	EscaneaCodigo	; Escanea hasta que se pulse
		CALL	LCD_Port_P0	;   o que pase 1 segundo

CNFG_PIN1?	MOVLW	0x01		;     Si (i = 1)
		SUBWF	Contador4,0
		BTFSS	STATUS,Z	;          &		
		GOTO	CNFG_PIN2?
		MOVF	Tecla,0		; ( Tecla = MASTER_PIN1),
		SUBWF	MASTER_PIN1,0	
		BTFSS	STATUS,Z
		GOTO	No_Config
		INCF	Contador4,1	;       i = i + 1
		GOTO	Tarjeta?

CNFG_PIN2?	MOVLW	0x02		;     Si (i = 2)
		SUBWF	Contador4,0
		BTFSS	STATUS,Z	;          &		
		GOTO	CNFG_PIN3?
		MOVF	Tecla,0		; ( Tecla = MASTER_PIN2),
		SUBWF	MASTER_PIN2,0	
		BTFSS	STATUS,Z
		GOTO	No_Config
		INCF	Contador4,1	;       i = i + 1
		GOTO	Tarjeta?

CNFG_PIN3?	MOVLW	0x03		;     Si (i = 3)
		SUBWF	Contador4,0
		BTFSS	STATUS,Z	;          &		
		GOTO	Error_CNFG
		MOVF	Tecla,0		; ( Tecla = MASTER_PIN3),
		SUBWF	MASTER_PIN3,0	
		BTFSS	STATUS,Z
		GOTO	No_Config
		CALL	Config		; Menú de Configuración
		CALL	Mostrar_Fecha	; Muestra la fecha y el número de visita
		MOVLW	0x01		; i = 1
		MOVWF	Contador4
		GOTO 	Tarjeta?

No_Config	MOVF	Tecla,0		; Si no ha pasado 1 seg.
		BTFSC	STATUS,Z	; pero se ha pulsado algo,
		GOTO	Tarjeta?
Error_CNFG	MOVLW	0x01		; i = 1
		MOVWF	Contador4

Tarjeta?	BTFSS	PORTC,CARD	; ¿ TARJETA ?
		GOTO	No_Tarjeta
		BSF	PCLATH,3	; Selecciona página 1
		CALL 	Tarjeta		; Gestión de tarjetas
		BCF	PCLATH,3	; Selecciona página 0
		CALL	Mostrar_Fecha	; Muestra la fecha y el número de visita
		MOVLW	0x01		; i = 1
		MOVWF	Contador4

No_Tarjeta	BTFSC	buffer,NuevoDia	   ; ¿ NUEVO DÍA ?
		CALL	Mostrar_Fecha	   ; Muestra la fecha y el número de visita
		BCF	buffer,NuevoDia    ; NuevoDia = 0

		CALL	Mostrar_Hora
		GOTO	Siempre

;***************************** Subrutina EscaneaCodigo ********************************
;
;	   Escanea el teclado hasta que se pulse o pase 1 segundo
;
;**************************************************************************************

EscaneaCodigo	call	KB_Port
		movf	ss,0
		addlw	0x01
		movwf	max		; max = ss + 1
		movlw 	.60
		subwf 	max,0		; ¿ max = 60 ?	
		btfsc	STATUS,Z
		clrf	max		; Si lo es, max = 0

ScanCode	CALL	KB_Scan
		MOVF	Tecla,0
		BTFSS	STATUS,Z	; Mientras (Tecla = 0)
		RETURN
		MOVF	ss,0		;          &
		SUBWF	max,0
		BTFSC	STATUS,Z	;     (ss != max)
		RETURN
		GOTO	ScanCode	;       Escanea

;********************************* Subrutina Mostrar_Hora *****************************
;
;			Muestra la hora en el LCD. Por ejemplo 23:59
;
;**************************************************************************************

Mostrar_Hora	MOVLW	0x82		; Posiciona el cursor en la 1ª línea 
		CALL	LCD_Comando_P0	; y en la 2ª columna del LCD	
		CALL	Pausa_5ms_P0

		MOVF	hh,0		; Muestra hh
		CALL	Bin8BCD
		MOVLW	30h
		ADDWF	Decenas,1
		ADDWF	Unidades,1
		MOVF	Decenas,0
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVF	Unidades,0
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVLW	':'		
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVF	mm,0		; Muestra mm
		CALL	Bin8BCD
		MOVLW	30h
		ADDWF	Decenas,1
		ADDWF	Unidades,1
		MOVF	Decenas,0
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVF	Unidades,0
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		RETURN	

;******************************* Subrutina Mostrar_Fecha ******************************
;
;	            Muestra la fecha en el LCD. Por ejemplo 31/12
;
;**************************************************************************************

Mostrar_Fecha	MOVLW	0x89		; Posiciona el cursor en la 1ª línea 
		CALL	LCD_Comando_P0	; y en la 10ª columna del LCD	
		CALL	Pausa_5ms_P0

		MOVF	DD,0		; Muestra DD
		CALL	Bin8BCD
		MOVLW	30h
		ADDWF	Decenas,1
		ADDWF	Unidades,1
		MOVF	Decenas,0
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVF	Unidades,0
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVLW	'/'		
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVF	MM,0		; Muestra MM
		CALL	Bin8BCD
		MOVLW	30h
		ADDWF	Decenas,1
		ADDWF	Unidades,1
		MOVF	Decenas,0
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		MOVF	Unidades,0
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

		GOTO	Mostrar_Visita		

;***************************** Subrutina Mostrar_Visita *******************************
;
;        Muestra el número de visitante en el LCD. Por ejemplo Visitante: 00000
;
;**************************************************************************************

Mostrar_Visita	MOVLW	LCDLinea2	; Posiciona el cursor en la 2ª fila
		CALL	LCD_Comando_P0
		CALL	Pausa_5ms_P0	

		CLRF	Contador3
Coge_Letra00	MOVF	Contador3,0
		CALL	Visitante	; "Visitante: "
		IORLW	00h
		BTFSC	STATUS,Z
		GOTO	FinMensaje00
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		INCF	Contador3,1
		GOTO	Coge_Letra00

FinMensaje00	MOVLW	0x00
		CALL	Leer_EE	
		MOVWF	L_byte		; L_byte = EE(0)
		MOVLW	0x01		; H_byte = EE(1)
		CALL	Leer_EE	
		MOVWF	H_byte
		CALL	Bin16BCD	; R0, R1, R2 = Dígitos BCD

		MOVLW	0x30
		ADDWF	R0,0		; Muestra Decenas de Millar
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

		SWAPF	R1,0
		ANDLW	0x0F
		ADDLW	0x30		; Muestra Unidades de Millar
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		
		MOVF	R1,0
		ANDLW	0x0F
		ADDLW	0x30		; Muestra Centenas
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

		SWAPF	R2,0
		ANDLW	0x0F
		ADDLW	0x30		; Muestra Decenas
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0

		MOVF	R2,0
		ANDLW	0x0F
		ADDLW	0x30		; Muestra Unidades
		CALL	LCD_Dato_P0
		CALL	Pausa_5ms_P0
		RETURN

;************************* Librerías llamadas desde la página 0 ***********************

		#include <RSI.inc>
		#include <LCD_P0.inc>
		#include <CONFIG.inc>
		#include <CNFG_AUX.inc>

;********************** Librerías llamadas desde las páginas 0 y 1 ********************

		#include <TRADUCIR.inc>
		#include <TECLADO.inc>
		#include <EEPROM.inc>

;********************* Librerías en la página 1  de la Memoria de Programa ************

		ORG      0x800		
		#include <TRJT_TAB.inc>
		#include <TRJT_CMD.inc>
		#include <TRJ_USER.inc>
		#include <SLE4442.inc>
		#include <TARJETA.inc>
		#include <LCD_P1.inc>

		ORG      0xF00
		#include <INFORMAR.inc>
	
		END
