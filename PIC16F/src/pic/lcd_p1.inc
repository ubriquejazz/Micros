;***************************************************************************************;
;											;
;					LCD_P0.inc					;
;											;
;				 Subrutinas para manejar el LCD.			;
;              Copia de LCD.lib para ser llamada desde la Página 1 (TARJETA.inc)	;
;											;
; 	       Modifica las variables globales Contador1 y Contador2			;
;											;
;***************************************************************************************;

Pausa_5ms_P1	CLRF	Contador1
		MOVLW	0x0F
		MOVWF	Contador2
Pausa1_P1	DECFSZ 	Contador1,1
		GOTO 	Pausa1_P1
		DECFSZ	Contador2,1
		GOTO	Pausa1_P1
		RETURN

LCD_Port_P1	BSF	STATUS,RP0
		CLRF	TRISB
		BCF	STATUS,RP0
		BCF	PORTA,LCDE
		RETURN

LCD_Comando_P1	BCF	PORTA,LCDModo
		BCF	PORTA,LCDRW
		MOVWF	PORTB
		GOTO	LCD_Habilita_P1

LCD_Dato_P1	BSF	PORTA,LCDModo
		BCF	PORTA,LCDRW
		MOVWF	PORTB
		GOTO	LCD_Habilita_P1

LCD_Habilita_P1	BSF	PORTA,LCDE
		NOP
		BCF	PORTA,LCDE
		RETURN
