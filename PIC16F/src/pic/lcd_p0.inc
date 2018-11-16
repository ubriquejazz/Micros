;***************************************************************************************;
;											;
;					LCD_P0.inc					;
;											;
;				Subrutinas para manejar el LCD.				;
;  Copia de LCD.lib para ser llamada desde la Página 0 (Programa Pral. y CONFIG.inc)	;
;											;
;  Modifica las variables globales Contador1 y Contador2				;
;											;
;***************************************************************************************;

Pausa_5ms_P0	CLRF	Contador1
		MOVLW	0x0F
		MOVWF	Contador2
Pausa1_P0	DECFSZ 	Contador1,1
		GOTO 	Pausa1_P0
		DECFSZ	Contador2,1
		GOTO	Pausa1_P0
		RETURN

LCD_Port_P0	BSF	STATUS,RP0
		CLRF	TRISB
		BCF	STATUS,RP0
		BCF	PORTA,LCDE
		RETURN

LCD_Inic_P0	MOVLW	LCDFuncion
		CALL	LCD_Comando_P0
		CALL	Pausa_5ms_P0
		MOVLW	LCDFuncion
		CALL	LCD_Comando_P0
		CALL	Pausa_5ms_P0
		MOVLW	LCDFuncion
		CALL	LCD_Comando_P0
		CALL	Pausa_5ms_P0
		MOVLW	LCDOn
		CALL	LCD_Comando_P0
		CALL	Pausa_5ms_P0
		MOVLW	LCDCLR
		CALL	LCD_Comando_P0
		CALL	Pausa_5ms_P0
		MOVLW	LCDInc
		CALL	LCD_Comando_P0
		CALL	Pausa_5ms_P0
		RETURN

LCD_Comando_P0	BCF	PORTA,LCDModo
		BCF	PORTA,LCDRW
		MOVWF	PORTB
		GOTO	LCD_Habilita_P0

LCD_Dato_P0	BSF	PORTA,LCDModo
		BCF	PORTA,LCDRW
		MOVWF	PORTB
		GOTO	LCD_Habilita_P0

LCD_Habilita_P0	BSF	PORTA,LCDE
		NOP
		BCF	PORTA,LCDE
		RETURN
